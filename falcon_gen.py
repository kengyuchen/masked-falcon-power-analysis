import numpy as np

RCDT = [
    3024686241123004913666,
    1564742784480091954050,
    636254429462080897535,
    199560484645026482916,
    47667343854657281903,
    8595902006365044063,
    1163297957344668388,
    117656387352093658,
    8867391802663976,
    496969357462633,
    20680885154299,
    638331848991,
    14602316184,
    247426747,
    3104126,
    28824,
    198,
    1
]

def gaussian0_sampler():
    """
    Sample an integer from a half-Gaussian centered on zero with standard deviation 1.8205.

    Adapted from Zf(gaussian0_sampler) in sign.c and Algorithm 12 (BaseSampler)
    of the Falcon specification.

    Returns:
        int: The sampled non-negative integer.
    """    
    # Get a random 72-bit value, into three 24-bit limbs v0..v2.
    u0 = np.random.randint(0, 2**24)
    u1 = np.random.randint(0, 2**24)
    u2 = np.random.randint(0, 2**24)
    u = (int(u0) << 48) + (int(u1) << 24) + int(u2)

    # The sampled value z is the number of table entries that u is lower than.
    z = 0
    for i in range(0, len(RCDT)):
        cc = (u - RCDT[i] < 0)
        z += int(cc)
    return z


# Adapted from falcon's document - Algorithm 15 SamplerZ
# sigma_min is 1.277833697 for Falcon-512 and 1.298280334 for Falcon-1024
def SamplerZ(mu, sigma_p, sigma_min = 1.277833697):
    """
    Sample an integer from a discrete Gaussian with center mu and standard deviation sigma_p.

    Adapted from Algorithm 15 (SamplerZ) of the Falcon specification.

    Args:
        mu (float): Center of the distribution.
        sigma_p (float): Standard deviation.
        sigma_min (float): 1.277833697 for Falcon-512 and 1.298280334 for Falcon-1024.

    Returns:
        float: The sampled integer value.
    """
    sigma_max = 1.8205
    r = mu - np.floor(mu)
    ccs = sigma_min / sigma_p
    while True:
        z0 = gaussian0_sampler()
        b = np.random.randint(0, 2)
        z = b + (2*b - 1)*z0
        x = ( (z-r)**2 / (2 * (sigma_p**2)) ) - ((z0**2) / (2 * (sigma_max**2)))
        t = np.random.binomial(1, ccs * np.exp(-x))
        if t == 1:
            return z + np.floor(mu)

def Samplef(n = 512, q = 12289):
    """
    Sample one coefficient of f.

    Adapted from Algorithm 5 (NTRUGen) and Equation 3.29 of the Falcon specification:
    the target standard deviation is 1.17 * sqrt(q / (2n)), obtained by summing
    4096 / n samples of standard deviation 1.17 * sqrt(q / 8192).

    Args:
        n (int): Ring degree.
        q (int): Modulus.

    Returns:
        float: The sampled coefficient.
    """
    # sigma_fg = 1.17 * np.sqrt(q / (2*n))
    sigma_star = 1.17 * np.sqrt(q / 8192)
    return sum(SamplerZ(0, sigma_star) for _ in range(4096 // n))


def gen_f(size_f):
    """
    Generate a polynomial f by sampling each coefficient from a discrete Gaussian.

    Args:
        size_f (int): Number of coefficients (ring degree n).

    Returns:
        np.ndarray: The coefficients of f.
    """
    return np.array([Samplef(n=size_f) for _ in range(size_f)], dtype=np.float64)


def gen_c(size_f, q=12289):
    """
    Generate a polynomial c with coefficients uniformly distributed in [0, q).
   
    This models the output of HashToPoint in the Falcon specification.
   
    Args:
        size_f (int): Number of coefficients (ring degree n).
        q (int): Modulus.

    Returns:
        np.ndarray: The coefficients of c.
    """
    return np.random.randint(0, q, size_f).astype(np.float64)

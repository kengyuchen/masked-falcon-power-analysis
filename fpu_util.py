import numpy as np
import struct

MANTISSA_BITS = 52
EXPONENT_BIAS = 1023
MANTISSA_MASK = (1 << MANTISSA_BITS) - 1

def fpuSplit(x: np.float64):
    """
    Split a float64 into its sign, biased exponent, and mantissa fields.

    Args:
        x (float): The value to split.

    Returns:
        tuple: (s, e, m) as Python ints, where s is the 1-bit sign, e is the
        11-bit biased exponent, and m is the 52-bit mantissa (without the
        implicit leading 1).
    """
    bits = int(np.float64(x).view(np.int64)) & 0xFFFFFFFFFFFFFFFF
    s = bits >> 63
    e = (bits >> MANTISSA_BITS) & 0x7FF
    m = bits & MANTISSA_MASK
    return (s, e, m)

def fpuMerge(s, e, m):
    """
    Rebuild a float64 from its sign, biased exponent, and mantissa fields.

    Only valid for normal numbers (0 < e < 2047).

    Args:
        s (int): Sign bit.
        e (int): Biased exponent.
        m (int): 52-bit mantissa (without the implicit leading 1).

    Returns:
        np.float64: The reconstructed value.
    """
    return (-1)**s * (2**np.float64(e-EXPONENT_BIAS)) * (1 + m * np.float64(2**(-MANTISSA_BITS)))

def HW(x, high=False):
    """
    Return the Hamming weight of an integer.

    Args:
        x (int): The integer. For negative values, the weight of |x| is returned.
        high (bool): If True, only count the bits above bit 31 (i.e. x >> 32).

    Returns:
        int: The number of set bits.
    """
    if high:
        x = (x >> 32)
    return bin(x).count('1')

def fpuHW(x: np.float64):
    """
    Return the Hamming weights of the sign, exponent, and mantissa of a float64.

    The mantissa weight only counts its upper 20 bits (see HW with high=True).

    Args:
        x (float): The value to analyze.

    Returns:
        tuple: (HW(s), HW(e), HW(m, high=True)).
    """
    s, e, m = fpuSplit(x)
    return HW(s), HW(e), HW(m, high=True)

def normalize53(x):
    """
    Shift a non-negative integer right until it fits in 53 bits.

    Args:
        x (int): The integer to normalize.

    Returns:
        int: x shifted right by the smallest amount such that the result is below 2**53.
    """
    x = int(x)
    return x >> max(0, x.bit_length() - 53)

def uint64_to_float64(x: int):
    """
    Reinterpret a 64-bit unsigned integer as a float64 with the same bit pattern.

    Args:
        x (int): The 64-bit pattern.

    Returns:
        float: The corresponding float64 value.
    """
    return struct.unpack('<d', struct.pack('<Q', x))[0]
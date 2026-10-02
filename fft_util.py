import numpy as np

class FFT:
    """FFT of fixed length n with precomputed twiddle factors."""
    def __init__(self, n):
        """
        Initialize the FFT object with a fixed length n.

        Precomputes the twiddle factors (GM and iGM) in bit-reversed order.

        Args:
            n (int): Transform length, which must be a power of 2.
        """
        if (n & (n - 1)) != 0:
            raise ValueError("Size n must be a power of 2.")
            
        self.n = n
        self.log_n = int(np.log2(n))
        
        # Initialize Twiddle Factor tables
        self.GM = np.zeros(n, dtype=complex)
        self.iGM = np.zeros(n, dtype=complex)
        
        # Base complex root of unity
        w = np.exp(1j * np.pi / n)
        
        for i in range(n):
            rev_i = self._reverse_bits(i, self.log_n)
            self.GM[i] = w ** rev_i
            self.iGM[i] = (1/w) ** rev_i

    def _reverse_bits(self, n, no_of_bits):
        """Internal helper to reverse the bits of an integer."""
        result = 0
        for i in range(no_of_bits):
            result <<= 1
            result |= n & 1
            n >>= 1
        return result

    def forward(self, x):
        """
        Compute the forward FFT.

        Args:
            x (array-like): Input of length n.

        Returns:
            np.ndarray: The complex FFT of x, in bit-reversed order.
        """
        n = self.n
        if len(x) != n:
            raise ValueError(f"Input array must be of length {n}")
            
        x = x.astype(complex).copy() # Copy to avoid modifying original array
        t = n
        m = 1
        
        while m < n:
            ht = t // 2
            for i1 in range(m):
                j1 = i1 * t
                s = self.GM[m + i1]
                for j in range(j1, j1 + ht):
                    a = x[j]
                    b = s * x[j + ht]
                    x[j] = a + b
                    x[j + ht] = a - b
            t = ht
            m *= 2
        return x

    def inverse(self, X):
        """
        Compute the inverse FFT.

        Args:
            X (array-like): Input of length n, in bit-reversed order.

        Returns:
            np.ndarray: The complex inverse FFT of X.
        """
        n = self.n
        if len(X) != n:
            raise ValueError(f"Input array must be of length {n}")
            
        X = X.astype(complex).copy()
        t = 1
        m = n
        
        while m > 1:
            hm = m // 2
            dt = t * 2
            for i1 in range(hm):
                j1 = i1 * dt
                s = self.iGM[hm + i1]
                for j in range(j1, j1 + t):
                    a = X[j]
                    b = X[j + t]
                    X[j] = a + b
                    X[j + t] = s * (a - b)
            t = dt
            m //= 2
            
        # Scaling by 1/n
        return X / n
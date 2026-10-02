import numpy as np
import glob
import os

def generate_elmo_plaintexts(list_re, list_im, filename="plaintexts.txt"):
    """
    Write a plaintexts file for ELMO from lists of real and imaginary parts.

    Each float is bit-cast to its 64-bit pattern and split into two 32-bit hex lines,
    giving 4 lines per trace: re_high, re_low, im_high, im_low.

    Args:
        list_re (list): Real parts.
        list_im (list): Imaginary parts.
        filename (str): Path to the ELMO plaintexts file to write.

    Returns:
        None
    """
    # Convert inputs to a NumPy array of floats
    re_floats = np.array(list_re, dtype=np.float64)
    im_floats = np.array(list_im, dtype=np.float64)

    # Bit-cast the floats to 64-bit unsigned integers (same bit pattern)
    # .tolist() converts them to native Python ints to support all bitwise ops
    re_bits = re_floats.view(np.uint64).tolist()
    im_bits = im_floats.view(np.uint64).tolist()

    with open(filename, "w") as f:
        for re_val, im_val in zip(re_bits, im_bits):
            re_high = (re_val >> 32) & 0xFFFFFFFF
            re_low  = re_val & 0xFFFFFFFF
            
            im_high = (im_val >> 32) & 0xFFFFFFFF
            im_low  = im_val & 0xFFFFFFFF
            
            # Write 4 lines per trace for elmo.c read32 logic
            f.write(f"{re_high:08x}\n")
            f.write(f"{re_low:08x}\n")
            f.write(f"{im_high:08x}\n")
            f.write(f"{im_low:08x}\n")

def load_combined_hex(filename='ELMO/output/printdata.txt'):
    """
    Read an ELMO printdata file and combine every 8 bytes into a 64-bit integer.

    Args:
        filename (str): Path to the ELMO output file, with one hex byte per line.

    Returns:
        np.ndarray: A 1D array of 64-bit unsigned integers.
    """
    with open(filename, 'r') as f:
        # # Each non-empty line holds one hex byte (e.g. "0a", "ff").
        raw_values = [int(line.strip(), 16) for line in f if line.strip()]

    # Ensure we have a multiple of 8
    if len(raw_values) % 8 != 0:
        raise ValueError(f"Expected a multiple of 8 bytes, got {len(raw_values)} in {filename}")
    num_full_ints = len(raw_values) // 8
    print(f"Loaded {num_full_ints} 64-bit values")
    combined_ints = []
    for i in range(num_full_ints):
        # Grab a chunk of 8 bytes
        chunk = raw_values[i*8 : (i+1)*8]

        # Combine in big-endian order to match the C code.
        val = 0
        for byte in chunk:
            val = (val << 8) | (byte & 0xFF)
        
        combined_ints.append(val)
    return np.array(combined_ints, dtype=np.uint64)

def load_elmo_traces(root_dir='ELMO/output/traces'):
    """
    Read ELMO .trc files from a directory and stack them into a 2D array.

    Args:
        root_dir (str): Path to the folder containing the .trc files.

    Returns:
        np.ndarray: A 2D array of shape (N_traces, Trace_length).
    """
    search_path = os.path.join(root_dir, "*.trc")
    trace_files = sorted(glob.glob(search_path))
    
    if not trace_files:
        raise FileNotFoundError(f"No .trc files found in {root_dir}")

    # ELMO traces are ASCII text files with one float per line
    traces_list = [np.loadtxt(path) for path in trace_files]

    # Axis 0 (rows) = individual traces
    # Axis 1 (cols) = time points (clock cycles)
    traces = np.vstack(traces_list)
    
    return traces

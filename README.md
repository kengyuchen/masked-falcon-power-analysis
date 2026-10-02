# A Power Analysis Attack on Masked Implementations of Falcon

This repository implements a side-channel attack on [Falcon](https://falcon-sign.info/), a post-quantum digital signature scheme selected by NIST for standardization (as FN-DSA, FIPS 206, currently in draft) in the [NIST Post-Quantum Cryptography Standardization Process](https://csrc.nist.gov/projects/post-quantum-cryptography). The attack targets a masked implementation of the preimage computation. The Falcon code is taken from Falcon's [Round 3 submission](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Falcon-Round3.zip), specifically the code under the directory `falcon-round3/Extra/c/`. The power traces are simulated with [ELMO](https://github.com/sca-research/ELMO).



## Requirements

### Python

The analysis code and notebooks were developed on Python 3.8. Install the Python dependencies with:

```
pip install -r requirements.txt
```

### C and ARM toolchains (for ELMO)

Building and running ELMO requires a C toolchain and the GNU ARM Embedded Toolchain. The versions below follow ELMO's [official instructions](https://github.com/sca-research/ELMO).

* **To compile ELMO** you need the GCC compiler collection (ELMO is tested with GCC 7.3.0 on Ubuntu) and `make`:
    * Ubuntu: `sudo apt install build-essential`
    * macOS: install the "Command Line Tools for Xcode", then check `gcc -v`.
* **To compile the target code** (`FprMul.bin`) into an ARM Thumb binary you need the GNU ARM Embedded Toolchain, available from [Arm's developer site](https://developer.arm.com/downloads/-/gnu-rm):
    * Ubuntu: unpack the toolchain tarball and add its `bin/` directory to your `PATH`; a (possibly older) version is also available via `sudo apt install gcc-arm-none-eabi`.
    * macOS: unpack the tarball and add its `bin/` directory to your `PATH`, or install it with `brew install arm-none-eabi-gcc`.



## Generate Traces

The notebook `gen_trace.ipynb` generates the traces: it runs ELMO and truncates each trace to its last 99 points to save space, storing the results in `truncated_traces/`.

Before running it, build the two binaries (see the Configuration section for the parameters in `FprMul.c`):

* In `ELMO/`, run `make` to build `elmo`.
* In `ELMO/FPR/`, run `make` to build `FprMul.bin`, which runs the masked complex-number multiplication.



To generate raw traces from the terminal instead, follow these steps:

1. Build the binaries as above.

2. Prepare the `plaintexts.txt` input file. It follows the format produced by the Python script `elmo_util.py`; see `gen_trace.ipynb`.

3. In `ELMO/`, run `./elmo FPR/FprMul.bin 2> /dev/null`.

    * There are redundant memory messages on stderr, so we redirect it.
    * The trace files are the `.trc` files in `ELMO/output/traces/`.



## Attack

The notebook `attack.ipynb` runs the attack. It uses the traces in `truncated_traces/` and the hash files in `hashes/`.

The attack correlates each hypothesis at specific **leakage cycle indices** (`T_REAL` and `T_IMAG`) within the truncated traces: one cycle for the real part of the product (`f1c1 - f2c2`) and one for the imaginary part (`f2c1 + f1c2`). These indices are set empirically by profiling the shipped `ELMO/FPR/FprMul.bin`. They are defined in `attack_idx` in `attack.ipynb` and must be re-profiled if that binary changes, since a different number of shares (`SHAREN`), a different compiler, or a change to the trace truncation can shift where the leakage occurs.



## Masked Falcon

### Configuration

The following parameters are set in `ELMO/FPR/FprMul.c`:

* `#define NOTRACES 200` sets the number of traces to record.
* `#define SHAREN 2` sets the number of shares in the masking.

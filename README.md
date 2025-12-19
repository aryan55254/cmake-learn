## quick build start 

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./chronos_runner
```


## Debugging Modes (The "Safety Gear")

Use these modes when developing or chasing bugs. **Do not use both at the same time.**

### 1. Detecting Race Conditions (ThreadSanitizer)

Use this if you suspect threads are fighting over data (race conditions, deadlocks).

```bash
cd build
# Clean old cache to switch modes
rm -rf * # Configure with TSan ON
cmake -DENABLE_TSAN=ON -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Run (See "Known Issues" below if this crashes)
./chronos_runner

```

### 2. Detecting Memory Leaks (AddressSanitizer)

Use this if you get Segfaults or think you are leaking memory.

```bash
cd build
rm -rf *

# Configure with ASan ON
cmake -DENABLE_ASAN=ON -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

./chronos_runner

```

---

##  Performance Mode (Release)

Use this when you want to measure the **real** speed of the scheduler. This turns off safety checks and turns on compiler optimizations (`-O3`).

### Native Optimization

This tells the compiler to use AVX/AVX2 instructions specific to your CPU architecture.

```bash
cd build
rm -rf *

# Release mode + Native Optimizations
cmake -DCMAKE_BUILD_TYPE=Release -DCHRONOS_NATIVE=ON ..
cmake --build .

./chronos_runner

```

---

## Troubleshooting & Known Issues

### "FATAL: ThreadSanitizer: unexpected memory mapping"

If running TSan gives you a fatal memory mapping error, it is because Linux ASLR (randomization) is conflicting with TSan's memory reservation.

**Fix:** Run the executable with ASLR disabled:

```bash
setarch $(uname -m) -R ./chronos_runner

```

### CMake "Could not find compiler" or weird Cache errors

If you change flags and things act weird, always nuke the build folder:

```bash
# Inside /build
rm -rf *
cmake ..

```

## Build Options Reference

| Option | Default | Description |
| --- | --- | --- |
| `ENABLE_TSAN` | `OFF` | Enables ThreadSanitizer (Detects data races) |
| `ENABLE_ASAN` | `OFF` | Enables AddressSanitizer (Detects memory errors) |
| `CHRONOS_NATIVE` | `OFF` | Enables `-march=native` for max CPU performance |
| `CMAKE_BUILD_TYPE` | `Debug` | `Debug` (slow, symbols) or `Release` (fast, -O3) |


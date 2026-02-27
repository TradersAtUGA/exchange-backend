# Simulation Engine — Build Instructions (Linux x86_64)

## Prerequisites

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install -y git cmake ninja-build g++-13 pkg-config curl zip unzip tar

# Fedora/RHEL
# sudo dnf install -y git cmake ninja-build gcc-c++ pkgconf curl zip unzip tar
```

## Set up vcpkg at `~/vcpkg`

```bash
# If you already downloaded it, skip clone and only bootstrap.
# git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg
./bootstrap-vcpkg.sh
export VCPKG_ROOT="$HOME/vcpkg"
```

## Build simulation tests

```bash
cd exchange-backend

# Configure — only builds simulation tests + matching_engine dependency
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DCMAKE_CXX_COMPILER=g++-13 \
  -DBUILD_SIMULATION=ON

# Build sim tests only
cmake --build build --target sim_tests

# Run all tests
cd build && ctest --output-on-failure
```

## Quick one-liner

```bash
sudo apt install -y git cmake ninja-build g++-13 curl zip unzip tar \
  && [ -d "$HOME/vcpkg" ] || git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg" \
  && "$HOME/vcpkg/bootstrap-vcpkg.sh" \
  && export VCPKG_ROOT="$HOME/vcpkg" \
  && cd exchange-backend \
  && cmake -S . -B build -G Ninja \
       -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
       -DCMAKE_CXX_COMPILER=g++-13 \
       -DBUILD_SIMULATION=ON \
  && cmake --build build --target sim_tests \
  && cd build && ctest --output-on-failure
```

## Notes

- vcpkg automatically fetches `quickfix`, `concurrentqueue`, `nlohmann-json`, and `gtest` on first configure (~5 min)
- `--target sim_tests` builds only the simulation binary and matching_engine, skipping gateway/outbound
- If Ninja is not available, drop `-G Ninja` to use Unix Makefiles
- If g++-13 is not available, any C++20-capable compiler works (g++-12, clang++-16, etc.) — just update `-DCMAKE_CXX_COMPILER`

## Running specific test suites

```bash
cd build

# Run only bug regression tests
ctest -R BugRegression --output-on-failure

# Run only stress tests
ctest -R Stress --output-on-failure

# Run only edge case tests
ctest -R EdgeCase --output-on-failure

# Run the multi-agent simulation
ctest -R MultiAgent --output-on-failure

# Run with verbose output
ctest -V --output-on-failure
```

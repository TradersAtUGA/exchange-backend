# Exchange Backend

Prototype exchange backend for order matching, market data, and simulation.

## Features
- Single-threaded matching engine with price-level order queues
- TCP gateways for order submission
- UDP multicast for market data
- Lock-free queue architecture for low-latency order processing
- Simulation and backtesting tools

## Getting Started

### Release mode
```sh
cmake --list-presets                    # list configure presets
cmake --preset release                  # configure
cmake --build --preset release-build      # build
./build/release/apps/test_comp            # run
```

### Debug mode
```sh
cmake --list-presets                    # list configure presets
cmake --preset debug                    # configure
cmake --build --preset debug-build      # build
./build/debug/apps/test_comp            # run
```

### Unit test
```sh
cmake --list-presets                   # list configure presets
cmake --preset unit-config             # configure
cmake --build --preset unit-build      # build
ctest --preset run-unit-test           # test
```



## Requirements
- Cmake 
- C++ 20
- G++ Compiler >= 13
- Docker

## Coding style
- Unless said otherwise use the google c++ style guide
- Variable, Function, and .cpp files names: snake_case
- Directory names: kebab-case
- Constant names: CAPITAL_SNAKE_CASE
- For header files use .hpp

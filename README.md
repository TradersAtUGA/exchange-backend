# Exchange Backend

Prototype exchange backend for order matching, market data, and simulation.

## Features
- Single-threaded matching engine with price-level order queues
- TCP gateways for order submission
- UDP multicast for market data
- Lock-free queue architecture for low-latency order processing
- Simulation and backtesting tools

## Getting Started

### Build with Docker
```bash
docker build -f docker/Dockerfile.matcher -t exchange-backend .
docker run --network host exchange-backend
```

## Build locally with Cmake
mkdir build && cd build
cmake ..
make

## Requirements
- C++ 20
- G++ Compiler >= 11
- Docker

## Coding style
- Unless said otherwise use the google c++ style guide
- Variable names: snake_case
- Folder names: kebab-case
- Constant names: CAPITAL_SNAKE_CASE
- For header files use .hpp

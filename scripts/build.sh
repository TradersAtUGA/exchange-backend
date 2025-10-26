#!/bin/bash
# Default to Release build if no arg is provided
# Usage: debug or release
BUILD_TYPE=${1:-Release}

docker run --rm -v "$(pwd)":/workspace exchange_backend_image \
bash -c "mkdir -p /workspace/build && cd /workspace/build \
&& cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
   -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake \
   -DBUILD_TESTING=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. \
&& cmake --build . --target exchange_backend"



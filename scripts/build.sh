#!/bin/bash
docker run --rm -v "$(pwd)":/workspace exchange_backend_image \
bash -c "mkdir -p /workspace/build && cd /workspace/build \
&& cmake -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake -DBUILD_TESTING=OFF .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. \
&& cmake --build . --target exchange_backend"


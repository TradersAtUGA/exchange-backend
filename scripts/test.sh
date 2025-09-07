#!/bin/bash
docker run --rm -v "$(pwd)":/workspace exchange_backend_image \
bash -c "mkdir -p /workspace/build && cd /workspace/build \
&& cmake -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake -DBUILD_TESTING=ON .. \
&& cmake --build . --target all && ctest"
# ngl the --target all is literally compiling everything so running tests kinda slow

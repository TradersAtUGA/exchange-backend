#!/bin/bash

# docker run --rm -it \
#     -v $(pwd):/app \
#     -v exchange_ccache:/ccache \
#     -v exchange_build:/app/build \
#     -v exchange_vcpkg:/app/vcpkg_installed \
#     exchange-dev

# docker run --rm -it \
#     -v $(pwd)/src:/app/src \
#     -v $(pwd)/include:/app/include \
#     -v $(pwd)/build:/app/build \
#     exchange-dev

docker run --rm -it -v $(pwd):/app exchange-dev

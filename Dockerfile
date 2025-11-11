# Base image with build tools
FROM ubuntu:22.04


# Install essentials
# RUN apt-get update && apt-get install -y \
#     build-essential cmake git curl unzip zip python3 pkg-config && \
#     rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get install -y software-properties-common \ 
    && add-apt-repository ppa:ubuntu-toolchain-r/test -y \
    && apt-get update \
    && apt-get install -y build-essential cmake git curl unzip zip python3 pkg-config gcc-13 g++-13 ccache \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 100 \
    && rm -rf /var/lib/apt/lists/*

# Cmake cache setup
ENV CCACHE_DIR=/ccache
ENV PATH="/usr/lib/ccache:${PATH}"

# Set working directory
WORKDIR /workspace

# Clone and bootstrap vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg && \
    /vcpkg/bootstrap-vcpkg.sh

# Copy project files
COPY . /workspace

# Install dependencies from manifest
RUN /vcpkg/vcpkg install --triplet x64-linux

# Port defined for the crow input network 
# this is tied to ./include/network-input/api_routing.hpp PORT_NUMBER variable


# Default command
CMD ["bash"]

FROM ubuntu:24.04

# download necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gcc-12 \
    g++-12 \
    git \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

WORKDIR /app

COPY . /app/


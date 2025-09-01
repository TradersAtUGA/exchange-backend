#!/bin/bash

docker run --rm -v "$(pwd)":/workspace exchange_backend_image \
bash -c "./build/bin/exchange_backend"

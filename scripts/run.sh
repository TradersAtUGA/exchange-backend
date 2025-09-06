    #!/bin/bash

    # adjust the port based on what is being exposed in the main dockerfile
    docker run -p 10000:10000 --rm -v "$(pwd)":/workspace exchange_backend_image \
    bash -c "./build/apps/exchange-server/exchange_backend"

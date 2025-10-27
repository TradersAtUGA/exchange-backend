#!/bin/bash

# Usage: ./send_signal.sh <PID> <SIGNAL>
# Example: ./send_signal.sh 12345 SIGINT
# u have to be inside the docker shell to run this
# to get in docker shell run
# docker exec -it <container_name> bash
# once inside run
# ./scripts/shutdown.sh <pid> 10
# the pid is usually 1

if [ $# -ne 2 ]; then
    echo "Usage: $0 <PID> <SIGNAL>"
    exit 1
fi

PID=$1
SIGNAL=$2

# Send the signal
kill -s $SIGNAL $PID

if [ $? -eq 0 ]; then
    echo "Signal $SIGNAL sent to process $PID"
else
    echo "Failed to send signal. Check PID and permissions."
fi
#!/bin/bash
URL="http://localhost:11000/order-send"
count=0
# Function to run when Ctrl+C (SIGINT) is pressed
cleanup() {
  echo -e "\nStopped. Total requests sent: $count"
  exit 0
}
# Trap SIGINT and call cleanup
trap cleanup SIGINT
# Infinite loop
while true; do
  JSON=$(cat <<EOF
{
  "client_id": 10,
  "order_id": $count,
  "side": "buy",
  "type": "market",
  "ticker": "XYZ",
  "price": 10,
  "quantity": 10,
  "timestamp": 10,
  "remaining": 10
}
EOF
)
  curl -s -X POST "$URL" \
    -H "Content-Type: application/json" \
    -d "$JSON"
  ((count++))
  # sleep 1  
done
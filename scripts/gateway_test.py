import socket
from datetime import datetime

SOH = '\x01'

def build_fix_message(fields: dict):
    # Convert dict to FIX list
    fix_fields = [f"{k}={v}" for k, v in fields.items()]

    # Build body (all fields after 9=)
    body = SOH.join(fix_fields[2:]) + SOH

    # Compute body length
    body_length = len(body.encode())

    # Header
    head = f"8=FIX.4.2{SOH}9={body_length}{SOH}"

    # Full message before checksum
    full_without_checksum = head + body

    # Compute checksum: sum of bytes mod 256
    checksum = sum(full_without_checksum.encode()) % 256

    return (full_without_checksum + f"10={checksum:03}{SOH}").encode()


def build_logon(sender="TESTCLIENT", target="EXCHANGE", seq=1, hb=20):
    ts = datetime.utcnow().strftime("%Y%m%d-%H:%M:%S")

    fields = {
        "8": "FIX.4.2",       # ignored for building, added later
        "9": "0",             # ignored, will be replaced
        "35": "A",            # Logon
        "49": sender,
        "56": target,
        "34": seq,
        "52": ts,
        "98": 0,              # encryption = none
        "108": hb             # Heartbeat interval
    }

    return build_fix_message(fields)


HOST = "127.0.0.1"
PORT = 11000

msg = build_logon()

print("Sending FIX Logon:")
print(msg)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.send(msg)

print("Waiting for response:")

resp = s.recv(4096)
print(resp)

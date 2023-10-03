import serial
import time

responses = {
              'd':7,
              'u':6
            }

port = "COM10"
connection = serial.Serial(port, timeout=1)

def send_command(cmd: str, responses_len:int) -> str:
  connection.write(cmd.encode())
  if (responses_len != 0):
    #connection.in_waiting <-> available
    resp = connection.read(responses_len)
    str_resp = resp.decode()
  return str_resp

while True:
  for command in responses:
    resp = send_command(command, responses[command])
    print(resp)
    time.sleep(1)
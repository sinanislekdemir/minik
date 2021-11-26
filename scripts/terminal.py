import glob
import sys
import threading
from time import sleep
from typing import List

import serial

active = True


def serial_ports() -> List[str]:
    """List serial port names."""
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


print("Serial ports:")
for s in serial_ports():
    print(s)

if len(sys.argv) > 1:
    selection = sys.argv[1]
else:
    selection = input("Port: ")

simba = serial.Serial(port=selection)
print("Connected")


def reader():
    """Read data thread."""
    while active:
        line = simba.readline()
        print(line.decode('ascii', errors="ignore"))


def writer():
    """Write data thread."""
    global active
    while active:
        line = input("")
        if line == 'exit':
            active = False
        line += "\n"
        bytes_written = simba.write(bytes(line, 'utf-8'))
        print(f"{bytes_written} bytes written")
        sleep(0.01)


r = threading.Thread(target=reader)
w = threading.Thread(target=writer)
r.start()
w.start()
r.join()
w.join()

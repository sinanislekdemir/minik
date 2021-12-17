#!/usr/bin/env python3
import glob
import sys
import threading
from time import sleep
from typing import List

import click
import serial

active = True
simba = None
ready = False


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


def reader():
    """Read data thread."""
    global ready
    while active:
        if simba is not None:
            line = simba.readline()
            tmp = line.decode('ascii', errors="ignore")
            if 'ready to receive' in tmp:
                ready = True
            print(tmp, end='')
            sys.stdout.flush()


@click.command('upload')
@click.option("--port", default="", help="Device address")
@click.option("--filename", help="File to upload")
def upload(port: str, filename: str):
    """Upload file to given port."""
    global simba
    global active
    global ready
    r = threading.Thread(target=reader)
    r.start()
    if port == "":
        port = serial_ports()[0]
    simba = serial.Serial(port=port)
    simba.write(bytes("#ignore\n", 'ascii'))
    print("Waiting for connection")
    while not ready:
        pass
    print(f"Sending file {filename} on port {port}")
    with open(filename, "r") as f:
        bytesw = simba.write(bytes(f.read(), 'ascii'))
    simba.write(bytes("\n.\n", 'ascii'))
    simba.flush()
    print(f"{bytesw} bytes written")
    print("Closing in 3 seconds")
    sleep(3)
    r.join()
    active = False


@click.command('list')
def list_ports():
    print("Serial ports:")
    for s in serial_ports():
        print(s)


@click.group()
def entry_point():
    pass


entry_point.add_command(upload)
entry_point.add_command(list_ports)

if __name__ == '__main__':
    entry_point()

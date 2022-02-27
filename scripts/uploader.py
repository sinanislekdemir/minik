#!/usr/bin/env python3
import glob
import sys
import threading
from time import sleep
from getpass import getpass
from typing import List

import click
import serial

active = True
socket = None
ready = False
shutdown = False


def check_script(fname: str) -> bool:
    data = open(fname).read().splitlines()
    main_check = "main:" in data
    sub_open = False
    for line in data:
        if line == "":
            continue
        if line[-1] == ":":
            if sub_open:
                print(f"{fname} sub remains open")
                return False
            sub_open = True
        if line == "---" and sub_open:
            sub_open = False
    if not main_check:
        print(f"{fname}: main sub not found")
        return False
    if sub_open:
        print(f"{fname}: sub remains open")
        return False
    return True


def serial_ports() -> List[str]:
    """List serial port names."""
    if sys.platform.startswith("win"):
        ports = ["COM%s" % (i + 1) for i in range(256)]
    elif sys.platform.startswith("linux") or sys.platform.startswith("cygwin"):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob("/dev/tty[A-Za-z]*")
    elif sys.platform.startswith("darwin"):
        ports = glob.glob("/dev/tty.*")
    else:
        raise EnvironmentError("Unsupported platform")

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
    global ready, shutdown
    buffer = ""
    while active:
        if socket is not None:
            while socket.in_waiting:
                data = socket.read(size=1)
                data_s = data.decode("ascii", errors="ignore")
                print(data_s, end="")
                buffer += data_s

            if buffer.endswith("\n"):
                buffer = buffer.replace("\r", "")

                if "ready to receive" in buffer:
                    ready = True

                buffer = ""

            sys.stdout.flush()


def writer():
    global active
    while active:
        c = input("").strip().replace("\n", "")
        if socket is not None:
            socket.write(bytes(c, "ascii"))
        if c == "quit":
            active = False

@click.command("upload")
@click.option("--port", default="", help="Device address")
@click.option("--filename", help="File to upload")
def upload(port: str, filename: str):
    """Upload file to given port."""
    global socket
    global active
    global ready
    r = threading.Thread(target=reader)
    r.start()
    if not port:
        port = serial_ports()[0]

    socket = serial.Serial(port=port)
    print("Waiting for connection")
    while not ready:
        print("Sending source code request")
        socket.write(bytes("program\n", "ascii"))
        sleep(1)
        print(f"Ready = {ready}")

    for fname in filename.split(","):
        if fname == "":
            continue
        if not check_script(fname):
            continue
        print(f"Sending file {fname} on port {port}")
        data = ""
        with open(fname, "r") as f:
            data = f.read()
        for line in data.splitlines():
            socket.write(bytes(line + "\n", "ascii"))
            socket.flush()
            sleep(0.10)
            if line == '---':
                sleep(1)
        socket.write(bytes("\n.\n", "ascii"))
        socket.flush()
        print("Sent program")

    w = threading.Thread(target=writer)
    w.start()
    r.join()
    r.join()


@click.command("list")
def list_ports():
    print("Serial ports:")
    for s in serial_ports():
        print(s)


@click.group()
def entry_point():
    pass


@click.command("check")
@click.option("--filename", help="File to check")
def check(filename: str):
    for fname in filename.split(","):
        if fname == "":
            continue
        check_script(fname)


entry_point.add_command(upload)
entry_point.add_command(list_ports)
entry_point.add_command(check)

if __name__ == "__main__":
    entry_point()

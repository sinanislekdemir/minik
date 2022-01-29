import sys

with open('drivers.txt', 'r') as drivers:
    driver_list = drivers.read().splitlines()
    for item in driver_list:
        print(f'-DWITH_{item} ', file=sys.stdout, end='')

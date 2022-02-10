import sys

with open('config.txt', 'r') as drivers:
    driver_list = drivers.read().splitlines()
    for item in driver_list:
        if '=' in item:
            parts = [part.strip() for part in item.split('=')]
            print(f'-D{parts[0]}={parts[1]} ', file=sys.stdout, end='')
        else:
            print(f'-DWITH_{item} ', file=sys.stdout, end='')

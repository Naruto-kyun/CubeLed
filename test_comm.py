from time import sleep
from random import choice

serial = '/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A6008dRc-if00-port0'
serial_r = open(serial, 'r')
serial_w = open(serial, 'w')

l = 0

while True:
    serial_w.write('W')
    for z in range(4):
        for y in range(4):
            for x in range(4):
                if l % 2:
                    val = x == z
                else:
                    val = x != z
                val = 4095 if val else 0
                serial_w.write('%s ' % val)
                serial_w.flush()
                sleep(0.001)

    sleep(0.1)
    l += 1

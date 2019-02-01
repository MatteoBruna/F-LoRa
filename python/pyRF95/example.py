#!/usr/bin/env python3

from rf95 import RF95, Bw31_25Cr48Sf512

if __name__ == "__main__":
    rf95 = RF95(0, 24, 25)
    if not rf95.init():
        print("RF95 not found")
        rf95.cleanup()
        quit(1)
    else:
        print("RF95 LoRa mode ok")

        # set frequency and power
    rf95.set_frequency(868.0)
    rf95.set_tx_power(23)
        # Custom predefined mode
    #rf95.set_modem_config(Bw31_25Cr48Sf512)

    while True:
        data = False
        while data is False:
            data = rf95.recv()

        print("".join(chr(i) for i in data[4:]))

    rf95.set_mode_idle()
    rf95.cleanup()

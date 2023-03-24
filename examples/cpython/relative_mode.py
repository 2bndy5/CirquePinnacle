"""
This example reads data from the Cirque trackpad in "relative mode" and prints the values.

See documentation at https://cirquepinnacle.rtfd.io/
"""
import argparse
import sys
from typing import Union
from cirque_pinnacle import (
    RelativeReport,
    PinnacleTouchSPI,
    PinnacleTouchI2C,
    PINNACLE_SW_DR,
)

# some CLI convenience (for testing)
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("-i", "--use_i2c", action="store_true", help="Use I2C instead of SPI")
parser.add_argument(
    "-d",
    "--use_sw_dr",
    action="store_true",
    help="Use software Data Ready flag instead of hardware DR pin",
)

# an object to hold data reported by the Cirque trackpad
report = RelativeReport()


class TouchController:
    def __init__(self, use_i2c: bool = False, use_sw_dr: bool = False):
        dr_pin = 25  # GPIO25
        if not use_sw_dr:
            dr_pin = PINNACLE_SW_DR  # uses internal DR flag

        self.trackpad: Union[PinnacleTouchSPI, PinnacleTouchI2C]
        if not use_i2c:
            ss_pin = 0  # uses /dev/spidev0.0 (CE0 or GPIO8)
            self.trackpad = PinnacleTouchSPI(dr_pin, ss_pin)
        else:  # If using I2C, then use the following line (not the line above)
            self.trackpad = PinnacleTouchI2C(dr_pin)

    def setup(self):
        if not self.trackpad.begin():
            raise OSError("Cirque Pinnacle not responding!")
        print("CirquePinnacle/examples/cpython/relative_mode")
        return True

    def loop(self):
        if self.trackpad.available():
            self.trackpad.read(report)
            print(report)


if __name__ == "__main__":
    args = parser.parse_args()
    controller = TouchController(args.use_i2c, args.use_sw_dr)
    if not controller.setup():  # if trackpad.begin() failed
        sys.exit(1)  # fail fast
    while True:  # use ctrl+C to exit
        try:
            controller.loop()
        except KeyboardInterrupt:
            break

"""
This example reads data from the Cirque trackpad in "anymeas mode" and prints the values.

See documentation at https://cirquepinnacle.rtfd.io/
"""
import argparse
import sys
from typing import Union
from cirque_pinnacle import (
    PinnacleTouchSPI,
    PinnacleTouchI2C,  # noqa: imported but unused
    PINNACLE_ANYMEAS,
)

# some CLI convenience (for testing)
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("-i", "--use_i2c", action="store_true", help="Use I2C instead of SPI")

# we'll use this list of tuples as args to PinnacleTouch.measure_adc()
vector_determinants = [
    # (toggle, polarity)
    (0x0000FFFF, 0x00000000),  # toggle all x-axis bits negatively (to 0)
    (0x0FFF0000, 0x00000000),  # toggle all y-axis bits negatively (to 0)
    (0x00FF00FF, 0x000000FF),  # toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
    (0x00FF00FF, 0x00FF0000),  # toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
]


class TouchController:
    def __init__(self, use_i2c: bool = False):
        dr_pin = 25  # GPIO25 (pin 22 if using MRAA driver)

        self.trackpad: Union[PinnacleTouchSPI, PinnacleTouchI2C]
        if not use_i2c:
            print("Using SPI interface")
            ss_pin = 0  # uses /dev/spidev0.0 (CE0 or GPIO8)
            self.trackpad = PinnacleTouchSPI(dr_pin, ss_pin)
        else:  # If using I2C, then use the following line (not the line above)
            self.trackpad = PinnacleTouchI2C(dr_pin)
            print("Using I2C interface")

        # a list of compensations to use with measured `vector_determinants`
        self.compensation = [0] * len(vector_determinants)

    def compensate(self):
        sweep = 5
        for i, (toggle, polarity) in enumerate(vector_determinants):
            value = 0
            for _ in range(sweep):
                value += self.trackpad.measure_adc(toggle, polarity)
            self.compensation[i] = int(value / sweep)

    def setup(self):
        if not self.trackpad.begin():
            raise OSError("Cirque Pinnacle not responding!")
        print("CirquePinnacle/examples/cpython/anymeas_mode")
        self.trackpad.data_mode = PINNACLE_ANYMEAS
        self.compensate()
        return True

    def loop(self):
        for i, (toggle, polarity) in enumerate(vector_determinants):
            measurement = self.trackpad.measure_adc(toggle, polarity)
            measurement -= self.compensation[i]
            print(f"meas{i}:", measurement, end="\t")
        print()  # end line


if __name__ == "__main__":
    args = parser.parse_args()
    controller = TouchController(args.use_i2c)
    if not controller.setup():  # if trackpad.begin() failed
        sys.exit(1)  # fail fast
    while True:  # use ctrl+C to exit
        try:
            controller.loop()
        except KeyboardInterrupt:
            break

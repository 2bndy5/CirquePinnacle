"""
This example reads data from the Cirque trackpad in "relative mode" and
prints the values.

See documentation at https://cirquepinnacle.rtfd.io/
"""

import time
from typing import Union
from cirque_pinnacle import (
    RelativeReport,
    PinnacleTouchSPI,
    PinnacleTouchI2C,
)

print("CirquePinnacle/examples/cpython/relative_mode\n")

# The digital input pin connected to the trackpad's DR (DataReady) pin
dr_pin = 25  # GPIO25

trackpad: Union[PinnacleTouchSPI, PinnacleTouchI2C]
if not input("Is the trackpad configured for I2C? [y/N] ").lower().startswith("y"):
    print("-- Using SPI interface.")
    ss_pin = 0  # uses /dev/spidev0.0 (CE0 or GPIO8)
    trackpad = PinnacleTouchSPI(dr_pin, ss_pin)
else:
    print("-- Using I2C interface")
    trackpad = PinnacleTouchI2C(dr_pin)

if not trackpad.begin():
    raise OSError("Cirque Pinnacle not responding!")

# an object to hold data reported by the Cirque trackpad
data = RelativeReport()


def print_data(timeout=6):
    """Print available data reports from the Pinnacle touch controller
    until there's no input for a period of ``timeout`` seconds."""
    print(
        "Touch the sensor to see the data. Exits after",
        timeout,
        "seconds of inactivity.",
    )
    start = time.monotonic()
    while time.monotonic() - start < timeout:
        while trackpad.available():  # is there new data?
            trackpad.read(data)
            print(data)
            start = time.monotonic()


def set_role():
    """Set the role using stdin stream. Arguments for functions can be
    specified using a space delimiter (e.g. 'M 10' calls `print_data(10)`)
    """
    user_input = (
        input(
            "\n*** Enter 'M' to measure and print data."
            "\n*** Enter 'Q' to quit example.\n"
        )
        or "?"
    ).split()
    if user_input[0].upper().startswith("M"):
        print_data(*[int(x) for x in user_input[1:2]])
        return True
    if user_input[0].upper().startswith("Q"):
        return False
    print(user_input[0], "is an unrecognized input. Please try again.")
    return set_role()


if __name__ == "__main__":
    try:
        while set_role():
            pass  # continue example until 'Q' is entered
    except KeyboardInterrupt:
        print(" Keyboard Interrupt detected.")
else:
    print("\nRun print_data() to read and print data.")

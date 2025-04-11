"""
This example reads data from the Cirque trackpad in "absolute mode" and
prints the values.

See documentation at https://cirquepinnacle.rtfd.io/
"""

import math
import time
from typing import Union
from cirque_pinnacle import (
    AbsoluteReport,
    PinnacleTouchSPI,
    PinnacleTouchI2C,
    PINNACLE_ABSOLUTE,
)

print("CirquePinnacle/examples/cpython/absolute_mode")

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

trackpad.data_mode = PINNACLE_ABSOLUTE
trackpad.absolute_mode_config(1)  # set count of z-idle packets to 1

# an object to hold data reported by the Cirque trackpad
data = AbsoluteReport()


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

            # specification sheet recommends clamping absolute position data of
            # X & Y axis for reliability
            if data.z:  # only clamp values if Z axis is not idle
                data.x = max(128, min(1920, data.x))  # 128 <= x < = 1920
                data.y = max(64, min(1472, data.y))  #   64 <= y < = 1472

            print(data)
            start = time.monotonic()


def print_trig(timeout=6):
    """Print available data reports from the Pinnacle touch controller as trigonometric
    calculations until there's no input for a period of ``timeout`` seconds."""
    print(
        "Touch the trackpad to see the data. Exits after",
        timeout,
        "seconds of inactivity.",
    )
    start = time.monotonic()
    while time.monotonic() - start < timeout:
        while trackpad.available():  # is there new data?
            trackpad.read(data)

            if not data.z:  # if not touching (or near) the sensor
                print("Idling")  # don't do calc when both axes are 0
            else:  # if touching (or near) the sensor
                # datasheet recommends clamping X & Y axis for reliability
                data.x = max(128, min(1920, data.x))  # 128 <= x <= 1920
                data.y = max(64, min(1472, data.y))  #   64 <= y <= 1472

                # coordinates assume axes have been clamped to recommended ranges
                coord_x = data.x - 960
                coord_y = data.y - 736  # NOTE: y-axis is inverted by default
                radius = math.sqrt(math.pow(coord_x, 2) + math.pow(coord_y, 2))
                # angle (in degrees) ranges [-180, 180];
                angle = math.atan2(coord_y, coord_x) * 180 / math.pi
                print("angle: %.2f\tradius: %.2f" % (angle, radius))
            start = time.monotonic()


def set_role():
    """Set the role using stdin stream. Arguments for functions can be
    specified using a space delimiter (e.g. 'M 10' calls `print_data(10)`)
    """
    user_input = (
        input(
            "\n*** Enter 'M' to measure and print raw data."
            "\n*** Enter 'T' to measure and print trigonometric calculations."
            "\n*** Enter 'Q' to quit example.\n"
        )
        or "?"
    ).split()
    if user_input[0].upper().startswith("M"):
        print_data(*[int(x) for x in user_input[1:2]])
        return True
    if user_input[0].upper().startswith("T"):
        print_trig(*[int(x) for x in user_input[1:2]])
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
    print(
        "\nRun print_data() to read and print raw data.",
        "Run print_trig() to measure and print trigonometric calculations.",
        sep="\n",
    )

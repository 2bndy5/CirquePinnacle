"""
This example reads data from the Cirque trackpad in "anymeas mode" and
prints the values.

See documentation at https://cirquepinnacle.rtfd.io/
"""

import time
from typing import Union
from cirque_pinnacle import (
    PinnacleTouchSPI,
    PinnacleTouchI2C,  # noqa: imported but unused
    PINNACLE_ANYMEAS,
)

print("CirquePinnacle/examples/cpython/anymeas_mode\n")

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
trackpad.data_mode = PINNACLE_ANYMEAS

# we'll use this list of tuples as args to PinnacleTouch.measure_adc()
vectors = [
    #  toggle  ,   polarity
    (0x0000FFFF, 0x00000000),  # toggle all x-axis bits negatively (to 0)
    (0x0FFF0000, 0x00000000),  # toggle all y-axis bits negatively (to 0)
    (
        0x00FF00FF,
        0x000000FF,
    ),  # toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
    (
        0x00FF00FF,
        0x00FF0000,
    ),  # toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
]

# a list of compensations to use with measured `vectors`
compensation = [0] * len(vectors)


def compensate():
    sweep = 5
    for i, (toggle, polarity) in enumerate(vectors):
        value = 0
        for _ in range(sweep):
            value += trackpad.measure_adc(toggle, polarity)
        compensation[i] = int(value / sweep)
        print("compensation {}: {}".format(i, compensation[i]))


def take_measurements(timeout=6):
    """Read ``len(vectors)`` number of measurements and print results for
    ``timeout`` number of seconds."""
    print("Taking measurements for", timeout, "seconds.")
    start = time.monotonic()
    while time.monotonic() - start < timeout:
        for i, (toggle, polarity) in enumerate(vectors):
            result = trackpad.measure_adc(toggle, polarity)
            print("meas{}: {}".format(i, result - compensation[i]), end="\t")
        print()


def set_role():
    """Set the role using stdin stream. Arguments for functions can be
    specified using a space delimiter (e.g. 'C 10' calls `compensate(10)`)
    """
    user_input = (
        input(
            "\n*** Enter 'C' to get compensations for measurements."
            "\n*** Enter 'M' to read and print measurements."
            "\n*** Enter 'Q' to quit example.\n"
        )
        or "?"
    ).split()
    if user_input[0].upper().startswith("C"):
        compensate(*[int(x) for x in user_input[1:2]])
        return True
    if user_input[0].upper().startswith("M"):
        take_measurements(*[int(x) for x in user_input[1:2]])
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
        "\nRun compensate() to set compensations for measurements.",
        "Run take_measurements() to read and print measurements.",
        sep="\n",
    )

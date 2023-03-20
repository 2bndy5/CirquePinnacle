"""
This example reads data from the Cirque trackpad in "absolute mode" and prints the values.

See documentation at https://cirquepinnacle.rtfd.io/
"""
import sys
from cirque_pinnacle import (
    AbsoluteReport,
    PinnacleTouchSPI,
    PinnacleTouchI2C,  # noqa: imported but unused
    PINNACLE_ABSOLUTE,
)

DR_PIN = 25
SS_PIN = 0

trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN)
# If using I2C, then use the following line (not the line above)
# trackpad = PinnacleTouchI2C(DR_PIN);

report = AbsoluteReport()


def setup():
    if not trackpad.begin():
        raise OSError("Cirque Pinnacle not responding!")
    print("found Cirque Pinnacle!")
    trackpad.data_mode = PINNACLE_ABSOLUTE
    trackpad.absolute_mode_config(1)  # set count of z-idle packets to 1
    return True


def loop():
    if trackpad.available():
        trackpad.read(report)
        print(report)


if __name__ == "__main__":
    if not setup():  # if trackpad.begin() failed
        sys.exit(1)  # fail fast
    while True:  # use ctrl+C to exit
        try:
            loop()
        except KeyboardInterrupt:
            break
    trackpad.shutdown = True

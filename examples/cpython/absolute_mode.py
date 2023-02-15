# pylint: disable=missing-docstring,import-error
import sys
from cirque_pinnacle import AbsoluteReport, PinnacleTouchSPI, PINNACLE_ABSOLUTE

trackpad = PinnacleTouchSPI(6, 0)
report = AbsoluteReport()


def setup():
    if not trackpad.begin():
        raise OSError("Cirque Pinnacle not responding!")
    print("found Cirque Pinnacle!")
    trackpad.data_mode = PINNACLE_ABSOLUTE.value
    trackpad.absolute_mode_config(1)  # set count of z-idle packets to 1
    return True


def loop():
    if trackpad.available():
        trackpad.read(report)
        print(
            f"Left: {report.buttons & 1}",
            f"Right: {report.buttons & 2}",
            f"Middle: {report.buttons & 4}",
            f"\tX: {report.x}",
            f"\tY: {report.y}",
            f"\tZ: {report.z}",
        )


if __name__ == "__main__":
    if not setup():  # if trackpad.begin() failed
        sys.exit(1)  # fail fast
    while True:  # use ctrl+C to exit
        loop()

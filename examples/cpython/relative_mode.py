# pylint: disable=missing-docstring,import-error
import sys
from cirque_pinnacle import RelativeReport, PinnacleTouchSPI

trackpad = PinnacleTouchSPI(6, 0)
report = RelativeReport()


def setup():
    if not trackpad.begin():
        raise OSError("Cirque Pinnacle not responding!")
    print("found Cirque Pinnacle!")
    return True


def loop():
    if trackpad.available():
        trackpad.read(report)
        print(
            f"Left: {report.buttons & 1}",
            f"Right: {report.buttons & 2}",
            f"Middle: {report.buttons & 4}",
            f"\tdelta X: {report.x}",
            f"\tdelta Y: {report.y}",
            f"\tdelta Scroll: {report.scroll}",
        )


if __name__ == "__main__":
    if not setup():  # if trackpad.begin() failed
        sys.exit(1)  # fail fast
    while True:  # use ctrl+C to exit
        loop()
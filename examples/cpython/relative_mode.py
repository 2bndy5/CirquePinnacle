"""
This example reads data from the Cirque trackpad in "relative mode" and prints the values.

See documentation at https://cirquepinnacle.rtfd.io/
"""
import sys
from cirque_pinnacle import RelativeReport, PinnacleTouchSPI

DR_PIN = 25
SS_PIN = 0

trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN)
# If using I2C, then use the following line (not the line above)
# trackpad = PinnacleTouchI2C(DR_PIN);

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
        try:
            loop()
        except KeyboardInterrupt:
            break
    trackpad.shutdown = True

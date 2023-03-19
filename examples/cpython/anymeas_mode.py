"""
This example reads data from the Cirque trackpad in "anymeas mode" and prints the values.

See documentation at https://cirquepinnacle.rtfd.io/
"""
import sys
from cirque_pinnacle import PinnacleTouchSPI, PINNACLE_ANYMEAS

DR_PIN = 25
SS_PIN = 0

trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN)
# If using I2C, then use the following line (not the line above)
# trackpad = PinnacleTouchI2C(DR_PIN);

vector_determinants = [
    # (toggle, polarity)
    (0x0000FFFF, 0x00000000),  # toggle all x-axis bits negatively (to 0)
    (0x0FFF0000, 0x00000000),  # toggle all y-axis bits negatively (to 0)
    (0x00FF00FF, 0x000000FF),  # toggle Y0-Y7 negatively (to 0) & X0-X7 positively (to 1)
    (0x00FF00FF, 0x00FF0000),  # toggle Y0-Y7 positively (to 1) & X0-X7 negatively (to 0)
]


def setup():
    if not trackpad.begin():
        raise OSError("Cirque Pinnacle not responding!")
    print("found Cirque Pinnacle!")
    trackpad.data_mode = PINNACLE_ANYMEAS
    return True


def loop():
    for i, (toggle, polarity) in enumerate(vector_determinants):
        measurement = trackpad.measure_adc(toggle, polarity)
        print(f"meas{i}:", measurement, end="\t")
    print()  # end line


if __name__ == "__main__":
    if not setup():  # if trackpad.begin() failed
        sys.exit(1)  # fail fast
    while True:  # use ctrl+C to exit
        try:
            loop()
        except KeyboardInterrupt:
            break
    trackpad.shutdown = True


.. image:: https://readthedocs.org/projects/cirquepinnacle/badge/?version=latest
    :target: https://cirquepinnacle.readthedocs.io/en/latest/?badge=latest
    :alt: Documentation Status
.. image:: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_arduino.yml/badge.svg
    :target: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_arduino.yml
    :alt: Arduino build
.. image:: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_platformio.yml/badge.svg
    :target: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_platformio.yml
    :alt: PlatformIO build
.. image:: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_pico_sdk.yml/badge.svg
    :target: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_pico_sdk.yml
    :alt: Pico SDK build
.. image:: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_linux.yml/badge.svg
    :target: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_linux.yml
    :alt: Linux build
.. image:: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_python.yml/badge.svg
    :target: https://github.com/2bndy5/CirquePinnacle/actions/workflows/build_python.yml
    :alt: Python build

Introduction
============

A driver library for interfacing with the Cirque Pinnacle (1CA027) touch controller used in Cirque Glidepoint Circle Trackpads.

This library was originally ported from
`CircuitPython_Cirque_Pinnacle  <https://gitHub.com/2bndy5/CircuitPython_Cirque_Pinnacle>`_.

Supported Features
------------------

* Use SPI or I2C bus protocols to interface with the Pinnacle ASIC (Application
  Specific Integrated Circuit) touch controller.
* Relative mode data reporting (AKA Mouse mode) with optional tap detection.
* Absolute mode data reporting (x, y, & z axis positions).
* AnyMeas mode data reporting. This mode exposes the ADC (Analog to Digital Converter) values and is
  not well documented in the numerous datasheets provided by the Cirque corporation about the
  Pinnacle (1CA027), thus this is a rather experimental mode.
* Hardware input buttons' states included in data reports. There are 3 button input lines on
  the Cirque circle trackpads -- see `Pinout`_ section.
* Configure measurements for finger or stylus (or automatically determine either) touch
  events. The Cirque circle trackpads are natively capable of measuring only 1 touch
  point per event.
* Download/upload the underlying compensation matrix for ADC measurements.
* Adjust the ADC matrix gain (sensitivity).

.. tip:: The SPI protocol is the preferred method for interfacing with more than 1 Cirque circle
    trackpad from the same MCU (microcontroller). The Cirque Pinnacle does not allow
    changing the I2C slave device address (via software); this means only 1 Cirque circle trackpad
    can be accessed over the I2C bus in the lifecycle of an application. That said, you could change
    the I2C address from ``0x2A`` to ``0x2C`` by soldering a 470K ohm resistor at the junction
    labeled "ADR" (see picture in `Pinout`_ section), although this is untested.

Unsupported Features
--------------------

* The legacy PS\\2 interface is pretty limited.
  Therefore, it has been neglected in this library.
* Cirque's circle trackpads ship with the newer non-AG (Advanced Gestures) variant of the
  Pinnacle touch controller ASIC. Thus, this library focuses on the the non-AG variant's
  functionality via testing, and it does not provide access to the older AG variant's features
  (register addresses slightly differ which breaks compatibility).

Pinout
======

.. warning::
    The GPIO pins on these trackpads are **not** 5v tolerant. If your microcontroller uses 5v logic
    (ie Arduino Nano, Uno, Pro, Micro), then you must remove the resistors at junctions "R7" and "R8".
    Reportedly, this allows powering the trackpad with 5v (to VDD pin) and the trackpad GPIO pins become
    tolerant of 5v logic levels.
.. image:: https://github.com/2bndy5/CircuitPython_Cirque_Pinnacle/raw/master/docs/_static/Cirque_GlidePoint-Circle-Trackpad.png
    :target: https://www.mouser.com/new/cirque/glidepoint-circle-trackpads/

The above picture is a example of the Cirque GlidePoint circle trackpad. This picture
is chosen as the test pads (larger copper circular pads) are clearly labeled. The test pads
are extended to the `12-pin FFC/FPC cable
<https://www.mouser.com/c/connectors/ffc-fpc/ffc-fpc-jumper-cables/?number%20of%20conductors=12%20Conductor&pitch=0.5%20mm>`_
connector (the white block near the bottom). The following table shows how the pins are connected in
the `examples <https://cirquepinnacle.readthedocs.io/en/latest/examples.html>`_

.. csv-table:: pinout (ordered the same as the FFC/FPC cable connector)
    :header: "cable pin number",Label,"MCU pin","RPi pin",Description

    1,SCK,SCK,"SCLK (GPIO11)","SPI clock line"
    2,SO,MISO,"MISO (GPIO9)","SPI Master Input Slave Output"
    3,SS,D2,"CE0 (GPIO8)","Slave Select (AKA Chip Select)"
    4,DR,D7,GPIO25,"Data Ready interrupt"
    5,SI,MOSI,"MOSI (GPIO10)","SPI Master Output Slave Input"
    6,B2,N/A,N/A,"Hardware input button #2"
    7,B3,N/A,N/A,"Hardware input button #3"
    8,B1,N/A,N/A,"Hardware input button #1"
    9,SCL,SCL,"SCL (GPIO3)","I2C clock line (no builtin pull-up resistor)"
    10,SDA,SDA,"SDA (GPIO2)","I2C data line (no builtin pull-up resistor)"
    11,GND,GND,Ground,Ground
    12,VDD,3V,3V3,"3V power supply"

.. tip::
    Of course, you can capture button data manually (if your application utilizes more
    than 3 buttons), but if you connect the pins B1, B2, B3 to momentary push buttons that
    (when pressed) provide a path to ground, the Pinnacle touch controller will report all 3
    buttons' states for each touch (or even button only) events.

.. note::
    These trackpads have no builtin pull-up resistors on the I2C bus' SDA and SCL lines.
    Examples were tested with a 4.7K ohm resistor for each I2C line tied to 3v.

    The Raspberry Pi boards (excluding any RP2040 boards) all have builtin 1.8K ohm pull-up
    resistors, so the Linux examples were tested with no addition pull-up resistance.

.. _HCO:

Model Labeling Scheme
---------------------

TM\ ``yyyxxx``\ -202\ ``i``\ -\ ``cc``\ ``o``

- ``yyyxxx`` stands for the respective vertical & horizontal width of the trackpad in millimeters.
- ``i`` stands for the hardwired interface protocol (3 = I2C, 4 = SPI). Notice, if there is a
  resistor populated at the R1 (470K ohm) junction (located just above the Pinnacle ASIC), it
  is configured for SPI, otherwise it is configured for I2C.
- ``cc`` stands for Custom Configuration which describes if a 470K ohm resistor is populated at
  junction R4. "30" (resistor at R4 exists) means that the hardware is configured to disable
  certain features despite what this library does. "00" (no resistor at R4) means that the
  hardware is configured to allow certain features to be manipulated by this library. These
  features include "secondary tap" (thought of as "right mouse button" in relative data mode),
  Intellimouse scrolling (Microsoft patented scroll wheel behavior -- a throw back to when
  scroll wheels were first introduced), and 180 degree orientation (your application can invert
  the axis data anyway).
- ``o`` stands for the overlay type (0 = none, 1 = adhesive, 2 = flat, 3 = curved)

Sphinx documentation
-----------------------

Sphinx is used to build the documentation. First, install dependencies (Python 3 & Python's pip
is required):

.. code-block:: shell

    pip3 install -r docs/requirements.txt

Now, run sphinx. The following command is executed from the repository's root folder.

.. code-block:: shell

    sphinx-build -E -W docs docs/_build/html

This will output the documentation to ``docs/_build/html``. Open the index.html in your browser to
view them. It will also (due to ``-W``) error out on any warning. This is a good way to locally verify
it will pass the CI workflow (and ReadTheDocs builds).

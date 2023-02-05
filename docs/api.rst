
PinnacleTouch API
==================

Datatypes for data reports
--------------------------

Relative Data
**************

.. seealso:: `Relative Mode example <examples.html#relative-mode>`_

.. doxygenstruct:: RelativeReport
    :members:

Absolute Data
**************

.. seealso:: `Absolute Mode example <examples.html#absolute-mode>`_.

.. doxygenstruct:: AbsoluteReport
    :members:

Accepted Constants
------------------

Data Modes
***********

.. doxygenenum:: PinnacleDataMode

AnyMeas mode Gain
******************

.. doxygenenum:: PinnacleAnyMeasGain

AnyMeas mode Frequencies
************************

.. doxygenenum:: PinnacleAnyMeasFreq

AnyMeas mode Muxing
*******************

.. doxygenenum:: PinnacleAnyMeasMuxing

AnyMeas mode Control
********************

.. doxygenenum:: PinnacleAnyMeasCtrl

PinnacleTouch
-------------

.. doxygenclass:: PinnacleTouch
    :members: PinnacleTouch
    :protected-members:

Data Mode
************************

.. doxygenfunction:: setDataMode

.. doxygenfunction:: getDataMode

Relative or Absolute mode
*************************

.. doxygenfunction:: feedEnabled

.. doxygenfunction:: isFeedEnabled

.. doxygenfunction:: isHardConfigured

.. doxygenfunction:: available

.. doxygenfunction:: absoluteModeConfig

.. doxygenfunction:: relativeModeConfig

.. doxygenfunction:: read(RelativeReport *report)

.. doxygenfunction:: read(AbsoluteReport *report)

.. doxygenfunction:: clearStatusFlags

.. doxygenfunction:: allowSleep

.. doxygenfunction:: isAllowSleep

.. doxygenfunction:: shutdown

.. doxygenfunction:: isShutdown

.. doxygenfunction:: setSampleRate

.. doxygenfunction:: getSampleRate

.. doxygenfunction:: detectFingerStylus

.. doxygenfunction:: calibrate

.. doxygenfunction:: setCalibrationMatrix

.. doxygenfunction:: getCalibrationMatrix

.. doxygenfunction:: setAdcGain

.. doxygenfunction:: tuneEdgeSensitivity

AnyMeas Mode
************************

.. doxygenfunction:: anyMeasModeConfig

.. doxygenfunction:: measureAdc

.. doxygenfunction:: startMeasureAdc

.. doxygenfunction:: getMeasureAdc

SPI & I2C Interfaces
------------------------

.. doxygenclass:: PinnacleTouchSPI
    :members:

.. doxygenclass:: PinnacleTouchI2C
    :members:

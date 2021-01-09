
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

setDataMode
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: setDataMode

getDataMode
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: getDataMode

Relative or Absolute mode
*************************

feedEnabled
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: feedEnabled

isFeedEnabled
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: isFeedEnabled

isHardConfigured
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: isHardConfigured

available
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: available

absoluteModeConfig
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: absoluteModeConfig

relativeModeConfig
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: relativeModeConfig

reportRelative
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: reportRelative

reportAbsolute
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: reportAbsolute

clearStatusFlags
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: clearStatusFlags

allowSleep
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: allowSleep

isAllowSleep
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: isAllowSleep

shutdown
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: shutdown

isShutdown
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: isShutdown

setSampleRate
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: setSampleRate

getSampleRate
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: getSampleRate

detectFingerStylus
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: detectFingerStylus

calibrate
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: calibrate

setCalibrationMatrix
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: setCalibrationMatrix

getCalibrationMatrix
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: getCalibrationMatrix

setAdcGain
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: setAdcGain

tuneEdgeSensitivity
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: tuneEdgeSensitivity

AnyMeas Mode
************************

anyMeasModeConfig
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: anyMeasModeConfig

measureAdc
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: measureAdc

startMeasureAdc
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: startMeasureAdc

getMeasureAdc
^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: getMeasureAdc

SPI & I2C Interfaces
------------------------

.. doxygenclass:: PinnacleTouchSPI
    :members:

.. doxygenclass:: PinnacleTouchI2C
    :members:

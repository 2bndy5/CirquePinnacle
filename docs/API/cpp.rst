
C++ API
==================

.. _Data Mode:

Data Mode
***********

.. cpp-apigen-group:: data-mode

Relative Data
**************

.. cpp-apigen-group:: relative-mode

Absolute Data
**************

.. cpp-apigen-group:: absolute-mode

AnyMeas Constants
*****************

.. cpp-apigen-group:: anymeas-const

PinnacleTouch API
*****************

.. cpp-apigen-group:: pinnacle-touch-api

..
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

    .. doxygenfunction:: PinnacleTouch::read(RelativeReport *report)

    .. doxygenfunction:: PinnacleTouch::read(AbsoluteReport *report)

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

    .. doxygenfunction:: anymeasModeConfig

    .. doxygenfunction:: measureAdc

    .. doxygenfunction:: startMeasureAdc

    .. doxygenfunction:: getMeasureAdc

/*
 * Copyright (c) 2023 Brendan Doherty (2bndy5)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/** Python bindings code for the CirquePinnacle C++ lib */
#ifndef ARDUINO
    #include <pybind11/pybind11.h>
    #include <pybind11/stl.h>
    #include "CirquePinnacle.h"

namespace py = pybind11;
namespace arduino = cirque_pinnacle_arduino_wrappers;

/**
 * PinnacleTouch has pure virtual functions used to abstract SPI & I2C interfaces.
 * To extend this functionality to python, we must create a wrapping class that
 * overrides the pure virtual functions with "placeholders" via PYBIND11_OVERRIDE_PURE.
 */
class PyPinnacleTouch : public PinnacleTouch
{
public:
    using PinnacleTouch::PinnacleTouch; // inherit c'tors

protected:
    void rapWriteCmd(uint8_t* sequence, uint8_t length) override
    {
        PYBIND11_OVERRIDE_PURE(
            void, PinnacleTouch, rapWriteCmd, sequence, length);
    }
    void rapWrite(uint8_t registerAddress, uint8_t registerValue) override
    {
        PYBIND11_OVERRIDE_PURE(
            void, PinnacleTouch, rapWrite, registerAddress, registerValue);
    }
    void rapWriteBytes(uint8_t registerAddress, uint8_t* registerValues, uint8_t registerCount) override
    {
        PYBIND11_OVERRIDE_PURE(
            void, PinnacleTouch, rapWriteBytes, registerAddress, registerValues, registerCount);
    }
    void rapRead(uint8_t registerAddress, uint8_t* data) override
    {
        PYBIND11_OVERRIDE_PURE(
            void, PinnacleTouch, rapRead, registerAddress, data);
    }
    void rapReadBytes(uint8_t registerAddress, uint8_t* data, uint8_t registerCount) override
    {
        PYBIND11_OVERRIDE_PURE(
            void, PinnacleTouch, rapReadBytes, registerAddress, data, registerCount);
    }
};

/**
 * Certain functions of the PinnacleTouch API use buffers passed by reference.
 * We must re-implement these functions globally with pybind11-specific data types because
 * the buffer parameters are not automatically casted properly by pybind11 machinery.
 */
py::list getCalibrationMatrix_wrapper(PinnacleTouch* self)
{
    int16_t* buff = new int16_t[46];
    self->getCalibrationMatrix(buff);
    py::list matrix = py::list(0);
    for (uint8_t i = 0; i < 46; ++i) {
        matrix.append(py::int_(buff[i]));
    }
    delete buff;
    return matrix;
};

void setCalibrationMatrix_wrapper(PinnacleTouch* self, py::list& matrix)
{
    ssize_t length = py::len(matrix);
    int16_t* buff = new int16_t[length];
    for (uint8_t i = 0; i < length; ++i) {
        buff[i] = py::cast<int16_t>(matrix[i]);
    }
    self->setCalibrationMatrix(buff, length);
    delete buff;
};

/**
 * All of CirquePinnacle.h will be exposed in 1 python module named :py:mod:`cirque_pinnacle`.
 */
PYBIND11_MODULE(cirque_pinnacle, m)
{
    m.attr("PINNACLE_SW_DR") = PINNACLE_SW_DR;

    // ******************** expose PinnacleDataMode
    py::enum_<PinnacleDataMode> dataMode(m, "PinnacleDataMode");
    dataMode.value("PINNACLE_RELATIVE", PINNACLE_RELATIVE);
    #ifdef PINNACLE_ANYMEAS_SUPPORT
    dataMode.value("PINNACLE_ANYMEAS", PINNACLE_ANYMEAS);
    #endif
    dataMode.value("PINNACLE_ABSOLUTE", PINNACLE_ABSOLUTE);
    dataMode.value("PINNACLE_ERROR", PINNACLE_ERROR);
    dataMode.export_values();

    #ifdef PINNACLE_ANYMEAS_SUPPORT

    // ******************** expose PinnacleAnyMeasGain
    py::enum_<PinnacleAnyMeasGain> anyMeasGain(m, "PinnacleAnyMeasGain");
    anyMeasGain.value("PINNACLE_GAIN_100", PINNACLE_GAIN_100);
    anyMeasGain.value("PINNACLE_GAIN_133", PINNACLE_GAIN_133);
    anyMeasGain.value("PINNACLE_GAIN_166", PINNACLE_GAIN_166);
    anyMeasGain.value("PINNACLE_GAIN_200", PINNACLE_GAIN_200);
    anyMeasGain.export_values();

    // ******************** expose PinnacleAnyMeasFreq
    py::enum_<PinnacleAnyMeasFreq> anyMeasFreq(m, "PinnacleAnyMeasFreq");
    anyMeasFreq.value("PINNACLE_FREQ_0", PINNACLE_FREQ_0);
    anyMeasFreq.value("PINNACLE_FREQ_1", PINNACLE_FREQ_1);
    anyMeasFreq.value("PINNACLE_FREQ_2", PINNACLE_FREQ_2);
    anyMeasFreq.value("PINNACLE_FREQ_3", PINNACLE_FREQ_3);
    anyMeasFreq.value("PINNACLE_FREQ_4", PINNACLE_FREQ_4);
    anyMeasFreq.value("PINNACLE_FREQ_5", PINNACLE_FREQ_5);
    anyMeasFreq.value("PINNACLE_FREQ_6", PINNACLE_FREQ_6);
    anyMeasFreq.value("PINNACLE_FREQ_7", PINNACLE_FREQ_7);
    anyMeasFreq.export_values();

    // ******************** expose PinnacleAnyMeasMuxing
    py::enum_<PinnacleAnyMeasMuxing> anyMeasMux(m, "PinnacleAnyMeasMuxing");
    anyMeasMux.value("PINNACLE_MUX_NPN", PINNACLE_MUX_NPN);
    anyMeasMux.value("PINNACLE_MUX_PNP", PINNACLE_MUX_PNP);
    anyMeasMux.value("PINNACLE_MUX_REF0", PINNACLE_MUX_REF0);
    anyMeasMux.value("PINNACLE_MUX_REF1", PINNACLE_MUX_REF1);
    anyMeasMux.export_values();

    // ******************** expose PinnacleAnyMeasCtrl
    py::enum_<PinnacleAnyMeasCtrl> anyMeasCtrl(m, "PinnacleAnyMeasCtrl");
    anyMeasCtrl.value("PINNACLE_CRTL_PWR_IDLE", PINNACLE_CRTL_PWR_IDLE);
    anyMeasCtrl.value("PINNACLE_CRTL_REPEAT", PINNACLE_CRTL_REPEAT);
    anyMeasCtrl.export_values();

    #endif // !defined(PINNACLE_ANYMEAS_SUPPORT)

    // ******************** bindings for RelativeReport
    py::class_<RelativeReport> relativeReports(m, "RelativeReport");
    relativeReports.def(py::init<>());
    relativeReports.def_readwrite("buttons", &RelativeReport::buttons);
    relativeReports.def_readwrite("x", &RelativeReport::x);
    relativeReports.def_readwrite("y", &RelativeReport::y);
    relativeReports.def_readwrite("scroll", &RelativeReport::scroll);
    relativeReports.def("__repr__", [](const RelativeReport& obj) {
        char* buf = new char[73];
        uint8_t used = sprintf(
            buf, "<RelativeReport Left: %d Right: %d Middle: %d X: %d Y: %d Scroll: %d>",
            obj.buttons & 1,
            obj.buttons & 2,
            obj.buttons & 4,
            obj.x,
            obj.y,
            obj.scroll);
        buf[used] = 0;
        std::string ret = std::string(buf);
        delete[] buf;
        return ret;
    });
    relativeReports.def_property_readonly("buffer", [](const RelativeReport& obj) {
        uint8_t* buf = new uint8_t[4];
        buf[0] = obj.buttons;
        buf[1] = obj.x;
        buf[2] = obj.y;
        buf[3] = obj.scroll;
        py::bytes ret = py::bytes(reinterpret_cast<char*>(buf), 4);
        delete[] buf;
        return ret;
    });

    // ******************** bindings for AbsoluteReports
    py::class_<AbsoluteReport> absoluteReports(m, "AbsoluteReport");
    absoluteReports.def(py::init<>());
    absoluteReports.def_readwrite("buttons", &AbsoluteReport::buttons);
    absoluteReports.def_readwrite("x", &AbsoluteReport::x);
    absoluteReports.def_readwrite("y", &AbsoluteReport::y);
    absoluteReports.def_readwrite("z", &AbsoluteReport::z);
    absoluteReports.def("__repr__", [](const AbsoluteReport& obj) {
        char* buf = new char[57];
        uint8_t used = sprintf(
            buf, "<AbsoluteReport B1: %d B2: %d B3: %d X: %d Y: %d Z: %d>",
            obj.buttons & 1,
            obj.buttons & 2,
            obj.buttons & 4,
            obj.x,
            obj.y,
            obj.z);
        buf[used] = 0;
        std::string ret = std::string(buf);
        delete[] buf;
        return ret;
    });

    // ******************** bindings for PinnacleTouch
    py::class_<PinnacleTouch, PyPinnacleTouch> pinnacleTouch(m, "PinnacleTouch");
    pinnacleTouch.def(py::init<pinnacle_gpio_t>(), py::arg("dataReadyPin"));
    pinnacleTouch.def_property("data_mode", &PinnacleTouch::getDataMode, &PinnacleTouch::setDataMode);
    pinnacleTouch.def("getDataMode", &PinnacleTouch::getDataMode);
    pinnacleTouch.def("setDataMode", &PinnacleTouch::setDataMode);
    pinnacleTouch.def_property("feed_enabled", &PinnacleTouch::isFeedEnabled, &PinnacleTouch::feedEnabled);
    pinnacleTouch.def("isFeedEnabled", &PinnacleTouch::isFeedEnabled);
    pinnacleTouch.def("feedEnabled", &PinnacleTouch::feedEnabled);
    pinnacleTouch.def_property_readonly("is_hard_configured", &PinnacleTouch::isHardConfigured);
    pinnacleTouch.def("isHardConfigured", &PinnacleTouch::isHardConfigured);
    pinnacleTouch.def("available", &PinnacleTouch::available);
    pinnacleTouch.def("absolute_mode_config", &PinnacleTouch::absoluteModeConfig,
                      py::arg("z_idle_count") = 30, py::arg("invert_x") = false, py::arg("invert_x") = false);
    pinnacleTouch.def("absoluteModeConfig", &PinnacleTouch::absoluteModeConfig,
                      py::arg("z_idle_count") = 30, py::arg("invert_x") = false, py::arg("invert_x") = false);
    pinnacleTouch.def("relative_mode_configure", &PinnacleTouch::relativeModeConfig,
                      py::arg("taps") = true, py::arg("rotate90") = false, py::arg("secondary_tap") = true,
                      py::arg("glide_extend") = false, py::arg("intellimouse") = false);
    pinnacleTouch.def("relativeModeConfig", &PinnacleTouch::relativeModeConfig,
                      py::arg("taps") = true, py::arg("rotate90") = false, py::arg("secondary_tap") = true,
                      py::arg("glide_extend") = false, py::arg("intellimouse") = false);
    pinnacleTouch.def("read", static_cast<void (PinnacleTouch::*)(AbsoluteReport*, bool)>(&PinnacleTouch::read), py::arg("report"), py::arg("read_buttons") = true);
    pinnacleTouch.def("read", static_cast<void (PinnacleTouch::*)(RelativeReport*, bool)>(&PinnacleTouch::read), py::arg("report"), py::arg("read_buttons") = true);
    pinnacleTouch.def("clear_status_flags", &PinnacleTouch::clearStatusFlags);
    pinnacleTouch.def("clearStatusFlags", &PinnacleTouch::clearStatusFlags);
    pinnacleTouch.def_property("allow_sleep", &PinnacleTouch::isAllowSleep, &PinnacleTouch::allowSleep);
    pinnacleTouch.def("isAllowSleep", &PinnacleTouch::isAllowSleep);
    pinnacleTouch.def("allowSleep", &PinnacleTouch::allowSleep);
    pinnacleTouch.def_property("shutdown", &PinnacleTouch::isShutdown, &PinnacleTouch::shutdown);
    pinnacleTouch.def_property("sample_rate", &PinnacleTouch::getSampleRate, &PinnacleTouch::setSampleRate);
    pinnacleTouch.def("getSampleRate", &PinnacleTouch::getSampleRate);
    pinnacleTouch.def("setSampleRate", &PinnacleTouch::setSampleRate);
    pinnacleTouch.def("detect_finger_stylus", &PinnacleTouch::detectFingerStylus,
                      py::arg("enable_finger") = true, py::arg("enable_stylus") = true, py::arg("sample_rate") = 100);
    pinnacleTouch.def("detectFingerStylus", &PinnacleTouch::detectFingerStylus,
                      py::arg("enable_finger") = true, py::arg("enable_stylus") = true, py::arg("sample_rate") = 100);
    pinnacleTouch.def("calibrate", &PinnacleTouch::calibrate,
                      py::arg("run") = true, py::arg("tap") = true, py::arg("track_error") = true,
                      py::arg("nerd") = true, py::arg("background") = true);
    pinnacleTouch.def_property("calibration_matrix", &getCalibrationMatrix_wrapper, &setCalibrationMatrix_wrapper);
    pinnacleTouch.def("getCalibrationMatrix", &getCalibrationMatrix_wrapper);
    pinnacleTouch.def("setCalibrationMatrix", &setCalibrationMatrix_wrapper, py::arg("matrix"));
    pinnacleTouch.def("tune_edge_sensitivity", &PinnacleTouch::tuneEdgeSensitivity,
                      py::arg("x_axis_wide_z_min") = 4, py::arg("y_axis_wide_z_min") = 3);
    pinnacleTouch.def("tuneEdgeSensitivity", &PinnacleTouch::tuneEdgeSensitivity,
                      py::arg("x_axis_wide_z_min") = 4, py::arg("y_axis_wide_z_min") = 3);
    pinnacleTouch.def("set_adc_gain", &PinnacleTouch::setAdcGain, py::arg("sensitivity"));
    pinnacleTouch.def("setAdcGain", &PinnacleTouch::setAdcGain, py::arg("sensitivity"));

    #if PINNACLE_ANYMEAS_SUPPORT

    pinnacleTouch.def("anymeas_mode_config", &PinnacleTouch::anymeasModeConfig,
                      py::arg("gain") = PINNACLE_GAIN_200, py::arg("frequency") = PINNACLE_FREQ_0, py::arg("sample_length") = 512,
                      py::arg("mux_control") = PINNACLE_MUX_PNP, py::arg("aperture_width") = 500, py::arg("control_power_count") = 1);
    pinnacleTouch.def("anymeasModeConfig", &PinnacleTouch::anymeasModeConfig,
                      py::arg("gain") = PINNACLE_GAIN_200, py::arg("frequency") = PINNACLE_FREQ_0, py::arg("sample_length") = 512,
                      py::arg("mux_control") = PINNACLE_MUX_PNP, py::arg("aperture_width") = 500, py::arg("control_power_count") = 1);
    pinnacleTouch.def("measure_adc", &PinnacleTouch::measureAdc, py::arg("bits_to_toggle"), py::arg("toggle_polarity"));
    pinnacleTouch.def("measureAdc", &PinnacleTouch::measureAdc, py::arg("bits_to_toggle"), py::arg("toggle_polarity"));
    pinnacleTouch.def("start_measure_adc", &PinnacleTouch::startMeasureAdc, py::arg("bits_to_toggle"), py::arg("toggle_polarity"));
    pinnacleTouch.def("get_measure_adc", &PinnacleTouch::getMeasureAdc);
    pinnacleTouch.def("startMeasureAdc", &PinnacleTouch::startMeasureAdc, py::arg("bits_to_toggle"), py::arg("toggle_polarity"));
    pinnacleTouch.def("getMeasureAdc", &PinnacleTouch::getMeasureAdc);

    #endif // PINNACLE_ANYMEAS_SUPPORT == true

    // ******************** bindings for PinnacleTouchSPI
    py::class_<PinnacleTouchSPI> pinnacleTouchSPI(m, "PinnacleTouchSPI", pinnacleTouch);
    pinnacleTouchSPI.def(py::init<pinnacle_gpio_t, pinnacle_gpio_t, uint32_t>(), py::arg("dataReadyPin"), py::arg("slaveSelectPin"), py::arg("spiSpeed") = PINNACLE_SPI_SPEED);
    pinnacleTouchSPI.def("begin", static_cast<bool (PinnacleTouchSPI::*)(void)>(&PinnacleTouchSPI::begin));
    // The overloaded begin(pinnacle_spi_t*) is not exposed since it would require binding the driver-specific implementation (a lot of work).
    // Additionally, begin(pinnacle_spi_t*) isn't needed on Linux because the SS_PIN param to the c'tor specifies both bus and CEx numbers.

    // ******************* bindings for TwoWire
    py::class_<arduino::TwoWire> twoWire(m, "TwoWire");
    twoWire.def(py::init<>());
    twoWire.def("begin", &arduino::TwoWire::begin, py::arg("busNumber") = 1);
    twoWire.def("end", &arduino::TwoWire::end);
    twoWire.def("beginTransmission", &arduino::TwoWire::beginTransmission, py::arg("address"));
    twoWire.def("endTransmission", &arduino::TwoWire::endTransmission, py::arg("sendStop") = true);
    twoWire.def("write", &arduino::TwoWire::write, py::arg("data"));
    twoWire.def("requestFrom", &arduino::TwoWire::requestFrom, py::arg("address"), py::arg("quantity"), py::arg("sendStop") = true);
    twoWire.def("available", &arduino::TwoWire::available);
    twoWire.def("read", &arduino::TwoWire::read);

    // ******************** bindings for PinnacleTouchI2C
    py::class_<PinnacleTouchI2C> pinnacleTouchI2C(m, "PinnacleTouchI2C", pinnacleTouch);
    pinnacleTouchI2C.def(py::init<pinnacle_gpio_t, uint8_t>(), py::arg("dataReadyPin"), py::arg("slaveAddress") = 0x2A);
    pinnacleTouchI2C.def("begin", static_cast<bool (PinnacleTouchI2C::*)(void)>(&PinnacleTouchI2C::begin));
    pinnacleTouchI2C.def("begin", static_cast<bool (PinnacleTouchI2C::*)(arduino::TwoWire*)>(&PinnacleTouchI2C::begin), py::arg("i2c_bus"));
}

#endif // !defined(ARDUINO)

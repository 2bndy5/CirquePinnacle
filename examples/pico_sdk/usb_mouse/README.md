# USB Mouse Example

This example was copied and modified from the example in the
[pico-examples/usb/device/dev_hid_composite](https://github.com/raspberrypi/pico-examples/tree/master/usb/device/dev_hid_composite)
(which is a modified copy of the original TinyUSB repo example).

## Modifications

Instead of using the board's builtin button, this example uses the Cirque circle trackpad as user input. All modifications are marked or preceded by a comment like:

```c
// MODIFIED for using the CirquePinnacle library
```

## Known issues

According to RP2040 datasheet (in Appendix B: Errata), "RP2040-E5", the board can fail to enumerate as a USB device. To fix this, uncomment a line in this example's CMakeLists.txt file:

```cmake
# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(dev_hid_composite PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)
```

Note, that GPIO15 should not be claimed in user code for this fix to work. This is usually required if multiple USB devices are connected, including the RP2040 board(s).

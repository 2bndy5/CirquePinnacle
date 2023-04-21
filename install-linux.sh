#!/bin/bash
args=("$@")
driver=""
USE_I2C="OFF"

if [[ ${#args[@]} -gt 0 ]]
then
    if [[ ${args[0]} == "linux_kernel" ]] || [[ ${args[0]} == "bcm2xxx" ]] || [[ ${args[0]} == "pigpio" ]] || [[ ${args[0]} == "mraa" ]]
    then
        driver=${args[0]}
    fi
fi

# verify git is installed
if ! command -v git &> /dev/null
then
    echo "*** Installing git from apt-get"$'\n'
    sudo apt-get install git
fi

# verify CMake is installed
if ! command -v cmake &> /dev/null
then
    echo "*** Installing cmake from apt-get"$'\n'
    sudo apt-get install cmake
fi

# set utility driver (if not set from CLI arg)
if [ -z $driver ]
then
    echo "*** Which hardware driver library do you wish to use? ***"
    echo "1. linux_kernel (default)"
    echo $'    compatible with most Linux distributions\n    fastest\n    sudo permission not needed for executables'
    echo "2. bcm2xxx"
    echo $'    only for RPi\n    a bit slow\n    sudo permission needed for executables'
    echo "3. pigpio"
    echo $'    requires PiGPIO lib installed\n    only for RPi\n    a bit slow\n    sudo permission needed for executables'
    echo "4. mraa"
    echo $'    requires MRAA lib installed\n    sudo permission not needed for executables'
    read -p "> " answer
    case ${answer^^} in
        1) driver="linux_kernel";;
        2) driver="bcm2xxx";;
        3) driver="pigpio";;
        4) driver="mraa";;
        *) driver="linux_kernel";;
    esac
fi

# set an env var for easier reuse (specific to RF24 repos).
# Any applicable CMakeLists.txt is configured to use this when it is set.
export PINNACLE_DRIVER=$driver


# Ensures we have a fresh build directory
# NOTE: navigate to source dir before entering this function
create_build_env() {
    if [[ -d "./build" ]]
    then
        echo "Purging build environment."$'\n'
        sudo rm -r build/
    fi
    mkdir build
    cd build
}

uninstall_lib() {
    if [[ -f "/usr/local/lib/libcirquepinnacle.so" ]]
    then
        echo $'\n*** Uninstalling older version of CirquePinnacle\n'
        sudo rm /usr/local/lib/libcirquepinnacle.so*
        sudo rm -r /usr/local/include/CirquePinnacle/
    fi
}

install_lib() {
    echo $'\n\n*** Installing CirquePinnacle library\n'
    create_build_env
    if ! cmake ../src
    then
        echo "!!! CMake failed to configure build system. Quitting now."
        exit 1
    fi
    if ! make
    then
        echo "!!! Building lib failed. Quitting now."
        exit 1
    fi
    uninstall_lib
    if ! sudo make install
    then
        echo "!!! Installing lib failed. Quitting now."
        exit 1
    fi
    # go back to repo root
    cd ../
}

build_examples() {
    echo $'\n\n*** Building CirquePinnacle examples\n'
    cd ./examples/linux
    create_build_env
    if ! cmake .. -DUSE_I2C=$USE_I2C
    then
        echo "!!! CMake failed to configure build system. Quitting now."
        exit 1
    fi
    if ! make
    then
        echo "!!! Building examples failed. Quitting now."
        exit 1
    fi
    # go back to repo root
    cd ../../../
}

install_python_binding() {
    echo $'\n\n*** Installing cirque-pinnacle python library\n'
    create_build_env
    # go back to repo root
    cd ../
    # ensure pybind11 submodule is present
    if [[ ! -f "src/pybind11/CMakeLists.txt" ]]
    then
        echo $'\n*** Initializing frozen submodules\n'
        git submodule update --init
    fi
    # ensure there are no previous wheels in the dist/ folder
    if [[ -d "dist" ]]
    then
        rm -r dist/
    fi
    if ! python3 -m pip wheel -w dist --no-deps -v .
    then
        echo "!!! Building Python distributable wheel failed. Quitting now."
        exit 1
    fi
    python3 -m pip install dist/cirque_pinnacle*.whl
}

INSTALL_LIB="N"
read -p "*** Install CirquePinnacle lib? [y/N] " INSTALL_LIB
BUILD_EXAMPLES="N"
read -p "*** Build CirquePinnacle examples? [y/N] " BUILD_EXAMPLES
if [[ ${BUILD_EXAMPLES^^} == "Y" ]]
then
    read -p "    Make examples use I2C (instead of SPI)? [y/N] " answer
    case ${answer^^} in
        Y ) USE_I2C="ON";;
    esac
fi
INSTALL_PY_BINDING="N"
read -p "*** Build/Install the python package (cirque-pinnacle) from source? [y/N] " INSTALL_PY_BINDING

case ${INSTALL_LIB^^} in
    Y ) install_lib;;
esac

case ${BUILD_EXAMPLES^^} in
    Y ) build_examples;;
esac

case ${INSTALL_PY_BINDING^^} in
    Y ) install_python_binding;;
esac

echo $'\n\n*** Done'
echo "*** See https://cirquepinnacle.rtfd.io for documentation"

# clean up env var
unset PINNACLE_DRIVER

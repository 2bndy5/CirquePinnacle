# Check if we are building a pico-sdk based project
# (or more exactly: if we just got included in a pico-sdk based project)
if (PICO_SDK_PATH)
    # If so, load the relevant CMakeLists-file but don't do anything else
    include(${CMAKE_CURRENT_LIST_DIR}/utility/rp2/CMakeLists.txt)
    return()
endif()

cmake_minimum_required(VERSION 3.15)

# Set the project name to your project name
project(cirque_pinnacle C CXX)

# optionally enable building the python binding
option(PINNACLE_PY_BINDING OFF) # enabled via setup.py

include(${CMAKE_CURRENT_LIST_DIR}/cmake/StandardProjectSettings.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/PreventInSourceBuilds.cmake)

# get library info from Arduino IDE's required library.properties file
include(${CMAKE_CURRENT_LIST_DIR}/cmake/GetLibInfo.cmake)

if(NOT PINNACLE_PY_BINDING)
    # Link this 'library' to set the c++ standard / compile-time options requested
    add_library(${LibTargetName}_project_options INTERFACE)
    target_compile_features(${LibTargetName}_project_options INTERFACE cxx_std_17)

    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
        if(ENABLE_BUILD_WITH_TIME_TRACE)
            add_compile_definitions(${LibTargetName}_project_options INTERFACE -ftime-trace)
        endif()
    endif()

    # Link this 'library' to use the warnings specified in CompilerWarnings.cmake
    add_library(${LibTargetName}_project_warnings INTERFACE)

    # enable cache system
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/Cache.cmake)

    # standard compiler warnings
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/CompilerWarnings.cmake)
    set_project_warnings(${LibTargetName}_project_warnings)

    # sanitizer options if supported by compiler
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/Sanitizers.cmake)
    enable_sanitizers(${LibTargetName}_project_options)

    # allow for static analysis options
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/StaticAnalyzers.cmake)

    option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
    option(ENABLE_TESTING "Enable Test Builds" OFF) # for end-user projects
    option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF) # for end-user projects

    if(ENABLE_TESTING)
        enable_testing()
        message("Building Tests.")
        add_subdirectory(test) # directory doesn't exist, so this does nothing.
    endif()

    if(ENABLE_FUZZING)
        message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
        add_subdirectory(fuzz_test) # directory doesn't exist, so this does nothing.
    endif()
endif()

#####################################
### Now we actually build the library
#####################################

# detect the CPU make and type
include(${CMAKE_CURRENT_LIST_DIR}/cmake/detectCPU.cmake) # sets the variable SOC accordingly

# auto-detect what driver to use
# auto-detect can be overriden using `cmake .. -D CIRQUE_PINNACLE_DRIVER=<supported driver>`
include(${CMAKE_CURRENT_LIST_DIR}/cmake/auto_config_driver.cmake)

#[[ adding the utility sub-directory will
    1. set variables CIRQUE_PINNACLE_LINKED_DRIVER and CIRQUE_PINNACLE_DRIVER_SOURCES
    2. set additional install rules according to the CIRQUE_PINNACLE_DRIVER specified
]]
add_subdirectory(utility)

if(NOT PINNACLE_PY_BINDING)
    add_library(${LibTargetName} SHARED
        CirquePinnacle.cpp
        utility/includes.h
        ${CIRQUE_PINNACLE_DRIVER_SOURCES}
    )

    target_compile_options(${LibTargetName} PUBLIC -Ofast -Wall -pthread)

    target_include_directories(${LibTargetName} PUBLIC utility)

    set_target_properties(
        ${LibTargetName}
        PROPERTIES
        SOVERSION ${${LibName}_VERSION_MAJOR}
        VERSION ${${LibName}_VERSION_STRING}
    )

    if(DEFINED CIRQUE_PINNACLE_SPI_SPEED)
        message(STATUS "CIRQUE_PINNACLE_SPI_SPEED set to ${CIRQUE_PINNACLE_SPI_SPEED}")
        target_compile_definitions(${LibTargetName} PUBLIC
            CIRQUE_PINNACLE_SPI_SPEED=${CIRQUE_PINNACLE_SPI_SPEED}
        )
    endif()

    if(NOT "${CIRQUE_PINNACLE_LINKED_DRIVER}" STREQUAL "") # linking to a pre-compiled utility driver
        message(STATUS "Using utility library: ${CIRQUE_PINNACLE_LINKED_DRIVER}")
        target_link_libraries(${LibTargetName} INTERFACE
            ${LibTargetName}_project_options
            ${LibTargetName}_project_warnings
            STATIC ${CIRQUE_PINNACLE_LINKED_DRIVER}
        )
    else() # utility driver is compiled with the library - not linking to a pre-compiled utility driver
        target_link_libraries(${LibTargetName} INTERFACE
            ${LibTargetName}_project_options
            ${LibTargetName}_project_warnings
        )
    endif()

    #####################################
    ### Install rules for root source dir
    ### There are separate install rules defined for each utility driver
    ### Installing the library requires sudo privileges
    #####################################

    # setup CPack options
    # package dependencies are resolved correctly only after utility subdirectory is added
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/CPackInfo.cmake)

    install(TARGETS ${LibTargetName}
        DESTINATION lib
    )

    install(FILES
            ${CMAKE_CURRENT_LIST_DIR}/CirquePinnacle.h
            ${CMAKE_CURRENT_LIST_DIR}/CirquePinnacle_arch_common.h
        DESTINATION include/CirquePinnacle
    )

    install(FILES
            ${CMAKE_CURRENT_LIST_DIR}/utility/includes.h
        DESTINATION include/CirquePinnacle/utility
    )

    # CMAKE_CROSSCOMPILING is only TRUE when CMAKE_TOOLCHAIN_FILE is specified via CLI
    if("${CMAKE_CROSSCOMPILING}" STREQUAL "FALSE")
        install(CODE "message(STATUS \"Updating ldconfig\")")
        install(CODE "execute_process(COMMAND ldconfig)")
    endif()
else()
    add_subdirectory(pybind11)
    pybind11_add_module(cirque_pinnacle 
        py_bindings.cpp
        CirquePinnacle.h
        CirquePinnacle_arch_common.h
        CirquePinnacle.cpp
        utility/includes.h
        ${CIRQUE_PINNACLE_DRIVER_SOURCES}
    )

    if(DEFINED CIRQUE_PINNACLE_SPI_SPEED)
        message(STATUS "CIRQUE_PINNACLE_SPI_SPEED set to ${CIRQUE_PINNACLE_SPI_SPEED}")
        target_compile_definitions(cirque_pinnacle PUBLIC
            CIRQUE_PINNACLE_SPI_SPEED=${CIRQUE_PINNACLE_SPI_SPEED}
        )
    endif()

    target_include_directories(cirque_pinnacle PUBLIC ${CMAKE_CURRENT_LIST_DIR})

    if(NOT "${CIRQUE_PINNACLE_LINKED_DRIVER}" STREQUAL "") # linking to a pre-compiled utility driver
        target_link_libraries(cirque_pinnacle STATIC ${CIRQUE_PINNACLE_LINKED_DRIVER}
        )
    endif()
endif()
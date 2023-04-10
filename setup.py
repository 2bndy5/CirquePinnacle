"""Build & Install script for the CirquePinnacle C++ library python wrappers."""
import os
import platform
import subprocess
import sys
from pathlib import Path
from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

IS_LINUX = platform.system().title() == "Linux"


class CMakeExtension(Extension):
    """A class that wraps the generic mechanism for compiling a python C-extension.

    A CMakeExtension needs a sourcedir instead of a file list.
    The name must be the single output extension from the CMake build.
    """

    def __init__(self, name, sourcedir="", **kwargs):
        kwargs.pop("sources", None)
        Extension.__init__(self, name, sources=[], **kwargs)
        self.sourcedir = str(Path(sourcedir).resolve())


class CMakeBuild(build_ext):
    """A class that wraps the builtin mechanism for building a
    distributable wheel from a compiled c-extension"""

    def build_extension(self, ext: Extension):
        # Must be in this form due to bug in .resolve() only fixed in Python 3.10+
        ext_fullpath = Path.cwd() / self.get_ext_fullpath(ext.name)
        extdir = ext_fullpath.parent.resolve()

        # Using this requires trailing slash for auto-detection & inclusion of
        # auxiliary "native" libs

        debug = int(os.environ.get("DEBUG", 0)) if self.debug is None else self.debug
        cfg = "Debug" if debug else "Release"

        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}{os.sep}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",
        ]
        build_args = []
        for key, value in ext.define_macros:
            macro = f"-D{key}"
            if value:
                macro += f"={value}"
            build_args.append(macro)

        # Adding CMake arguments set as environment variable
        # (needed e.g. to build for ARM OSx on conda-forge).
        if "CMAKE_ARGS" in os.environ:
            for item in os.environ["CMAKE_ARGS"].split(" "):
                if item:
                    cmake_args.append(item)
        # lastly, specify the PINNACLE_DRIVER to configure the build with
        for arg in cmake_args:
            if arg.split("=")[0].endswith("PINNACLE_DRIVER"):
                break
        else:
            driver = os.environ.get("PINNACLE_DRIVER", "linux_kernel")
            cmake_args.append(f"-DPINNACLE_DRIVER={driver}")

        build_temp = Path(self.build_temp, ext.name)
        if not build_temp.exists():
            build_temp.mkdir(parents=True)

        cmake_args.append("-DPINNACLE_PY_BINDING=on")
        subprocess.check_call(["cmake", ext.sourcedir] + cmake_args, cwd=build_temp)
        # for verbose make output:
        # build_args.append("--verbose")
        # build_args.append(f"--target={ext.name}")
        subprocess.check_call(["cmake", "--build", "."] + build_args, cwd=build_temp)


setup_args = dict(
    zip_safe=False,
    packages=["cirque_pinnacle-stubs"],
    package_data={"cirque_pinnacle-stubs": ["__init__.pyi"]},
    package_dir={"cirque_pinnacle-stubs": "src/cirque_pinnacle-stubs"},
)

if IS_LINUX:
    setup_args["cmdclass"] = {"build_ext": CMakeBuild}
    setup_args["ext_modules"] = [CMakeExtension("cirque_pinnacle", sourcedir="src")]

setup(**setup_args)

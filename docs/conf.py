# pylint: disable=invalid-name,too-few-public-methods
"""This file is for `sphinx-build` configuration"""

import os
from pathlib import Path
import sys
import time


sys.path.insert(0, os.path.abspath(".."))

time_t = time.localtime()

# -- Project information -----------------------------------------------------

project = "Cirque Pinnacle"
copyright = f"{time_t.tm_year}, Brendan Doherty"
author = "Brendan Doherty"

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = "latest"
# The full version, including alpha/beta/rc tags.
release = "1.0"

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.todo",
    "sphinx.ext.viewcode",
    "sphinx_immaterial",
    "sphinx_immaterial.apidoc.cpp.cppreference",
    "sphinx_immaterial.apidoc.cpp.apigen",
    "sphinx.ext.intersphinx",
]

intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
}

# Add any paths that contain templates here, relative to this directory.
templates_path = ["_templates"]

source_suffix = ".rst"

# The master toctree document.
master_doc = "index"

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
exclude_patterns = [
    "_build",
    "Thumbs.db",
    ".DS_Store",
    ".env",
    "API/cpp-generated/__gnu_cxx.__alloc_traits.rst",
    "API/cpp-generated/__cxxabiv1.__forced_unwind.rst",
]

# The reST default role (used for this markup: `text`) to use for all
# documents.
#
default_role = "any"

# Tell sphinx what the primary language being documented is.
primary_domain = "cpp"

# Tell sphinx what the pygments highlight language should be.
highlight_language = "cpp"

# -- Options for HTML output
# ----------------------------------------------
repo_url = "https://github.com/2bndy5/CirquePinnacle"

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
html_theme = "sphinx_immaterial"

html_theme_options = {
    "features": [
        "search.share",
        "navigation.top",
        "content.tabs.link",
    ],
    # Set the color and the accent color
    "palette": [
        {
            "media": "(prefers-color-scheme)",
            "toggle": {
                "icon": "material/brightness-auto",
                "name": "Switch to light mode",
            },
        },
        {
            "media": "(prefers-color-scheme: light)",
            "scheme": "default",
            "primary": "green",
            "accent": "light-blue",
            "toggle": {
                "icon": "material/lightbulb-outline",
                "name": "Switch to dark mode",
            },
        },
        {
            "media": "(prefers-color-scheme: dark)",
            "scheme": "slate",
            "primary": "green",
            "accent": "light-blue",
            "toggle": {
                "icon": "material/lightbulb",
                "name": "Switch to light mode",
            },
        },
    ],
    # Set the repo location to get a badge with stats
    "repo_url": repo_url,
    "repo_name": "CirquePinnacle",
    "social": [
        {
            "icon": "fontawesome/brands/github",
            "link": repo_url,
        },
    ],
}

sphinx_immaterial_custom_admonitions = [
    {
        "name": "warning",
        "color": (255, 66, 66),
        "icon": "octicons/alert-24",
        "override": True,
    },
    {
        "name": "note",
        "icon": "octicons/pencil-24",
        "override": True,
    },
    {
        "name": "seealso",
        "color": (255, 66, 252),
        "icon": "octicons/eye-24",
        "title": "See Also",
        "override": True,
    },
    {
        "name": "hint",
        "icon": "material/school",
        "override": True,
    },
    {
        "name": "tip",
        "icon": "material/school",
        "override": True,
    },
    {
        "name": "important",
        "icon": "material/school",
        "override": True,
    },
]

# enable custom checkboxes
custom_checkbox = True

object_description_options = [("py:method", dict(include_fields_in_toc=False))]
cpp_src_dir = str(Path(__file__).parent.parent / "src")

cpp_apigen_configs = [
    dict(
        document_prefix="API/cpp-generated/",
        api_parser_config=dict(
            input_content="\n".join(
                ['#include "CirquePinnacle.h"', '#include "utility/includes.h"']
            ),
            compiler_flags=["-std=c++17", "-I", cpp_src_dir, "-x", "c++"],
            include_directory_map={
                f"{cpp_src_dir}/": "",
            },
            allow_paths=["utility/template/", "utility/includes.h", "", "__.*"],
            disallow_paths=[
                "utility/[^template]/",
                ".*/bits/char_traits.h",
                ".*_init_exception.h",
                ".*/numeric_traits.h",
                ".*/type_traits",
            ],
            ignore_diagnostics=["'stddef.h' file not found"],
            disallow_namespaces=["^std$"],
            allow_macros=["^PINNACLE_"],
            disallow_macros=["PINNACLE_ANYMEAS_SUPPORT", "PINNACLE_SS_CTRL"],
            allow_symbols=["^PINNACLE_"],
            verbose=True,
        ),
    ),
]

rst_prolog = """
.. role:: python(code)
   :language: python
   :class: highlight
.. role:: cpp(code)
   :language: cpp
   :class: highlight
.. |dr_pin_parameter| replace:: The input pin connected to the Pinnacle ASIC's "Data
    Ready" pin.
.. |dr_pin_required| replace::
    Previously, this parameter conditionally accepted a sentinel value to use the
    "SW Data Ready" flag in the STATUS_1 register.
.. |rev2025| replace:: not supported on trackpads manufactured on or after 2025.
    Defer to `~PinnacleTouch::isRev2025`.
.. |rev2025-no-effect| replace:: on newer trackpads will have no effect
"""

cpp_apigen_rst_prolog = """
.. default-literal-role:: cpp
.. highlight:: cpp
"""

includes_header = Path(__file__).parent.parent / "src/utility/includes.h"
includes_header_conf = includes_header.with_name("includes.h.in")
includes_header.write_text(
    includes_header_conf.read_text(encoding="utf-8").replace(
        "@PINNACLE_DRIVER@", "template"
    )
)

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ["_static"]

# These paths are either relative to html_static_path
# or fully qualified paths (eg. https://...)
html_css_files = ["extra_css.css"]

# The name of an image file (relative to this directory) to use as a favicon of
# the docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
html_favicon = "_static/favicon.ico"

html_logo = "_static/Logo.png"

html_title = "Cirque Pinnacle library"

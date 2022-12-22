#!/usr/bin/env python
from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules=cythonize('icp.pyx', language_level=3)
)

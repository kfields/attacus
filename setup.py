import sys
import setuptools
from skbuild import setup

setup(
    name             = 'attacus',
    description      = 'Python Flutter integration',
    version          = '0.1.0',
    url              = 'http://github.com/kfields/attacus',
    license          = 'MIT',
    author           = 'kfields',
    packages         = setuptools.find_packages(exclude=["__aimgen__"]),
    package_data={'attacus': ['*.pyd', '*.dll', "*.dat"]},
    #package_data={'aimgui': ['*.so']},
    #ext_modules      = [module],
    #setup_requires   = ['pybind11', "wheel", "scikit-build", "cmake", "ninja"],
    #install_requires=INSTALL_REQUIRES,
)

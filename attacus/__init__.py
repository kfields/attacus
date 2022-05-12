__version__ = '0.1.0'

import sys
import platform
from pathlib import Path

def add_plugin(location):
    version = platform.python_version_tuple()
    os = platform.system().lower()

    bin_dir = 'cmake-build'
    if os == 'windows':
        os = 'win'
        bin_dir = "cmake-build/Debug"

    LIB_PATH = Path(location).parent.parent / '_skbuild' / f"{os}-{platform.machine()}-{version[0]}.{version[1]}" / bin_dir
    #print('LIB_PATH:  ', LIB_PATH)

    sys.path.insert(0, str(LIB_PATH))
    #print('SYS_PATH:  ',sys.path)

add_plugin(__file__)

from attacus_py import *
#import attacus_py

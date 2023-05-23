__version__ = '0.1.0'

from pathlib import Path
from importlib import resources

from .attacus import *
from . import attacus as core

class FlutterView(core.FlutterView):
    def __init__(self, app :core.App) -> None:
        super().__init__(app)
        self.icu_data_path = str(resources.path('attacus', 'icudtl.dat'))
        


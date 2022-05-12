import unittest

import sys
sys.path.append("../")

from attacus import run_engine, RendererConfig, RendererType

'''
    libattacus.def("run_engine", []( const FlutterRendererConfig* config,
                                     const FlutterProjectArgs* args,
                                     void* user_data)
'''

class Test(unittest.TestCase):
    def test(self):
        config = RendererConfig()
        config.type = RendererType.K_OPEN_GL
        config.open_gl.struct_size = 0
        print(config)
        exit()
        args = None
        #engine = run_engine(config, args, None)
        engine = run_engine(config)

if __name__ == "__main__":
    unittest.main()

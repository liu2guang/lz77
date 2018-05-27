from building import *
import rtconfig

cwd     = GetCurrentDir()
src     = Glob('./lz77/*.c') + Glob('./lz77/*.cpp')
CPPPATH = ["./" + cwd]

LOCAL_CCFLAGS = ''

if rtconfig.CROSS_TOOL == 'gcc':
    LOCAL_CCFLAGS += ' -lm'

group = DefineGroup('lz77', src, depend = ['PKG_USING_LZ77'], CPPPATH = CPPPATH, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')

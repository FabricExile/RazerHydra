#
# Copyright 2010-2014 Fabric Software Inc. All rights reserved.
#

import os, sys, platform

if not platform.system().lower().startswith('win'):
  raise Exception("This extension only builds for Windows.")

thirdpartyDirs = {
  'FABRIC_DIR': "Should point to Fabric Engine's installation folder.",
  'SIXENSE_INCLUDE_DIR': 'Should point to the include folder of the Sixense Core SDK.',
  'SIXENSE_LIBRARY_DIR': 'Should point to the library folder of the Sixense Core SDK.',
  'BOOST_INCLUDE_DIR': "Should point to Boost include folder.",
  'BOOST_LIBRARY_DIR': "Should point to Boost library folder."
}

# help debug print
if GetOption('help'):
  print ''
  print 'Fabric Engine extension build script.'
  print '-----------------------------------'
  print 'Required environment variables: '
  for thirdpartyDir in thirdpartyDirs:
    print thirdpartyDir + ': ' + thirdpartyDirs[thirdpartyDir]
  print ''
  Exit()

# helper functions
def which(program):
  def is_exe(fpath):
    return os.path.isfile(fpath) and os.access(fpath, os.X_OK)
  for path in os.environ["PATH"].split(os.pathsep):
    path = path.strip('"')
    exe_file = os.path.join(path, program)
    if platform.system() == 'Windows':
      exe_file += ".exe"
    if is_exe(exe_file):
      return exe_file
  return None

#determine the suffix for the extension
extSuffix = platform.system()+'-x86_64.dll'
Export('extSuffix')

# try to find kl2edk and kl
kl2edk = which('kl2edk')
if kl2edk is None:
  raise Exception('kl2edk could not be found on the PATH.')
kl = which('kl')
if kl is None:
  raise Exception('kl could not be found on the PATH.')
kl2dfg = which('kl2dfg')
if kl2dfg is None:
  raise Exception('kl2dfg could not be found on the PATH.')
Export('kl2edk', 'kl2dfg', 'kl')

# setup the environment and define some methods
def RunKL2EDK(
  env,
  targets,
  sources
  ):

  targetFolder = os.path.split(targets[0].path)[0]
  cmdLine = [[kl2edk] + [sources[0].srcnode().path]]
  cmdLine[0] += ['-o', targetFolder]
  result = env.Command(
    targets,
    sources,
    cmdLine   
  )

  return result

def RunKL2DFG(
  env,
  targets,
  sources
  ):

  targetFolder = os.path.split(targets[0].path)[0]

  cmdLine = [[kl2dfg] + [sources[0].srcnode().path] + [targetFolder]]
  result = env.Command(
    targets,
    sources,
    cmdLine   
  )

  return result

# for windows for now use Visual Studio 2010. 
# if you upgrade this you will also have to provide
# boost libs for the corresponding VS version
os.environ['FABRIC_EXTS_PATH'] += os.pathsep + str(Dir('stage').Dir('Exts'))
env = Environment(ENV = os.environ, MSVC_VERSION='10.0')
env.AddMethod(RunKL2EDK)
env.AddMethod(RunKL2DFG)

# find the third party libs
for thirdpartyDir in thirdpartyDirs:
  if not os.environ.has_key(thirdpartyDir):
    raise Exception(thirdpartyDir+' env variable not defined. '+thirdpartyDirs[thirdpartyDir])

env.Append(CPPPATH = [os.path.join(os.environ['FABRIC_DIR'], 'include')])
env.Append(CPPPATH = [os.environ['SIXENSE_INCLUDE_DIR']])
env.Append(LIBPATH = [os.environ['SIXENSE_LIBRARY_DIR']])
env.Append(CPPPATH = [os.environ['BOOST_INCLUDE_DIR']])
env.Append(LIBPATH = [os.environ['BOOST_LIBRARY_DIR']])

alias = SConscript('src/SConscript', variant_dir = 'build', exports = {'parentEnv': env, 'STAGE_DIR': env.Dir('stage')}, duplicate=0)

env.Default(alias)

import os
import sys
import glob
import subprocess
import signal
import tempfile
import time
import unittest

class KLTest(unittest.TestCase):

  def __init__(self, klFilePath):
    super(KLTest, self).__init__()
    self.__klFilePath = klFilePath

  def id(self):
    return os.path.split(self.__klFilePath)[1].partition('.')[0]

  def shortDescription(self):
    return self.id()

  def runTest(self):
    stageFolder = os.path.abspath(os.path.join(os.path.split(self.__klFilePath)[0], '..', '..', 'stage'))

    env = {}
    env.update(os.environ)
    if not env.has_key('FABRIC_EXTS_PATH'):
      env['FABRIC_EXTS_PATH'] = stageFolder
    else:
      env['FABRIC_EXTS_PATH'] += os.pathsep + stageFolder

    p = None

    def handler(signum, frame):
      if p:
        os.kill(p.pid, signal.SIGTERM)
      sys.exit(0)

    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGTERM, handler)

    klArgs = ['kl'] + ['--showthread', '--loadexts', self.__klFilePath]

    logFile = tempfile.TemporaryFile()
    logFilePath = logFile.name
    logFile.file.flush()
    logFile.file.close()

    logFile = open(logFilePath, 'wb')    

    p = subprocess.Popen(
      klArgs,
      env = env,
      cwd = os.path.abspath(os.path.split(__file__)[0]),
      shell=True, 
      universal_newlines=True,
      stdout = logFile
    )

    while True:
      time.sleep(1)
      p.poll()
      if not p.returncode is None:
        break

    logFile.close()

    if not os.path.exists(logFilePath):
      self.fail('logFile was not created.')
      return

    currContent = open(logFilePath, 'rb').read()
    currContent = currContent.replace('\r', '')
    currContentSplit = currContent.split('\n')
    currContent = []
    for line in currContentSplit:
      if line.startswith('['):
        currContent += [line]
    currContent = '\n'.join(currContent)
    currContent = currContent.strip('\n')

    print '------- '+self.__klFilePath+' --------'
    print currContent
    print '----------------------------------'

    outFilePath = self.__klFilePath.rpartition('.')[0]+'.out'
    if not os.path.exists(outFilePath):
      self.fail('.out file does not exist.')

    prevContent = open(outFilePath, 'rb').read()
    prevContent = prevContent.replace('\r', '')
    prevContent = prevContent.strip('\n')

    self.assertEqual(currContent, prevContent)

if __name__ == '__main__':
  klFolder = os.path.join(os.path.split(os.path.abspath(__file__))[0], 'kl')
  klFiles = glob.glob(os.path.join(klFolder, '*.kl'))

  suite = unittest.TestSuite()

  for klFile in klFiles:
    test = KLTest(klFile)
    suite.addTest(test)

  runner = unittest.TextTestRunner()
  result = runner.run(suite)

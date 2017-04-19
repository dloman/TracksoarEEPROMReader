#!/usr/bin/python2.7

# Dan Loman
# 6/11/2016
#
# This Script sends the magic packet to the tracksoar and then dumps the
# data to a csv until EOF is sent

import glob
import serial
import sys
import time

################################################################################
def getSerialConnection():
  SerialPorts = []
  if sys.platform.startswith('win'):
    SerialPorts = ['COM%s' % (i + 1) for i in range(256)]
  elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
    # this excludes your current terminal "/dev/tty"
    SerialPorts = glob.glob('/dev/tty[A-Za-z]*')
  elif sys.platform.startswith('darwin'):
    SerialPorts = glob.glob('/dev/tty.*')
  else:
    raise EnvironmentError('Unsupported platform')

  for ttyName in SerialPorts:
    try:
      return serial.Serial(ttyName, 9600, timeout=.1)
    except:
      pass
  else:
    return None

################################################################################
################################################################################
if __name__ == "__main__":
  SerialPort = getSerialConnection()
  Data = ''
  if SerialPort:
    while(True):
      NewData = SerialPort.read()
      if NewData == '':
        SerialPort.write('!')
        SerialPort.flush()
      else:
        Data += NewData
        if Data.find("EOF") != -1:
          FileName = 'Output-' + time.asctime().replace(' ', '-')
          print 'Writing to', FileName
          with open(FileName, 'w') as Output:
            Output.write(Data[:-5])
          exit()


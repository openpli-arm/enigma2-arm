from struct import *
from Screens.MessageBox import MessageBox
from Components.MenuList import MenuList
from Screens.Screen import Screen
from Components.ActionMap import ActionMap, NumberActionMap
from Components.Sources.StaticText import StaticText
from Components.MultiContent import MultiContentEntryText, MultiContentEntryPixmapAlphaTest
from Tools.LoadPixmap import LoadPixmap
from enigma import eListboxPythonMultiContent, gFont

import array, fcntl, struct, termios, os
import time
               
class LnbTest:

	FE_SET_VOLTAGE = 28483
	FE_SET_TONE  = 28482
	FE_DISEQC_SEND_MASTER_CMD = 1074229055

	voltageStep = 0
	diseqcstep = 0

	def __init__(self,lnbindex):
		self.lnvdev = "/dev/dvb/adapter0/frontend%d" %lnbindex
		self.voltageTestlist = [self.setVoltage13,self.setVoltage18]
		self.diseqcTestList = [self.setDiSEqC]*4

	def checkVoltageTestFinish(self):
		if LnbTest.voltageStep in [0,1]:
			return False
		else:
			return True
			
	def testVoltage(self,first):
		if first:
			LnbTest.voltageStep = 0
		if LnbTest.voltageStep not in [0,1]:
			LnbTest.voltageStep = 0
		
		LnbTest.voltageStep += 1
		step = LnbTest.voltageStep - 1
		return self.voltageTestlist[step]()
		
	def setVoltage13(self):
		print "set Voltage 13V"
		test = -1
		try:
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_VOLTAGE,0)
			lnbfd.close()
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			return "set Voltage 13V"
		else:
			return "Fail,Check hardware!!"
		
	def setVoltage18(self):
		print "set Voltage 18V"
		test = -1
		try:
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_VOLTAGE,1)
			lnbfd.close()
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			return "set Voltage 18V"
		else:
			return "Fail,Check hardware!!"
			
	def checkDiSEqCTestFinish(self):
		if LnbTest.diseqcstep in [0,1,2,3]:
			return False
		else:
			return True
			
	def testDiSEqC(self,first):
		if first:
			LnbTest.diseqcstep = 0
		if LnbTest.diseqcstep not in [0,1,2,3]:
			LnbTest.diseqcstep = 0
		
		LnbTest.diseqcstep += 1
		step = LnbTest.diseqcstep - 1
		return self.diseqcTestList[step](step)
		
	def setDiSEqC(self,port):
	  	test = -1
		try:
			#close 22K		
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_TONE,1)
			print "close 22K result:",test
			time.sleep(0.025)
			
			if LnbTest.diseqcstep == 1:
				buffreset = array.array('B', [0xe0,0x00,0x00,0x00,0x00,0x00,0x03])
				test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_DISEQC_SEND_MASTER_CMD,buffreset,True)
				time.sleep(0.05)
				buffvoltage = array.array('B', [0xe0,0x00,0x03,0x00,0x00,0x00,0x03])
				test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_DISEQC_SEND_MASTER_CMD,buffvoltage,True)
				time.sleep(0.15)

			buffport1 = array.array('B', [0xe0,0x10,0x38,0xF3,0x00,0x00,0x04])
			buffport2 = array.array('B', [0xe0,0x10,0x38,0xF7,0x00,0x00,0x04])
			buffport3 = array.array('B', [0xe0,0x10,0x38,0xFB,0x00,0x00,0x04])
			buffport4 = array.array('B', [0xe0,0x10,0x38,0xFF,0x00,0x00,0x04])
			buff = [buffport1,buffport2,buffport3,buffport4]
			
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_DISEQC_SEND_MASTER_CMD,buff[port],True)
			time.sleep(0.05)
			lnbfd.close()
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			return "22K Off port%d" %(port+1)
		else:
			return "Fail,Check hardware!!"
	
	def setSton(self):
		try:
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_TONE,1)
			print "test :",test
			time.sleep(3)
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_TONE,0)
			lnbfd.close()
		except:
			pass
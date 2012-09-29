from struct import *
from Screens.MessageBox import MessageBox
from Components.MenuList import MenuList
from Screens.Screen import Screen
from Components.ActionMap import ActionMap, NumberActionMap
from Components.Sources.StaticText import StaticText
from Components.MultiContent import MultiContentEntryText, MultiContentEntryPixmapAlphaTest
from Tools.LoadPixmap import LoadPixmap
from enigma import eListboxPythonMultiContent, gFont,eTimer
import array, fcntl, struct, termios, os
import time
from Components.Console import Console
class LnbTest:

	FE_SET_VOLTAGE = 28483
	FE_SET_TONE  = 28482
	FE_DISEQC_SEND_MASTER_CMD = 1074229055

	voltageStep = 0
	diseqcstep = 0

	def __init__(self,lnbindex):
		self.lnbindex = lnbindex
		self.lnvdev = "/dev/dvb/adapter0/frontend%d" %lnbindex
		self.voltageTestlist = [self.setVoltage13,self.setVoltage18,self.playSevice,self.playSevice]
		self.dvbtvoltageTestlist = [self.setVoltage5dvbt,self.setVoltage0dvbt]
		
		self.diseqcTestList = [self.setDiSEqC]*4
		self.tunertype = "DVB-S"
		if lnbindex == 2:
			self.tunertype = "DVB-T"
		self.playsevice = False
		
		self.playTimer = eTimer()
		self.playTimer.callback.append(self.doPlay)
		self.playConsole = Console()
	def checkVoltageTestFinish(self):
		if LnbTest.voltageStep in [0,1,2,3]:
			return False
		else:
			return True
			
	def testVoltage(self,first):
		if first:
			LnbTest.voltageStep = 0
		if LnbTest.voltageStep not in [0,1,2,3]:
			LnbTest.voltageStep = 0
		step = LnbTest.voltageStep
		if self.tunertype == "DVB-S":
			return self.voltageTestlist[step]()
		elif self.tunertype == "DVB-T":
			return self.dvbtvoltageTestlist[step]()
		else:
			return "Tuner type error!!"
			
	def setVoltage13(self):
		print "set Voltage 13V"
		test = -1
		try:
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_VOLTAGE,0)
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			testresult = "Set voltage 13V ok\n"
		else:
			LnbTest.voltageStep +=1
			return "Fail,Check hardware!!"
#22k on
		try:
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_TONE,0)	
			lnbfd.close()
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			testresult += "Set 22K on ok"
		else:
			testresult = "Fail,Check hardware!!"
			
		LnbTest.voltageStep +=1
		return testresult
		
	def setVoltage18(self):
		print "set Voltage 18V"
		test = -1
		try:
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_VOLTAGE,1)
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			testresult = "Set voltage 18V ok\n"
		else:
			LnbTest.voltageStep +=1
			return "Fail,Check hardware!!"
#set 22k on
		try:
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_TONE,0)	
			lnbfd.close()
		except IOError:
			print "Fail,Check hardware!!"	
		if test == 0:
			testresult += "Set 22K on ok"
		else:
			return "Fail,Check hardware!!"
			
		LnbTest.voltageStep +=1	
		return testresult
		
	def playSevice(self):
		if self.playsevice == False:
			self.playsevice = True
			if self.lnbindex == 0:
				self.tunerid = "0"
			else:
				self.tunerid = "1"
			if LnbTest.voltageStep == 2:
				self.frq = "3660"
				self.sr = "27500"
				self.vpid = "2317"
				self.apid = "2318"
				self.pcrpid= "2317"
				self.voltage = "0"
				testresult = "13V on Play service:\n" +"Name: ALKASS"
			else:
				self.frq = "3960"
				self.sr = "27500"
				self.vpid = "69"
				self.apid = "68"
				self.pcrpid= "69"
				self.voltage = "1"
				testresult = "18V on Play service:\n" +"Name: VKT TEST"
			self.playTimer.start(1)
		else:
			testresult = "Wait for last sevice play end!"
		return testresult

	def doPlay(self):
		self.playTimer.stop()
		print "play start"
		
		clearcmd = "dvb_test "+self.tunerid + " "+self.frq+" "+self.sr+" "+self.vpid+" "+self.apid+" "+self.pcrpid+ " 5 "+self.voltage
		os.system(clearcmd)
		
		LnbTest.voltageStep +=1	
		self.playsevice = False
		print "play finish!"
	#	cmds = []
	#	cmds.append("0\n")
	#	cmds.append("./dvb_test " + "0 1500 27500 600 601 600 5")
	#	self.playConsole.eBatch(cmd, self.doplaycb)
	#	if self.lnbindex == 0:
	#		clearcmd = "./dvb_test 1 "+self.frq+" "+self.sr+" "+self.vpid+" "+self.apid+" "+self.pcrpid+ " 0"
	#	else:
	#		clearcmd = "./dvb_test 0 "+self.frq+" "+self.sr+" "+self.vpid+" "+self.apid+" "+self.pcrpid+ " 0"
	#	self.playConsole.ePopen(clearcmd, self.doclerncb)
	#	print "play start!"
	#	playcmd = "./dvb_test "+self.tunerid+" "+self.frq+" "+self.sr+" "+self.vpid+" "+self.apid+" "+self.pcrpid+ " 5" 
	#	self.playConsole.ePopen(playcmd, self.doplaycb)
		
	def doclerncb(self,result,retval=None, extra_args=None):
		print "clear!"
		print "play start!"
		playcmd = "./dvb_test "+self.tunerid+" "+self.frq+" "+self.sr+" "+self.vpid+" "+self.apid+" "+self.pcrpid+ " 5" 
		self.playConsole.ePopen(playcmd, self.doplaycb)
		
	def doplaycb(self,result,retval=None, extra_args=None):		
		LnbTest.voltageStep +=1	
		self.playsevice = False
		print "play finish!"
		
	def setVoltage5dvbt(self):
		print "set DVB-t Voltage 5V"
		test = -1
		try:
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_VOLTAGE,0)
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			return "set:DVB-T Voltage 5V"
		else:
			return "set Voltage 5V fail,Check hardware!!"

	def setVoltage0dvbt(self):
		print "set DVB-t Voltage 0V"
		test = -1
		try:
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_VOLTAGE,2)
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			return "set:DVB-T Voltage 0V"
		else:
			return "set Voltage 0V fail,Check hardware!!"
		
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
#####
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

from struct import *
from Screens.MessageBox import MessageBox
from Components.MenuList import MenuList
from Screens.Screen import Screen
from Components.ActionMap import ActionMap, NumberActionMap
from Components.Sources.StaticText import StaticText
from Components.MultiContent import MultiContentEntryText, MultiContentEntryPixmapAlphaTest
from Tools.LoadPixmap import LoadPixmap
from enigma import eListboxPythonMultiContent, gFont,eTimer
import array, fcntl, struct, termios, os, signal,string
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
		self.dvbtvoltageTestlist = [self.setVoltage5dvbt,self.setVoltage0dvbt,self.playSevice]
		
		self.diseqcTestList = [self.setDiSEqC]*4
		self.tunertype = "DVB-S"
		if lnbindex == 2:
			self.tunertype = "DVB-T"
		
		self.playTimer = eTimer()
		self.playTimer.callback.append(self.doPlay)
		self.playConsole = Console()
	def checkVoltageTestFinish(self):
		if self.tunertype == "DVB-S":
			if LnbTest.voltageStep in [0,1,2]:
				return False
		elif self.tunertype == "DVB-T":
			if LnbTest.voltageStep in [0,1,2]:
				return False
		else:
			pass
		return True
			
	def testVoltage(self,first):
		if first:
			LnbTest.voltageStep = 0		
		if self.tunertype == "DVB-S":
			if LnbTest.voltageStep not in [0,1,2]:
				LnbTest.voltageStep = 0
			return self.voltageTestlist[LnbTest.voltageStep]()
		elif self.tunertype == "DVB-T":
			if LnbTest.voltageStep not in [0,1,2]:
				LnbTest.voltageStep = 0
			return self.dvbtvoltageTestlist[LnbTest.voltageStep]()
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
		if self.lnbindex == 0:
			if LnbTest.voltageStep == 2:
				self.frq = "1490"
				self.sr = "27500"
				self.vpid = "2317"
				self.apid = "2318"
				self.pcrpid= "2317"
				self.voltage = "0"
				testresult = "set voltage 13V,Play service: " +"ALKASS"
		elif self.lnbindex == 1:
				self.frq = "1190"
				self.sr = "27500"
				self.vpid = "69"
				self.apid = "68"
				self.pcrpid= "69"
				self.voltage = "1"
				testresult = "set voltage 18V,Play service: " +"VKT TEST"
		else:
				self.frq = "474000"
				self.sr = "0"
				self.vpid = "1501"
				self.apid = "1503"
				self.pcrpid= "1501"
				self.voltage = "1"
				testresult = "set voltage 5V,Play service."
				
		self.playTimer.start(1)
		LnbTest.voltageStep +=1	
		return testresult

	def get_Pid(self,process_name):
		cmd = "ps"
		print cmd
		pline = []
		pid = None
		try:
			resultstring = os.popen(cmd).read()
			if resultstring:
				for line in resultstring.splitlines():
					line = line.strip()
					if process_name in line:
						pline = line.split(" ")
						pid = pline[0].strip()
						print pid
			else:
				print "no run dvb_test"
				pid = None
		except Exception,e:
			print e
		return pid
		
	def doPlay(self):
		self.playTimer.stop()
	
		testnaem = "dvb_test"
		pid = self.get_Pid(testnaem)
		playcmd =testnaem + " " +str(self.lnbindex)+ " "+self.frq+" "+self.sr+" "+self.vpid+" "+self.apid+" "+self.pcrpid+" "+self.voltage
	
		sendsignal = signal.SIGINT
		try:
			if pid == None:
				print "start dvb_test play!"
				self.playConsole.ePopen(playcmd, self.doplaycb)
			else:
				print "send signal SIGINT",pid
				os.kill(string.atoi(pid),sendsignal)
				
				self.doPlay()
		except Exception,e:
			print e
			
	def doclerncb(self,result,retval=None, extra_args=None):
		print "clear!"
		print "play start!"
		playcmd = "./dvb_test "+self.tunerid+" "+self.frq+" "+self.sr+" "+self.vpid+" "+self.apid+" "+self.pcrpid+ " 5" 
		self.playConsole.ePopen(playcmd, self.doplaycb)
		
	def doplaycb(self,result,retval=None, extra_args=None):
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
			testresult = "set voltage 5V ok"
		else:
			testresult= "Fail,Check hardware!!"
			
		LnbTest.voltageStep +=1	
		return testresult
		
	def setVoltage0dvbt(self):
		print "set DVB-t Voltage 0V"
		test = -1
		try:
			lnbfd = open(self.lnvdev,'wb')
			test  = fcntl.ioctl(lnbfd.fileno(),LnbTest.FE_SET_VOLTAGE,2)
		except IOError:
			print "Fail,Check hardware!!"
		if test == 0:
			testresult = "set voltage 0V ok"
		else:
			testresult = "Fail,Check hardware!!"
		LnbTest.voltageStep +=1	
		return testresult
		
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

import os
import time
import sys
from enigma import eTimer
from select import select
from sys import stdout, stdin

class Rs232Test:
	def __init__(self):
		self.teststring  = "Test RS232"
		self.testTimer = eTimer()
		self.testTimer.callback.append(self.sendData)
		
	def sendData(self):
#		self.testTimer.stop()
		print self.teststring

	def Rs232ConnetctTest(self,timeout):
		rd = select([stdin],[],[],timeout)[0]
		if not rd:
#			return None
			return False
		else:
#			return raw_input()
			return True

	def StartTest(self):
#		self.testTimer.start(10)
		recive = ""
		testresult = False
		stdin.flush()

		self.sendData()
		if self.Rs232ConnetctTest(3):	
#			fd = open("/retest",'a')
			cout = 0
			while (cout < 10000):
				cout += 1
				stdin.flush()
				self.sendData()
				recive = raw_input()			
#				fd.write(recive)
				if recive == self.teststring:
					break	
#			fd.close()		
			if cout <10000:
				testresult = True
			else:
				testresult = False
		else:
			testresult = False
		return testresult

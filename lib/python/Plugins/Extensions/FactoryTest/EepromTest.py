import os
#from os import system, listdir, statvfs, popen, makedirs, stat, major, minor, path, access
#from Screens.Screen import Screen
#from Components.Harddisk import *
#from Components.Sources.StaticText import StaticText
#from Components.ActionMap import ActionMap, NumberActionMap
#from FactoryTestPublic import *

class EepromTest:

	MACADDRESS = "00:25:8F"

	def __init__(self):
		self.eepromFile = "/proc/stb/info/eeprom/mac_address"
		self.readMacaddrbuff = []
		
	def checkEepromExist(self):
		if os.path.exists(self.eepromFile):
			return True
		else:
			print"Eeprom not found"
			return False
			
	def readMacaddr(self):
		print "read maccaddress from eeprom"
		try:
			fd = open(self.eepromFile,'r')	
			self.readMacaddrbuff = fd.read(8)
			print self.readMacaddrbuff
			fd.close()
		except IOError:
			return False
		return True	
			
	def checkEeprom(self):
		if self.checkEepromExist():
			self.readMacaddrbuff = None
			if self.readMacaddr():
				print "read data is:",self.readMacaddrbuff
				return self.checkMacaddr()
			else:
				return False
		else:
			return False
			
	def checkMacaddr(self):
		print "self.readMacaddrbuff:",self.readMacaddrbuff
		print "MACADDRESS:",EepromTest.MACADDRESS
		if EepromTest.MACADDRESS == self.readMacaddrbuff:
			return True
		else:
			return False
#	
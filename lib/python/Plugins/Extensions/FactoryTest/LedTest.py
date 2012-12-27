from struct import *
from Screens.MessageBox import MessageBox
from Components.MenuList import MenuList
from Screens.Screen import Screen
from Components.ActionMap import ActionMap, NumberActionMap
from Components.Sources.StaticText import StaticText

from Components.MultiContent import MultiContentEntryText, MultiContentEntryPixmapAlphaTest
from Tools.LoadPixmap import LoadPixmap
from enigma import eListboxPythonMultiContent, gFont

from FactoryTestPublic import *

class LedTest:
	step = 0
	def __init__(self):
		self.testList = [self.displayBlank,self.displayWhite]
		
	def checkFinish(self):
		if LedTest.step in [0,1]:
			return False
		else:
			return True
			
	def test(self,first):
		if first:
			LedTest.step = 0
		if LedTest.step not in [0,1]:
			LedTest.step = 0
		LedTest.step += 1
		step = LedTest.step - 1
		return self.testList[step]()
			
	def displayBlank(self):
		print "start displayBlank"
		try:
			ledfd = open('/dev/dbox/oled0','wb')
			ledbuff = "\x00"*128*8
#			print "ledbuff:",ledbuff
			ledfd.write(ledbuff)
			ledfd.close()
		except IOError:
			return "Fail"
		return "Display Blank"
		
	def displayWhite(self):
		print "test Led display white ..."
#		print "ledbuff:",ledbuff
		try:
			ledfd = open('/dev/dbox/oled0','wb')
			ledbuff = "\xff"*128*8
			ledfd.write(ledbuff)
			ledfd.close()
		except IOError:
			return "Fail"
		return "Display White"
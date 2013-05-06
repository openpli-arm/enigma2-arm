import os
from os import system, listdir, statvfs, popen, makedirs, stat, major, minor, path, access
from Screens.Screen import Screen
from Components.Harddisk import *
from Components.Sources.StaticText import StaticText
from Components.ActionMap import ActionMap, NumberActionMap
from FactoryTestPublic import *

def checkUsbAvailable(devpath):
	if os.path.exists(devpath):
		return True
	else:
		return False
			
class SDCardTest(Screen):
	skin = """
		<screen name="SDCardTest" position="220,57" size="840,605" title="SDCardTest" flags="wfNoBorder">
			<ePixmap position="0,0" zPosition="-10" size="1100,605" pixmap="DMConcinnity-HD-Transp/menu/setupbg.png" />
			<widget source="global.CurrentTime" render="Label" position="20,20" size="80,25" font="Regular;23" foregroundColor="black" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Default</convert>
			</widget>
			<widget source="global.CurrentTime" render="Label" position="110,20" size="140,25" font="Regular;23" foregroundColor="blue" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Format:%d.%m.%Y</convert>
			</widget>
			<eLabel text="USB information" position="270,20" size="540,43" font="Regular;35" halign="right" foregroundColor="black" backgroundColor="grey" transparent="1" />
			<widget source="usbtitle" render="Label" position="110,145" size="700,35" font="Regular;26" foregroundColor="yellow" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbname" render="Label" position="140,190" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbsize" render="Label" position="140,220" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbfreesize" render="Label" position="140,250" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
		</screen>"""
	def __init__(self,session,testitem):
		Screen.__init__(self, session)
		self.testitem = testitem
		self.devpath = "mmcblk0"
		self.usbdevice = Harddisk(self.devpath)
		self.usbdevicename = self.usbdevice.model()
		self.usbsize= self.usbdevice.capacity()
#		self.usbfreesize = (stat.f_bfree/1000) * (stat.f_bsize/1000)
		self.usbfreesize = self.usbdevice.free()
		
		print "usbdevicename:",self.usbdevicename
		
		print "usbsize:",self.usbsize

		print "usbfreesize",self.usbfreesize

		self["usbtitle"] = StaticText("SDCard Device:"+self.usbdevicename)
		self["usbname"] = StaticText("CardInfor: "+self.usbdevicename)
		self["usbsize"] = StaticText("Card Size: "+self.usbsize)
		self["usbfreesize"]=StaticText(_("Free Size: %d M") % (self.usbfreesize))
		
		self["actions"] = ActionMap(["SetupActions", "ColorActions"], 
			{
				"cancel": self.testfinish,
				"ok": self.testfinish
			})

	def testfinish(self):
		self.testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
		self.close()
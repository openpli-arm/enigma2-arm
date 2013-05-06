import os
from os import system, listdir, statvfs, popen, makedirs, stat, major, minor, path, access
from Screens.Screen import Screen
from Components.Harddisk import *
from Components.Sources.StaticText import StaticText
from Components.ActionMap import ActionMap, NumberActionMap
from FactoryTestPublic import *
from Components.Harddisk import harddiskmanager

def checkUsbAvailable(devpath):
	if os.path.exists(devpath):
		return True
	else:
		return False
			
class UsbTest(Screen):
	skin = """
		<screen name="About" position="220,57" size="840,605" title="About" flags="wfNoBorder">
			<ePixmap position="0,0" zPosition="-10" size="1100,605" pixmap="DMConcinnity-HD-Transp/menu/setupbg.png" />
			<widget source="global.CurrentTime" render="Label" position="20,20" size="80,25" font="Regular;23" foregroundColor="black" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Default</convert>
			</widget>
			<widget source="global.CurrentTime" render="Label" position="110,20" size="140,25" font="Regular;23" foregroundColor="blue" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Format:%d.%m.%Y</convert>
			</widget>
			<eLabel text="USB information" position="270,20" size="540,43" font="Regular;35" halign="right" foregroundColor="black" backgroundColor="grey" transparent="1" />
			<widget source="usbtitle" render="Label" position="50,75" size="700,35" font="Regular;26" foregroundColor="yellow" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbname" render="Label" position="60,110" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbsize" render="Label" position="60,140" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbfreesize" render="Label" position="60,170" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />

			<widget source="usbtitle2" render="Label" position="50,210" size="700,35" font="Regular;26" foregroundColor="yellow" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbname2" render="Label" position="60,245" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbsize2" render="Label" position="60,275" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbfreesize2" render="Label" position="60,305" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />

			<widget source="usbtitle3" render="Label" position="50,345" size="700,35" font="Regular;26" foregroundColor="yellow" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbname3" render="Label" position="60,380" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbsize3" render="Label" position="60,410" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
			<widget source="usbfreesize3" render="Label" position="60,440" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />

		</screen>"""
	def __init__(self,session,testitem):
		Screen.__init__(self, session)
		self.testitem = testitem
		self.testport = ["1","2","3"]
		
		self["actions"] = ActionMap(["SetupActions", "ColorActions"], 
			{
				"cancel": self.testfinish,
				"ok": self.testfinish
			})
		self.testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
		self.displayInfor()
		
	def displayInfor(self):
		hdd = self.getUsbPortDevice(self.testport[0])
		print "hdd:",hdd
		if hdd:
			self["usbtitle"] = StaticText("USB Port 1:"+hdd[1].model())
			self["usbname"] = StaticText("USB Name: "+hdd[1].model())
			self["usbsize"] = StaticText("USB Size: "+hdd[1].capacity())
			self["usbfreesize"]=StaticText(_("Free Size: %d M") % (hdd[1].free()))	
		else:
			self["usbtitle"] = StaticText("USB Port 1:"+"Have no Device check hardware!")
			self["usbname"] = StaticText("")
			self["usbsize"] = StaticText("")
			self["usbfreesize"]=StaticText("")
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
			
		hdd = self.getUsbPortDevice(self.testport[1])
		print "hdd:",hdd
		if hdd:
			self["usbtitle2"] = StaticText("USB Port 2:"+hdd[1].model())
			self["usbname2"] = StaticText("USB Name: "+hdd[1].model())
			self["usbsize2"] = StaticText("USB Size: "+hdd[1].capacity())
			self["usbfreesize2"]=StaticText(_("Free Size: %d M") % (hdd[1].free()))	
		else:
			self["usbtitle2"] = StaticText("USB Port 2:"+"Have no Device check hardware!")
			self["usbname2"] = StaticText("")
			self["usbsize2"] = StaticText("")
			self["usbfreesize2"]=StaticText("")
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)

		hdd = self.getUsbPortDevice(self.testport[2])
		print "hdd:",hdd
		if hdd:
			self["usbtitle3"] = StaticText("USB Port 3:"+hdd[1].model())
			self["usbname3"] = StaticText("USB Name: "+hdd[1].model())
			self["usbsize3"] = StaticText("USB Size: "+hdd[1].capacity())
			self["usbfreesize3"]=StaticText(_("Free Size: %d M") % (hdd[1].free()))	
		else:
			self["usbtitle3"] = StaticText("USB Port 3:"+"Have no Device check hardware!")
			self["usbname3"] = StaticText("")
			self["usbsize3"] = StaticText("")
			self["usbfreesize3"]=StaticText("")
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)

	def testfinish(self):
#		self.testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
		self.close()

	def getUsbPortDevice(self,port):
		hddlist = harddiskmanager.HDDList()
		for hdd in hddlist:
			dhhpath = hdd[1].sysfsPath("")
			if port == (dhhpath.split('/')[-7])[-1]:
				print port[-1]
				return hdd
		return None

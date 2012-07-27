import os
import re
from Screens.Screen import Screen
from Components.Harddisk import *
from Components.Sources.StaticText import StaticText
from Components.ActionMap import ActionMap, NumberActionMap
from FactoryTestPublic import *
import time
class NetworkTest(Screen):
	
	skin = """
		<screen name="About" position="220,57" size="840,605" title="About" flags="wfNoBorder">
			<ePixmap position="0,0" zPosition="-10" size="1100,605" pixmap="DMConcinnity-HD-Transp/menu/setupbg.png" />
			<widget source="global.CurrentTime" render="Label" position="20,20" size="80,25" font="Regular;23" foregroundColor="black" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Default</convert>
			</widget>
			<widget source="global.CurrentTime" render="Label" position="110,20" size="140,25" font="Regular;23" foregroundColor="blue" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Format:%d.%m.%Y</convert>
			</widget>
			<eLabel text="Network Test" position="270,20" size="540,43" font="Regular;35" halign="right" foregroundColor="black" backgroundColor="grey" transparent="1" />
			<widget source="workstatus" render="Label" position="110,145" size="700,70" font="Regular;26" foregroundColor="yellow" backgroundColor="transpBlack" transparent="1" />
			<widget source="testinfo" render="Label" position="120,230" size="660,330" font="Regular;15" backgroundColor="transpBlack" transparent="1" />
			<ePixmap pixmap="DMConcinnity-HD-Transp/buttons/green.png" position="30,570" size="35,27" alphatest="blend" />
			<eLabel text="Start" position="80,573" size="200,26" zPosition="1" font="Regular;22" halign="left" foregroundColor="black" backgroundColor="grey" transparent="1" />
		</screen>"""
#			<widget source="usbtitle" render="Label" position="110,145" size="700,35" font="Regular;26" foregroundColor="yellow" backgroundColor="transpBlack" transparent="1" />
#			<widget source="usbname" render="Label" position="140,190" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
#			<widget source="usbsize" render="Label" position="140,220" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
#			<widget source="usbfreesize" render="Label" position="140,250" size="700,30" font="Regular;24" backgroundColor="transpBlack" transparent="1" />		
	def __init__(self,session,testitem):
		Screen.__init__(self, session)
		self.testserver = "8.8.8.8"
		self.testitem = testitem
		self._runing = False
		self.result = False
		
		self["workstatus"] = StaticText("Check the network cable is connected,Press green\n key Start Test")
		self["testinfo"] = StaticText("")
		self["teststatus"] = StaticText("Start")
		self["actions"] = ActionMap(["SetupActions", "ColorActions"], 
			{
				"green": self.startTest,
				"ok": self.doclose
			})
			
	def startTest(self):
		print "key green"
		if self._runing:
			return 
		else:
		   self.pingServer()
		   
	def doclose(self):
		self.updeteResult()
		self.close()
		
	def updeteResult(self):
		if self.result:
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
		else:
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
			
	def test(self):
		self._runing = True
		print "test"
		time.sleep(5)
		self._runing = False
		
	def dhcp(self):
		cmd = "ifconfig eth0 down"
		p = os.popen(cmd)
		info = p.read()  
		print "info read",info

	def pingServer(self):
		self["workstatus"].setText("Ping Server .........")
		
		cmd = "ping -c 3 %s" % self.testserver
		p = os.popen(cmd)
		info = p.read()
		print "info read",info
		p.close()

		self["testinfo"].setText(info)

		re_lost_str = '(\d+)% packet loss'
		lost_packet = int(re.search(re_lost_str,info).group(1))
		print "lost package is :",lost_packet
		if lost_packet == 100:
			self["workstatus"].setText("Network Connection Error!!! Check cable and Hardware")
			print "device Error"
			self.result = False
#			return False
		self["workstatus"].setText("Network Connection OK")
		self.result = True
		
#		return True
		
#	def monitoring(self):	
#	    cmd = "ping -c 5 %s" % self.testserver
#	    p = os.popen(cmd)  
#	    info = p.read()  
#	    print "info read",info
#	    
#	    p.close()  
#	    re_lost_str = '(\d+)% packet loss'  
#	    lost_packet = int(re.search(re_lost_str, info).group(1))  
#	    print "lost package is :",lost_packet
#	    if lost_packet == 100:
#	    	print "device Error"
#	        return False
#		return True

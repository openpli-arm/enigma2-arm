import os
import re
from os import system, popen, path as os_path, listdir

from Screens.Screen import Screen
from Components.Harddisk import *
from Components.Sources.StaticText import StaticText
from Components.ActionMap import ActionMap, NumberActionMap
from FactoryTestPublic import *
import time
from enigma import eTimer
from Components.Network import Network,iNetwork

from Components.Label import Label,MultiColorLabel
from Components.Pixmap import Pixmap,MultiPixmap

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
		</screen>"""
		
	def __init__(self,session,testitem):
		Screen.__init__(self, session)
		self.testserver = "8.8.8.8"
		self.testitem = testitem
		self._runing = False
		self.result = False
		
		self["workstatus"] = StaticText("Check the network cable is connected,Press OK\n key Start Test")
		self["testinfo"] = StaticText("")
		self["teststatus"] = StaticText("Start")
		self["actions"] = ActionMap(["SetupActions", "ColorActions"], 
			{
				"cancel": self.doclose,
				"ok": self.startTest
			})
			
		self.testTimer = eTimer()
		self.testTimer.callback.append(self.pingServer)
		
	def startTest(self):
		print "key green"
		if self._runing == False:
			self["workstatus"].setText("Network Connecting ....")
			self["testinfo"].setText("")
			self.testTimer.start(1000)
		   	
	def doclose(self):
		self.updeteResult()
		self.close()
		
	def updeteResult(self):
		if self.result:
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
		else:
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
			
	def dhcp(self):
		cmd = "ifconfig eth0 down"
		p = os.popen(cmd)
		info = p.read()  
		print "info read",info

	def pingServer(self):
		self.testTimer.stop()
		self._runing = True
		lost_packet = 100
		try:
			cmd = "ping -c 3 %s" % self.testserver
			p = os.popen(cmd)
			info = p.read()
			print "info read",info
			p.close()
			self["testinfo"].setText(info)
		except:
			print "exception"
			print "Network Connection Error!!! Check cable and Hardware"
			self.result = False

		if info == "":
			self["workstatus"].setText("Network Connection Error!!! Check cable and Hardware")
			print "Network Connection Error!!! Check cable and Hardware"
			self.result = False
		else:
			try:
				re_lost_str = '(\d+)% packet loss'
				lost_packet = int(re.search(re_lost_str,info).group(1))
				print "lost package is :",lost_packet
			except:
				self["workstatus"].setText("Network Connection Error!!! Check cable and Hardware")
				print "Network Connection Error!!! Check cable and Hardware"
				self.result = False
			if lost_packet == 100:
				self["workstatus"].setText("Network Connection Error!!! Check cable and Hardware")
				print "Network Connection Error!!! Check cable and Hardware"
				self.result = False
			else:
				self["workstatus"].setText("Network Connection OK")
				print "Network Connection OK"
				self.result = True
		self._runing = False
		return self.result
		
class WifiTest(Screen):
	skin = """
		<screen name="WifiTest" position="220,57" size="840,605" title="WifiTest" flags="wfNoBorder">
			<ePixmap position="0,0" zPosition="-10" size="1100,605" pixmap="DMConcinnity-HD-Transp/menu/setupbg.png" />
			<widget source="global.CurrentTime" render="Label" position="20,20" size="80,25" font="Regular;23" foregroundColor="black" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Default</convert>
			</widget>
			<widget source="global.CurrentTime" render="Label" position="110,20" size="140,25" font="Regular;23" foregroundColor="blue" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Format:%d.%m.%Y</convert>
			</widget>
			<eLabel text="Network test" position="270,20" size="540,43" font="Regular;35" halign="right" foregroundColor="black" backgroundColor="grey" transparent="1" />

			<widget name="ConfigWifiText" position="70,100" size="400,25" zPosition="1" font="Regular;22" backgroundColor="transpBlack" transparent="1" />
			<widget name="ConfigTestInfo" position="70,130" size="600,25" zPosition="1" font="Regular;22" foregroundColors="#8c8c93,#f23d21,#1cff1c" backgroundColor="transpBlack" transparent="1" />
			<widget name="ConfigTest_OK" position="730,100" size="35,27" pixmaps="DMConcinnity-HD-Transp/buttons/green.png,DMConcinnity-HD-Transp/buttons/red.png" zPosition="2" alphatest="blend" />
			<eLabel position="70,168" size="700,2" backgroundColor="darkgrey" />

			<widget name="DhcpText" position="70,180" size="400,25" zPosition="1" font="Regular;22" backgroundColor="transpBlack" transparent="1" />
			<widget name="DhcpTestInfo" position="70,210" size="600,55" zPosition="1" font="Regular;22" foregroundColors="#8c8c93,#f23d21,#1cff1c" backgroundColor="transpBlack" transparent="1" />
			<widget name="DhcpTest_OK" position="730,180" size="35,27" pixmaps="DMConcinnity-HD-Transp/buttons/green.png,DMConcinnity-HD-Transp/buttons/red.png" zPosition="2" alphatest="blend" />
			<eLabel position="70,278" size="700,2" backgroundColor="darkgrey" />

			<widget name="connectText" position="70,290" size="400,25" zPosition="1" font="Regular;22" backgroundColor="transpBlack" transparent="1" />
			<widget name="connectTestInfo" position="70,320" size="600,25" zPosition="1" font="Regular;22" foregroundColors="#8c8c93,#f23d21,#1cff1c" backgroundColor="transpBlack" transparent="1" />
			<widget name="connectTest_OK" position="730,290" size="35,27" pixmaps="DMConcinnity-HD-Transp/buttons/green.png,DMConcinnity-HD-Transp/buttons/red.png" zPosition="2" alphatest="blend" />
			<eLabel position="70,358" size="700,2" backgroundColor="darkgrey" />

			<widget name="opreateInfo" position="170,450" size="400,200" zPosition="1" font="Regular;22" foregroundColors="#8c8c93,#f23d21,#1cff1c" backgroundColor="transpBlack" transparent="1" />
    	</screen>"""
		
	def __init__(self,session,testitem,testiface):
		Screen.__init__(self, session)
		self.testitem = testitem
		self._runing = False
		self.result = False
		self.ipConsole = Console()
		self.testiface = testiface
		self.testitem = testitem
		
		self.interfacename = "/etc/network/interfaces"
		self.interfacebackupname = "/etc/network/interfacesbackup"
		self.wlanconfigname = "/etc/wpa_supplicant.wlan0.test.conf"
		
		self.testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
		
		self.creatLables()
		self.teststep = 0
		self.retest = False
		self["actions"] = ActionMap(["SetupActions", "ColorActions"], 
			{
				"cancel": self.doclose,
				"ok": self.startTest
			})
			
		self.testTimer = eTimer()
		self.testTimer.callback.append(self.doTest)
		
	def creatLables(self):
		if iNetwork.isWirelessInterface(self.testiface):
			self["ConfigWifiText"] = MultiColorLabel(_("WLAN connection config"))#
		else:
			self["ConfigWifiText"] = MultiColorLabel(_("LAN connection config"))#
			
		self["ConfigTestInfo"] = MultiColorLabel()#Teststatus
#		self["ConfigTestInfo"] = StaticText("")
		self["ConfigTest_OK"] = MultiPixmap()#testicon
		self["ConfigTest_OK"].hide()

		self["DhcpText"] = MultiColorLabel(_("DHCP"))#
		self["DhcpTestInfo"] = MultiColorLabel()#Teststatus
		self["DhcpTest_OK"] = MultiPixmap()#testicon
		self["DhcpTest_OK"].hide()

		self["connectText"] = MultiColorLabel(_("Network Connection Test"))#
		self["connectTestInfo"] = MultiColorLabel()#Teststatus
		self["connectTest_OK"] = MultiPixmap()#testicon
		self["connectTest_OK"].hide()

		self["opreateInfo"] = Label(_("Press 'OK' start"))
		
	def initLables(self):
		self.checkip = False
		self.teststep = 0
		self["ConfigTestInfo"].setText(_(" "))
		self["ConfigTestInfo"].setForegroundColorNum(0)
		self["ConfigTest_OK"].hide()
		
		self["DhcpTestInfo"].setText(_(" "))
		self["DhcpTestInfo"].setForegroundColorNum(0)
		self["DhcpTest_OK"].hide()
		
		self["connectTestInfo"].setText(_(" "))
		self["connectTestInfo"].setForegroundColorNum(0)
		self["connectTest_OK"].hide()
		self["opreateInfo"].setText(_("please wait,network testing ....."))
		
	def doclose(self):
		if self._runing == False:
#			self.updeteResult()
			self.close()
#			self.testTimer = eTimer()
#			self.testTimer.callback.append(self.pingServer)

	def startTest(self):
		if self._runing == False:
			self._runing = True
			self.initLables()
			self.doTest()
		else:
			pass
			
	def doTest(self):
		print "timer runin doTest()"
		self.testTimer.stop()
		self.runTest()
				
	def runTest(self):
		print "tun in runTest()"
		step = self.teststep
		if step == 0:
			self.test()
		elif step == 1:
			self["ConfigTestInfo"].setText(_("config Wifi network,please wait..."))
			self["ConfigTestInfo"].setForegroundColorNum(0)
			self.test()
		elif step == 2:
			self["DhcpTestInfo"].setText(_("dhcp get ip address,please wait..."))
			self["DhcpTestInfo"].setForegroundColorNum(0)
			self.test()
		elif step == 3:
			self["connectTestInfo"].setText(_("ping network server,please wait..."))
			self["connectTestInfo"].setForegroundColorNum(0)
			self.test()
		else:
			self._runing = False
			self.teststep = 0
		self.teststep += 1

	def test(self):
		if self.teststep == 0:
			self.testTimer.start(100)
		if self.teststep == 1:
			self.doNetworkconfig()
		if self.teststep == 2:
			self.checkipaddr()
		if self.teststep == 3:
			self.pingserver()
		else:
			pass
			
	def doNetworkconfig(self):		
		if iNetwork.isWirelessInterface(self.testiface):
			#back up /etc/network/interfaces
			ret = os.system("cp "+self.interfacename+" "+self.interfacebackupname)
			if ret != 0:
				print "backup interfaces file fail ret:\n",ret
				self["ConfigTestInfo"].setText("backup file(interfaces) fail,Wifi test stop!!")
				self["ConfigTestInfo"].setForegroundColorNum(1)
				self["ConfigTest_OK"].setPixmapNum(1)
				self["ConfigTest_OK"].show()
				return "backup interfaces file fail"
				
			#config interfaces
			self.writeTestInterfaces()
			
			interfaces = ["eth0"]
			self.writeWifiConfig(self.testiface)
			iNetwork.deactivateInterface(interfaces,self.activeNetworkCB)
		else:
			interfaces = ["wlan0"]
			iNetwork.deactivateInterface(interfaces,self.activeNetworkCB)
#			self.doLanconfig(true)

	def writeTestInterfaces(self):
		fp = file(self.interfacename, 'w')
		fp.write("# automatically generated by enigma 2\n# do NOT change manually!\n\n")
		fp.write("auto lo\n")
		fp.write("iface lo inet loopback\n\n")
		fp.write("iface wlan0 inet dhcp\n")
		fp.write("\tpre-up wpa_supplicant -i wlan0 -c /etc/wpa_supplicant.wlan0.test.conf -B -dd -D wext || true\n")
		fp.write("\tpre-down wpa_cli -i wlan0 terminate || true\n\n")
		fp.write("auto eth0\n")
		fp.write("iface eth0 inet dhcp\n")
		fp.close()
				
	def getWlanConfigName(self,iface):
		return self.wlanconfigname
		
	def writeWifiConfig(self, iface):
		print "do write wifi config(wpa_supplicant.wlan.config)..."
	
		essid = "wifi-test"
		hiddenessid = False
		encrypted = True
		encryption = "WPA/WPA2"
		wepkeytype = "ASCII"
		psk = "12345678"
		fp = file(self.getWlanConfigName(iface), 'w')
		fp.write('#WPA Supplicant Configuration by enigma2\n')
		fp.write('ctrl_interface=/var/run/wpa_supplicant\n')
		fp.write('eapol_version=1\n')
		fp.write('fast_reauth=1\n')	

		if hiddenessid:
			fp.write('ap_scan=2\n')
		else:
			fp.write('ap_scan=1\n')
		fp.write('network={\n')
		fp.write('\tssid="'+essid+'"\n')
		fp.write('\tscan_ssid=0\n')			
		if encrypted:
			if encryption in ('WPA', 'WPA2', 'WPA/WPA2'):
				fp.write('\tkey_mgmt=WPA-PSK\n')
		
				if encryption == 'WPA':
					fp.write('\tproto=WPA\n')
					fp.write('\tpairwise=TKIP\n')
					fp.write('\tgroup=TKIP\n')
				elif encryption == 'WPA2':
					fp.write('\tproto=RSN\n')
					fp.write('\tpairwise=CCMP\n')
					fp.write('\tgroup=CCMP\n')
				else:
					fp.write('\tproto=WPA RSN\n')
					fp.write('\tpairwise=CCMP TKIP\n')
					fp.write('\tgroup=CCMP TKIP\n')
				fp.write('\tpsk="'+psk+'"\n')
			elif encryption == 'WEP':
				fp.write('\tkey_mgmt=NONE\n')
				if wepkeytype == 'ASCII':
					fp.write('\twep_key0="'+psk+'"\n')
				else:
					fp.write('\twep_key0='+psk+'\n')
		else:
			fp.write('\tkey_mgmt=NONE\n')			
		fp.write('}')
		fp.write('\n')
		fp.close()
		#system('cat ' + getWlanConfigName(iface))
		
	def activeNetwork(self,data):
		if data is True:
			iNetwork.activateInterface(self.testiface,self.activeNetworkCB)
			
	def activeNetworkCB(self,data):
		if data is True:
			print "active network done"
			self["ConfigTestInfo"].setText("Network config ok")
			self["ConfigTestInfo"].setForegroundColorNum(2)
			self["ConfigTest_OK"].setPixmapNum(0)
			self["ConfigTest_OK"].show()
			self.testTimer.start(500)
		else:
			print "active network fail"
	#####DHCP
	def checkipaddr(self):
		cmd = "ifconfig " + self.testiface
		self.ipConsole.ePopen(cmd, self.checkipaddrCB)

	def checkipaddrCB(self, result, retval, extra_args):
		ipadd = None
		for line in result.splitlines():
			line = line.strip()
			if "inet addr" in line:
				ipadd = line
				self["DhcpTestInfo"].setText(ipadd)
				self["DhcpTestInfo"].setForegroundColorNum(2)
				
				self["DhcpTest_OK"].setPixmapNum(0)
				self["DhcpTest_OK"].show()
				self.testTimer.start(1000)
				break
		if ipadd is None:
			if self.checkip is False:
				cmds = []
				cmds.append("ifdown " + self.testiface)
				cmds.append("ip addr flush dev " + self.testiface)
				# HACK: wpa_supplicant sometimes doesn't quit properly on SIGTERM
				if os_path.exists('/var/run/wpa_supplicant/'+ self.testiface):
					cmds.append("wpa_cli -i" + self.testiface + " terminate")
					
				cmds.append("ifup " + self.testiface)
				self.ipConsole.eBatch(cmds, self.getipaddrCB)
			else:
				self["DhcpTestInfo"].setText("Get ip fail! ")
				self["DhcpTestInfo"].setForegroundColorNum(1)
				self["DhcpTest_OK"].setPixmapNum(1)
				self["DhcpTest_OK"].show()	
				self.testTimer.start(1000)
				
	def getipaddrCB(self, result, retval=None, extra_args=None):
		self.checkip = True
		cmd = "ifconfig " + self.testiface
		self.ipConsole.ePopen(cmd, self.checkipaddrCB)

	def pingserver(self):
		iNetwork.checkNetworkState(self.checkNetworkCB)

	def checkNetworkCB(self,data):
		if data <= 2:#OK
			self["connectTestInfo"].setText(_("Network connection ok "))
			self["connectTestInfo"].setForegroundColorNum(2)
			self["connectTest_OK"].setPixmapNum(0)
			self["connectTest_OK"].show()
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
			self["opreateInfo"].setText(_("Press 'OK' restart     Press 'Exit' exit"))
			self._runing = False
		else:
			self["connectTestInfo"].setText(_("ping server fail! check network and check hardware!"))
			self["connectTestInfo"].setForegroundColorNum(1)
			self["connectTest_OK"].setPixmapNum(1)
			self["connectTest_OK"].show()
			self["opreateInfo"].setText(_("Press 'OK' restart     Press 'Exit' exit"))
			self._runing = False
		if iNetwork.isWirelessInterface(self.testiface):
			os.system("mv "+self.interfacebackupname+" "+self.interfacename)
			os.system("rm "+self.wlanconfigname)
		
def checkWifiIface():
	wiressface = []
	interfaces = iNetwork.getAdapterList()
	for face in interfaces:
		if iNetwork.isWirelessInterface(face):
			wiressface.append(face)
	return wiressface

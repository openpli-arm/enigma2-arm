from Plugins.Plugin import PluginDescriptor

from Components.ActionMap import ActionMap, NumberActionMap
from Components.Input import Input
from Components.Sources.StaticText import StaticText
from Components.ScrollLabel import ScrollLabel
from Components.Pixmap import Pixmap
from Components.MenuList import MenuList
from Components.Sources.List import List

from Screens.About import About
from Screens.Screen import Screen
from Screens.Ci import MMIDialog

from Components.PluginList import *

from FactoryTestPublic import *

from LnbTest import *
from os import listdir
import time

from LedTest import LedTest
from EepromTest import EepromTest
from NetworkTest import WifiTest
from FrontPanelTest import FrontPanelTest
from Rs232Test import Rs232Test
from SDCardTest import SDCardTest
from UsbTest import *
from Plugins.Extensions.OscamStatus.plugin import StatusDataScreen
from Plugins.Extensions.OscamStatus.OscamStatusSetup import readCFG,LASTSERVER
from Components.Network import iNetwork
from enigma import eDVBCI_UI

class FactoryTest:
	"""Factory test class"""
	
	#define Factory test module

	#test led display, dispaly all,then display blank
	FACTORYTEST_LED = 0
	FACTORYTEST_FRONTPANEL = 1
	FACTORYTEST_USB = 2
	FACTORYTEST_SDCARD = 3
	FACTORYTEST_LNB = 4
	FACTORYTEST_DISEQC = 5
	FACTORYTEST_LNB2 = 6
	FACTORYTEST_DISEQC2 = 7
	FACTORYTEST_LNBT = 8
	FACTORYTEST_EEPROM = 9
	FACTORYTEST_NETWORK = 10
	FACTORYTEST_WIFI = 11
	FACTORYTEST_CA = 12
	FACTORYTEST_CI = 13
	FACTORYTEST_RS232 = 14
	
	retest = True
	
	def __init__(self,testlist,session):
		self.factoryTestList = testlist
		self.session = session
		self.path = "/usr/lib/enigma2/python/Plugins/Extensions/FactoryTest"
		
	def test(self,testitem):
		currentitem = self.factoryTestList.index(testitem)
#		print  "[FactoryTest]->currentitem:",currentitem
		resultstring = None
		
		if testitem.testType == FactoryTest.FACTORYTEST_LED:
			ledtestinstance = LedTest()
			resultstring = ledtestinstance.test(FactoryTest.retest)
			print resultstring
			if ledtestinstance.checkFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)
			return resultstring
			
		elif testitem.testType == FactoryTest.FACTORYTEST_FRONTPANEL:
			self.session.open(FrontPanelTest,testitem,self.path)
			return "FrontPanel Test Finish"
			
		elif testitem.testType == FactoryTest.FACTORYTEST_USB:
			self.session.open(UsbTest,testitem)
			return "USB Tested"
				
		elif testitem.testType == FactoryTest.FACTORYTEST_SDCARD:
			if checkUsbAvailable("/dev/mmcblk0"):
				self.session.open(SDCardTest,testitem)
				return "SDCard OK"
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
				print "input device"
				return "Check SDCard and hardware"
				
		elif testitem.testType == FactoryTest.FACTORYTEST_LNB:
			lnbTestInstance = LnbTest(0)
			resultstring =lnbTestInstance.testVoltage(FactoryTest.retest)
			
			if lnbTestInstance.checkVoltageTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)
			if resultstring == "Fail,Check hardware!!":
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
			return resultstring
				
		elif testitem.testType == FactoryTest.FACTORYTEST_LNB2:
			lnbTestInstance = LnbTest(1)
			resultstring =lnbTestInstance.testVoltage(FactoryTest.retest)
			
			if lnbTestInstance.checkVoltageTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)
			if resultstring == "Fail,Check hardware!!":
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
			return resultstring
			
		elif testitem.testType == FactoryTest.FACTORYTEST_DISEQC:
			diseqcTestInstance = LnbTest(0)
			resultstring =diseqcTestInstance.testDiSEqC(FactoryTest.retest)
			
			if diseqcTestInstance.checkDiSEqCTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)
			if resultstring == "Fail,Check hardware!!":
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
			return resultstring
			
		elif testitem.testType == FactoryTest.FACTORYTEST_DISEQC2:
			diseqcTestInstance = LnbTest(1)
			resultstring =diseqcTestInstance.testDiSEqC(FactoryTest.retest)
			
			if diseqcTestInstance.checkDiSEqCTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)
			if resultstring == "Fail,Check hardware!!":
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
			return resultstring
			
		elif testitem.testType == FactoryTest.FACTORYTEST_LNBT:
			lnbTestInstance = LnbTest(2)
			resultstring =lnbTestInstance.testVoltage(FactoryTest.retest)
			
			if lnbTestInstance.checkVoltageTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)
			if resultstring == "Fail,Check hardware!!":
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
			return resultstring	
			
		elif testitem.testType == FactoryTest.FACTORYTEST_EEPROM:
			if EepromTest().checkEeprom():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
				return "Eeprom OK"
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
				return "Eeprom test Error!! Check hardware"
				
		elif testitem.testType == FactoryTest.FACTORYTEST_CA:
#			self.session.open(StatusDataScreen)
			oscamServers = readCFG()
			index = LASTSERVER.value
			if index+1 > len(oscamServers):
				index = 0
			self.session.open(StatusDataScreen, "readers", "status", oscamServers[index])
			testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			return "CA cardreader test finish"
			
		elif testitem.testType == FactoryTest.FACTORYTEST_CI:
#			oscamServers = readCFG()
#			index = LASTSERVER.value
#			if index+1 > len(oscamServers):
#				index = 0
#			self.session.open(StatusDataScreen, "readers", "status", oscamServers[index])
#			testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
#			return "CI cardreader test finish"		
			if -1 != eDVBCI_UI.getInstance().getState(1):
				self.session.open(MMIDialog, 0, 2)
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
				return "CI cardreader test finish"	
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
				return "check hardware!!!"

		elif testitem.testType == FactoryTest.FACTORYTEST_RS232:
#			print "test RS232"
			rs232 = Rs232Test()
			if rs232.StartTest():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
				return "RS232 OK"
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
				return "RS232 test Error!! Check hardware"
				
		elif testitem.testType == FactoryTest.FACTORYTEST_NETWORK:
			self.session.open(WifiTest,testitem,"eth0")
			return "Network test finish"
			
		elif testitem.testType == FactoryTest.FACTORYTEST_WIFI:
			interfaces = listdir('/sys/class/net')
			if "wlan0" in interfaces:
				self.session.open(WifiTest,testitem,"wlan0")
				return "Wi-fi OK"
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
				return "Check hardware"
		else:
			return resultstring
		


#Factory Test interface
class FactoryTestMenu(Screen):
	skin = """
		<screen name="FactoryTestMenu" position="220,57" size="840,605" title="Factory test" flags="wfNoBorder">
			<ePixmap position="0,0" zPosition="-10" size="1100,605" pixmap="DMConcinnity-HD-Transp/menu/setupbg.png" />
			<widget source="global.CurrentTime" render="Label" position="20,20" size="80,25" font="Regular;23" foregroundColor="black" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Default</convert>
			</widget>
			<widget source="global.CurrentTime" render="Label" position="110,20" size="140,25" font="Regular;23" foregroundColor="blue" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Format:%d.%m.%Y</convert>
			</widget>
		    <eLabel text="Factory test" position="270,20" size="540,43" font="Regular;35" halign="right" foregroundColor="black" backgroundColor="grey" transparent="1" />
			<widget name="list" position="330,90" size="480,450" scrollbarMode="showOnDemand" backgroundColor="transpBlack" selectionPixmap="DMConcinnity-HD-Transp/menu/sel800x50.png" transparent="1" />
			<widget source="infotitle" render="Label" position="40,90" size="250,35" font="Regular;26" foregroundColor="yellow" backgroundColor="transpBlack" transparent="1" />
			<widget source="testinfo" render="Label" position="40,150" size="250,380" font="Regular;24" backgroundColor="transpBlack" transparent="1" />
  		</screen>"""
#<ePixmap pixmap="DMConcinnity-HD-Transp/buttons/green.png" position="30,570" size="35,27" alphatest="blend" />
#<eLabel text="TestReport" position="80,573" size="200,26" zPosition="1" font="Regular;22" halign="left" foregroundColor="black" backgroundColor="grey" transparent="1" />	

	def __init__(self, session):
		Screen.__init__(self, session)
		self.plug_path = "/usr/lib/enigma2/python/Plugins/Extensions/FactoryTest"
		print "path:",self.plug_path
		
		self.list = []
		self["list"] = PluginList(self.list)
		self.creatTestList() #creat testlist item
		self.testinfo = None
		self.lastSelect = None
		self["infotitle"] =	StaticText("Test Info:")
		self["testinfo"] = StaticText("Select list press OK start test")
		
		self.testInstance = FactoryTest(self.testlist,session)
		
		#map actions
		self["actions"] = ActionMap(["SetupActions", "ColorActions"], 
			{
				"cancel": self.close,
				"ok": self.goTest,
				"green": self.close
			})
			
		self.onShown.append(self.updateList)
		self.onChangedEntry = []
		self["list"].onSelectionChanged.append(self.selectionChanged)

#create test list 		
	def creatTestList(self):
		self.testlist = []
		self.testlist.append(FactoryTestItem("Led Test",FactoryTest.FACTORYTEST_LED,"led.png","display Blank,display White(all)"))
		self.testlist.append(FactoryTestItem("FrontPanel Test",FactoryTest.FACTORYTEST_FRONTPANEL,"frontpanel.png","jump to frontpanel test menu"))
		self.testlist.append(FactoryTestItem("USB Test",FactoryTest.FACTORYTEST_USB,"usb.png","in put usb"))
		self.testlist.append(FactoryTestItem("SDCard Test",FactoryTest.FACTORYTEST_SDCARD,"sd.png","in put SDCard"))
		self.testlist.append(FactoryTestItem("LNB Test",FactoryTest.FACTORYTEST_LNB,"lnb.png","13V,18V"))
		self.testlist.append(FactoryTestItem("Diseqc Test",FactoryTest.FACTORYTEST_DISEQC,"diseqc.png","port1,port2,port3,port4"))
		self.testlist.append(FactoryTestItem("LNB2 Test",FactoryTest.FACTORYTEST_LNB2,"lnb.png","13V,18V"))
		self.testlist.append(FactoryTestItem("Diseqc2 Test",FactoryTest.FACTORYTEST_DISEQC2,"diseqc.png","port1,port2,port3,port4"))
		self.testlist.append(FactoryTestItem("DVB-T Test",FactoryTest.FACTORYTEST_LNBT,"lnb.png","5V on,off"))
		self.testlist.append(FactoryTestItem("Eeprom Test",FactoryTest.FACTORYTEST_EEPROM,"eeprom.png","Eeprom Test"))
		self.testlist.append(FactoryTestItem("Network Test",FactoryTest.FACTORYTEST_NETWORK,"wired.png","jump network Test"))
		self.testlist.append(FactoryTestItem("Wi-Fi Test",FactoryTest.FACTORYTEST_WIFI,"wireless.png","jump wireless network Test"))
		
		self.testlist.append(FactoryTestItem("CA Test",FactoryTest.FACTORYTEST_CA,"ca.png","CA oscam status.."))
		self.testlist.append(FactoryTestItem("CI Test",FactoryTest.FACTORYTEST_CI,"ca.png","CI card status.."))
		self.testlist.append(FactoryTestItem("RS232 Test",FactoryTest.FACTORYTEST_RS232,"rs232.png","press again after finish Test"))
		
#update test result ,menu Refresh
	def updateList(self):
		self.list = [FactoryTestEntryComponent(listitem,self.plug_path) for listitem in self.testlist]
		self["list"].l.setList(self.list)

#after press OK key start test item
	def goTest(self):
		currentitem = self["list"].l.getCurrentSelection()[0]
#		print currentitem
		if currentitem != self.lastSelect:
			self.lastSelect = currentitem
			FactoryTest.retest = True
			print "test new item"
		else:
			FactoryTest.retest = False
			print "test nextstape"
		self.testinfo = self.testInstance.test(currentitem)
		self["testinfo"].setText(self.testinfo)
		self.updateList()
		

#display menu text on Led
	def createSummary(self):
		from Screens.PluginBrowser import PluginBrowserSummary
		return PluginBrowserSummary
		
	def selectionChanged(self):
		item = self["list"].getCurrent()
		if item:
			p = item[0]
			name = p.itemDisplayName
			desc = p.description
		else:
			name = "-"
			desc = ""
		for cb in self.onChangedEntry:
			cb(name, desc)

			
#plugin 			
#def factoryTestStart(session, **kwargs):
#	session.open(FactoryTestMenu)
	
#def main(menuid, **kwargs):
#	if menuid == "mainmenu":
#		return [(_("Factory Test"), factoryTestStart, "factory_test", 50)]
#
#	return []

#def Plugins(path,**kwargs):
#	global plugin_path
#	plugin_path = path
#	return PluginDescriptor(name = "FactoryTest",description = "plugin for factory test in produce", where = PluginDescriptor.WHERE_MENU, needsRestart = False, fnc = main)
#	return PluginDescriptor(name = "FactoryTest",description = "plugin for factory test in produce", where = PluginDescriptor.WHERE_MENU, needsRestart = False, fnc = main)


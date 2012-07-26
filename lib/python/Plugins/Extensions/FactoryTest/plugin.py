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

from Components.PluginList import *

from FactoryTestPublic import *

from LnbTest import *

import time

from LedTest import LedTest
from EepromTest import EepromTest
from NetworkTest import NetworkTest
from FrontPanelTest import FrontPanelTest

from Plugins.Extensions.OscamStatus.plugin import *
from Plugins.Extensions.OscamStatus.OscamStatusSetup import readCFG,LASTSERVER

#Factory Test class


class FactoryTest:
	"""Factory test class"""
	
	#define Factory test module

	#test led display, dispaly all,then display blank
	FACTORYTEST_LED = 0
	FACTORYTEST_FRONTPANEL = 1
	FACTORYTEST_USB = 2
	FACTORYTEST_LNB = 3
	FACTORYTEST_DISEQC = 4
	FACTORYTEST_LNB2 = 5
	FACTORYTEST_DISEQC2 = 6
	FACTORYTEST_EEPROM = 7
	FACTORYTEST_RS232 = 9
	FACTORYTEST_NETWORK = 10
	FACTORYTEST_CA = 11
	retest = True
	
	def __init__(self,testlist,session):
		self.factoryTestList = testlist
		self.session = session
		self.path = "/usr/lib/enigma2/python/Plugins/SystemPlugins/FactoryTest"
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
				
		elif testitem.testType == FactoryTest.FACTORYTEST_FRONTPANEL:
			self.session.open(FrontPanelTest,testitem,self.path)
			
		elif testitem.testType == FactoryTest.FACTORYTEST_USB:
			from UsbTest import *
			if checkUsbAvailable("/dev/sda"):
				self.session.open(UsbTest,testitem)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
				print "input device"
				
		elif testitem.testType == FactoryTest.FACTORYTEST_LNB:
			lnbTestInstance = LnbTest(0)
			resultstring =lnbTestInstance.testVoltage(FactoryTest.retest)
			
			if lnbTestInstance.checkVoltageTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)
				
		elif testitem.testType == FactoryTest.FACTORYTEST_LNB2:
			lnbTestInstance = LnbTest(1)
			resultstring =lnbTestInstance.testVoltage(FactoryTest.retest)
			
			if lnbTestInstance.checkVoltageTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)
				
		elif testitem.testType == FactoryTest.FACTORYTEST_DISEQC:
			diseqcTestInstance = LnbTest(0)
			resultstring =diseqcTestInstance.testDiSEqC(FactoryTest.retest)
			
			if diseqcTestInstance.checkDiSEqCTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)

		elif testitem.testType == FactoryTest.FACTORYTEST_DISEQC2:
			diseqcTestInstance = LnbTest(1)
			resultstring =diseqcTestInstance.testDiSEqC(FactoryTest.retest)
			
			if diseqcTestInstance.checkDiSEqCTestFinish():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_TESTED)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_NOFINISH)				
		elif testitem.testType == FactoryTest.FACTORYTEST_EEPROM:
			if EepromTest().checkEeprom():
				testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
				
		elif testitem.testType == FactoryTest.FACTORYTEST_CA:
#			self.session.open(StatusDataScreen)
			oscamServers = readCFG()
			index = LASTSERVER.value
			if index+1 > len(oscamServers):
				index = 0
			self.session.open(StatusDataScreen, "readers", "status", oscamServers[index])
		elif testitem.testType == FactoryTest.FACTORYTEST_RS232:
			print "test"
#			recive = "test\n"
			recive = raw_input()
			fd = open("/retest",'w')
			fd.write(recive)
			fd.close()
			if recive == "test":
				testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
			else:
				testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
				
		elif testitem.testType == FactoryTest.FACTORYTEST_NETWORK:
			NetworkTest().monitoring()
		else:
			pass
		return self.factoryTestList
		
	def reportResult():
		pass
	


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
			<widget name="list" position="30,90" size="780,450" scrollbarMode="showOnDemand" backgroundColor="transpBlack" selectionPixmap="DMConcinnity-HD-Transp/menu/sel800x50.png" transparent="1" />

			<ePixmap pixmap="DMConcinnity-HD-Transp/buttons/green.png" position="30,570" size="35,27" alphatest="blend" />
			<eLabel text="TestReport" position="80,573" size="200,26" zPosition="1" font="Regular;22" halign="left" foregroundColor="black" backgroundColor="grey" transparent="1" />
  		</screen>"""
  		
	def __init__(self, session):
		Screen.__init__(self, session)
#		global plugin_path = "/usr/lib/enigma2/python/Plugins/SystemPlugins/FactoryTest"
		self.plug_path = "/usr/lib/enigma2/python/Plugins/SystemPlugins/FactoryTest"
		print "path:",self.plug_path
		self.list = []
		self["list"] = PluginList(self.list)
		self.creatTestList() #creat testlist item

		self.lastSelect = None
		
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
		self.testlist.append(FactoryTestItem("Led Test",FactoryTest.FACTORYTEST_LED,"led.png","display Blank,display write(all)"))
		self.testlist.append(FactoryTestItem("FrontPanel Test",FactoryTest.FACTORYTEST_FRONTPANEL,"led.png","press frontpanel key to test.."))
		self.testlist.append(FactoryTestItem("USB Test",FactoryTest.FACTORYTEST_USB,"led.png","in put usb"))
		self.testlist.append(FactoryTestItem("LNB Test",FactoryTest.FACTORYTEST_LNB,"led.png","13V,18V"))
		self.testlist.append(FactoryTestItem("Diseqc Test",FactoryTest.FACTORYTEST_DISEQC,"led.png","port1,port2,port3,port4"))
		self.testlist.append(FactoryTestItem("LNB2 Test",FactoryTest.FACTORYTEST_LNB2,"led.png","13V,18V"))
		self.testlist.append(FactoryTestItem("Diseqc2 Test",FactoryTest.FACTORYTEST_DISEQC2,"led.png","port1,port2,port3,port4"))
		self.testlist.append(FactoryTestItem("Eeprom Test",FactoryTest.FACTORYTEST_EEPROM,"led.png","Eeprom Test"))
#		self.testlist.append(FactoryTestItem("RS232 Test",FactoryTest.FACTORYTEST_RS232,"led.png","RS232 Test"))
		self.testlist.append(FactoryTestItem("Network Test",FactoryTest.FACTORYTEST_NETWORK,"led.png","jump network Test"))
		self.testlist.append(FactoryTestItem("CA Test",FactoryTest.FACTORYTEST_CA,"led.png","CA oscam status.."))
		
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
		self.testlist = self.testInstance.test(currentitem)
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
#	return []
#
#def Plugins(path,**kwargs):
#	global plugin_path
#	plugin_path = path
#	return PluginDescriptor(name = "FactoryTest",description = "plugin for factory test in produce", where = PluginDescriptor.WHERE_MENU, needsRestart = False, fnc = main)


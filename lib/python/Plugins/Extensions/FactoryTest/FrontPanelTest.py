from struct import *
from Screens.MessageBox import MessageBox
from Components.MenuList import MenuList
from Screens.Screen import Screen
from Components.ActionMap import ActionMap, NumberActionMap
from Components.Sources.StaticText import StaticText
from Components.Pixmap import Pixmap

from Components.MultiContent import MultiContentEntryText, MultiContentEntryPixmapAlphaTest
from Tools.LoadPixmap import LoadPixmap
from enigma import eListboxPythonMultiContent, gFont

from FactoryTestPublic import *
from GlobalActions import globalActionMap

class FrontPanelTest(Screen):
	skin = """
		<screen name="FrontPanelTest" position="220,57" size="840,605" title="Front Panel test" flags="wfNoBorder">
			<ePixmap position="0,0" zPosition="-10" size="1100,605" pixmap="DMConcinnity-HD-Transp/menu/setupbg.png" />
			<widget source="global.CurrentTime" render="Label" position="20,20" size="80,25" font="Regular;23" foregroundColor="black" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Default</convert>
			</widget>
			<widget source="global.CurrentTime" render="Label" position="110,20" size="140,25" font="Regular;23" foregroundColor="blue" backgroundColor="grey" transparent="1">
				<convert type="ClockToText">Format:%d.%m.%Y</convert>
			</widget>
		    <eLabel text="Led test" position="270,20" size="540,43" font="Regular;35" halign="right" foregroundColor="black" backgroundColor="grey" transparent="1" />
		    <widget name="chuppic" position="190,100" size="500,35" alphatest="blend" />
		    <widget name="chdownpic" position="190,140" size="500,35" alphatest="blend" />
		    <widget name="voluppic" position="190,180" size="500,35" alphatest="blend" />
		    <widget name="voldownpic" position="190,220" size="500,35" alphatest="blend" />
		    <widget name="menupic" position="190,260" size="500,35" alphatest="blend" />
		    <widget name="okpic" position="190,300" size="500,35" alphatest="blend" />
  		</screen>"""
#  	<widget source="text" render="Label" position="100,300" size="800,100" font="Regular;18" transparent="1" />
#  	<widget name="correctpic" pixmap="%s" position="50,300" zPosition="10" size="150,150" transparent="1" alphatest="on"/>
#	<widget name="list" position="100,400" size="600,200" scrollbarMode="showOnDemand" backgroundColor="transpBlack" selectionPixmap="DMConcinnity-HD-Transp/menu/sel800x50.png" transparent="1" />	
#	<widget name="okpic" position="190,155" size="500,35" alphatest="blend" />
	def __init__(self,session,testitem,ppath):
		Screen.__init__(self, session)
		self.testitem = testitem
		self.testitem.setTestResult(FactoryTestItem.TESTRESULT_ERROR)
		self.pluginpath = ppath
		self["chuppic"] = Pixmap()
		self["chdownpic"] = Pixmap()
		self["voluppic"] = Pixmap()
		self["voldownpic"] = Pixmap()
		self["menupic"] = Pixmap()
		self["okpic"] = Pixmap()
		self.testresult = [0]*6
		
		self["actions"] = ActionMap(["FrontPanelTest"], 
			{
				"cancel": self.closemenu,
				"panelchup":self.chuptest,
				"panelchdown":self.chdowtest,
				"panelvolup":self.voluptest,
				"panelvoldown":self.voldowntest,
				"panelmenu":self.menutest,
				"panelok":self.oktest
			})
		globalActionMap.execEnd()	
		
		self.onLayoutFinish.append(self.layoutFinished)
		
	def layoutFinished(self):
		self.iconpath = self.pluginpath +"/icons/chup.png"
		self["chuppic"].instance.setPixmapFromFile(self.iconpath)
		
		self.iconpath = self.pluginpath +"/icons/chdown.png"
		self["chdownpic"].instance.setPixmapFromFile(self.iconpath)
		
		self.iconpath = self.pluginpath +"/icons/volup.png"
		self["voluppic"].instance.setPixmapFromFile(self.iconpath)
		
		self.iconpath = self.pluginpath +"/icons/voldown.png"
		self["voldownpic"].instance.setPixmapFromFile(self.iconpath)
		
		self.iconpath = self.pluginpath +"/icons/menu.png"
		self["menupic"].instance.setPixmapFromFile(self.iconpath)
		
		self.iconpath = self.pluginpath +"/icons/okpic.png"
		self["okpic"].instance.setPixmapFromFile(self.iconpath)
		
	def chuptest(self):
		print "pressed ch+"
		self.testresult[0] = 1
		self.iconpath = self.pluginpath +"/icons/chupok.png"
		self["chuppic"].instance.setPixmapFromFile(self.iconpath)
		self.checkResult()
		
	def chdowtest(self):
		print "pressed ch-"
		self.testresult[1] = 1
		self.iconpath = self.pluginpath +"/icons/chdownok.png"
		self["chdownpic"].instance.setPixmapFromFile(self.iconpath)
		self.checkResult()
		
	def voluptest(self):
		print "pressed vol+"
		self.testresult[2] = 1
		self.iconpath = self.pluginpath +"/icons/volupok.png"
		self["voluppic"].instance.setPixmapFromFile(self.iconpath)
		self.checkResult()
		
	def voldowntest(self):
		print "pressed vol-"
		self.testresult[3] = 1
		self.iconpath = self.pluginpath +"/icons/voldownok.png"
		self["voldownpic"].instance.setPixmapFromFile(self.iconpath)
		self.checkResult()
		
	def menutest(self):
		print "pressed menu"
		self.testresult[4] = 1
		self.iconpath = self.pluginpath +"/icons/menuok.png"
		self["menupic"].instance.setPixmapFromFile(self.iconpath)
		self.checkResult()
		
	def oktest(self):
		print "pressed ok"
		self.testresult[5] = 1
		self.iconpath = self.pluginpath +"/icons/okpicok.png"
		self["okpic"].instance.setPixmapFromFile(self.iconpath)
		self.checkResult()
		
	def checkResult(self):
		if self.testresult == [1]*6:
			self.testitem.setTestResult(FactoryTestItem.TESTRESULT_OK)
			self.closemenu()
			
	def	closemenu(self):
		globalActionMap.execBegin()
		self.close()
		
	def setText(self, text):
		self["text"].setText(text)
		
			

from Components.MultiContent import MultiContentEntryText, MultiContentEntryPixmapAlphaTest
from Tools.LoadPixmap import LoadPixmap

#Test List Unit,sava test infomation
class FactoryTestItem:
	"""factory test list item"""
	#define test result
	#result:
	TESTRESULT_NOTEST = 0
	TESTRESULT_OK = 1
	TESTRESULT_ERROR = 2
	TESTRESULT_TESTED = 3
	TESTRESULT_NOFINISH = 4
	
	def __init__(self,itemname,testtype,indexicon,description = "",teststates = TESTRESULT_NOTEST):
		self.itemDisplayName = itemname
		self.testType = testtype
		self.description = description
		self.testResult = teststates
		self.indexIcon = indexicon
		
	def setTestResult(self,result):
		if result not in [0,1,2,3,4]:
			print "Invalid value"
		else:
			self.testResult = result

#style
def FactoryTestEntryComponent(listitem,path,width = 700):
	pngpath = path + "/icons/"+listitem.indexIcon
	indexpng = LoadPixmap(pngpath)
	if listitem.testResult == FactoryTestItem.TESTRESULT_OK or listitem.testResult == FactoryTestItem.TESTRESULT_TESTED:
#		print "teststates is OK"
		stateiconpath = path + "/icons/ok.png"
		stateicon = LoadPixmap(stateiconpath)
	elif listitem.testResult == FactoryTestItem.TESTRESULT_ERROR:
#		print "teststates is Error"
		stateiconpath = path + "/icons/error.png"
		stateicon = LoadPixmap(stateiconpath)
	else:
#		print "teststates is no Tested"
		stateicon = None
	
	return [
		listitem,
		MultiContentEntryText(pos=(120, 5), size=(300, 25), font=0, text=listitem.itemDisplayName),
		MultiContentEntryText(pos=(120, 26), size=(300, 17), font=1, text=listitem.description),
		MultiContentEntryPixmapAlphaTest(pos=(10, 5), size=(100, 40), png = indexpng),
		MultiContentEntryPixmapAlphaTest(pos=(600, 5), size=(40, 40), png = stateicon)
	]

def FactoryTestResultComponent(result,path,width = 700):
	if(result):
		pngpath = path + "/icons/"+"ok.png"
		indexpng = LoadPixmap(pngpath)
	else:
		pngpath = path + "/icons/"+"error.png"
		indexpng = LoadPixmap(pngpath)
		
	return [
		result,
		MultiContentEntryPixmapAlphaTest(pos=(350, 5), size=(40, 40), png = indexpng)
	]
	
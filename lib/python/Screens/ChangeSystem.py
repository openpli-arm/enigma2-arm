from Screens.Standby import QuitMainloopScreen
from enigma import quitMainloop, iRecordableService
from Screens.MessageBox import MessageBox
from time import time
from Components.Task import job_manager
from enigma import eTimer
	
class SwicthSystem(MessageBox):
	def __init__(self, session, timeout=-1, default_yes = True):
#		self.retval = retvalue
		recordings = session.nav.getRecordings()
		jobs = len(job_manager.getPendingJobs())
		self.connected = False
		reason = ""
		next_rec_time = -1
		self.testTimer = eTimer()
		self.testTimer.callback.append(self.switchAndroid)
		
		if not recordings:
			next_rec_time = session.nav.RecordTimer.getNextRecordingTime()	
		if recordings or (next_rec_time > 0 and (next_rec_time - time()) < 360):
			reason = _("Recording(s) are in progress or coming up in few seconds!") + '\n'
		if jobs:
			if jobs == 1:
				job = job_manager.getPendingJobs()[0]
				reason += "%s: %s (%d%%)\n" % (job.getStatustext(), job.name, int(100*job.progress/float(job.end)))
			else:
				reason += (_("%d jobs are running in the background!") % jobs) + '\n'
				
		if reason:
			text = "Really switch system now?"
			MessageBox.__init__(self, session, reason+text, type = MessageBox.TYPE_YESNO, timeout = timeout, default = default_yes)
			self.skinName = "MessageBox"
			session.nav.record_event.append(self.getRecordEvent)
			self.connected = True
			self.onShow.append(self.__onShow)
			self.onHide.append(self.__onHide)
			return
		else:
			text = "Really switch system now?"
			MessageBox.__init__(self, session, text, type = MessageBox.TYPE_YESNO, timeout = timeout, default = default_yes)
			self.skinName = "MessageBox"
			self.onShow.append(self.__onShow)
			self.onHide.append(self.__onHide)
			return
#		self.skin = """<screen position="0,0" size="0,0"/>"""
#		Screen.__init__(self, session)
#		self.close(True)

	def getRecordEvent(self, recservice, event):
		if event == iRecordableService.evEnd:
			recordings = self.session.nav.getRecordings()
			if not recordings: # no more recordings exist
				rec_time = self.session.nav.RecordTimer.getNextRecordingTime()
				if rec_time > 0 and (rec_time - time()) < 360:
					self.initTimeout(360) # wait for next starting timer
					self.startTimer()
				else:
					self.close(True) # immediate shutdown
		elif event == iRecordableService.evStart:
			self.stopTimer()

	def close(self, value):
		if self.connected:
			self.conntected=False
			self.session.nav.record_event.remove(self.getRecordEvent)
		if value:
			self.hide()
			self.session.nav.stopService()
			print "show image info"
			self.quitScreen = self.session.instantiateDialog(QuitMainloopScreen,retvalue=10)
			self.quitScreen.show()
# switch system
			self.testTimer.start(2000)
		else:
			MessageBox.close(self, True)
			
	def switchAndroid(self):
		self.testTimer.stop()
		#from os import system, _exit
		#_exit(10)
		quitMainloop(10)
		
	def __onShow(self):
		global inTryQuitMainloop
		inTryQuitMainloop = True

	def __onHide(self):
		global inTryQuitMainloop
		inTryQuitMainloop = False


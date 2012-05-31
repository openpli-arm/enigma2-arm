from Screens.MessageBox import MessageBox

class ChangeSystem(MessageBox):
	def __init__(self, session):
		MessageBox.__init__(self, session, _("Are you sure to shut down Enigma2 and go to Android?\n"), MessageBox.TYPE_YESNO)
		self.skinName = "MessageBox"

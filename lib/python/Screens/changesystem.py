from Screens.MessageBox import MessageBox

class changesystem(MessageBox):
	def __init__(self, session):
		MessageBox.__init__(self, session, _("Are you sure to shut down Enigma2 and go to Andriod?\n"), MessageBox.TYPE_YESNO)
		self.skinName = "MessageBox"

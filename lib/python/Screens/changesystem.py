from Screens.MessageBox import MessageBox

class changesystem(MessageBox):
	def __init__(self, session):
		MessageBox.__init__(self, session, _("When you do a change system, the system will reboot"
			" and change into Andriod system\n"
			"Really do a change system?"), MessageBox.TYPE_YESNO)
		self.skinName = "MessageBox"
/* DVB CI MMI */

#include <lib/dvb_ci/dvbci_mmi.h>
#include <lib/dvb_ci/dvbci_ui.h>

#include <string>

/*
PyObject *list = PyList_New(len);
for (i=0; i<len; ++i) {
	PyObject *tuple = PyTuple_New(3); // 3 eintrge im tuple
	PyTuple_SetItem(tuple, 0, PyString_FromString("eintrag 1"))
	PyTuple_SetItem(tuple, 1, PyInt_FromLong(31337));
	PyTuple_SetItem(tuple, 2, PyString_FromString("eintrag 3"))
	PyList_SetItem(list, i, tuple);
}
return list;
*/

eDVBCIMMISession::eDVBCIMMISession(eDVBCISlot *tslot)
{
	printf("----->abing<----- eDVBCIMMISession create.\n");
	slot = tslot;
	slot->setMMIManager(this);
}

eDVBCIMMISession::~eDVBCIMMISession()
{
	printf("----->abing<----- eDVBCIMMISession delete.\n");
	slot->setMMIManager(NULL);
	eDVBCI_UI::getInstance()->mmiSessionDestroyed(slot->getSlotID());
}

int eDVBCIMMISession::receivedAPDU(const unsigned char *tag, const void *data, int len)
{
	printf("----->abing<----- eDVBCIMMISession receivedAPDU:  %02x %02x %02x: ", session_nb, tag[0], tag[1],tag[2]);
	for (int i=0; i<len; i++)
		printf("%02x ", ((const unsigned char*)data)[i]);
	printf("\n");

	if ((tag[0]==0x9f) && (tag[1]==0x88))
		if (eDVBCI_UI::getInstance()->processMMIData(slot->getSlotID(), tag, data, len) == 1)
		{
			state=stateDisplayReply;
			return 1;
		}

	return 0;
}

int eDVBCIMMISession::doAction()
{
	switch (state)
	{
	case stateStarted:
		printf("----->abing<----- eDVBCIMMISession doAction stateStarted do nothing.\n");
		state=stateIdle;
		break;
	case stateDisplayReply:
	{
		printf("----->abing<----- eDVBCIMMISession doAction - DisplayReply.\n");
		unsigned char tag[]={0x9f, 0x88, 0x02};// display reply  abing:attention  if it is done in trid lib?  
		unsigned char data[]={0x01, 0x01};
		sendAPDU(tag, data, 2);
		state=stateIdle;
		//state=stateFakeOK;
		//return 1;
		break;
	}
	case stateFakeOK:
	{
		printf("----->abing<----- eDVBCIMMISession doAction - stateFakeOK   ----  Trid_CI_MMI_SendMenuAnsw.\n");
		unsigned char tag[]={0x9f, 0x88, 0x0b};// menu answer  Trid_CI_MMI_SendMenuAnsw
		unsigned char data[]={5}; // choice_ref, 用户选择的选项编号
		sendAPDU(tag, data, 1);
		state=stateIdle;
		break;
	}
	case stateIdle:
		break;
	default:
		break;
	}
	return 0;
}

int eDVBCIMMISession::stopMMI()
{
	//eDebug("eDVBCIMMISession::stopMMI()");
	printf("----->abing<----- eDVBCIMMISession stopMMI   ----  Trid_CI_MMI_SendCloseMMI.\n");

	unsigned char tag[]={0x9f, 0x88, 0x00};
	unsigned char data[]={0x00};
	sendAPDU(tag, data, 1);
	
	return 0;
}

int eDVBCIMMISession::answerText(int answer)
{
	//eDebug("eDVBCIMMISession::answerText(%d)",answer);
	printf("----->abing<----- eDVBCIMMISession answerText(%d)   ----  Trid_CI_MMI_SendMenuAnsw.\n",answer);

	unsigned char tag[]={0x9f, 0x88, 0x0B};
	unsigned char data[]={0x00};
	data[0] = answer & 0xff;
	sendAPDU(tag, data, 1);
	
	return 0;
}

int eDVBCIMMISession::answerEnq(char *answer)
{
	unsigned int len = strlen(answer);
	//eDebug("eDVBCIMMISession::answerEnq(%d bytes)", len);
	printf("----->abing<----- eDVBCIMMISession answerEnq(%d bytes)   ----  Trid_CI_MMI_SendEnqAnsw 1.\n", len);

	unsigned char data[len+1];
	data[0] = 0x01; // answer ok
	memcpy(data+1, answer, len);

	unsigned char tag[]={0x9f, 0x88, 0x08};
	sendAPDU(tag, data, len+1);

	return 0;
}

int eDVBCIMMISession::cancelEnq()
{
	//eDebug("eDVBCIMMISession::cancelEnq()");
	printf("----->abing<----- eDVBCIMMISession cancelEnq   ----  Trid_CI_MMI_SendEnqAnsw 0.\n");

	unsigned char tag[]={0x9f, 0x88, 0x08};
	unsigned char data[]={0x00}; // canceled
	sendAPDU(tag, data, 1);
	
	return 0;
}


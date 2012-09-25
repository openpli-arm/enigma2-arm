/* DVB CI Application Manager */

#include <lib/base/eerror.h>
#include <lib/dvb_ci/dvbci_appmgr.h>
#include <lib/dvb_ci/dvbci_ui.h>

eDVBCIApplicationManagerSession::eDVBCIApplicationManagerSession(eDVBCISlot *tslot)
{
	printf("----->abing<----- eDVBCIApplicationManagerSession allocate .\n");
	slot = tslot;
	slot->setAppManager(this);
}

eDVBCIApplicationManagerSession::~eDVBCIApplicationManagerSession()
{
	printf("----->abing<----- eDVBCIApplicationManagerSession dellocate .\n");
	slot->setAppManager(NULL);
}

int eDVBCIApplicationManagerSession::receivedAPDU(const unsigned char *tag,const void *data, int len)
{
	printf("----->abing<----- SESSION(%d)/APP %02x %02x %02x: ", session_nb, tag[0], tag[1], tag[2]);
	for (int i=0; i<len; i++)
		printf("%02x ", ((const unsigned char*)data)[i]);
	printf("\n");

	if ((tag[0]==0x9f) && (tag[1]==0x80))
	{
		switch (tag[2])
		{
		case 0x21:
		{
			int dl;
			/*eDebug*/printf("application info:");
			/*eDebug*/printf("  len: %d", len);
			/*eDebug*/printf("  application_type: %d", ((unsigned char*)data)[0]);
			/*eDebug*/printf("  application_manufacturer: %02x %02x", ((unsigned char*)data)[2], ((unsigned char*)data)[1]);
			/*eDebug*/printf("  manufacturer_code: %02x %02x", ((unsigned char*)data)[4],((unsigned char*)data)[3]);
			/*eDebug*/printf("\n ----->abing<----- menu string: ");
			dl=((unsigned char*)data)[5];
			if ((dl + 6) > len)
			{
				/*eDebug*/printf("warning, invalid length (%d vs %d)", dl+6, len);
				dl=len-6;
			}
			char str[dl + 1];
			memcpy(str, ((char*)data) + 6, dl);
			str[dl] = '\0';
			for (int i = 0; i < dl; ++i)
				/*eDebug*/printf("%c", ((unsigned char*)data)[i+6]);
			/*eDebug*/printf("\n");

			eDVBCI_UI::getInstance()->setAppName(slot->getSlotID(), str);

			eDVBCI_UI::getInstance()->setState(slot->getSlotID(), 2);
			break;
		}
		default:
			/*eDebug*/printf("unknown APDU tag 9F 80 %02x", tag[2]);
			break;
		}
	}
	return 0;
}

int eDVBCIApplicationManagerSession::doAction()
{
  switch (state)
  {
  case stateStarted:
  {
  	printf("----->abing<----- application doAction stateStarted, menu info enquire - Trid_CI_AppInfo_GetMenuStr.\n");
    const unsigned char tag[3]={0x9F, 0x80, 0x20}; // application manager info e    sendAPDU(tag);  Trid_CI_AppInfo_GetMenuStr
		sendAPDU(tag);
    state=stateFinal;
    return 1;
  }
  case stateFinal:
    eDebug("in final state.");
	printf("----->abing<----- application doAction stateFinal\n");
		wantmenu = 0;// why here set 0, and next to check it?  the original is this?
    if (wantmenu)
    {
    printf("----->abing<----- wantmenu: sending Tenter_menu, attention, in normal logic no come here\n");
      eDebug("wantmenu: sending Tenter_menu");
      const unsigned char tag[3]={0x9F, 0x80, 0x22};  // Tenter_menu
      sendAPDU(tag);
      wantmenu=0;
      return 0;
    } else
      return 0;
  default:
    return 0;
  }
}

int eDVBCIApplicationManagerSession::startMMI()
{
	eDebug("in appmanager -> startmmi()");
	printf("----->abing<----- ApplicationManager startMMI, Trid_CI_AppInfo_EnterMenu\n");
	const unsigned char tag[3]={0x9F, 0x80, 0x22};  // Tenter_menu
	sendAPDU(tag);
	return 0;
}


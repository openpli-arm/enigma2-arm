#include <lib/dvb_ci/dvbci_ui.h>
#include <lib/dvb_ci/dvbci.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <lib/base/init.h>
#include <lib/base/init_num.h>
#include <lib/base/eerror.h>
#include <lib/base/estring.h>

#define MAX_SLOTS 1

eDVBCI_UI *eDVBCI_UI::instance;

eDVBCI_UI::eDVBCI_UI()
	:eMMI_UI(MAX_SLOTS)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>create now.\n");
	ASSERT(!instance);
	instance = this;
}

eDVBCI_UI *eDVBCI_UI::getInstance()
{
    printf("<dvbci_ui.cpp eDVBCI_UI>getInstance.\n");
	return instance;
}

void eDVBCI_UI::setInit(int slot)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>setInit.\n");
	eDVBCIInterfaces::getInstance()->initialize(slot);
}

void eDVBCI_UI::setReset(int slot)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>setReset.\n");
	eDVBCIInterfaces::getInstance()->reset(slot);
}

int eDVBCI_UI::startMMI(int slot)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>startMMI.\n");
	eDVBCIInterfaces::getInstance()->startMMI(slot);
	return 0;
}

int eDVBCI_UI::stopMMI(int slot)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>stopMMI.\n");
	eDVBCIInterfaces::getInstance()->stopMMI(slot);
	return 0;
}

int eDVBCI_UI::answerMenu(int slot, int answer)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>answerMenu.\n");
	eDVBCIInterfaces::getInstance()->answerText(slot, answer);
	return 0;
}

int eDVBCI_UI::answerEnq(int slot, char *value)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>answerEnq.\n");
	eDVBCIInterfaces::getInstance()->answerEnq(slot, value);
	return 0;
}

int eDVBCI_UI::cancelEnq(int slot)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>cancelEnq.\n");
	eDVBCIInterfaces::getInstance()->cancelEnq(slot);
	return 0;
}

int eDVBCI_UI::getMMIState(int slot)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>getMMIState.\n");
	return eDVBCIInterfaces::getInstance()->getMMIState(slot);
}

int eDVBCI_UI::setClockRate(int slot, int rate)
{
    printf("<dvbci_ui.cpp eDVBCI_UI>setClockRate.\n");
	return eDVBCIInterfaces::getInstance()->setCIClockRate(slot, rate);
}

//FIXME: correct "run/startlevel"
eAutoInitP0<eDVBCI_UI> init_dvbciui(eAutoInitNumbers::rc, "DVB-CI UI");

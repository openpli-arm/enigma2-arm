#include <lib/mmi/mmi_ui.h>
#include <lib/dvb_ci/dvbci_session.h> // for parseLengthField

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <lib/base/init.h>
#include <lib/base/init_num.h>
#include <lib/base/eerror.h>
#include <lib/base/estring.h>

eMMI_UI::eMMI_UI(int max_slots)
	:m_max_slots(max_slots)
{
	slotdata = new slot_ui_data[m_max_slots];
	for(int i=0;i<m_max_slots;++i)
	{
		slotdata[i].mmiScreenReady=0;
		slotdata[i].mmiTuplePos=0;
		slotdata[i].state=-1;
	}
}

eMMI_UI::~eMMI_UI()
{
	for(int i=0;i<m_max_slots;++i)
	{
		if (slotdata[i].mmiScreen)
			Py_DECREF(slotdata[i].mmiScreen);
	}
	delete [] slotdata;
}

int eMMI_UI::processMMIData(int slot_id, const unsigned char *tag, const void *data, int len)
{
	switch (tag[2])
	{
	case 0x00:		//Tmmi_close
	{
		unsigned char *d=(unsigned char*)data;
		int timeout=0;
		if (d[0] == 1)
		{
			if (len > 1)
				timeout = d[1];
			else
			{
				eDebug("mmi close tag incorrect.. no timeout given.. assume 5 seconds");
				timeout = 5;
			}
		}
		else if (d[0] > 1)
			eDebug("mmi close tag incorrect.. byte 4 should be 0 or 1");
		mmiScreenClose(slot_id, timeout);
		break;
	}
	case 0x01:
		eDebug("MMI display control");
		if (((unsigned char*)data)[0] != 1)
			eDebug("kann ich nicht. aber das sag ich dem modul nicht.");
		return 1;
	case 0x07:		//Tmenu_enq
#if 1
{
	/*typedef struct _Trid_T_Enq_
	{
		trid_uint8 blind_answ;
		trid_uint8 answ_len;
		Trid_T_Text text;
	}Trid_T_Enq;*/
	if (data != NULL)
	{
		Trid_T_Enq *d=(Trid_T_Enq*)data;
		printf("<mmi_ui.cpp eMMI_UI processMMIData> menu enqure blind answ(%d), text %s,len %d.\n", d->blind_answ, d->text.strings, d->answ_len);
		mmiScreenEnq(slot_id, d->blind_answ, d->answ_len, d->text.strings);
	}
	break;
}

#else
{
	unsigned char *d=(unsigned char*)data;
	unsigned char *max=((unsigned char*)d) + len;
	int textlen = len - 2;
	eDebug("in enq");
	if ((d+2) > max)
		break;
	int blind = *d++ & 1;
	int alen = *d++;
		eDebug("%d bytes text", textlen);
	if ((d+textlen) > max)
		break;
	char str[textlen + 1];
	memcpy(str, ((char*)d), textlen);
	str[textlen] = '\0';
	eDebug("enq-text: %s",str);
	mmiScreenEnq(slot_id, blind, alen, (char*)convertDVBUTF8(str).c_str());
	break;
}
#endif
	case 0x09:		//Tmenu_last
#if 1
{
	/*typedef struct _Trid_T_Menu_
	{
		trid_uint8 choice_nb;
		Trid_T_Text title;
		Trid_T_Text sub_title;
		Trid_T_Text bottom;
		Trid_T_Text texts[TRID_MMI_MAX_MENU_ITEMS];
	}Trid_T_Menu;*/
	Trid_T_Menu *d=(Trid_T_Menu*)data;
	int i=0;
	//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tmenu mmiScreenBegin.maybe choice_nb(%d) texts in.\n", d->choice_nb);
	mmiScreenBegin(slot_id, 0);

	//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tmenu title %s. strlen %d.\n", d->title.strings, d->title.len);
	if (d->title.len > 0)
      {
		mmiScreenAddText(slot_id, /*i++*/0, d->title.strings);/*这里有点风险，怕没有结束符号*/
	}
	//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tmenu sub_title %s. strlen %d.\n", d->sub_title.strings, d->sub_title.len);
	if (d->sub_title.len > 0)
      {
		mmiScreenAddText(slot_id, /*i++*/1, d->sub_title.strings);
	}
	//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tmenu bottom %s. strlen %d.\n", d->bottom.strings, d->bottom.len);
	if (d->bottom.len > 0)
      {
		mmiScreenAddText(slot_id, /*i++*/2, d->bottom.strings);
	}
	
	for (i=0; i < (d->choice_nb); ++i)
	{
		//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tmenu %d text %s. strlen %d.\n", i, d->texts[i].strings, d->texts[i].len);
		mmiScreenAddText(slot_id, i+3, d->texts[i].strings);
	}
	mmiScreenFinish(slot_id);
}

#else
{
		unsigned char *d=(unsigned char*)data;
		unsigned char *max=((unsigned char*)d) + len;
		int pos = 0;
		eDebug("Tmenu_last");
		if (d > max)
			break;
		int n=*d++;
		if(tag[2] == 0x09)	//menu
			mmiScreenBegin(slot_id, 0);
		else								//list
			mmiScreenBegin(slot_id, 1);
		if (n == 0xFF)
			n=0;
		else
			n++;
		eDebug("%d texts", n);
		for (int i=0; i < (n+3); ++i)
		{
			int textlen;
			if ((d+3) > max)
				break;
			eDebug("text tag: %02x %02x %02x", d[0], d[1], d[2]);
			d+=3;
			d+=eDVBCISession::parseLengthField(d, textlen);
			eDebug("%d bytes text", textlen);
			if ((d+textlen) > max)
				break;
			char str[textlen + 1];
			memcpy(str, ((char*)d), textlen);
			str[textlen] = '\0';
			mmiScreenAddText(slot_id, pos++, (char*)convertDVBUTF8(str).c_str());
			while (textlen--)
				eDebugNoNewLine("%c", *d++);
			eDebug("");
		}
		mmiScreenFinish(slot_id);
	}

#endif
		break;
	case 0x0c:		//Tlist_last
#if 1
{
	/*
	typedef struct _Trid_T_List_
	{
		trid_uint8 item_nb;
		Trid_T_Text title;
		Trid_T_Text sub_title;
		Trid_T_Text bottom;
		Trid_T_Text texts[TRID_MMI_MAX_MENU_ITEMS];
	}Trid_T_List;
	*/
	Trid_T_List *d=(Trid_T_List*)data;
	int i=0;
	
	mmiScreenBegin(slot_id, 1);
	//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tlist mmiScreenBegin.\n");
	
	mmiScreenAddText(slot_id, /*i++*/0, d->title.strings);
	//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tlist title %s. strlen %d.\n", d->title.strings, d->title.len);
	mmiScreenAddText(slot_id, /*i++*/1, d->sub_title.strings);
	//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tlist sub_title %s. strlen %d.\n", d->sub_title.strings, d->sub_title.len);
	mmiScreenAddText(slot_id, /*i++*/2, d->bottom.strings);
	//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tlist bottom %s. strlen %d.\n", d->bottom.strings, d->bottom.len);
	
	for (i=0; i < (d->item_nb); ++i)
	{
		//printf("<mmi_ui.cpp eMMI_UI processMMIData> Tlist %d text %s. strlen %d.\n", i, d->texts[i].strings, d->texts[i].len);
		mmiScreenAddText(slot_id, i+3, d->texts[i].strings);
	}
	mmiScreenFinish(slot_id);
	break;
}		
#else
	{
		unsigned char *d=(unsigned char*)data;
		unsigned char *max=((unsigned char*)d) + len;
		int pos = 0;
		eDebug("Tmenu_last");
		if (d > max)
			break;
		int n=*d++;
		if(tag[2] == 0x09)	//menu
			mmiScreenBegin(slot_id, 0);
		else								//list
			mmiScreenBegin(slot_id, 1);
		if (n == 0xFF)
			n=0;
		else
			n++;
		eDebug("%d texts", n);
		for (int i=0; i < (n+3); ++i)
		{
			int textlen;
			if ((d+3) > max)
				break;
			eDebug("text tag: %02x %02x %02x", d[0], d[1], d[2]);
			d+=3;
			d+=eDVBCISession::parseLengthField(d, textlen);
			eDebug("%d bytes text", textlen);
			if ((d+textlen) > max)
				break;
			char str[textlen + 1];
			memcpy(str, ((char*)d), textlen);
			str[textlen] = '\0';
			mmiScreenAddText(slot_id, pos++, (char*)convertDVBUTF8(str).c_str());
			while (textlen--)
				eDebugNoNewLine("%c", *d++);
			eDebug("");
		}
		mmiScreenFinish(slot_id);
	}	
#endif
		break;
	
	default:
		eDebug("unknown APDU tag 9F 88 %02x", tag[2]);
		break;
	}
	return 0;
}

int eMMI_UI::getState(int slot)
{
	if (slot < m_max_slots)
		return slotdata[slot].state;
	return 0;
}

void eMMI_UI::setState(int slot, int newState)
{
	if (slot < m_max_slots)
	{
		slotdata[slot].state = newState;
		stateChanged(slot);
	}
}

std::string eMMI_UI::getAppName(int slot)
{
	if (slot < m_max_slots)
		return slotdata[slot].appName;
	return "";
}

void eMMI_UI::setAppName(int slot, const char *name)
{
	if (slot < m_max_slots)
		slotdata[slot].appName = name;
}

int eMMI_UI::availableMMI(int slot)
{
	if (slot < m_max_slots)
		return slotdata[slot].mmiScreenReady;
	return false;
}

int eMMI_UI::mmiScreenClose(int slot, int timeout)
{
	if (slot >= m_max_slots)
		return 0;

	slot_ui_data &data = slotdata[slot];

	data.mmiScreenReady = 0;

	if (data.mmiScreen)
		Py_DECREF(data.mmiScreen);
	data.mmiScreen = PyList_New(1);

	ePyObject tuple = PyTuple_New(2);
	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("CLOSE"));
	PyTuple_SET_ITEM(tuple, 1, PyLong_FromLong(timeout));
	PyList_SET_ITEM(data.mmiScreen, 0, tuple);
	data.mmiScreenReady = 1;
	stateChanged(slot);
	return 0;
}

int eMMI_UI::mmiScreenEnq(int slot, int blind, int answerLen, char *text)
{
	if (slot >= m_max_slots)
		return 0;

	slot_ui_data &data = slotdata[slot];

	data.mmiScreenReady = 0;

	if (data.mmiScreen)
		Py_DECREF(data.mmiScreen);
	data.mmiScreen = PyList_New(2);

	ePyObject tuple = PyTuple_New(1);
	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("ENQ"));
	PyList_SET_ITEM(data.mmiScreen, 0, tuple);

	tuple = PyTuple_New(4);
	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("PIN"));
	PyTuple_SET_ITEM(tuple, 1, PyInt_FromLong(answerLen));
	PyTuple_SET_ITEM(tuple, 2, PyString_FromString(text));
	PyTuple_SET_ITEM(tuple, 3, PyInt_FromLong(blind));

	PyList_SET_ITEM(data.mmiScreen, 1, tuple);

	data.mmiScreenReady = 1;

	stateChanged(slot);

	return 0;
}

int eMMI_UI::mmiScreenBegin(int slot, int listmenu)
{
	if (slot >= m_max_slots)
		return 0;

	eDebug("eMMI_UI::mmiScreenBegin");

	slot_ui_data &data = slotdata[slot];

	data.mmiScreenReady = 0;

	if (data.mmiScreen)
		Py_DECREF(data.mmiScreen);

	data.mmiScreen = PyList_New(1);

	ePyObject tuple = PyTuple_New(1);
	if (listmenu == 0)				//menu
	 	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("MENU"));
	else 	//list
	 	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("LIST"));

	PyList_SET_ITEM(data.mmiScreen, 0, tuple);

	data.mmiTuplePos = 1;

	return 0;
}

int eMMI_UI::mmiScreenAddText(int slot, int type, char *value)
{
	if (slot >= m_max_slots)
		return 0;

	eDebug("eMMI_UI::mmiScreenAddText(%s)",value ? value : "");

	slot_ui_data &data = slotdata[slot];

	ePyObject tuple = PyTuple_New(3);

	if (type == 0)					//title
	 	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("TITLE"));
	else if (type == 1)				//subtitle
	 	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("SUBTITLE"));
	else if (type == 2)				//bottom
	 	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("BOTTOM"));
	else
	 	PyTuple_SET_ITEM(tuple, 0, PyString_FromString("TEXT"));

	eDebug("addText %s with id %d", value, type);

	PyTuple_SET_ITEM(tuple, 1, PyString_FromString(value));

	if (type > 2)
		PyTuple_SET_ITEM(tuple, 2, PyInt_FromLong(type-2));
	else
		PyTuple_SET_ITEM(tuple, 2, PyInt_FromLong(-1));

	PyList_Append(data.mmiScreen, tuple);
	Py_DECREF(tuple);

	return 0;
}

int eMMI_UI::mmiScreenFinish(int slot)
{
	if (slot < m_max_slots)
	{
		eDebug("eMMI_UI::mmiScreenFinish");
		slotdata[slot].mmiScreenReady = 1;
		stateChanged(slot);
	}
	return 0;
}

void eMMI_UI::mmiSessionDestroyed(int slot)
{
	if (slot < m_max_slots)
		mmiScreenClose(slot, 0);
	else
	{
		printf(">>>>>>>>>><mmi_ui.cpp eMMI_UI mmiSessionDestroyed>very seriously error. slot >= m_max_slots\n");
	}
}

PyObject *eMMI_UI::getMMIScreen(int slot)
{
	if (slot < m_max_slots)
	{
		slot_ui_data &data = slotdata[slot];
		if (data.mmiScreenReady)
		{
			data.mmiScreenReady = 0;
			Py_INCREF(data.mmiScreen);
			return data.mmiScreen;
		}
	}
	Py_RETURN_NONE;
}

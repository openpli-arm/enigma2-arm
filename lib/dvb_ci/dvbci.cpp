#include <fcntl.h>
#include <sys/ioctl.h>

#include <lib/base/init.h>
#include <lib/base/init_num.h>
#include <lib/base/ebase.h>

#include <lib/base/eerror.h>
#include <lib/base/nconfig.h> // access to python config
#include <lib/dvb/db.h>
#include <lib/dvb/pmt.h>
#include <lib/dvb_ci/dvbci.h>
#include <lib/dvb_ci/dvbci_session.h>
#include <lib/dvb_ci/dvbci_camgr.h>
#include <lib/dvb_ci/dvbci_ui.h>
#include <lib/dvb_ci/dvbci_appmgr.h>
#include <lib/dvb_ci/dvbci_mmi.h>

#include <dvbsi++/ca_program_map_section.h>

//#define CIDEBUG 1

#ifdef CIDEBUG
	#define eDebugCI(x...) eDebug(x)
#else
	#define eDebugCI(x...)
#endif

eDVBCIInterfaces *eDVBCIInterfaces::instance = 0;

#if trid_ci
#define __cplusplus
#include <lib/ciplus/driver_dvbci.h>

#include <lib/ciplus/inc/trid_datatype.h>
#include <lib/ciplus/inc/trid_errno.h>
#include <lib/ciplus/inc/trid_ci_types.h>
#include <lib/ciplus/inc/trid_ci_api.h>

int dvbci_slot1_gpio_cs = 1;
int dvbci_slot1_gpio_cd1 = 3;
int dvbci_slot1_gpio_cd2 = 4;
int dvbci_slot1_gpio_ireq = 52;
int dvbci_slot1_gpio_reset = 49;

int dvbci_slot2_gpio_cs = 0;
int dvbci_slot2_gpio_cd1 = 0;
int dvbci_slot2_gpio_cd2 = 0;
int dvbci_slot2_gpio_ireq  = 0;
int dvbci_slot2_gpio_reset = 0;

trid_uint8 app_info[128];
trid_uint8 language[32];
trid_uint8 country[32];

int ca_manager = 0;

unsigned int CardStatus;
std::vector<uint16_t> caids;

enum
{
NOTIFY_CARD_STATUS=0x01,
NOTIFY_MENU_DATA = 0x02,
NOTIFY_LIST_DATA = 0x04,
NOTIFY_ENQ_DATA = 0x08,
NOTIFY_CLOSE_MMI = 0x10
};
Trid_T_Menu menu_data;
Trid_T_List list_data;
Trid_T_Enq enq_data;

int Tridci_cb_status = 0;
int Tridci_init = 0;
void DVBCI_TridInit();
 
int DVBCI_GetCbStatus()
{
    if (Tridci_init == 0)
    {
       DVBCI_TridInit();
       Tridci_init = 1;
    }
 
    return Tridci_cb_status;
}

#define ci_ca
void DVBCI_Packcaids()
{	
	uint16_t ca_system_id[64]; 
	trid_sint32 num;
	RETURN_TYPE ret;/*abing:doing here the max num of ca system id isn't know now, try 64*/


	//eDebugCI("--->abing<--- very good, call function Trid_CI_GetCASystemId now.\n");

	ret = Trid_CI_GetCASystemId((trid_uint16*)ca_system_id, &num);
	if (!num)
	{
		for (int i=0; i<num; i++)
		{
			caids.push_back(ca_system_id[i]);
		}
		std::sort(caids.begin(), caids.end());
	}
	else
	{
		for (int i=0; i<num; i++)
		{
			caids.push_back(ca_system_id[i]);
		}
		std::sort(caids.begin(), caids.end());
	}
	eDebugCI("--->abing<--- very good, DVBCI_Packcaids.\n");
}

const std::vector<uint16_t> DVBCI_GetCAIDs(void)
{
	DVBCI_Packcaids(); 
	return caids;
}

#define call_driver
Trid_CI_CardStatus_t DVBCI_GetCardStatus() 
{
    Trid_CI_CardStatus_t CardStatus;
	eDebugCI("--->abing<--- very good, call function Trid_CI_GetCardStatus (%d).\n", CardStatus);
	Trid_CI_GetCardStatus(&CardStatus); 
	return CardStatus;
}

int DVBCI_StartMMI()
{
	eDebugCI("--->abing<--- very good, Trid_CI_AppInfo_EnterMenu.\n");

	Trid_CI_AppInfo_EnterMenu(); 

	return 0;
}

int DVBCI_StopMMI() 
{
	eDebugCI("--->abing<--- very good, Trid_CI_MMI_SendCloseMMI.\n");

	Trid_CI_MMI_SendCloseMMI();
	return 0;
}

int DVBCI_AnswerText(int answer)
{
	eDebugCI("--->abing<--- very good, Trid_CI_MMI_SendMenuAnsw.\n");

	Trid_CI_MMI_SendMenuAnsw((trid_uint8)answer);
	return 0;
}

int DVBCI_AnswerEnq(unsigned char *answer)
{
	eDebugCI("--->abing<--- very good, Trid_CI_MMI_SendEnqAnsw.\n");

	Trid_CI_MMI_SendEnqAnsw(1, strlen(answer), (trid_uint8*)answer); 
	return 0;
}

int DVBCI_CancelEnq()
{
	eDebugCI("--->abing<--- very good, Trid_CI_MMI_CloseEnqAnsw.\n");

	Trid_CI_MMI_SendEnqAnsw(0, 0, 0); 
	return 0;
}
#if 0
char pmt_save[] = {/*0x47, 0x41, 0x93, 0x16, 0x00, */
	0x02, 0xB0, 0x3B, 0x01, 0x92, 0xF5, 0x00, 0x00, 
	0xE1, 0xf6, 
	0xF0, 0x00, 
	0x02, 0xE1, 0xF6, 0xF0, 0x06, 0x09, 0x04, 0x0B, 0x00, 0xF7, 0x71, 
	0x03, 0xE2, 0x5A, 0xF0, 0x0C, 0x0A, 0x04, 0x65, 0x6E, 0x67, 0x00, 0x09, 0x04, 0x0B, 0x00, 0xF7, 0x72, 
    0x06, 0xE2, 0xBE, 0xF0, 0x0D, 0x56, 0x05, 0x65, 0x6E, 0x67, 0x09, 0x00, 0x09, 0x04, 0x0B, 0x00, 0xF7, 0x72, 
    
    0xA5, 0xD2, 0x38, 0xF7, 
    0xfF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

char pmt_save1[] = {/*0x47, 0x41, 0x93, 0x16, 0x00, */
	0x02, 0xb0, /*0x48, */0x27, 0x01, 0x93, 0xf7, 0x00, 0x00, 
	0xe1, 0xf7, 
	0xf0, 0x06,/*0x0c, 0x09, 0x04, 0x05, 0x00, 0xf9, 0xc9, */0x09, 0x04, 0x0b, 0x00, 0xf7, 0x71, 
	
	0x02, 0xe1, 0xf7, 0xf0, 0x00, 
	0x03, 0xe2, 0x5b, 0xf0, /*0x0a, 0x0a, 0x08, 0x65, 0x6e, 0x67, 0x00, 0x64, 0x61, 0x6e, 0x00, */0x00,
	0x03, 0xe2, 0x65, 0xf0, /*0x0a, 0x0a, 0x08, 0x73, 0x77, 0x65, 0x00, 0x6e, 0x6f, 0x72, 0x00, */0x00,
	0x06, 0xe2, 0xbf, 0xf0, /*0x07, 0x56, 0x05, 0x64, 0x61, 0x6e, 0x09, 0x00, */0x00,
	
	0xe5, 0x36, 0xf5, 0xbf, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

#endif
trid_uint16 audioPid;
trid_uint16 videoPid;


	
int DVBCI_SendCAPMT(unsigned char *pmt, int len)
{
	trid_sint32 ca_system_id_match;RETURN_TYPE ret;

	eDebugCI("--->abing<--- very good, Trid_CI_SendCAPmt.\n");

	eDebugCI("PMT to send:%d.\n", len);
    for (ret=0; ret<len; ret++)
    {
        printf("0x%02x, ", pmt[ret]);
    }
	eDebugCI("\n");

	ret = Trid_CI_SendCAPmt((trid_uint8* )pmt, len, &ca_system_id_match);//((trid_uint8* )pmt_save1, /*75*/42, &ca_system_id_match);//

    eDebugCI("ret is %d.\n", ret);
    eDebugCI("ca_system_id_match is %d.\n", ca_system_id_match);

	return 0;
}

#define golobal_callback
trid_sint32 DVBCI_CardStatusChangeNotifyCallback(Trid_CI_CardStatus_t status)
{
	eDebugCI("<dvbci.cpp DVBCI_CardStatusChangeNotifyCallback>status %d.\n", status);
	
	eDVBCIInterfaces::getInstance()->CardStatusChangeNotifyCallback(0, status);
	return 0;
}/*abing:doing*/

trid_sint32 DVBCI_CardGetHostLanguageCountryNotifyCallback(trid_uint8 language_code[], trid_uint8 country_code[])/*this function needs the support of appilication*/
{
	eDebugCI("<dvbci.cpp DVBCI_CardGetHostLanguageCountryNotifyCallback>language_code %s, country_code %s.\n", language_code, country_code);
	return 0;
}

/*the application callbacks from module*/
trid_sint32 DVBCI_CardGetHostAVPIDCallback(trid_uint16 *AudioPID, trid_uint16 *VideoPID)
{
	eDebugCI("<dvbci.cpp DVBCI_CardGetHostAVPIDCallback> audio:%d, video:%d.\n", *AudioPID, *VideoPID);
	*AudioPID = audioPid;
	*VideoPID = videoPid;
//	*AudioPID = 0x25b;
//	*VideoPID = 0x1f7;
	eDebugCI("<dvbci.cpp DVBCI_CardGetHostAVPIDCallback> set to audio:%d, video:%d.\n", *AudioPID, *VideoPID);
	return 0;
}

trid_sint32 DVBCI_CardTuneChannelCallback(trid_uint16 network_id, trid_uint16 orignal_network_id, trid_uint16 ts_id, trid_uint16 service_id)
{
	eDebugCI("<dvbci.cpp DVBCI_CardTuneChannelCallback>      sorry   no   realize   now .\n");
	return 0;
}

void DVBCI_CardSetDesKeyCallback(trid_uint8 key[/*8*/],trid_uint8 odd_even)
{
	eDebugCI("<dvbci.cpp DVBCI_CardSetDesKeyCallback>      sorry   no   realize   now .\n");
	return ;
}

void DVBCI_CardSetAesKeyCallback(trid_uint8 key[/*16*/],trid_uint8 iv[/*16*/],trid_uint8 odd_even)
{
	eDebugCI("<dvbci.cpp DVBCI_CardSetAesKeyCallback>      sorry   no   realize   now .\n");
	return ;
}


//RETURN_TYPE Trid_CI_MHEG_RegisterAppQuerySupport(MHEGAppQuerySupportCallback_t callback);
trid_bool DVBCI_MHEGAppQuerySupportCallback(
    trid_uint8                  app_domain_id[],
    trid_uint8                  app_domain_id_len,
    trid_uint8                  init_object[],
    trid_uint8                  init_object_len,
    Trid_CI_MHEG_StartAckCode_e *p_ack_code
)
{
	eDebugCI("<dvbci.cpp DVBCI_MHEGAppQuerySupportCallback>      sorry   no   realize   now .\n");
}

//RETURN_TYPE Trid_CI_MHEG_RegisterFileAckNotify(MHEGFileAckNotifyCallback_t callback);
void DVBCI_MHEGFileAckNotifyCallback
(
    trid_uint8 file_name[], 
    trid_uint32  file_name_len,
    trid_uint8 file_data[], 
    trid_uint32 file_data_len
)
{
	eDebugCI("<dvbci.cpp DVBCI_MHEGFileAckNotifyCallback>      sorry   no   realize   now .\n");
}

//RETURN_TYPE Trid_CI_MHEG_RegisterDataAckNotify(MHEGDataAckNotifyCallback_t callback);
void DVBCI_MHEGDataAckNotifyCallback
(
    trid_uint8 data[], 
    trid_uint32 data_len
)
{
	eDebugCI("<dvbci.cpp DVBCI_MHEGDataAckNotifyCallback>      sorry   no   realize   now .\n");
}

//RETURN_TYPE Trid_CI_MHEG_RegisterAppAbortNotify(MHEGAppAbortNotifyCallback_t callback)
void DVBCI_MHEGAppAbortNotifyCallback(void)
{
	eDebugCI("<dvbci.cpp DVBCI_MHEGAppAbortNotifyCallback>      sorry   no   realize   now .\n");
}

#define data_callback 0
/*收到菜单数据，要求显示*/
//Trid_CI_MMI_RegisterMenuDataNotify  
trid_sint32 DVBCI_MenuDataNotifyCallback(Trid_T_Menu* menu)
{
	eDebugCI("<dvbci.cpp DVBCI_MenuDataNotifyCallback>      sorry   no   realize   now .\n");

	eDVBCIInterfaces::getInstance()->MenuDataNotifyCallback(menu);
	return 0;
}


/*收到list数据*/
//Trid_CI_MMI_RegisterListDataNotify  
trid_sint32 DVBCI_ListDataNotifyCallback(Trid_T_List* list)
{
	eDebugCI("<dvbci.cpp DVBCI_ListDataNotifyCallback>      sorry   no   realize   now .\n");
	eDVBCIInterfaces::getInstance()->ListDataNotifyCallback(list);
	return 0;
}

/*收到enquiry数据*/
//Trid_CI_MMI_RegisterEnqDataNotify   
trid_sint32 DVBCI_EnqDataNotifyCallback(Trid_T_Enq* enq)
{
	eDebugCI("<dvbci.cpp DVBCI_EnqDataNotifyCallback>      sorry   no   realize   now .\n");
	eDVBCIInterfaces::getInstance()->EnqDataNotifyCallback(enq);
	return 0;
}


/*这个是否因为意外，比如模块被拔出，造成的由主机底层发出的停止动作*/
//Trid_CI_MMI_RegisterCloseMMINotify  
trid_sint32 DVBCI_CloseMMINotifyCallback()
{
	eDebugCI("<dvbci.cpp DVBCI_CloseMMINotifyCallback>      sorry   no   realize   now .\n");
	eDVBCIInterfaces::getInstance()->CloseMMINotifyCallback();
	return 0;
}
#define init_fun
RETURN_TYPE DVBCI_Set_Pcmcia_Func(void)
{
	CI_phys_driver phys_funcs;
	RETURN_TYPE ret = 0;

	phys_funcs.pcmcia_init = cnxt_dvbci_init;
	phys_funcs.pcmcia_term = cnxt_dvbci_term;
	phys_funcs.pcmcia_open = cnxt_dvbci_open;
	phys_funcs.pcmcia_close = cnxt_dvbci_close;
	phys_funcs.pcmcia_module_init = cnxt_dvbci_module_init;
	phys_funcs.pcmcia_register_read = cnxt_dvbci_register_read;
	phys_funcs.pcmcia_register_write = cnxt_dvbci_register_write;
	phys_funcs.pcmcia_attribute_mem_read = cnxt_dvbci_attribute_mem_read;
	phys_funcs.pcmcia_attribute_mem_write = cnxt_dvbci_attribute_mem_write;
	phys_funcs.pcmcia_signal_get_state = cnxt_dvbci_signal_get_state;
	phys_funcs.pcmcia_signal_set_state = cnxt_dvbci_signal_set_state;
	phys_funcs.pcmcia_enable_ts = cnxt_dvbci_enable_ts;
	phys_funcs.pcmcia_disable_ts = cnxt_dvbci_disable_ts;
	ret = Trid_CI_Set_Pcmcia_Func(&phys_funcs);
	eDebugCI("!!!----->abing<----- Trid_CI_Set_Pcmcia_Func return is %d.\n", ret);
	return 0;
}

void DVBCI_TridInit()
{
	RETURN_TYPE ret = 0;

	ret = cnxt_kal_initialize();

	ret = cnxt_dvbci_drv_init();

	DVBCI_Set_Pcmcia_Func();

	ret = Trid_CI_Start(DVBCI_CardStatusChangeNotifyCallback, 
							DVBCI_CardGetHostLanguageCountryNotifyCallback, 
							DVBCI_CardGetHostAVPIDCallback, 
							DVBCI_CardTuneChannelCallback, 
							DVBCI_CardSetDesKeyCallback,
							DVBCI_CardSetAesKeyCallback);

	ret = Trid_CI_MMI_RegisterMenuDataNotify(DVBCI_MenuDataNotifyCallback/*trid_sint32 (*callback)(Trid_T_Menu* menu)*/);

	ret = Trid_CI_MMI_RegisterListDataNotify(DVBCI_ListDataNotifyCallback/*trid_sint32 (*callback)(Trid_T_List* list)*/);

	ret = Trid_CI_MMI_RegisterEnqDataNotify(DVBCI_EnqDataNotifyCallback/*trid_sint32 (*callback)(Trid_T_Enq* enq)*/);

	ret = Trid_CI_MMI_RegisterCloseMMINotify(DVBCI_CloseMMINotifyCallback);
	
	ret = Trid_CI_MHEG_RegisterAppQuerySupport(DVBCI_MHEGAppQuerySupportCallback);
	ret = Trid_CI_MHEG_RegisterFileAckNotify(DVBCI_MHEGFileAckNotifyCallback);
	ret = Trid_CI_MHEG_RegisterDataAckNotify(DVBCI_MHEGDataAckNotifyCallback);
	ret = Trid_CI_MHEG_RegisterAppAbortNotify(DVBCI_MHEGAppAbortNotifyCallback);
	return ;
}

#endif

eDVBCIInterfaces::eDVBCIInterfaces()
{
	int num_ci = 0;
	
	instance = this;

	eDebug("scanning for common interfaces..");	
#if trid_ci
	ePtr<eDVBCISlot> cislot;
	
	cislot = new eDVBCISlot(eApp, num_ci);
	m_slots.push_back(cislot);
	num_ci++;
	
	//DVBCI_TridInit();
#else


	while (1)
	{
		struct stat s;
		char filename[128];
		sprintf(filename, "/dev/ci%d", num_ci);

		if (stat(filename, &s))
			break;

		ePtr<eDVBCISlot> cislot;

		cislot = new eDVBCISlot(eApp, num_ci);
		m_slots.push_back(cislot);

		++num_ci;
	}

	for (eSmartPtrList<eDVBCISlot>::iterator it(m_slots.begin()); it != m_slots.end(); ++it)
		it->setSource(TUNER_A);

	if (num_ci > 1) // // FIXME .. we force DM8000 when more than one CI Slot is avail
	{
		setInputSource(0, TUNER_A);
		setInputSource(1, TUNER_B);
		setInputSource(2, TUNER_C);
		setInputSource(3, TUNER_D);
	}
	else
	{
		setInputSource(0, TUNER_A);
		setInputSource(1, TUNER_B);
	}
#endif
	eDebug("done, found %d common interface slots", num_ci);
}

eDVBCIInterfaces::~eDVBCIInterfaces()
{
	cnxt_kal_terminate();
}

eDVBCIInterfaces *eDVBCIInterfaces::getInstance()
{
	return instance;
}

eDVBCISlot *eDVBCIInterfaces::getSlot(int slotid)
{
	for(eSmartPtrList<eDVBCISlot>::iterator i(m_slots.begin()); i != m_slots.end(); ++i)
		if(i->getSlotID() == slotid)
			return i;

	eDebug("FIXME: request for unknown slot");
			
	return 0;
}

int eDVBCIInterfaces::getSlotState(int slotid)
{
	eDVBCISlot *slot;

	eDebugCI("eDVBCIInterfaces getSlotState slot(%d).\n", slotid);
	if ((slot = getSlot(slotid)) == 0)
	{
		eDebugCI("eDVBCIInterfaces getSlotState invalid slot!!!.\n");
		return eDVBCISlot::stateInvalid;
	}

	return slot->getState();
}

int eDVBCIInterfaces::reset(int slotid)
{
	eDVBCISlot *slot;

	eDebugCI("eDVBCIInterfaces reset slot(%d).\n", slotid);/*abing:0518*/

	if( (slot = getSlot(slotid)) == 0 )
		return -1;

	return slot->reset();
}

int eDVBCIInterfaces::initialize(int slotid)
{
	eDVBCISlot *slot;
	eDebugCI("eDVBCIInterfaces initialize slot(%d).\n", slotid);/*abing:0518*/

	if( (slot = getSlot(slotid)) == 0 )
		return -1;

	slot->removeService();

	return sendCAPMT(slotid);
}

int eDVBCIInterfaces::sendCAPMT(int slotid)
{
	eDVBCISlot *slot;

	eDebugCI("eDVBCIInterfaces sendCAPMT slot(%d).\n", slotid);/*abing:0518*/

	if( (slot = getSlot(slotid)) == 0 )
		return -1;

	PMTHandlerList::iterator it = m_pmt_handlers.begin();
	while (it != m_pmt_handlers.end())
	{
		eDVBCISlot *tmp = it->cislot;
		while (tmp && tmp != slot)
			tmp = tmp->linked_next;
		if (tmp)
		{
			tmp->sendCAPMT(it->pmthandler);  // send capmt
			break;
		}
		++it;
	}

	return 0;
}

int eDVBCIInterfaces::startMMI(int slotid)
{
	eDVBCISlot *slot;
	eDebugCI("eDVBCIInterfaces start MMI slot(%d).\n", slotid);/*abing:0518*/

	if( (slot = getSlot(slotid)) == 0 )
		return -1;
	
	return slot->startMMI();
}

int eDVBCIInterfaces::stopMMI(int slotid)
{
	eDVBCISlot *slot;

	eDebugCI("eDVBCIInterfaces stop MMI slot(%d).\n", slotid);/*abing:0518*/
	
	if( (slot = getSlot(slotid)) == 0 )
		return -1;
	
	return slot->stopMMI();
}

int eDVBCIInterfaces::answerText(int slotid, int answer)
{
	eDVBCISlot *slot;
	eDebugCI("eDVBCIInterfaces answerText slot(%d), answer 0x%x.\n", slotid, answer);/*abing:0518*/

	if( (slot = getSlot(slotid)) == 0 )
		return -1;
	
	return slot->answerText(answer);
}

int eDVBCIInterfaces::answerEnq(int slotid, char *value)
{
	eDVBCISlot *slot;
	eDebugCI("eDVBCIInterfaces answerEnq slot(%d).\n", slotid);/*abing:0518*/

	if( (slot = getSlot(slotid)) == 0 )
		return -1;
	
	return slot->answerEnq(value);
}

int eDVBCIInterfaces::cancelEnq(int slotid)
{
	eDVBCISlot *slot;

	eDebugCI("eDVBCIInterfaces cancelEnq slot(%d).\n", slotid);/*abing:0518*/
	if( (slot = getSlot(slotid)) == 0 )
		return -1;
	
	return slot->cancelEnq();
}

void eDVBCIInterfaces::ciRemoved(eDVBCISlot *slot)
{
    eDebugCI("eDVBCIInterfaces ciRemoved.\n");/*abing:0518*/
	if (slot->use_count)
	{
		//eDebug("CI Slot %d: removed... usecount %d", slot->getSlotID(), slot->use_count);
		eDebugCI("CI Slot %d: removed... usecount %d.\n", slot->getSlotID(), slot->use_count);/*abing:0518*/
		for (PMTHandlerList::iterator it(m_pmt_handlers.begin());
			it != m_pmt_handlers.end(); ++it)
		{
			if (it->cislot == slot) // remove the base slot
				it->cislot = slot->linked_next;
			else if (it->cislot)
			{
				eDVBCISlot *prevSlot = it->cislot, *hSlot = it->cislot->linked_next;
				while (hSlot)
				{
					if (hSlot == slot) {
						prevSlot->linked_next = slot->linked_next;
						break;
					}
					prevSlot = hSlot;
					hSlot = hSlot->linked_next;
				}
			}
		}
		if (slot->linked_next)
			slot->linked_next->setSource(slot->current_source);
		else // last CI in chain
			setInputSource(slot->current_tuner, slot->current_source);
		slot->linked_next = 0;
		slot->use_count=0;
		slot->plugged=true;
		slot->user_mapped=false;
		slot->removeService(0xFFFF);
		eDebugCI("eDVBCIInterfaces ciRemoved.recheckPMTHandlers\n");
		recheckPMTHandlers();
	}
}

int eDVBCIInterfaces::CardStatusChangeNotifyCallback(int slotid, Trid_CI_CardStatus_t status)
{
	eDebugCI("CardStatusChangeNotifyCallback. status %d.\n", status);/*abing:0518*/
	switch (status)
	{				
	case _TRID_CARD_REMOVE_:/* card is removed from the slot, no card available */
		CardStatus=0;
		break;
	case _TRID_CARD_INSERT_:/* card is inserted inot the slot, but maybe not a valid CI card */
		CardStatus=1<<1;
		break;
	case _TRID_CARD_INVALID_CARD_:/* card in the slot is not a valid CI card */
		CardStatus=1<<2;
		break;
	case _TRID_CARD_MMI_READY_:/* card is ready for MMI menu access */
		CardStatus|=1<<3;
		break;
	case _TRID_CARD_CA_READY_:/* card is ready for CA access,eg: ready to accept CA_PMT*/
		CardStatus|=1<<4;
		break;
	case _TRID_CARD_MMI_CA_READY_:/* card in ready for both CA and MMI access */
		CardStatus|=1<<5;

		break;
	case _TRID_CARD_UPGRADE_START_:
		CardStatus|=1<<6;
		break;
	case _TRID_CARD_UPGRADE_FINISH_:
		CardStatus|=1<<7;
		break;
	case _TRID_CARD_RESET_START_:
		CardStatus|=1<<8;
		break;
	case _TRID_CARD_IO_ERROR_:/* error occurs while accessing the card */
		CardStatus=1<<9;
		break;
	}
	//eDVBCIInterfaces::getInstance()->CardStatusChangeNotifyCallback(0, status);
	Tridci_cb_status |= NOTIFY_CARD_STATUS;
	//getInstance()->getSlot(slotid)->cdata(status);
	//getInstance()->getSlot(0)->notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);
	return 0;
}

/*收到菜单数据，要求显示*/
//Trid_CI_MMI_RegisterMenuDataNotify  
trid_sint32 eDVBCIInterfaces::MenuDataNotifyCallback(Trid_T_Menu* menu)
{
	Trid_T_Menu* pmenu;
	pmenu = &menu_data;
	eDebugCI("MenuDataNotifyCallback.\n");/*abing:0518*/

	memcpy((char *)pmenu, (char *)menu, sizeof(Trid_T_Menu));
	Tridci_cb_status |= NOTIFY_MENU_DATA;
	//getInstance()->getSlot(0)->notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);
	return 0;
}


/*收到list数据*/
//Trid_CI_MMI_RegisterListDataNotify  
trid_sint32 eDVBCIInterfaces::ListDataNotifyCallback(Trid_T_List* list)
{
	Trid_T_List* plist;
	plist = &list_data;
	eDebugCI("ListDataNotifyCallback.\n");/*abing:0518*/

	memcpy((char *)plist, (char *)list, sizeof(Trid_T_List));
	Tridci_cb_status |= NOTIFY_LIST_DATA;
	//getInstance()->getSlot(0)->notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);
	return 0;
}


/*收到enquiry数据*/
//Trid_CI_MMI_RegisterEnqDataNotify   
trid_sint32 eDVBCIInterfaces::EnqDataNotifyCallback(Trid_T_Enq* enq)
{
	Trid_T_Enq* penq;
	eDebugCI("EnqDataNotifyCallback.\n");/*abing:0518*/
	penq = &enq_data;
	memcpy((char *)penq, (char *)enq, sizeof(Trid_T_Enq));
	Tridci_cb_status |= NOTIFY_ENQ_DATA;
	//getInstance()->getSlot(0)->notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);
	return 0;
}


/*这个是否因为意外，比如模块被拔出，造成的由主机底层发出的停止动作*/
//Trid_CI_MMI_RegisterCloseMMINotify  
trid_sint32 eDVBCIInterfaces::CloseMMINotifyCallback()
{
	Tridci_cb_status |= NOTIFY_CLOSE_MMI;
	eDebugCI("CloseMMINotifyCallback.\n");/*abing:0518*/

	//getInstance()->getSlot(0)->notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);

	return 0;
}


/*这个是否因为意外，比如模块被拔出，造成的由主机底层发出的停止动作*/
//Trid_CI_MMI_RegisterCloseMMINotify  
/*abing:0518*/
trid_sint32 eDVBCIInterfaces::GetHostAVPIDCallback(trid_uint16 *AudioPID, trid_uint16 *VideoPID)
{
	Tridci_cb_status |= NOTIFY_CLOSE_MMI;
	eDebugCI("CloseMMINotifyCallback.\n");

	//getInstance()->getSlot(0)->notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);

	return 0;
}


static bool canDescrambleMultipleServices(int slotid)
{
	char configStr[255];
	eDebugCI("canDescrambleMultipleServices.\n");/*abing:0518*/
	
	snprintf(configStr, 255, "config.ci.%d.canDescrambleMultipleServices", slotid);
	std::string str;
	ePythonConfigQuery::getConfigValue(configStr, str);
    eDebugCI("canDescrambleMultipleServices slot(%d).\n", slotid);/*abing:0518*/
	if ( str == "auto" )
	{
		eDebugCI("canDescrambleMultipleServices.auto.\n");/*abing:0518*/
		std::string appname = eDVBCI_UI::getInstance()->getAppName(slotid);
		if (appname.find("AlphaCrypt") != std::string::npos)
		{
			eDebugCI("canDescrambleMultipleServices.true.\n");/*abing:0518*/
			return true;
		}
	}
	else if (str == "yes")
	{
		eDebugCI("canDescrambleMultipleServices.yes.\n");/*abing:0518*/
		return true;
	}
	//else
	{
		eDebugCI("canDescrambleMultipleServices.no.\n");/*abing:0518*/
		return false;
	}
	
}

#if trid_ci
void eDVBCIInterfaces::recheckPMTHandlers()
{
	//eDebugCI("recheckPMTHAndlers()");
    eDebugCI("eDVBCIInterfaces recheckPMTHandlers.\n");/*abing:0518*/
	for (PMTHandlerList::iterator it(m_pmt_handlers.begin());
		it != m_pmt_handlers.end(); ++it)
	{
		CAID_LIST caids;
		ePtr<eDVBService> service;
		eServiceReferenceDVB ref;
		eDVBCISlot *tmp = it->cislot;
		eDVBServicePMTHandler *pmthandler = it->pmthandler;
		eDVBServicePMTHandler::program p;
		bool plugged_cis_exist = false;

		pmthandler->getServiceReference(ref);
		pmthandler->getService(service);

		eDebugCI("recheck %p %s", pmthandler, ref.toString().c_str());
		for (eSmartPtrList<eDVBCISlot>::iterator ci_it(m_slots.begin()); ci_it != m_slots.end(); ++ci_it)
			if (ci_it->plugged && ci_it->getCAManager())
			{
				eDebug("Slot %d plugged", ci_it->getSlotID());
				ci_it->plugged = false;
				plugged_cis_exist = true;
			}

		// check if this pmt handler has already assigned CI(s) .. and this CI(s) are already running
		if (!plugged_cis_exist)
		{
			while(tmp)
			{
				if (!tmp->running_services.empty())
					break;
				tmp=tmp->linked_next;
			}
			if (tmp) // we dont like to change tsmux for running services
			{
				eDebugCI("already assigned and running CI!\n");
				continue;
			}
		}

		if (!pmthandler->getProgramInfo(p))
		{
			int cnt=0;
			std::set<eDVBServicePMTHandler::program::capid_pair> set(p.caids.begin(), p.caids.end());
			for (std::set<eDVBServicePMTHandler::program::capid_pair>::reverse_iterator x(set.rbegin()); x != set.rend(); ++x, ++cnt)
				caids.push_front(x->caid);
			if (service && cnt)
				service->m_ca = caids;
		}

		if (service)
		{
			caids = service->m_ca;
		}

		if (caids.empty())
		{
			continue; // unscrambled service
		}

		for (eSmartPtrList<eDVBCISlot>::iterator ci_it(m_slots.begin()); ci_it != m_slots.end(); ++ci_it)
		{
			eDebugCI("check Slot %d", ci_it->getSlotID());
			bool useThis=false;
			bool user_mapped=true;
			int ca_manager = ci_it->getCAManager();

			eDebugCI("if ca_manager %d.", ca_manager);
			if (ca_manager)
			{
				int mask=0;
				if (!ci_it->possible_services.empty())
				{
					eDebugCI("if (!ci_it->possible_services.empty()).");
					mask |= 1;
					serviceSet::iterator it = ci_it->possible_services.find(ref);
					if (it != ci_it->possible_services.end())
					{
						eDebug("'%s' is in service list of slot %d... so use it", ref.toString().c_str(), ci_it->getSlotID());
						useThis = true;
					}
					else // check parent
					{
						eServiceReferenceDVB parent_ref = ref.getParentServiceReference();
						if (parent_ref)
						{
							it = ci_it->possible_services.find(ref);
							if (it != ci_it->possible_services.end())
							{
								eDebug("parent '%s' of '%s' is in service list of slot %d... so use it",
									parent_ref.toString().c_str(), ref.toString().c_str(), ci_it->getSlotID());
								useThis = true;
							}
						}
					}
				}
				if (!useThis && !ci_it->possible_providers.empty())
				{
					eDebugCI("if (!useThis && !ci_it->possible_providers.empty())");
					eDVBNamespace ns = ref.getDVBNamespace();
					mask |= 2;
					if (!service) // subservice?
					{
						eServiceReferenceDVB parent_ref = ref.getParentServiceReference();
						eDVBDB::getInstance()->getService(parent_ref, service);
					}
					if (service)
					{
						providerSet::iterator it = ci_it->possible_providers.find(providerPair(service->m_provider_name, ns.get()));
						if (it != ci_it->possible_providers.end())
						{
							eDebug("'%s/%08x' is in provider list of slot %d... so use it", service->m_provider_name.c_str(), ns.get(), ci_it->getSlotID());
							useThis = true;
						}
					}
				}
				if (!useThis && !ci_it->possible_caids.empty())
				{
					eDebugCI("if (!useThis && !ci_it->possible_caids.empty())");
					mask |= 4;
					for (CAID_LIST::iterator ca(caids.begin()); ca != caids.end(); ++ca)
					{
						caidSet::iterator it = ci_it->possible_caids.find(*ca);
						if (it != ci_it->possible_caids.end())
						{
							eDebug("caid '%04x' is in caid list of slot %d... so use it", *ca, ci_it->getSlotID());
							useThis = true;
							break;
						}
					}
				}
				if (!useThis && !mask)
				{
					eDebugCI("if (!useThis && !mask)");
					const std::vector<uint16_t> &ci_caids = DVBCI_GetCAIDs();
					for (CAID_LIST::iterator ca(caids.begin()); ca != caids.end(); ++ca)
					{
						std::vector<uint16_t>::const_iterator z =
							std::lower_bound(ci_caids.begin(), ci_caids.end(), *ca);
						if ( z != ci_caids.end() && *z == *ca )
						{
							eDebug("The CI in Slot %d has said it can handle caid %04x... so use it", ci_it->getSlotID(), *z);
							useThis = true;
							user_mapped = false;
							break;
						}
					}
				}
			}

			eDebugCI("if (useThis) %d.", useThis);
			if (useThis)
			{
				// check if this CI is already assigned to this pmthandler
				eDVBCISlot *tmp = it->cislot;
				while(tmp)
				{
					if (tmp == ci_it)
						break;
					tmp=tmp->linked_next;
				}
				if (tmp) // ignore already assigned cislots...
				{
					eDebugCI("already assigned!");
					continue;
				}
				eDebugCI("current slot %d usecount %d", ci_it->getSlotID(), ci_it->use_count);
				if (ci_it->use_count)  // check if this CI can descramble more than one service
				{
					bool found = false;
					useThis = false;
					PMTHandlerList::iterator tmp = m_pmt_handlers.begin();
					while (!found && tmp != m_pmt_handlers.end())
					{
						eDebugCI(".");
						eDVBCISlot *tmp_cislot = tmp->cislot;
						while (!found && tmp_cislot)
						{
							eDebugCI("..");
							eServiceReferenceDVB ref2;
							tmp->pmthandler->getServiceReference(ref2);
							if ( tmp_cislot == ci_it && it->pmthandler != tmp->pmthandler )
							{
								eDebugCI("check pmthandler %s for same service/tp", ref2.toString().c_str());
								eDVBChannelID s1, s2;
								if (ref != ref2)
								{
									eDebugCI("different services!");
									ref.getChannelID(s1);
									ref2.getChannelID(s2);
								}
								if (ref == ref2 || (s1 == s2 && canDescrambleMultipleServices(tmp_cislot->getSlotID())))
								{
									found = true;
									eDebugCI("found!");
									eDVBCISlot *tmpci = it->cislot = tmp->cislot;
									while(tmpci)
									{
										++tmpci->use_count;
										eDebug("(2)CISlot %d, usecount now %d", tmpci->getSlotID(), tmpci->use_count);
										tmpci=tmpci->linked_next;
									}
								}
							}
							tmp_cislot=tmp_cislot->linked_next;
						}
						eDebugCI("...");
						++tmp;
					}
				}

				if (useThis)
				{
					if (ci_it->user_mapped)  // we dont like to link user mapped CIs
					{
						eDebugCI("user mapped CI already in use... dont link!");
						continue;
					}

					++ci_it->use_count;
					eDebug("(1)CISlot %d, usecount now %d", ci_it->getSlotID(), ci_it->use_count);

					data_source ci_source=CI_A;
					switch(ci_it->getSlotID())
					{
						case 0: ci_source = CI_A; break;
						case 1: ci_source = CI_B; break;
						case 2: ci_source = CI_C; break;
						case 3: ci_source = CI_D; break;
						default:
							eDebug("try to get source for CI %d!!\n", ci_it->getSlotID());
							break;
					}

					if (!it->cislot)
					{
						int tunernum = -1;
						eUsePtr<iDVBChannel> channel;
						if (!pmthandler->getChannel(channel))
						{
							ePtr<iDVBFrontend> frontend;
							if (!channel->getFrontend(frontend))
							{
								eDVBFrontend *fe = (eDVBFrontend*) &(*frontend);
								tunernum = fe->getSlotID();
							}
						}
						ASSERT(tunernum != -1);
						data_source tuner_source = TUNER_A;
						switch (tunernum)
						{
							case 0: tuner_source = TUNER_A; break;
							case 1: tuner_source = TUNER_B; break;
							case 2: tuner_source = TUNER_C; break;
							case 3: tuner_source = TUNER_D; break;
							default:
								eDebug("try to get source for tuner %d!!\n", tunernum);
								break;
						}
						ci_it->current_tuner = tunernum;
						setInputSource(tunernum, ci_source);
						ci_it->setSource(tuner_source);
					}
					else
					{
						ci_it->current_tuner = it->cislot->current_tuner;
						ci_it->linked_next = it->cislot;
						ci_it->setSource(ci_it->linked_next->current_source);
						ci_it->linked_next->setSource(ci_source);
					}
					it->cislot = ci_it;
					eDebugCI("assigned!");
					gotPMT(pmthandler);
				}

				if (it->cislot && user_mapped) // CI assigned to this pmthandler in this run.. and user mapped? then we break here.. we dont like to link other CIs to user mapped CIs
				{
					eDebugCI("user mapped CI assigned... dont link CIs!");
					break;
				}
			}
		}
	}
	eDebugCI("eDVBCIInterfaces recheckPMTHandlers.end\n");/*abing:0518*/
}
#else
void eDVBCIInterfaces::recheckPMTHandlers()
{
	//eDebugCI("recheckPMTHAndlers()");
    eDebugCI("eDVBCIInterfaces recheckPMTHandlers.\n");/*abing:0518*/
	for (PMTHandlerList::iterator it(m_pmt_handlers.begin());
		it != m_pmt_handlers.end(); ++it)
	{
		CAID_LIST caids;
		ePtr<eDVBService> service;
		eServiceReferenceDVB ref;
		eDVBCISlot *tmp = it->cislot;
		eDVBServicePMTHandler *pmthandler = it->pmthandler;
		eDVBServicePMTHandler::program p;
		bool plugged_cis_exist = false;

		pmthandler->getServiceReference(ref);
		pmthandler->getService(service);

		eDebugCI("recheck %p %s", pmthandler, ref.toString().c_str());
		for (eSmartPtrList<eDVBCISlot>::iterator ci_it(m_slots.begin()); ci_it != m_slots.end(); ++ci_it)
			if (ci_it->plugged && ci_it->getCAManager())
			{
				eDebug("Slot %d plugged", ci_it->getSlotID());
				ci_it->plugged = false;
				plugged_cis_exist = true;
			}

		// check if this pmt handler has already assigned CI(s) .. and this CI(s) are already running
		if (!plugged_cis_exist)
		{
			while(tmp)
			{
				if (!tmp->running_services.empty())
					break;
				tmp=tmp->linked_next;
			}
			if (tmp) // we dont like to change tsmux for running services
			{
				eDebugCI("already assigned and running CI!\n");
				continue;
			}
		}

		if (!pmthandler->getProgramInfo(p))
		{
			int cnt=0;
			std::set<eDVBServicePMTHandler::program::capid_pair> set(p.caids.begin(), p.caids.end());
			for (std::set<eDVBServicePMTHandler::program::capid_pair>::reverse_iterator x(set.rbegin()); x != set.rend(); ++x, ++cnt)
				caids.push_front(x->caid);
			if (service && cnt)
				service->m_ca = caids;
		}

		if (service)
			caids = service->m_ca;

		if (caids.empty())
			continue; // unscrambled service

		for (eSmartPtrList<eDVBCISlot>::iterator ci_it(m_slots.begin()); ci_it != m_slots.end(); ++ci_it)
		{
			eDebugCI("check Slot %d", ci_it->getSlotID());
			bool useThis=false;
			bool user_mapped=true;
			eDVBCICAManagerSession *ca_manager = ci_it->getCAManager();

			if (ca_manager)
			{
				int mask=0;
				if (!ci_it->possible_services.empty())
				{
					mask |= 1;
					serviceSet::iterator it = ci_it->possible_services.find(ref);
					if (it != ci_it->possible_services.end())
					{
						eDebug("'%s' is in service list of slot %d... so use it", ref.toString().c_str(), ci_it->getSlotID());
						useThis = true;
					}
					else // check parent
					{
						eServiceReferenceDVB parent_ref = ref.getParentServiceReference();
						if (parent_ref)
						{
							it = ci_it->possible_services.find(ref);
							if (it != ci_it->possible_services.end())
							{
								eDebug("parent '%s' of '%s' is in service list of slot %d... so use it",
									parent_ref.toString().c_str(), ref.toString().c_str(), ci_it->getSlotID());
								useThis = true;
							}
						}
					}
				}
				if (!useThis && !ci_it->possible_providers.empty())
				{
					eDVBNamespace ns = ref.getDVBNamespace();
					mask |= 2;
					if (!service) // subservice?
					{
						eServiceReferenceDVB parent_ref = ref.getParentServiceReference();
						eDVBDB::getInstance()->getService(parent_ref, service);
					}
					if (service)
					{
						providerSet::iterator it = ci_it->possible_providers.find(providerPair(service->m_provider_name, ns.get()));
						if (it != ci_it->possible_providers.end())
						{
							eDebug("'%s/%08x' is in provider list of slot %d... so use it", service->m_provider_name.c_str(), ns.get(), ci_it->getSlotID());
							useThis = true;
						}
					}
				}
				if (!useThis && !ci_it->possible_caids.empty())
				{
					mask |= 4;
					for (CAID_LIST::iterator ca(caids.begin()); ca != caids.end(); ++ca)
					{
						caidSet::iterator it = ci_it->possible_caids.find(*ca);
						if (it != ci_it->possible_caids.end())
						{
							eDebug("caid '%04x' is in caid list of slot %d... so use it", *ca, ci_it->getSlotID());
							useThis = true;
							break;
						}
					}
				}
				if (!useThis && !mask)
				{
					const std::vector<uint16_t> &ci_caids = ca_manager->getCAIDs();
					for (CAID_LIST::iterator ca(caids.begin()); ca != caids.end(); ++ca)
					{
						std::vector<uint16_t>::const_iterator z =
							std::lower_bound(ci_caids.begin(), ci_caids.end(), *ca);
						if ( z != ci_caids.end() && *z == *ca )
						{
							eDebug("The CI in Slot %d has said it can handle caid %04x... so use it", ci_it->getSlotID(), *z);
							useThis = true;
							user_mapped = false;
							break;
						}
					}
				}
			}

			if (useThis)
			{
				// check if this CI is already assigned to this pmthandler
				eDVBCISlot *tmp = it->cislot;
				while(tmp)
				{
					if (tmp == ci_it)
						break;
					tmp=tmp->linked_next;
				}
				if (tmp) // ignore already assigned cislots...
				{
					eDebugCI("already assigned!");
					continue;
				}
				eDebugCI("current slot %d usecount %d", ci_it->getSlotID(), ci_it->use_count);
				if (ci_it->use_count)  // check if this CI can descramble more than one service
				{
					bool found = false;
					useThis = false;
					PMTHandlerList::iterator tmp = m_pmt_handlers.begin();
					while (!found && tmp != m_pmt_handlers.end())
					{
						eDebugCI(".");
						eDVBCISlot *tmp_cislot = tmp->cislot;
						while (!found && tmp_cislot)
						{
							eDebugCI("..");
							eServiceReferenceDVB ref2;
							tmp->pmthandler->getServiceReference(ref2);
							if ( tmp_cislot == ci_it && it->pmthandler != tmp->pmthandler )
							{
								eDebugCI("check pmthandler %s for same service/tp", ref2.toString().c_str());
								eDVBChannelID s1, s2;
								if (ref != ref2)
								{
									eDebugCI("different services!");
									ref.getChannelID(s1);
									ref2.getChannelID(s2);
								}
								if (ref == ref2 || (s1 == s2 && canDescrambleMultipleServices(tmp_cislot->getSlotID())))
								{
									found = true;
									eDebugCI("found!");
									eDVBCISlot *tmpci = it->cislot = tmp->cislot;
									while(tmpci)
									{
										++tmpci->use_count;
										eDebug("(2)CISlot %d, usecount now %d", tmpci->getSlotID(), tmpci->use_count);
										tmpci=tmpci->linked_next;
									}
								}
							}
							tmp_cislot=tmp_cislot->linked_next;
						}
						eDebugCI("...");
						++tmp;
					}
				}

				if (useThis)
				{
					if (ci_it->user_mapped)  // we dont like to link user mapped CIs
					{
						eDebugCI("user mapped CI already in use... dont link!");
						continue;
					}

					++ci_it->use_count;
					eDebug("(1)CISlot %d, usecount now %d", ci_it->getSlotID(), ci_it->use_count);

					data_source ci_source=CI_A;
					switch(ci_it->getSlotID())
					{
						case 0: ci_source = CI_A; break;
						case 1: ci_source = CI_B; break;
						case 2: ci_source = CI_C; break;
						case 3: ci_source = CI_D; break;
						default:
							eDebug("try to get source for CI %d!!\n", ci_it->getSlotID());
							break;
					}

					if (!it->cislot)
					{
						int tunernum = -1;
						eUsePtr<iDVBChannel> channel;
						if (!pmthandler->getChannel(channel))
						{
							ePtr<iDVBFrontend> frontend;
							if (!channel->getFrontend(frontend))
							{
								eDVBFrontend *fe = (eDVBFrontend*) &(*frontend);
								tunernum = fe->getSlotID();
							}
						}
						ASSERT(tunernum != -1);
						data_source tuner_source = TUNER_A;
						switch (tunernum)
						{
							case 0: tuner_source = TUNER_A; break;
							case 1: tuner_source = TUNER_B; break;
							case 2: tuner_source = TUNER_C; break;
							case 3: tuner_source = TUNER_D; break;
							default:
								eDebug("try to get source for tuner %d!!\n", tunernum);
								break;
						}
						ci_it->current_tuner = tunernum;
						setInputSource(tunernum, ci_source);
						ci_it->setSource(tuner_source);
					}
					else
					{
						ci_it->current_tuner = it->cislot->current_tuner;
						ci_it->linked_next = it->cislot;
						ci_it->setSource(ci_it->linked_next->current_source);
						ci_it->linked_next->setSource(ci_source);
					}
					it->cislot = ci_it;
					eDebugCI("assigned!");
					gotPMT(pmthandler);
				}

				if (it->cislot && user_mapped) // CI assigned to this pmthandler in this run.. and user mapped? then we break here.. we dont like to link other CIs to user mapped CIs
				{
					eDebugCI("user mapped CI assigned... dont link CIs!");
					break;
				}
			}
		}
	}
}
#endif
void eDVBCIInterfaces::addPMTHandler(eDVBServicePMTHandler *pmthandler)
{
	// check if this pmthandler is already registered
	PMTHandlerList::iterator it = m_pmt_handlers.begin();
    eDebugCI("eDVBCIInterfaces addPMTHandler.\n");/*abing:0518*/
	while (it != m_pmt_handlers.end())
	{
		if ( *it++ == pmthandler )
			return;
	}

	eServiceReferenceDVB ref;
	pmthandler->getServiceReference(ref);
	eDebug("[eDVBCIInterfaces] addPMTHandler %s", ref.toString().c_str());

	m_pmt_handlers.push_back(CIPmtHandler(pmthandler));
	recheckPMTHandlers();
	eDebugCI("out addPMTHandler.\n");/*abing:0518*/
}

void eDVBCIInterfaces::removePMTHandler(eDVBServicePMTHandler *pmthandler)
{
	PMTHandlerList::iterator it=std::find(m_pmt_handlers.begin(),m_pmt_handlers.end(),pmthandler);
    eDebugCI("eDVBCIInterfaces removePMTHandler.\n");/*abing:0518*/
	if (it != m_pmt_handlers.end())
	{
		eDVBCISlot *slot = it->cislot;
		eDVBCISlot *base_slot = slot;
		eDVBServicePMTHandler *pmthandler = it->pmthandler;
		m_pmt_handlers.erase(it);

		eServiceReferenceDVB service_to_remove;
		pmthandler->getServiceReference(service_to_remove);

		bool sameServiceExist=false;
		for (PMTHandlerList::iterator i=m_pmt_handlers.begin(); i != m_pmt_handlers.end(); ++i)
		{
			if (i->cislot)
			{
				eServiceReferenceDVB ref;
				i->pmthandler->getServiceReference(ref);
				if ( ref == service_to_remove )
				{
					sameServiceExist=true;
					break;
				}
			}
		}

		while(slot)
		{
			eDVBCISlot *next = slot->linked_next;
			if (!sameServiceExist)
			{
				eDebug("[eDVBCIInterfaces] remove last pmt handler for service %s send empty capmt",
					service_to_remove.toString().c_str());
				std::vector<uint16_t> caids;
				caids.push_back(0xFFFF);
				slot->sendCAPMT(pmthandler, caids);  // send a capmt without caids to remove a running service
				slot->removeService(service_to_remove.getServiceID().get());
			}

			if (!--slot->use_count)
			{
				if (slot->linked_next)
					slot->linked_next->setSource(slot->current_source);
				else
					setInputSource(slot->current_tuner, slot->current_source);

				if (base_slot != slot)
				{
					eDVBCISlot *tmp = it->cislot;
					while(tmp->linked_next != slot)
						tmp = tmp->linked_next;
					ASSERT(tmp);
					if (slot->linked_next)
						tmp->linked_next = slot->linked_next;
					else
						tmp->linked_next = 0;
				}
				else // removed old base slot.. update ptr
					base_slot = slot->linked_next;
				slot->linked_next = 0;
				slot->user_mapped = false;
			}
			eDebug("(3) slot %d usecount is now %d", slot->getSlotID(), slot->use_count);
			slot = next;
		}
		// check if another service is waiting for the CI
		eDebugCI("very good, removePMTHandler.\n");
		recheckPMTHandlers();
	}
}

void eDVBCIInterfaces::gotPMT(eDVBServicePMTHandler *pmthandler)
{
	//eDebug("[eDVBCIInterfaces] gotPMT");
    eDebugCI("eDVBCIInterfaces gotPMT.very good.....................\n");/*abing:0518*/
	PMTHandlerList::iterator it=std::find(m_pmt_handlers.begin(), m_pmt_handlers.end(), pmthandler);
	if (it != m_pmt_handlers.end() && it->cislot)
	{
		eDVBCISlot *tmp = it->cislot;
		eDebugCI("eDVBCIInterfaces gotPMT  tmp = it->cislot.\n");/*abing:0518*/
		while(tmp)
		{
			eDebugCI("eDVBCIInterfaces gotPMT  do next tmp = tmp->linked_next.\n");/*abing:0518*/
			eDebugCI("check slot %d %d %d", tmp->getSlotID(), tmp->running_services.empty(), canDescrambleMultipleServices(tmp->getSlotID()));
			if (tmp->running_services.empty() || canDescrambleMultipleServices(tmp->getSlotID()))
			{
				eDebugCI("tmp->sendCAPMT(pmthandler);.\n");/*abing:0518*/
				tmp->sendCAPMT(pmthandler);
			}
			tmp = tmp->linked_next;
		}
	}
    eDebugCI("eDVBCIInterfaces gotPMT.end\n");/*abing:0518*/
}

int eDVBCIInterfaces::getMMIState(int slotid)
{
	eDVBCISlot *slot;

    eDebugCI("eDVBCIInterfaces getMMIState slot(%d).\n", slotid);/*abing:0518*/
	if( (slot = getSlot(slotid)) == 0 )
		return -1;

	return slot->getMMIState();
}

int eDVBCIInterfaces::setInputSource(int tuner_no, data_source source)
{
//	eDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	eDebugCI("eDVBCIInterfaces::setInputSource(%d %d)", tuner_no, (int)source);
	if (getNumOfSlots() > 1) // FIXME .. we force DM8000 when more than one CI Slot is avail
	{
		char buf[64];
		snprintf(buf, 64, "/proc/stb/tsmux/input%d", tuner_no);

		FILE *input=0;
		if((input = fopen(buf, "wb")) == NULL) {
			eDebug("cannot open %s", buf);
			return 0;
		}

		if (tuner_no > 3)
			eDebug("setInputSource(%d, %d) failed... dm8000 just have four inputs", tuner_no, (int)source);

		switch(source)
		{
			case CI_A:
				fprintf(input, "CI0");
				break;
			case CI_B:
				fprintf(input, "CI1");
				break;
			case CI_C:
				fprintf(input, "CI2");
			break;
			case CI_D:
				fprintf(input, "CI3");
				break;
			case TUNER_A:
				fprintf(input, "A");
				break;
			case TUNER_B:
				fprintf(input, "B");
				break;
			case TUNER_C:
				fprintf(input, "C");
				break;
			case TUNER_D:
				fprintf(input, "D");
				break;
			default:
				eDebug("setInputSource for input %d failed!!!\n", (int)source);
				break;
		}

		fclose(input);
	}
	else  // DM7025
	{
		char buf[64];
		snprintf(buf, 64, "/proc/stb/tsmux/input%d", tuner_no);

		if (tuner_no > 2) //su980 have 3 tuner
			eDebug("setInputSource(%d, %d) failed... dm7025 just have three inputs", tuner_no, (int)source);

		FILE *input=0;
		if((input = fopen(buf, "wb")) == NULL) {
			eDebug("cannot open %s", buf);
			return 0;
		}

		switch(source)
		{
			case CI_A:
				fprintf(input, "CI");
				break;
			case TUNER_A:
				fprintf(input, "A");
				break;
			case TUNER_B:
				fprintf(input, "B");
				break;
			case TUNER_C:
				fprintf(input, "C");
				break;
			default:
				eDebug("setInputSource for input %d failed!!!\n", (int)source);
				break;
		}

		fclose(input);
	}
	eDebug("eDVBCIInterfaces->setInputSource(%d, %d)", tuner_no, (int)source);
	return 0;
}

PyObject *eDVBCIInterfaces::getDescrambleRules(int slotid)
{
	eDVBCISlot *slot = getSlot(slotid);
	eDebugCI("eDVBCIInterfaces getDescrambleRules slot(%d).\n", slotid);/*abing:0518*/
	if (!slot)
	{
		char tmp[255];
		snprintf(tmp, 255, "eDVBCIInterfaces::getDescrambleRules try to get rules for CI Slot %d... but just %zd slots are available", slotid, m_slots.size());
		PyErr_SetString(PyExc_StandardError, tmp);
		return 0;
	}
	ePyObject tuple = PyTuple_New(3);
	int caids = slot->possible_caids.size();
	int services = slot->possible_services.size();
	int providers = slot->possible_providers.size();
	ePyObject caid_list = PyList_New(caids);
	ePyObject service_list = PyList_New(services);
	ePyObject provider_list = PyList_New(providers);
	caidSet::iterator caid_it(slot->possible_caids.begin());
	while(caids)
	{
		--caids;
		PyList_SET_ITEM(caid_list, caids, PyLong_FromLong(*caid_it));
		++caid_it;
	}
	serviceSet::iterator ref_it(slot->possible_services.begin());
	while(services)
	{
		--services;
		PyList_SET_ITEM(service_list, services, PyString_FromString(ref_it->toString().c_str()));
		++ref_it;
	}
	providerSet::iterator provider_it(slot->possible_providers.begin());
	while(providers)
	{
		ePyObject tuple = PyTuple_New(2);
		PyTuple_SET_ITEM(tuple, 0, PyString_FromString(provider_it->first.c_str()));
		PyTuple_SET_ITEM(tuple, 1, PyLong_FromUnsignedLong(provider_it->second));
		--providers;
		PyList_SET_ITEM(provider_list, providers, tuple);
		++provider_it;
	}
	PyTuple_SET_ITEM(tuple, 0, service_list);
	PyTuple_SET_ITEM(tuple, 1, provider_list);
	PyTuple_SET_ITEM(tuple, 2, caid_list);
	return tuple;
}

const char *PyObject_TypeStr(PyObject *o)
{
	eDebugCI("PyObject_TypeStr.\n");/*abing:0518*/
	return o->ob_type && o->ob_type->tp_name ? o->ob_type->tp_name : "unknown object type";
}

RESULT eDVBCIInterfaces::setDescrambleRules(int slotid, SWIG_PYOBJECT(ePyObject) obj )
{
	eDVBCISlot *slot = getSlot(slotid);
	eDebugCI("setDescrambleRules slot(%d).\n", slotid);/*abing:0518*/
	if (!slot)
	{
		char tmp[255];
		snprintf(tmp, 255, "eDVBCIInterfaces::setDescrambleRules try to set rules for CI Slot %d... but just %zd slots are available", slotid, m_slots.size());
		PyErr_SetString(PyExc_StandardError, tmp);
		return -1;
	}
	if (!PyTuple_Check(obj))
	{
		char tmp[255];
		snprintf(tmp, 255, "2nd argument of setDescrambleRules is not a tuple.. it is a '%s'!!", PyObject_TypeStr(obj));
		PyErr_SetString(PyExc_StandardError, tmp);
		return -1;
	}
	if (PyTuple_Size(obj) != 3)
	{
		const char *errstr = "eDVBCIInterfaces::setDescrambleRules not enough entrys in argument tuple!!\n"
			"first argument should be a pythonlist with possible services\n"
			"second argument should be a pythonlist with possible providers/dvbnamespace tuples\n"
			"third argument should be a pythonlist with possible caids";
		PyErr_SetString(PyExc_StandardError, errstr);
		return -1;
	}
	ePyObject service_list = PyTuple_GET_ITEM(obj, 0);
	ePyObject provider_list = PyTuple_GET_ITEM(obj, 1);
	ePyObject caid_list = PyTuple_GET_ITEM(obj, 2);
	if (!PyList_Check(service_list) || !PyList_Check(provider_list) || !PyList_Check(caid_list))
	{
		char errstr[512];
		snprintf(errstr, 512, "eDVBCIInterfaces::setDescrambleRules incorrect data types in argument tuple!!\n"
			"first argument(%s) should be a pythonlist with possible services (reference strings)\n"
			"second argument(%s) should be a pythonlist with possible providers (providername strings)\n"
			"third argument(%s) should be a pythonlist with possible caids (ints)",
			PyObject_TypeStr(service_list), PyObject_TypeStr(provider_list), PyObject_TypeStr(caid_list));
		PyErr_SetString(PyExc_StandardError, errstr);
		return -1;
	}
	slot->possible_caids.clear();
	slot->possible_services.clear();
	slot->possible_providers.clear();
	int size = PyList_Size(service_list);
	while(size)
	{
		--size;
		ePyObject refstr = PyList_GET_ITEM(service_list, size);
		if (!PyString_Check(refstr))
		{
			char buf[255];
			snprintf(buf, 255, "eDVBCIInterfaces::setDescrambleRules entry in service list is not a string.. it is '%s'!!", PyObject_TypeStr(refstr));
			PyErr_SetString(PyExc_StandardError, buf);
			return -1;
		}
		char *tmpstr = PyString_AS_STRING(refstr);
		eServiceReference ref(tmpstr);
		if (ref.valid())
			slot->possible_services.insert(ref);
		else
			eDebug("eDVBCIInterfaces::setDescrambleRules '%s' is not a valid service reference... ignore!!", tmpstr);
	};
	size = PyList_Size(provider_list);
	while(size)
	{
		--size;
		ePyObject tuple = PyList_GET_ITEM(provider_list, size);
		if (!PyTuple_Check(tuple))
		{
			char buf[255];
			snprintf(buf, 255, "eDVBCIInterfaces::setDescrambleRules entry in provider list is not a tuple it is '%s'!!", PyObject_TypeStr(tuple));
			PyErr_SetString(PyExc_StandardError, buf);
			return -1;
		}
		if (PyTuple_Size(tuple) != 2)
		{
			char buf[255];
			snprintf(buf, 255, "eDVBCIInterfaces::setDescrambleRules provider tuple has %zd instead of 2 entries!!", PyTuple_Size(tuple));
			PyErr_SetString(PyExc_StandardError, buf);
			return -1;
		}
		if (!PyString_Check(PyTuple_GET_ITEM(tuple, 0)))
		{
			char buf[255];
			snprintf(buf, 255, "eDVBCIInterfaces::setDescrambleRules 1st entry in provider tuple is not a string it is '%s'", PyObject_TypeStr(PyTuple_GET_ITEM(tuple, 0)));
			PyErr_SetString(PyExc_StandardError, buf);
			return -1;
		}
		if (!PyLong_Check(PyTuple_GET_ITEM(tuple, 1)))
		{
			char buf[255];
			snprintf(buf, 255, "eDVBCIInterfaces::setDescrambleRules 2nd entry in provider tuple is not a long it is '%s'", PyObject_TypeStr(PyTuple_GET_ITEM(tuple, 1)));
			PyErr_SetString(PyExc_StandardError, buf);
			return -1;
		}
		char *tmpstr = PyString_AS_STRING(PyTuple_GET_ITEM(tuple, 0));
		uint32_t orbpos = PyLong_AsUnsignedLong(PyTuple_GET_ITEM(tuple, 1));
		if (strlen(tmpstr))
			slot->possible_providers.insert(std::pair<std::string, uint32_t>(tmpstr, orbpos));
		else
			eDebug("eDVBCIInterfaces::setDescrambleRules ignore invalid entry in provider tuple (string is empty)!!");
	};
	size = PyList_Size(caid_list);
	while(size)
	{
		--size;
		ePyObject caid = PyList_GET_ITEM(caid_list, size);
		if (!PyLong_Check(caid))
		{
			char buf[255];
			snprintf(buf, 255, "eDVBCIInterfaces::setDescrambleRules entry in caid list is not a long it is '%s'!!", PyObject_TypeStr(caid));
			PyErr_SetString(PyExc_StandardError, buf);
			return -1;
		}
		int tmpcaid = PyLong_AsLong(caid);
		if (tmpcaid > 0 && tmpcaid < 0x10000)
			slot->possible_caids.insert(tmpcaid);
		else
			eDebug("eDVBCIInterfaces::setDescrambleRules %d is not a valid caid... ignore!!", tmpcaid);
	};
	return 0;
}

PyObject *eDVBCIInterfaces::readCICaIds(int slotid)
{
	eDVBCISlot *slot = getSlot(slotid);
	eDebugCI("----->abing<----- eDVBCIInterfaces readCICaIds slot(%d).\n", slotid);/*abing:0518*/
	if (!slot)
	{
		char tmp[255];
		snprintf(tmp, 255, "eDVBCIInterfaces::readCICaIds try to get CAIds for CI Slot %d... but just %zd slots are available", slotid, m_slots.size());
		PyErr_SetString(PyExc_StandardError, tmp);
	}
	else
	{
		int idx=0;
#if trid_ci
		int ca_manager = slot->getCAManager();
		const std::vector<uint16_t> *ci_caids = ca_manager ? &DVBCI_GetCAIDs() : 0;
#else
		eDVBCICAManagerSession *ca_manager = slot->getCAManager();
		const std::vector<uint16_t> *ci_caids = ca_manager ? &ca_manager->getCAIDs() : 0;
#endif
		ePyObject list = PyList_New(ci_caids ? ci_caids->size() : 0);
		if (ci_caids)
		{
			for (std::vector<uint16_t>::const_iterator it = ci_caids->begin(); it != ci_caids->end(); ++it)
				PyList_SET_ITEM(list, idx++, PyLong_FromLong(*it));
		}
		return list;
	}
	return 0;
}

int eDVBCIInterfaces::setCIClockRate(int slotid, int rate)
{
	eDVBCISlot *slot = getSlot(slotid);
	eDebugCI("eDVBCIInterfaces eDVBCIInterfaces:setCIClockRate %d , slot(%d).\n", rate, slotid);/*abing:0518*/
	if (slot)
		return slot->setClockRate(rate);
	return -1;
}

int eDVBCISlot::send(const unsigned char *data, size_t len)
{
	int res=0;
	//int i;
	//eDebugNoNewLine("< ");
	//for(i=0;i<len;i++)
	//	eDebugNoNewLine("%02x ",data[i]);
	//eDebug("");
#if trid_ci
	return 0;
#else
	eDebugCI("eDVBCISlot send.\n");/*abing:0518*/
	if (len > 6)/*abing:0518*/
	{
		eDebugCI("---->abing<----- send data 0123456 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x.\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
	}
	else
	{
		int i;
		
		for(i=0;i<len;i++)
			printf("0x%x, ", data[i]);
		printf(".\n");
	}

	if (sendqueue.empty())
		res = ::write(fd, data, len);

	if (res < 0 || (unsigned int)res != len)
	{
		unsigned char *d = new unsigned char[len];
		memcpy(d, data, len);
		sendqueue.push( queueData(d, len) );
		notifier->setRequested(eSocketNotifier::Read | eSocketNotifier::Priority | eSocketNotifier::Write);
	}
	return res;
#endif
}
#if trid_ci
void eDVBCISlot::cdata(int/*Trid_CI_CardStatus_t*/ status)
{
	switch (status)
      {
      case _TRID_CARD_REMOVE_:/* card is removed from the slot, no card available */
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_REMOVE_ set ui state to 0.\n");			 
	  	/*{stateRemoved, stateInserted, stateInvalid, stateResetted}*/
		if(state != stateRemoved) {
			state = stateRemoved;
			eDebugCI("<dvbci.cpp eDVBCISlot data> state = stateRemoved\n");
			while(sendqueue.size())
			{
				delete [] sendqueue.top().data;
				sendqueue.pop();
			}
#if trid_ci
			application_manager = 0;
			ca_manager = 0;
			mmi_session = 0;
#else
			eDVBCISession::deleteSessions(this);
#endif
			eDVBCIInterfaces::getInstance()->ciRemoved(this);
			//notifier->setRequested(eSocketNotifier::Read);
			eDVBCI_UI::getInstance()->setState(getSlotID(),0);
		}
	  	break;
	case _TRID_CARD_INSERT_:/* card is inserted inot the slot, but maybe not a valid CI card */
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_INSERT_  set ui state to 1.\n");			 
		if(state != stateInserted) {
			eDebug("ci inserted in slot %d", getSlotID());
			state = stateInserted;
			eDebugCI("<dvbci.cpp eDVBCISlot data> state == stateInserted  set ui state 1\n");
			eDVBCI_UI::getInstance()->setState(getSlotID(),1);
			//notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);
			/* enable PRI to detect removal or errors */
		}
		break;
	case _TRID_CARD_INVALID_CARD_:/* card in the slot is not a valid CI card */
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_INVALID_CARD_ <nothing>\n");			 
		//eDVBCI_UI::getInstance()->setState(/*getSlotID()*/0,0);
		break;
	case _TRID_CARD_MMI_READY_:/* card is ready for MMI menu access */
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_MMI_READY_  set ui state to 2.\n");			 
		{
			RETURN_TYPE ret;
			trid_uint8 menu_str_len;
			
			setAppManager(1);
			
			ret = Trid_CI_AppInfo_GetMenuStr(app_info,&menu_str_len);
			eDebugCI("<dvbci.cpp eDVBCISlot data> get app info name: %s.\n", app_info);
			app_info[menu_str_len] = 0;
			
			eDVBCI_UI::getInstance()->setAppName(getSlotID(), app_info);
			
			eDVBCI_UI::getInstance()->setState(getSlotID(), 2);
		}
#if 0
		eDebugCI("--->abing<--- CardStatusChange: set mmi state to 1.\n");			 
		if(state != stateInserted) {
			eDebug("ci inserted in slot %d", getSlotID());
			state = stateInserted;
			eDebugCI("!!!----->abing<----- eDVBCISlot data state == stateInserted  set ui state 1\n");
			eDVBCI_UI::getInstance()->setState(getSlotID(),1);
	/*			notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);*//*abing:attention what here for*/
	/*			enable PRI to detect removal or errors */
		}
		state = stateResetted;
#endif
		break;
	case _TRID_CARD_CA_READY_:/* card is ready for CA access,eg: ready to accept CA_PMT*/
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_CA_READY_  eDVBCICAManagerSession ca info:\n");
		setCAManager(1);
		DVBCI_Packcaids();
		eDVBCIInterfaces::getInstance()->recheckPMTHandlers();
		break;
	case _TRID_CARD_MMI_CA_READY_:/* card in ready for both CA and MMI access */
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_MMI_CA_READY_ set mmi state to 1.\n");		
		DVBCI_Packcaids();
		eDVBCIInterfaces::getInstance()->recheckPMTHandlers();
		
		setCAManager(1);
		//setAppManager(1);
		//setMMIManager(1);
		break;
	case _TRID_CARD_UPGRADE_START_:
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_UPGRADE_START_<nothing>\n");			 
		break;
	case _TRID_CARD_UPGRADE_FINISH_:
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_UPGRADE_FINISH_<nothing>\n");			 
		break;
	case _TRID_CARD_RESET_START_:
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_RESET_START_\n");		
		break;
	case _TRID_CARD_IO_ERROR_: /* error occurs while accessing the card */
		eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_IO_ERROR_<nothing>\n");			 
		break;
		
      }	
}

void eDVBCISlot::data(int/*Trid_CI_CardStatus_t*/ status)
{
	eDebugCI("-------------------abing-------------------------data what ? %d.\n", status);
#if 1
	if (status & NOTIFY_CARD_STATUS)
	{
	/*{stateRemoved, stateInserted, stateInvalid, stateResetted}*/
		if (CardStatus == 0)/* card is removed from the slot, no card available */
		{
			eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_REMOVE_ set ui state to 0.\n");			   
			if(state != stateRemoved) 
			{
				state = stateRemoved;
				eDebugCI("<dvbci.cpp eDVBCISlot data> state = stateRemoved\n");
				while(sendqueue.size())
				{
					delete [] sendqueue.top().data;
					sendqueue.pop();
			  	}
			  	application_manager = 0;
			  	ca_manager = 0;
			  	mmi_session = 0;
			  	eDVBCIInterfaces::getInstance()->ciRemoved(this);
			  	//notifier->setRequested(eSocketNotifier::Read);
			  	eDVBCI_UI::getInstance()->setState(getSlotID(),0);
		  	}
		}
		else if (CardStatus == 1<<_TRID_CARD_IO_ERROR_)//case _TRID_CARD_IO_ERROR_: /* error occurs while accessing the card */
		{
			eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_IO_ERROR_<nothing>\n");
		}
		else if (CardStatus == 1<<_TRID_CARD_INVALID_CARD_)/*_TRID_CARD_INVALID_CARD_: card in the slot is not a valid CI card */
		{
			eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_INVALID_CARD_\n");
			if(state != stateInvalid) {
				eDebug("ci inserted in slot %d", getSlotID());
				state = stateInvalid;
				eDVBCI_UI::getInstance()->setState(getSlotID(),-1);
			}
		}
		else
		{
			if (CardStatus & (1<<_TRID_CARD_INSERT_))/*_TRID_CARD_INSERT_: card is inserted inot the slot, but maybe not a valid CI card */
			{
				/*if(state != stateRemoved) 
				{
					state = stateRemoved;
					eDebugCI("<dvbci.cpp eDVBCISlot data> state = stateRemoved\n");
					while(sendqueue.size())
					{
						delete [] sendqueue.top().data;
						sendqueue.pop();
					}
					application_manager = 0;
					ca_manager = 0;
					mmi_session = 0;
					eDVBCIInterfaces::getInstance()->ciRemoved(this);
					//notifier->setRequested(eSocketNotifier::Read);
					eDVBCI_UI::getInstance()->setState(getSlotID(),0);
				}*/

				eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_INSERT_  set ui state to 1.\n");			 
				if(state != stateInserted) 
				{
					eDebugCI("<dvbci.cpp eDVBCISlot data> state == stateInserted	set ui state 1\n");
					state = stateInserted;
					eDVBCI_UI::getInstance()->setState(getSlotID(),1);
				}
				CardStatus &= ~(1<<_TRID_CARD_INSERT_);
			}
			if (CardStatus & (1<<_TRID_CARD_RESET_START_))//case _TRID_CARD_RESET_START_:
			{
				eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_RESET_START_\n");		
				if(state != stateInserted) {
					state = stateInserted;
					eDVBCI_UI::getInstance()->setState(getSlotID(),1);
				}
				CardStatus &= ~(1<<_TRID_CARD_RESET_START_);
			}
			if (CardStatus & (1<<_TRID_CARD_MMI_READY_))/*case _TRID_CARD_MMI_READY_: card is ready for MMI menu access */
			{
				eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_MMI_READY_  set ui state to 2.\n");			 
				if(state != stateInserted) {
					eDebugCI("<dvbci.cpp eDVBCISlot data> state == stateInserted	set ui state 1\n");
					state = stateInserted;
					eDVBCI_UI::getInstance()->setState(getSlotID(),1);
				}
				{
					RETURN_TYPE ret;
					trid_uint8 menu_str_len;
					
					setAppManager(1);
					setMMIManager(1);
					
					ret = Trid_CI_AppInfo_GetMenuStr(app_info,&menu_str_len);
					eDebugCI("<dvbci.cpp eDVBCISlot data> get app info name: %s.\n", app_info);
					app_info[menu_str_len] = 0;
					
					eDVBCI_UI::getInstance()->setAppName(getSlotID(), app_info);
					
					eDVBCI_UI::getInstance()->setState(getSlotID(), 2);
				}
				CardStatus &= ~(1<<_TRID_CARD_MMI_READY_);
			}
			if (CardStatus & (1<<_TRID_CARD_CA_READY_))/*case _TRID_CARD_CA_READY_: card is ready for CA access,eg: ready to accept CA_PMT*/
			{
				eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_CA_READY_	eDVBCICAManagerSession ca info:\n");
				if(state != stateInserted) 
				{
					state = stateInserted;
					{
						RETURN_TYPE ret;
						trid_uint8 menu_str_len;
						
						setAppManager(1);
						
						ret = Trid_CI_AppInfo_GetMenuStr(app_info,&menu_str_len);
						app_info[menu_str_len] = 0;
						
						eDVBCI_UI::getInstance()->setAppName(getSlotID(), app_info);
						
						eDVBCI_UI::getInstance()->setState(getSlotID(), 2);
					}
				}
				DVBCI_Packcaids();
				eDVBCIInterfaces::getInstance()->recheckPMTHandlers();
				setCAManager(1);
				CardStatus &= ~(1<<_TRID_CARD_CA_READY_);
			}
			if (CardStatus & (1<<_TRID_CARD_MMI_CA_READY_))/*case _TRID_CARD_MMI_CA_READY_: card in ready for both CA and MMI access */
			{
				eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_MMI_CA_READY_ set mmi state to 1.\n"); 	
				if(state != stateInserted) 
				{
					state = stateInserted;
					{
						RETURN_TYPE ret;
						trid_uint8 menu_str_len;
						
						
						ret = Trid_CI_AppInfo_GetMenuStr(app_info,&menu_str_len);
						app_info[menu_str_len] = 0;
						
						eDVBCI_UI::getInstance()->setAppName(getSlotID(), app_info);
						
						eDVBCI_UI::getInstance()->setState(getSlotID(), 2);
					}
				}
				setAppManager(1);
				setMMIManager(1);
				DVBCI_Packcaids();
				setCAManager(1);
				eDVBCIInterfaces::getInstance()->recheckPMTHandlers();
				CardStatus &= ~(1<<_TRID_CARD_MMI_CA_READY_);
			}
			if (CardStatus & (1<<_TRID_CARD_UPGRADE_START_))//case _TRID_CARD_UPGRADE_START_:
			{
				eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_UPGRADE_START_<nothing>\n");			 
				CardStatus &= ~(1<<_TRID_CARD_UPGRADE_START_);
			}
			if (CardStatus & (1<<_TRID_CARD_UPGRADE_FINISH_))//case _TRID_CARD_UPGRADE_FINISH_:
			{
				eDebugCI("<dvbci.cpp eDVBCISlot data> CardStatusChange: _TRID_CARD_UPGRADE_FINISH_<nothing>\n");			 
				CardStatus &= ~(1<<_TRID_CARD_UPGRADE_FINISH_);
			}
		}
		Tridci_cb_status &= ~NOTIFY_CARD_STATUS;
	}
#endif
	if (status & NOTIFY_MENU_DATA)
	{
		MenuDataNotifyCallbackProcess(&menu_data);
		Tridci_cb_status &= ~NOTIFY_MENU_DATA;
	}
	if (status & NOTIFY_LIST_DATA)
	{
		ListDataNotifyCallbackProcess(&list_data);
		Tridci_cb_status &= ~NOTIFY_LIST_DATA;
	}
	if (status & NOTIFY_ENQ_DATA)
	{
		EnqDataNotifyCallbackProcess(&enq_data);
		Tridci_cb_status &= ~NOTIFY_ENQ_DATA;
	}
	if (status & NOTIFY_CLOSE_MMI)
	{
		CloseMMINotifyCallbackProcess();
		Tridci_cb_status &= ~NOTIFY_CLOSE_MMI;
	}
	//notifier->setRequested(eSocketNotifier::Read);
	notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);
	/*and something else? of course*/
	return 0;
}

#else
void eDVBCISlot::data(int what)
{
	if (what == eSocketNotifier::Priority)
	{
		eDebugCI("----->abing<----- CIslot what %d. Priority\n", what);/*abing:0518*/
	}
	if (what & eSocketNotifier::Read)
	{
		eDebugCI("----->abing<----- CIslot what %d. read\n", what);/*abing:0518*/
	}
	else if (what & eSocketNotifier::Write) 
	{
		eDebugCI("----->abing<----- CIslot what %d. Write\n", what);/*abing:0518*/
	}
    //eDebugCI("CISlot %d what %d\n", getSlotID(), what);
	if(what == eSocketNotifier::Priority) {
		if(state != stateRemoved) {
			state = stateRemoved;
			eDebugCI("!!!----->abing<----- eDVBCISlot data state = stateRemoved\n");
			while(sendqueue.size())
			{
				delete [] sendqueue.top().data;
				sendqueue.pop();
			}
			eDVBCISession::deleteSessions(this);
			eDVBCIInterfaces::getInstance()->ciRemoved(this);
			notifier->setRequested(eSocketNotifier::Read);
			eDVBCI_UI::getInstance()->setState(getSlotID(),0);
		}
		return;
	}

	if (state == stateInvalid)
      {
		eDebugCI("!!!----->abing<----- eDVBCISlot data state == stateInvalid  to reset\n");
		reset();
      }

	if(state != stateInserted) {
		eDebug("ci inserted in slot %d", getSlotID());
		state = stateInserted;
		//eDebugCI("<dvbci.cpp eDVBCISlot data> data state == stateInserted  set ui state 1\n");
		eDVBCI_UI::getInstance()->setState(getSlotID(),1);
		notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);
		/* enable PRI to detect removal or errors */
	}

	if (what & eSocketNotifier::Read) {
		__u8 data[4096];
		int r;
		r = ::read(fd, data, 4096);
		if(r > 0) {
//			int i;
//			eDebugNoNewLine("> ");
//			for(i=0;i<r;i++)
//				eDebugNoNewLine("%02x ",data[i]);
//			eDebug("");
			eDebugCI("----->abing<----- read %d data 0123456 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x.\n", r, data[0], data[1], data[2], data[3], data[4], data[5], data[6]);/*abing:0518*/
			eDVBCISession::receiveData(this, data, r);
			eDVBCISession::pollAll();
			return;
		}
	}
	else if (what & eSocketNotifier::Write) {
		if (!sendqueue.empty()) {
			const queueData &qe = sendqueue.top();
			eDebugCI("----->abing<----- send %d data in data, why no use send??? .\n", qe.len);/*abing:0518*/
			int res = ::write(fd, qe.data, qe.len);
			if (res >= 0 && (unsigned int)res == qe.len)
			{
				delete [] qe.data;
				sendqueue.pop();
			}
		}
		else
			notifier->setRequested(eSocketNotifier::Read|eSocketNotifier::Priority);
	}
}
#endif

DEFINE_REF(eDVBCISlot);

eDVBCISlot::eDVBCISlot(eMainloop *context, int nr)
{
	char filename[128];

	application_manager = 0;
	mmi_session = 0;
	ca_manager = 0;
	use_count = 0;
	linked_next = 0;
	user_mapped = false;
	plugged = true;
	
	slotid = nr;

	eDebugCI("----->abing<----- eDVBCISlot nr(%d).\n", nr);/*abing:0518*/
#if trid_ci
	state = stateInvalid;
#if data_callback

#else
	fd= 0x12345678;

	eDebugCI("CI Slot %d has fd %d", getSlotID(), fd);

	if (fd >= 0)
	{
		notifier = eSocketNotifier::create(context, fd, eSocketNotifier::Read | eSocketNotifier::Priority | eSocketNotifier::Write);
		CONNECT(notifier->activated, eDVBCISlot::data);
	}
#endif
#else
	sprintf(filename, "/dev/ci%d", nr);
//	possible_caids.insert(0x1702);
//	possible_providers.insert(providerPair("PREMIERE", 0xC00000));
//	possible_services.insert(eServiceReference("1:0:1:2A:4:85:C00000:0:0:0:"));

	fd = ::open(filename, O_RDWR | O_NONBLOCK);

	eDebugCI("CI Slot %d has fd %d", getSlotID(), fd);
	state = stateInvalid;

	if (fd >= 0)
	{
		notifier = eSocketNotifier::create(context, fd, eSocketNotifier::Read | eSocketNotifier::Priority | eSocketNotifier::Write);
		CONNECT(notifier->activated, eDVBCISlot::data);
	} else
	{
		perror(filename);
	}
#endif
}

eDVBCISlot::~eDVBCISlot()
{
	eDebugCI("----->abing<----- eDVBCISlot delete.\n");/*abing:0518*/
#if trid_ci
	mmi_session = 0;
	application_manager = 0;
	ca_manager = 0;
#else
	eDVBCISession::deleteSessions(this);
#endif
}
#if trid_ci
void eDVBCISlot::setAppManager( int session )
{
	eDebugCI("----->abing<----- eDVBCISlot setAppManager.\n");/*abing:0518*/
	application_manager=session;
}

void eDVBCISlot::setMMIManager( int session )
{
	eDebugCI("----->abing<----- eDVBCISlot setMMIManager. %d\n", session);/*abing:0518*/
	mmi_session = session;
}

void eDVBCISlot::setCAManager( int session )
{
	eDebugCI("----->abing<----- eDVBCISlot setCAManager.\n");/*abing:0518*/
	ca_manager = session;
}
#else
void eDVBCISlot::setAppManager( eDVBCIApplicationManagerSession *session )
{
	eDebugCI("----->abing<----- eDVBCISlot setAppManager.\n");/*abing:0518*/
	application_manager=session;
}

void eDVBCISlot::setMMIManager( eDVBCIMMISession *session )
{
	eDebugCI("----->abing<----- eDVBCISlot setMMIManager.\n");/*abing:0518*/
	mmi_session = session;
}

void eDVBCISlot::setCAManager( eDVBCICAManagerSession *session )
{
	eDebugCI("----->abing<----- eDVBCISlot setCAManager.\n");/*abing:0518*/
	ca_manager = session;
}
#endif

int eDVBCISlot::getSlotID()
{
	eDebugCI("----->abing<----- eDVBCISlot getSlotID %d.\n", slotid);/*abing:0518*/
	return slotid;
}

int eDVBCISlot::reset()
{
	//eDebug("CI Slot %d: reset requested", getSlotID());
#if trid_ci
/*abing:doing is trid ci has reset interface?*/
#else
	eDebugCI("----->abing<----- eDVBCISlot reset slot(%d).\n", getSlotID());/*abing:0518*/
	if (state == stateInvalid)
	{
		unsigned char buf[256];
		eDebug("ci flush");
		while(::read(fd, buf, 256)>0);
		state = stateResetted;
	}

	while(sendqueue.size())
	{
		delete [] sendqueue.top().data;
		sendqueue.pop();
	}

	ioctl(fd, 0);
#endif

	return 0;
}

int eDVBCISlot::startMMI()
{
	//eDebug("CI Slot %d: startMMI()", getSlotID());
	eDebugCI("----->abing<----- eDVBCISlot startMMI slot(%d).\n", getSlotID());/*abing:0518*/
#if trid_ci
	if(application_manager)
		DVBCI_StartMMI();
#else
	if(application_manager)
		application_manager->startMMI();
#endif
	
	return 0;
}

int eDVBCISlot::stopMMI()
{
	//eDebug("CI Slot %d: stopMMI()", getSlotID());
	eDebugCI("----->abing<----- eDVBCISlot stopMMI slot(%d).\n", getSlotID());/*abing:0518*/

#if trid_ci
	if(mmi_session)
		DVBCI_StopMMI();
#else
	if(mmi_session)
		mmi_session->stopMMI();
#endif
	
	return 0;
}

int eDVBCISlot::answerText(int answer)
{
	//eDebug("CI Slot %d: answerText(%d)", getSlotID(), answer);
	eDebugCI("----->abing<----- eDVBCISlot answerText slot(%d)-answerText(%d).\n", getSlotID(), answer);/*abing:0518*/

#if trid_ci
	if(mmi_session)
		DVBCI_AnswerText(answer);
#else
	if(mmi_session)
		mmi_session->answerText(answer);
#endif
	return 0;
}

int eDVBCISlot::getMMIState()
{
	if(mmi_session)
	{
		eDebugCI("----->abing<----- eDVBCISlot getMMIState: yes.\n");/*abing:0518*/
		return 1;
	}
	else
	{
		eDebugCI("----->abing<----- eDVBCISlot getMMIState: no.\n");/*abing:0518*/
		return 0;
    }
}

int eDVBCISlot::answerEnq(char *value)
{
	//eDebug("CI Slot %d: answerENQ(%s)", getSlotID(), value);
	eDebugCI("----->abing<----- eDVBCISlot answerEnq Slot(%d): answerENQ(%s)", getSlotID(), value);
#if trid_ci
	if(mmi_session)
		DVBCI_AnswerEnq(value);
#else
	if(mmi_session)
		mmi_session->answerEnq(value);
#endif

	return 0;
}

int eDVBCISlot::cancelEnq()
{
	//eDebug("CI Slot %d: cancelENQ", getSlotID());
	eDebugCI("----->abing<----- eDVBCISlot cancelEnq Slot(%d).\n", getSlotID());/*abing:0518*/
#if trid_ci
	if(mmi_session)
		DVBCI_CancelEnq();
#else
	if(mmi_session)
		mmi_session->cancelEnq();
#endif

	return 0;
}

void PackDesc(unsigned char *pack, unsigned char *Desc)
{
	int len, i, j, k;
	len = (Desc[0]<<8) | (Desc[1]);
	eDebugCI("Desc len is %d.\n", len);
	i=2;j=2;
	while (i<len)
	{
		memcpy(pack+j, Desc+i+1, Desc[i+2]+2);
		k=Desc[i+2]+2;
		i+=k+1;
		j+=k;
	}
	j-=2;
	pack[0]=(j>>8) & 0xff;
	pack[1]=j & 0xff;
}

int eDVBCISlot::sendCAPMT(eDVBServicePMTHandler *pmthandler, const std::vector<uint16_t> &ids)
{
	if (!ca_manager)
	{
		//eDebug("no ca_manager (no CI plugged?)");
		eDebugCI("----->abing<----- eDVBCISlot sendCAPMT no ca_manager (no CI plugged?).\n");/*abing:0518*/
		return -1;
	}
	eDebugCI("----->abing<----- eDVBCISlot sendCAPMT.\n");/*abing:0518*/
	
	const std::vector<uint16_t> &caids = ids.empty() ? DVBCI_GetCAIDs() : ids;
	ePtr<eTable<ProgramMapSection> > ptr;
	if (pmthandler->getPMT(ptr))
	{
	    eDebugCI("get pmt from pmthandler fail.\n");
		return -1;
	}
	else
	{
		eDVBTableSpec table_spec;
		ptr->getSpec(table_spec);
		int pmt_version = table_spec.version & 0x1F; // just 5 bits

		eServiceReferenceDVB ref;
		pmthandler->getServiceReference(ref);
		uint16_t program_number = ref.getServiceID().get();
		std::map<uint16_t, uint8_t>::iterator it =
			running_services.find(program_number);
		bool sendEmpty = caids.size() == 1 && caids[0] == 0xFFFF;

		if ( it != running_services.end() &&
			(pmt_version == it->second) &&
			!sendEmpty )
		{
			eDebug("[eDVBCISlot] don't send self capmt version twice");
			return -1;
		}

		std::vector<ProgramMapSection*>::const_iterator i=ptr->getSections().begin();
		if ( i == ptr->getSections().end() )
		{
			eDebugCI("ProgramMapSection maybe none.\n");
			return -1;
		}
		else
		{
			unsigned char raw_data[1024];
			unsigned char pack_data[1024];

//			eDebug("send %s capmt for service %04x to slot %d",
//				it != running_services.end() ? "UPDATE" : running_services.empty() ? "ONLY" : "ADD",
//				program_number, slotid);

			CaProgramMapSection capmt(*i++,
				it != running_services.end() ? 0x05 /*update*/ : running_services.empty() ? 0x03 /*only*/ : 0x04 /*add*/, 0x01, caids);
			while( i != ptr->getSections().end() )
			{
		//			eDebug("append");
				capmt.append(*i++);
			}
			capmt.writeToBuffer(raw_data);

// begin calc capmt length
			int wp=0;
			int hlen;
			if ( raw_data[3] & 0x80 )
			{
				int i=0;
				int lenbytes = raw_data[3] & ~0x80;
				while(i < lenbytes)
					wp = (wp << 8) | raw_data[4 + i++];
				wp+=4;
				wp+=lenbytes;
				hlen = 4 + lenbytes;
			}
			else
			{
				wp = raw_data[3];
				wp+=4;
				hlen = 4;
			}
// end calc capmt length

			if (sendEmpty)
			{
				eDebugNoNewLine("SEND EMPTY CAPMT.. old version is %02x", raw_data[hlen+3]);
				if (sendEmpty && running_services.size() == 1)  // check if this is the capmt for the last running service
					raw_data[hlen] = 0x03; // send only instead of update... because of strange effects with alphacrypt
				raw_data[hlen+3] &= ~0x3E;
				raw_data[hlen+3] |= ((pmt_version+1) & 0x1F) << 1;
				eDebug(" new version is %02x", raw_data[hlen+3]);
			}

			eDebug("ca_manager %p dump capmt:%d.", ca_manager, wp);
			for(int i=0;i<wp;i++)
				eDebugNoNewLine("%02x, ", raw_data[i]);
			eDebug("");

			//dont need tag and lenfield

/*
47 41 93 16 00 02 b0 48 01 93 f7 00 00 e1 f7 f0 0c 09 04 05 
00 f9 c9 09 04 0b 00 f7 71 02 e1 f7 f0 00 03 e2 5b f0 0a 0a
08 65 6e 67 00 64 61 6e 00 03 e2 65 f0 0a 0a 08 73 77 65 00
6e 6f 72 00 06 e2 bf f0 07 56 05 64 61 6e 09 00 e5 36 f5 bf
ff ff ff ff ff ff ff 
*/

#if trid_ci
            int ipack, iraw;
            unsigned char *pPack, *pRaw;

			ePtr<eDVBService> service;

			pmthandler->getService(service);
			videoPid = service->getCacheEntry(eDVBService::cVPID);
			audioPid = service->getCacheEntry(eDVBService::cAPID);
			eDebugCI("video pid: 0x%x.\n", videoPid);
			eDebugCI("audio pid: 0x%x.\n", audioPid);

            pack_data[0] = 0x02;

			pack_data[3] = raw_data[5];
			pack_data[4] = raw_data[6];
			
			pack_data[5] = (pmt_version<<1) | 0xC1;
			
			pack_data[6] = 0x00;//section num
			pack_data[7] = 0x00;
			
			pack_data[8] = 0x00;//PCR
			pack_data[9] = 0x00;

			pPack = pack_data+10;
			pRaw = raw_data+8;
			wp -= 8;

			PackDesc(pPack, pRaw);
			wp -= (((pRaw[0]&0xF)<<8) | pRaw[1])+2;
			pRaw += (((pRaw[0]&0xF)<<8) | pRaw[1])+2;
			pPack += (((pPack[0]&0xF)<<8) | pPack[1])+2;

			//videoPid = 0;
			//audioPid = 0;

            while (wp > 0)
            {
            	pPack[0] = pRaw[0];
            	pPack[1] = pRaw[1];
            	pPack[2] = pRaw[2];
				/*if ((pPack[0] == 0x02)&&(videoPid==0))
				{
					videoPid = (pPack[1]<<8) | pPack[2];
					printf("video pid: 0x%x, 0x%x.\n", pPack[1], pPack[2]);
				}
				if ((pPack[0] == 0x03)&&(audioPid==0))
				{
					audioPid = (pPack[1]<<8) | pPack[2];
					eDebugCI("audio pid: 0x%x, 0x%x.\n", pPack[1], pPack[2]);
				}*/
				wp-=3;
				pRaw+=3;
				pPack+=3;
				PackDesc(pPack, pRaw);
				wp -= (((pRaw[0]&0xF)<<8) | pRaw[1])+2;
				pRaw += (((pRaw[0]&0xF)<<8) | pRaw[1])+2;
				pPack += (((pPack[0]&0xF)<<8) | pPack[1])+2;
            }

			wp = pPack-pack_data+4;
			
			pack_data[1] = 0xB0 | (((wp-3)>>8)&0x0F);
			pack_data[2] = (wp-3)&0xFF;
/**/			
			DVBCI_SendCAPMT(pack_data, wp);//(raw_data + hlen, wp - hlen);//
#else
			ca_manager->sendCAPMT(raw_data + hlen, wp - hlen);
#endif
			running_services[program_number] = pmt_version;
		}
	}
	eDebugCI("----->abing<----- eDVBCISlot sendCAPMT. end.\n");
	return 0;
}

void eDVBCISlot::removeService(uint16_t program_number)
{
	eDebugCI("----->abing<----- eDVBCISlot removeService serviceno(%d).\n", program_number);/*abing:0518*/
	if (program_number == 0xFFFF)
		running_services.clear();  // remove all
	else
		running_services.erase(program_number);  // remove single service
}

int eDVBCISlot::setSource(data_source source)
{
	eDebugCI("eDVBCISlot::setSource source=%d\n", source);
	
	current_source = source;
	if (eDVBCIInterfaces::getInstance()->getNumOfSlots() > 1) // FIXME .. we force DM8000 when more than one CI Slot is avail
	{
		char buf[64];
		snprintf(buf, 64, "/proc/stb/tsmux/ci%d_input", slotid);
		FILE *ci = fopen(buf, "wb");
		switch(source)
		{
			case CI_A:
				fprintf(ci, "CI0");
				break;
			case CI_B:
				fprintf(ci, "CI1");
				break;
			case CI_C:
				fprintf(ci, "CI2");
				break;
			case CI_D:
				fprintf(ci, "CI3");
				break;
			case TUNER_A:
				fprintf(ci, "A");
				break;
			case TUNER_B:
				fprintf(ci, "B");
				break;
			case TUNER_C:
				fprintf(ci, "C");
				break;
				case TUNER_D:
				fprintf(ci, "D");
				break;
			default:
				eDebug("CI Slot %d: setSource %d failed!!!\n", getSlotID(), (int)source);
				break;
		}
		fclose(ci);
	}
	else // DM7025
	{
//		eDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//		eDebug("eDVBCISlot::enableTS(%d %d)", enable, (int)source);
//		FILE *ci = fopen("stb/tsmux/input", "wb");
		FILE *ci = fopen("/proc/stb/tsmux/ci0_input", "wb");
	
		if(ci == NULL) {
			//eDebug("cannot open /proc/stb/tsmux/input2");
			eDebug("cannot open /proc/stb/tsmux/ci0_input");
			return 0;
		}

		switch(source)
		{
			case TUNER_A:
				fprintf(ci, "A");
				break;
			case TUNER_B:
				fprintf(ci, "B");
				break;
			case TUNER_C:
				fprintf(ci, "C");
				break;
			default:
				eDebug("CI Slot %d: setSource %d failed!!!\n", getSlotID(), (int)source);
				break;
		}

		fclose(ci);
	}
	eDebug("CI Slot %d setSource(%d)", getSlotID(), (int)source);
	return 0;
}

int eDVBCISlot::setClockRate(int rate)
{
	char buf[64];
    eDebugCI("eDVBCISlot setClockRate slot(%d).\n", slotid);/*abing:0518*/
	snprintf(buf, 64, "/proc/stb/tsmux/ci%d_tsclk", slotid);
	FILE *ci = fopen(buf, "wb");
	if (ci)
	{
		if (rate)
			fprintf(ci, "high");
		else
			fprintf(ci, "normal");
		fclose(ci);
		return 0;
	}
	return -1;
}

/*收到菜单数据，要求显示*/
//Trid_CI_MMI_RegisterMenuDataNotify  
trid_sint32 eDVBCISlot::MenuDataNotifyCallbackProcess(Trid_T_Menu* menu)
{
	unsigned char tag[3] = {0x9f, 0x88, 0x09};
	eDVBCISlot *slot;
	if (getMMIManager())
	{
		setMMIManager(1);
	}

	eDVBCI_UI::getInstance()->processMMIData(0, tag, (const void *)menu, 0);

	return 0;
}


/*收到list数据*/
//Trid_CI_MMI_RegisterListDataNotify  
trid_sint32 eDVBCISlot::ListDataNotifyCallbackProcess(Trid_T_List* list)
{
	unsigned char tag[3] = {0x9f, 0x88, 0x0c};
	if (!getMMIManager())
	{
		setMMIManager(1);
	}

	eDVBCI_UI::getInstance()->processMMIData(/*slot->getSlotID()*/0, tag, (const void *)list, 0);
	return 0;
}


/*收到enquiry数据*/
//Trid_CI_MMI_RegisterEnqDataNotify   
trid_sint32 eDVBCISlot::EnqDataNotifyCallbackProcess(Trid_T_Enq* enq)
{
	unsigned char tag[3] = {0x9f, 0x88, 0x07};

	if (!getMMIManager())
	{
		setMMIManager(1);
	}

	eDVBCI_UI::getInstance()->processMMIData(0, tag, (const void *)enq, 0);
	return 0;
}


/*这个是否因为意外，比如模块被拔出，造成的由主机底层发出的停止动作*/
//Trid_CI_MMI_RegisterCloseMMINotify  
trid_sint32 eDVBCISlot::CloseMMINotifyCallbackProcess()
{
	unsigned char tag[3] = {0x9f, 0x88, 0x00};
	unsigned char data[1] = {1};

	eDVBCI_UI::getInstance()->processMMIData(/*slot->getSlotID()*/0, tag, data, 1);

	return 0;
}

eAutoInitP0<eDVBCIInterfaces> init_eDVBCIInterfaces(eAutoInitNumbers::/*dvb*/main, "CI Slots");






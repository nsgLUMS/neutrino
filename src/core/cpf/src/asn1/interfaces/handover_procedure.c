#include "../compiled/S1AP-PDU.h"
#include "../compiled/E-RABToBeSetupItemHOReq.h"
#include "../compiled/E-RABToBeSetupItemHOReqIEs.h"
#include "../compiled/E-RABToBeSetupListHOReq.h"
#include "../compiled/E-RABToBeSetupListHOReq-Item.h"
#include "../compiled/SecurityContext.h"
#include "../compiled/SecurityContext-Item.h"
#include "../compiled/HandoverRequest-IEs.h"
#include "../compiled/HandoverRequest.h"
#include "../compiled/E-RABAdmittedItem.h"
#include "../compiled/E-RABAdmittedItemIEs.h"
#include "../compiled/E-RABAdmittedList.h"
#include "../compiled/E-RABAdmittedList-Item.h"
#include "../compiled/Target-To-Source-TContainer-Item.h"
#include "../compiled/HandoverRequestAcknowledge-IEs.h"
#include "../compiled/HandoverRequestAcknowledge.h"
#include "../compiled/COUNTvalue.h"
#include "../compiled/Bearers-SubjectToStatusTransfer-Item.h"
#include "../compiled/Bearers-SubjectToStatusTransfer-ItemIEs.h"
#include "../compiled/Bearers-SubjectToStatusTransferList.h"
#include "../compiled/ENB-StatusTransfer-TransparentContainer-Item.h"
#include "../compiled/ENB-StatusTransfer-TransparentContainer.h"
#include "../compiled/ENBStatusTransfer-IEs.h"
#include "../compiled/ENBStatusTransfer.h"
#include "../compiled/HandoverNotify.h"
#include "../compiled/HandoverNotify-IEs.h"
#include "../compiled/UE-S1AP-ID-pair.h"
#include "../compiled/UE-S1AP-IDs-Item.h"
#include "../compiled/UE-S1AP-IDs.h"
#include "../compiled/UeContextReleaseCommand-IEs.h"
#include "../compiled/UeContextReleaseCommand.h"
#include "../compiled/UeContextReleaseComplete.h"
#include "../compiled/UeContextReleaseComplete-IEs.h"
#include "../compiled/UeContextReleaseRequest.h"
#include "../compiled/UeContextReleaseRequest-IEs.h"

#include "structs.h"

struct Data CreateHandoverRequired(struct HandRequired);
struct Data CreateHandoverRequest(struct HandRequest);
struct Data CreateHandoverRequestAcknowledge(struct HandRequestAck);
struct Data CreateHandoverCommand(struct HandCommand);
struct Data CreateENBStatusTransfer(struct ENBStatus);
struct Data CreateMMEStatusTransfer(struct ENBStatus);
struct Data CreateHandoverNotify(struct HandNotify);
struct Data CreateUEContextReleaseCommand(struct ReleaseCommand);
struct Data CreateUEContextReleaseComplete(struct ReleaseComplete);
struct Data CreateUEContextReleaseRequest(struct ReleaseRequest);

struct Data Encode(S1AP_PDU_t *pdu, uint8_t *buffer)
{

  asn_per_constraints_t *constraints = NULL;

  const ssize_t ec = uper_encode_to_new_buffer(&asn_DEF_S1AP_PDU,
                                               constraints,
                                               pdu,
                                               (void **)&buffer);

  struct Data data = {buffer, ec};
  free(constraints);
  return data;
}

///////////////////////////Encoding of Handover messages.////////////////////////////////
struct Data HandoverRequired()
{
  struct HandRequired req;

  req.mme.id = 0;
  req.mme.criticality = 0;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 0;
  req.enub.value = 1;

  req.type.id = 1;
  req.type.criticality = 0;
  req.type.type = 0;

  req.cause.id = 2;
  req.cause.criticality = 1;
  req.cause.radioNetwork = 16;

  req.target.id = 4;
  req.target.criticality = 0;
  char *plmn = "55f531";
  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_Global_ENB_ID, plmn, strlen(plmn));
  req.target.plmnidentity = *s1;
  req.target.macroenb = "000020";
  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, plmn, strlen(plmn));
  req.target.plmnidentity_tai = *s2;
  char *tac = "45678";
  OCTET_STRING_t *s3 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, tac, strlen(tac));
  req.target.tac = *s3;

  req.stt.id = 104;
  req.stt.criticality = 0;
  char *tac_stt = "40809b0a101681380004001060e4c204f84f84f84f84faf06ec4000015e800000591382002900838f0841e1f041c78808081cf80400002008c0240b56ddd5ad813fd9405c13fd94041f51b582f1f28a2e08c5d04348565caa8430fb380004850d4340007c229bd96000008155509ec9b8000040605fc60901081816f20f20327630dda980606c091d018002080240a7da2a827623517260c8591404a845000004e400245000055f531000020100055f53100001011800000";
  OCTET_STRING_t *s4 = OCTET_STRING_new_fromBuf(&asn_DEF_Source_To_Target_TContainer_Item, tac_stt, strlen(tac_stt));
  req.stt.value = *s4;

  req.s1ap.procedureCode = 0;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 1;

  struct Data data = CreateHandoverRequired(req);

  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);
  FREEMEM(s3->buf);
  FREEMEM(s3);
  FREEMEM(s4->buf);
  FREEMEM(s4);
  return data;
}

struct Data HandoverRequest()
{
  struct HandRequest req;

  req.mme.id = 0;
  req.mme.criticality = 0;
  req.mme.value = 0;

  req.cause.id = 2;
  req.cause.criticality = 1;
  req.cause.radioNetwork = 33;

  req.type.id = 1;
  req.type.criticality = 0;
  req.type.type = 0;

  req.bitrate.id = 66;
  req.bitrate.criticality = 0;
  req.bitrate.bitratedl = 97000000;
  req.bitrate.bitrateul = 47000000;

  req.horeq.id = 53;
  req.horeq.criticality = 0;
  req.horeq.horeq_id = 27;
  req.horeq.horeq_criticality = 0;
  req.horeq.erab_id = 5;
  req.horeq.transport_layer = "10.200.10.253";
  char *gtpd = "02000000";
  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_E_RABToBeSetupItemHOReq, gtpd, strlen(gtpd));
  req.horeq.gtpe_id = *s1;
  req.horeq.qci = 5;
  req.horeq.priority_level = 1;
  req.horeq.preemption_cap = 1;
  req.horeq.preemption_vul = 1;

  req.stt.id = 104;
  req.stt.criticality = 0;
  char *tac_stt = "40809b0a101681380004001060e4c204f84f84f84f84faf06ec4000015e800000591382002900838f0841e1f041c78808081cf80400002008c0240b56ddd5ad813fd9405c13fd94041f51b582f1f28a2e08c5d04348565caa8430fb380004850d4340007c229bd96000008155509ec9b8000040605fc60901081816f20f20327630dda980606c091d018002080240a7da2a827623517260c8591404a845000004e400245000055f531000020100055f53100001011800000";
  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_Source_To_Target_TContainer_Item, tac_stt, strlen(tac_stt));
  req.stt.value = *s2;

  req.seccap.id = 107;
  req.seccap.criticality = 0;
  req.seccap.encryption = "c000";
  req.seccap.integrity = "c000";

  req.sec_context.id = 40;
  req.sec_context.criticality = 0;
  req.sec_context.next_hop = 2;
  req.sec_context.next_hop_param = "92160cdabe2b8ae4f42250dcafea2494a42d0953df715903786734ce0cb9507c";

  req.s1ap.procedureCode = 1;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 2;

  struct Data data = CreateHandoverRequest(req);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);
  return data;
}

struct Data HandoverRequestAcknowledge()
{
  struct HandRequestAck req;

  req.mme.id = 0;
  req.mme.criticality = 1;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 1;
  req.enub.value = 1;

  req.admitted.id = 18;
  req.admitted.criticality = 1;
  req.admitted.id_erab = 20;
  req.admitted.criticality_erab = 1;
  req.admitted.erab_id = 5;
  req.admitted.transport_layer = "10.200.10.38";
  char *gtpe = "00000001";
  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_E_RABAdmittedItem, gtpe, strlen(gtpe));
  req.admitted.gtpe_id = *s1;

  req.tts.id = 123;
  req.tts.criticality = 0;
  char *tac_stt = "002301091058215b37b281327630dda8181b0200c003f50004869b380004850d0001f09110";
  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_Source_To_Target_TContainer_Item, tac_stt, strlen(tac_stt));
  req.tts.value = *s2;

  req.s1ap.procedureCode = 1;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 3;

  struct Data data = CreateHandoverRequestAcknowledge(req);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);
  return data;
}

struct Data HandoverCommand()
{
  struct HandCommand req;

  req.mme.id = 0;
  req.mme.criticality = 0;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 0;
  req.enub.value = 1;

  req.type.id = 1;
  req.type.criticality = 0;
  req.type.type = 0;

  req.forwarding.id = 12;
  req.forwarding.criticality = 1;
  req.forwarding.id_for = 14;
  req.forwarding.criticality_for = 1;
  req.forwarding.erab_id = 5;

  req.tts.id = 123;
  req.tts.criticality = 0;
  char *tac_stt = "002301091058215b37b281327630dda8181b0200c003f50004869b380004850d0001f09110";
  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_Target_To_Source_TContainer_Item, tac_stt, strlen(tac_stt));
  req.tts.value = *s1;

  req.s1ap.procedureCode = 0;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 4;

  struct Data data = CreateHandoverCommand(req);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  return data;
}

struct Data ENBStatusTransfer()
{
  struct ENBStatus req;

  req.mme.id = 0;
  req.mme.criticality = 0;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 0;
  req.enub.value = 1;

  req.status.id = 90;
  req.status.criticality = 0;
  req.status.bearer_id = 89;
  req.status.bearer_criticality = 1;
  req.status.erab_id = 5;
  req.status.ul_pdcp = 231;
  req.status.ul_hfn = 0;
  req.status.dl_pdcp = 51;
  req.status.dl_hfn = 0;

  req.s1ap.procedureCode = 24;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 5;

  return CreateENBStatusTransfer(req);
}

struct Data MMEStatusTransfer()
{
  struct ENBStatus req;

  req.mme.id = 0;
  req.mme.criticality = 0;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 0;
  req.enub.value = 1;

  req.status.id = 90;
  req.status.criticality = 0;
  req.status.bearer_id = 89;
  req.status.bearer_criticality = 1;
  req.status.erab_id = 5;
  req.status.ul_pdcp = 231;
  req.status.ul_hfn = 0;
  req.status.dl_pdcp = 51;
  req.status.dl_hfn = 0;

  req.s1ap.procedureCode = 25;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 6;

  return CreateENBStatusTransfer(req);
}

struct Data HandoverNotify()
{
  struct HandNotify req;

  req.mme.id = 0;
  req.mme.criticality = 0;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 0;
  req.enub.value = 1;

  char *plmn_identity = "55f531";
  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_EUTRAN_CGI, plmn_identity, strlen(plmn_identity));
  req.eutran.plmnidentity = *s1;
  req.eutran.cell_id = "0x00000201";
  req.eutran.id = 100;
  req.eutran.criticality = 1;

  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, plmn_identity, strlen(plmn_identity));
  req.tai.plmnidentity = *s2;
  char *tac = "45678";
  OCTET_STRING_t *s3 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, tac, strlen(tac));
  req.tai.tac = *s3;
  req.tai.id = 67;
  req.tai.criticality = 1;

  req.s1ap.procedureCode = 2;
  req.s1ap.criticality = 1;
  req.s1ap.msgType = 7;

  struct Data data = CreateHandoverNotify(req);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);
  FREEMEM(s3->buf);
  FREEMEM(s3);
  return data;
}

struct Data UEContextReleaseCommand()
{
  struct ReleaseCommand req;

  req.ids.mme_pair = 0;
  req.ids.enub_pair = 1;
  req.ids.id = 99;
  req.ids.criticality = 0;

  req.cause.id = 2;
  req.cause.criticality = 1;
  req.cause.radioNetwork = 2;

  req.s1ap.procedureCode = 23;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 8;
  req.choice = 0;

  return CreateUEContextReleaseCommand(req);
}

struct Data UEContextReleaseComplete()
{
  struct ReleaseComplete req;

  req.mme.id = 0;
  req.mme.criticality = 1;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 1;
  req.enub.value = 1;

  req.s1ap.procedureCode = 23;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 9;

  return CreateUEContextReleaseComplete(req);
}

struct Data UEContextReleaseRequest()
{
  struct ReleaseRequest req;

  req.mme.id = 0;
  req.mme.criticality = 0;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 0;
  req.enub.value = 1;

  req.cause.id = 2;
  req.cause.criticality = 1;
  req.cause.transport = 0;

  req.s1ap.procedureCode = 18;
  req.s1ap.criticality = 1;
  req.s1ap.msgType = 10;

  return CreateUEContextReleaseRequest(req);
}

struct Data UEContextReleaseCommandNas()
{
  struct ReleaseCommand req;

  req.ids.mme_pair = 0;
  req.ids.enub_pair = 1;
  req.ids.id = 99;
  req.ids.criticality = 0;

  req.cause.id = 2;
  req.cause.criticality = 1;
  req.cause.nas = 0;

  req.s1ap.procedureCode = 23;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 8;
  req.choice = 2;

  return CreateUEContextReleaseCommand(req);
}

struct Data UEContextReleaseCompleteFin()
{
  struct ReleaseComplete req;

  req.mme.id = 0;
  req.mme.criticality = 1;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 1;
  req.enub.value = 1;

  req.s1ap.procedureCode = 23;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 9;

  return CreateUEContextReleaseComplete(req);
}

///////////////////Messages////////////////////////////

struct Data CreateHandoverRequired(struct HandRequired req)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = req.mme.id;
  mme_item->criticality = req.mme.criticality;
  mme_item->value = req.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = req.enub.id;
  enub_item->criticality = req.enub.criticality;
  enub_item->value = req.enub.value;

  HandoverType_Item_t *hand_item = (HandoverType_Item_t *)calloc(1, sizeof *hand_item);
  hand_item->id = req.type.id;
  hand_item->criticality = req.type.criticality;
  hand_item->value = req.type.type;

  Cause_t *cause = (Cause_t *)calloc(1, sizeof *cause);
  cause->present = Cause_PR_radioNetwork;
  cause->choice.radioNetwork = req.cause.radioNetwork;
  Cause_Item_t *cause_item = (Cause_Item_t *)calloc(1, sizeof *cause_item);
  cause_item->id = req.cause.id;
  cause_item->criticality = req.cause.criticality;
  cause_item->value = *cause;

  ENB_ID_t *enb = (ENB_ID_t *)calloc(1, sizeof *enb);
  enb->present = ENB_ID_PR_macroENB_ID;
  enb->choice.macroENB_ID.buf = req.target.macroenb;
  enb->choice.macroENB_ID.size = strlen(req.target.macroenb);
  Global_ENB_ID_t *g_enb = (Global_ENB_ID_t *)calloc(1, sizeof *g_enb);
  g_enb->pLMNidentity = req.target.plmnidentity;
  g_enb->eNB_ID = *enb;
  TAI_t *TAI = (TAI_t *)calloc(1, sizeof *TAI);
  TAI->pLMNidentity = req.target.plmnidentity_tai;
  TAI->tAC = req.target.tac;
  TargeteNB_ID_t *t_enb = (TargeteNB_ID_t *)calloc(1, sizeof *t_enb);
  t_enb->global_ENB_ID = *g_enb;
  t_enb->selected_TAI = *TAI;
  TargetID_t *target = (TargetID_t *)calloc(1, sizeof *target);
  target->present = TargetID_PR_targeteNB_ID;
  target->choice.targeteNB_ID = *t_enb;
  TargetID_Item_t *target_item = (TargetID_Item_t *)calloc(1, sizeof *target_item);
  target_item->id = req.target.id;
  target_item->criticality = req.target.criticality;
  target_item->value = *target;

  Source_To_Target_TContainer_Item_t *con_item = (Source_To_Target_TContainer_Item_t *)calloc(1, sizeof *con_item);
  con_item->id = req.stt.id;
  con_item->criticality = req.stt.criticality;
  con_item->value = req.stt.value;

  HandoverRequired_IEs_t *ies = (HandoverRequired_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->type = *hand_item;
  ies->cause = *cause_item;
  ies->target = *target_item;
  ies->container = *con_item;

  HandoverRequired_t *msg = (HandoverRequired_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_handoverRequired;
  pros->choice.handoverRequired = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);
  free(mme_item);
  free(enub_item);
  free(hand_item);
  free(cause);
  free(cause_item);
  free(enb);
  free(g_enb);
  free(TAI);
  free(t_enb);
  free(target);
  free(target_item);
  free(con_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);
  return data;
}

struct Data CreateHandoverRequest(struct HandRequest req)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = req.mme.id;
  mme_item->criticality = req.mme.criticality;
  mme_item->value = req.mme.value;

  HandoverType_Item_t *hand_item = (HandoverType_Item_t *)calloc(1, sizeof *hand_item);
  hand_item->id = req.type.id;
  hand_item->criticality = req.type.criticality;
  hand_item->value = req.type.type;

  Cause_t *cause = (Cause_t *)calloc(1, sizeof *cause);
  cause->present = Cause_PR_radioNetwork;
  cause->choice.radioNetwork = req.cause.radioNetwork;
  Cause_Item_t *cause_item = (Cause_Item_t *)calloc(1, sizeof *cause_item);
  cause_item->id = req.cause.id;
  cause_item->criticality = req.cause.criticality;
  cause_item->value = *cause;

  UE_Aggregate_Maxbitrate_t *rate = (UE_Aggregate_Maxbitrate_t *)calloc(1, sizeof *rate);
  rate->ue_Aggregate_MaximumBitRateDL = req.bitrate.bitratedl;
  rate->ue_Aggregate_MaximumBitRateUL = req.bitrate.bitrateul;
  UE_Aggregate_Maxbitrate_Item_t *bitrate_item = (UE_Aggregate_Maxbitrate_Item_t *)calloc(1, sizeof *bitrate_item);
  bitrate_item->id = req.bitrate.id;
  bitrate_item->criticality = req.bitrate.criticality;
  bitrate_item->value = *rate;

  AllocationAndRetentionPriority_t *priority = (AllocationAndRetentionPriority_t *)calloc(1, sizeof *priority);
  priority->priorityLevel = req.horeq.priority_level;
  priority->pre_emptionCapability = req.horeq.preemption_cap;
  priority->pre_emptionVulnerability = req.horeq.preemption_vul;
  E_RABLevelQoSParameters_t *qos = (E_RABLevelQoSParameters_t *)calloc(1, sizeof *qos);
  qos->qCI = req.horeq.qci;
  qos->allocationRetentionPriority = *priority;
  E_RABToBeSetupItemHOReq_t *horeq = (E_RABToBeSetupItemHOReq_t *)calloc(1, sizeof *horeq);
  horeq->e_RAB_ID = req.horeq.erab_id;
  horeq->transportLayerAddress.buf = req.horeq.transport_layer;
  horeq->transportLayerAddress.size = strlen(req.horeq.transport_layer);
  horeq->gTP_TEID = req.horeq.gtpe_id;
  horeq->e_RABlevelQosParameters = *qos;
  E_RABToBeSetupItemHOReqIEs_t *horeq_ie = (E_RABToBeSetupItemHOReqIEs_t *)calloc(1, sizeof *horeq_ie);
  horeq_ie->id = req.horeq.horeq_id;
  horeq_ie->criticality = req.horeq.horeq_criticality;
  horeq_ie->type = *horeq;
  E_RABToBeSetupListHOReq_t *horeq_list = (E_RABToBeSetupListHOReq_t *)calloc(1, sizeof *horeq_list);
  A_SEQUENCE_OF(struct E_RABToBeSetupItemHOReqIEs) *arr = calloc(1, sizeof *arr);
  asn_sequence_add(arr, horeq_ie);
  horeq_list->list.array = arr->array;
  horeq_list->list.count = arr->count;
  horeq_list->list.size = arr->size;
  horeq_list->list.free = arr->free;
  E_RABToBeSetupListHOReq_Item_t *horeq_item = (E_RABToBeSetupListHOReq_Item_t *)calloc(1, sizeof *horeq_item);
  horeq_item->id = req.horeq.id;
  horeq_item->criticality = req.horeq.criticality;
  horeq_item->value = *horeq_list;

  Source_To_Target_TContainer_Item_t *con_item = (Source_To_Target_TContainer_Item_t *)calloc(1, sizeof *con_item);
  con_item->id = req.stt.id;
  con_item->criticality = req.stt.criticality;
  con_item->value = req.stt.value;

  UESecurityCapabilities_t *seccaps = (UESecurityCapabilities_t *)calloc(1, sizeof *seccaps);
  seccaps->encryptionAlgorithms.buf = req.seccap.encryption;
  seccaps->encryptionAlgorithms.size = strlen(req.seccap.encryption);
  seccaps->integrityProtectionAlgorithms.buf = req.seccap.integrity;
  seccaps->integrityProtectionAlgorithms.size = strlen(req.seccap.integrity);
  UESecurityCapabilities_Item_t *seccaps_item = (UESecurityCapabilities_Item_t *)calloc(1, sizeof *seccaps_item);
  seccaps_item->id = req.seccap.id;
  seccaps_item->criticality = req.seccap.criticality;
  seccaps_item->value = *seccaps;

  SecurityContext_t *sec_context = (SecurityContext_t *)calloc(1, sizeof *sec_context);
  sec_context->nextHopChainingCount = req.sec_context.next_hop;
  sec_context->nextHopParameter.buf = req.sec_context.next_hop_param;
  sec_context->nextHopParameter.size = strlen(req.sec_context.next_hop_param);
  SecurityContext_Item_t *sec_item = (SecurityContext_Item_t *)calloc(1, sizeof *sec_item);
  sec_item->id = req.sec_context.id;
  sec_item->criticality = req.sec_context.criticality;
  sec_item->value = *sec_context;

  HandoverRequest_IEs_t *ies = (HandoverRequest_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->type = *hand_item;
  ies->cause = *cause_item;
  ies->bitrate = *bitrate_item;
  ies->horeq = *horeq_item;
  ies->container = *con_item;
  ies->secCap = *seccaps_item;
  ies->secContext = *sec_item;

  HandoverRequest_t *msg = (HandoverRequest_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_handoverRequest;
  pros->choice.handoverRequest = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);

  free(mme_item);
  free(hand_item);
  free(cause);
  free(cause_item);
  free(rate);
  free(bitrate_item);
  free(priority);
  free(qos);
  free(horeq);
  free(horeq_ie);
  free(horeq_list);
  free(arr->array);
  free(arr);
  free(horeq_item);
  free(con_item);
  free(seccaps);
  free(seccaps_item);
  free(sec_context);
  free(sec_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);

  return data;
}

struct Data CreateHandoverRequestAcknowledge(struct HandRequestAck req)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = req.mme.id;
  mme_item->criticality = req.mme.criticality;
  mme_item->value = req.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = req.enub.id;
  enub_item->criticality = req.enub.criticality;
  enub_item->value = req.enub.value;

  E_RABAdmittedItem_t *admitted = (E_RABAdmittedItem_t *)calloc(1, sizeof *admitted);
  admitted->e_RAB_ID = req.admitted.erab_id;
  admitted->transportLayerAddress.buf = req.admitted.transport_layer;
  admitted->transportLayerAddress.size = strlen(req.admitted.transport_layer);
  admitted->gTP_TEID = req.admitted.gtpe_id;
  E_RABAdmittedItemIEs_t *admitted_ies = (E_RABAdmittedItemIEs_t *)calloc(1, sizeof *admitted_ies);
  admitted_ies->id = req.admitted.id_erab;
  admitted_ies->criticality = req.admitted.criticality_erab;
  admitted_ies->type = *admitted;
  E_RABAdmittedList_t *admitted_item = (E_RABAdmittedList_t *)calloc(1, sizeof *admitted_item);
  A_SEQUENCE_OF(struct E_RABAdmittedItemIEs) *arr = calloc(1, sizeof *arr);
  asn_sequence_add(arr, admitted_ies);
  admitted_item->list.array = arr->array;
  admitted_item->list.count = arr->count;
  admitted_item->list.size = arr->size;
  admitted_item->list.free = arr->free;
  E_RABAdmittedList_Item_t *erab_item = (E_RABAdmittedList_Item_t *)calloc(1, sizeof *erab_item);
  erab_item->id = req.admitted.id;
  erab_item->criticality = req.admitted.criticality;
  erab_item->value = *admitted_item;

  Target_To_Source_TContainer_Item_t *con_item = (Target_To_Source_TContainer_Item_t *)calloc(1, sizeof *con_item);
  con_item->id = req.tts.id;
  con_item->criticality = req.tts.criticality;
  con_item->value = req.tts.value;

  HandoverRequestAcknowledge_IEs_t *ies = (HandoverRequestAcknowledge_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->erab = *erab_item;
  ies->container = *con_item;

  HandoverRequestAcknowledge_t *msg = (HandoverRequestAcknowledge_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_handoverRequestAck;
  pros->choice.handoverRequestAck = *msg;

  SuccessfulOutcome_t *init = (SuccessfulOutcome_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_successfulOutcome;
  pdu_type->choice.successfulOutcome = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(admitted);
  free(admitted_ies);
  free(admitted_item);
  free(arr->array);
  free(arr);
  free(erab_item);
  free(con_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);
  return data;
}

struct Data CreateHandoverCommand(struct HandCommand req)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = req.mme.id;
  mme_item->criticality = req.mme.criticality;
  mme_item->value = req.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = req.enub.id;
  enub_item->criticality = req.enub.criticality;
  enub_item->value = req.enub.value;

  HandoverType_Item_t *hand_item = (HandoverType_Item_t *)calloc(1, sizeof *hand_item);
  hand_item->id = req.type.id;
  hand_item->criticality = req.type.criticality;
  hand_item->value = req.type.type;

  E_RABDataForwardingItem_t *erab_for = (E_RABDataForwardingItem_t *)calloc(1, sizeof *erab_for);
  erab_for->e_RAB_ID = req.forwarding.erab_id;
  E_RABDataForwardingItemIEs_t *ies_for = (E_RABDataForwardingItemIEs_t *)calloc(1, sizeof *ies_for);
  ies_for->id = req.forwarding.id_for;
  ies_for->criticality = req.forwarding.criticality_for;
  ies_for->type = *erab_for;
  E_RABSubjecttoDataForwardingList_t *list = (E_RABSubjecttoDataForwardingList_t *)calloc(1, sizeof *list);
  A_SEQUENCE_OF(struct E_RABDataForwardingItemIEs) *arr = calloc(1, sizeof *arr);
  asn_sequence_add(arr, ies_for);
  list->list.array = arr->array;
  list->list.count = arr->count;
  list->list.size = arr->size;
  list->list.free = arr->free;
  E_RABSubjecttoDataForwardingList_Item_t *for_item = (E_RABSubjecttoDataForwardingList_Item_t *)calloc(1, sizeof *for_item);
  for_item->id = req.forwarding.id;
  for_item->criticality = req.forwarding.criticality;
  for_item->value = *list;

  Target_To_Source_TContainer_Item_t *con_item = (Target_To_Source_TContainer_Item_t *)calloc(1, sizeof *con_item);
  con_item->id = req.tts.id;
  con_item->criticality = req.tts.criticality;
  con_item->value = req.tts.value;

  HandoverCommand_IEs_t *ies = (HandoverCommand_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->type = *hand_item;
  ies->forwardList = *for_item;
  ies->container = *con_item;

  HandoverCommand_t *msg = (HandoverCommand_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_handoverCommand;
  pros->choice.handoverCommand = *msg;

  SuccessfulOutcome_t *init = (SuccessfulOutcome_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_successfulOutcome;
  pdu_type->choice.successfulOutcome = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(hand_item);
  free(erab_for);
  free(ies_for);
  free(list);
  free(arr->array);
  free(arr);
  free(for_item);
  free(con_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);
  return data;
}

struct Data CreateENBStatusTransfer(struct ENBStatus req)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = req.mme.id;
  mme_item->criticality = req.mme.criticality;
  mme_item->value = req.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = req.enub.id;
  enub_item->criticality = req.enub.criticality;
  enub_item->value = req.enub.value;

  COUNTvalue_t *ul_count = (COUNTvalue_t *)calloc(1, sizeof *ul_count);
  ul_count->pDCP_SN = req.status.ul_pdcp;
  ul_count->hFN = req.status.ul_hfn;
  COUNTvalue_t *dl_count = (COUNTvalue_t *)calloc(1, sizeof *dl_count);
  dl_count->pDCP_SN = req.status.dl_pdcp;
  dl_count->hFN = req.status.dl_hfn;
  Bearers_SubjectToStatusTransfer_Item_t *bearses = (Bearers_SubjectToStatusTransfer_Item_t *)calloc(1, sizeof *bearses);
  bearses->e_RAB_ID = req.status.erab_id;
  bearses->ul_CountValue = *ul_count;
  bearses->dl_CountValue = *dl_count;
  Bearers_SubjectToStatusTransfer_ItemIEs_t *bearer_ie = (Bearers_SubjectToStatusTransfer_ItemIEs_t *)calloc(1, sizeof *bearer_ie);
  bearer_ie->id = req.status.bearer_id;
  bearer_ie->criticality = req.status.bearer_criticality;
  bearer_ie->bearse = *bearses;
  Bearers_SubjectToStatusTransferList_t *list = (Bearers_SubjectToStatusTransferList_t *)calloc(1, sizeof *list);
  A_SEQUENCE_OF(struct Bearers_SubjectToStatusTransfer_ItemIEs) *arr = calloc(1, sizeof *arr);
  asn_sequence_add(arr, bearer_ie);
  list->list.array = arr->array;
  list->list.count = arr->count;
  list->list.size = arr->size;
  list->list.free = arr->free;
  ENB_StatusTransfer_TransparentContainer_t *bearer = (ENB_StatusTransfer_TransparentContainer_t *)calloc(1, sizeof *bearer);
  bearer->bearers = *list;
  ENB_StatusTransfer_TransparentContainer_Item_t *bearer_item = (ENB_StatusTransfer_TransparentContainer_Item_t *)calloc(1, sizeof *bearer_item);
  bearer_item->id = req.status.id;
  bearer_item->criticality = req.status.criticality;
  bearer_item->value = *bearer;

  ENBStatusTransfer_IEs_t *ies = (ENBStatusTransfer_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->status = *bearer_item;

  ENBStatusTransfer_t *msg = (ENBStatusTransfer_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_eNBStatusTransfer;
  pros->choice.eNBStatusTransfer = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(ul_count);
  free(dl_count);
  free(bearses);
  free(bearer_ie);
  free(list);
  free(arr->array);
  free(arr);
  free(bearer);
  free(bearer_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);
  return data;
}

struct Data CreateHandoverNotify(struct HandNotify req)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = req.mme.id;
  mme_item->criticality = req.mme.criticality;
  mme_item->value = req.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = req.enub.id;
  enub_item->criticality = req.enub.criticality;
  enub_item->value = req.enub.value;

  EUTRAN_CGI_t *eutran_cgi = (EUTRAN_CGI_t *)calloc(1, sizeof *eutran_cgi);
  eutran_cgi->pLMNidentity = req.eutran.plmnidentity;
  eutran_cgi->cell_ID.buf = req.eutran.cell_id;
  eutran_cgi->cell_ID.size = strlen(req.eutran.cell_id);
  EUTRAN_Item_t *eutran_item = (EUTRAN_Item_t *)calloc(1, sizeof *eutran_item);
  eutran_item->id = req.eutran.id;
  eutran_item->criticality = req.eutran.criticality;
  eutran_item->value = *eutran_cgi;

  TAI_t *TAI = (TAI_t *)calloc(1, sizeof *TAI);
  TAI->pLMNidentity = req.tai.plmnidentity;
  TAI->tAC = req.tai.tac;
  TAI_Item_t *tai_item = (TAI_Item_t *)calloc(1, sizeof *tai_item);
  tai_item->id = req.tai.id;
  tai_item->criticality = req.tai.criticality;
  tai_item->value = *TAI;

  HandoverNotify_IEs_t *ies = (HandoverNotify_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->eutran = *eutran_item;
  ies->tai = *tai_item;

  HandoverNotify_t *msg = (HandoverNotify_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_handoverNotify;
  pros->choice.handoverNotify = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(eutran_cgi);
  free(eutran_item);
  free(TAI);
  free(tai_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);
  return data;
}

struct Data CreateUEContextReleaseCommand(struct ReleaseCommand req)
{
  uint8_t *buffer;

  UE_S1AP_ID_pair_t *pair = (UE_S1AP_ID_pair_t *)calloc(1, sizeof *pair);
  pair->mme = req.ids.mme_pair;
  pair->enub = req.ids.enub_pair;

  UE_S1AP_IDs_t *ids = (UE_S1AP_IDs_t *)calloc(1, sizeof *ids);
  ids->present = UE_S1AP_IDs_PR_uE_S1AP_ID_pair;
  ids->choice.uE_S1AP_ID_pair = *pair;
  UE_S1AP_IDs_Item_t *ids_item = (UE_S1AP_IDs_Item_t *)calloc(1, sizeof *ids_item);
  ids_item->id = req.ids.id;
  ids_item->criticality = req.ids.criticality;
  ids_item->value = *ids;

  Cause_t *cause = (Cause_t *)calloc(1, sizeof *cause);

  if (req.choice == 0)
  {
    cause->present = Cause_PR_radioNetwork;
    cause->choice.radioNetwork = req.cause.radioNetwork;
  }
  else if (req.choice == 2)
  {
    cause->present = Cause_PR_nas;
    cause->choice.nas = req.cause.nas;
  }

  Cause_Item_t *cause_item = (Cause_Item_t *)calloc(1, sizeof *cause_item);
  cause_item->id = req.cause.id;
  cause_item->criticality = req.cause.criticality;
  cause_item->value = *cause;

  UeContextReleaseCommand_IEs_t *ies = (UeContextReleaseCommand_IEs_t *)calloc(1, sizeof *ies);
  ies->cause = *cause_item;
  ies->s1ap = *ids_item;

  UeContextReleaseCommand_t *msg = (UeContextReleaseCommand_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_ueContextReleaseCommand;
  pros->choice.ueContextReleaseCommand = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);

  free(pair);
  free(ids);
  free(ids_item);
  free(cause);
  free(cause_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);
  return data;
}

struct Data CreateUEContextReleaseComplete(struct ReleaseComplete req)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = req.mme.id;
  mme_item->criticality = req.mme.criticality;
  mme_item->value = req.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = req.enub.id;
  enub_item->criticality = req.enub.criticality;
  enub_item->value = req.enub.value;

  UeContextReleaseComplete_IEs_t *ies = (UeContextReleaseComplete_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;

  UeContextReleaseComplete_t *msg = (UeContextReleaseComplete_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_ueContextReleaseComplete;
  pros->choice.ueContextReleaseComplete = *msg;

  SuccessfulOutcome_t *init = (SuccessfulOutcome_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_successfulOutcome;
  pdu_type->choice.successfulOutcome = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);
  return data;
}

struct Data CreateUEContextReleaseRequest(struct ReleaseRequest req)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = req.mme.id;
  mme_item->criticality = req.mme.criticality;
  mme_item->value = req.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = req.enub.id;
  enub_item->criticality = req.enub.criticality;
  enub_item->value = req.enub.value;

  Cause_t *cause = (Cause_t *)calloc(1, sizeof *cause);
  cause->present = Cause_PR_transport;
  cause->choice.transport = req.cause.transport;
  Cause_Item_t *cause_item = (Cause_Item_t *)calloc(1, sizeof *cause_item);
  cause_item->id = req.cause.id;
  cause_item->criticality = req.cause.criticality;
  cause_item->value = *cause;

  UeContextReleaseRequest_IEs_t *ies = (UeContextReleaseRequest_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->cause = *cause_item;

  UeContextReleaseRequest_t *msg = (UeContextReleaseRequest_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_ueContextReleaseRequest;
  pros->choice.ueContextReleaseRequest = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = req.s1ap.procedureCode;
  init->criticality = req.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = req.s1ap.msgType;

  struct Data data = Encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(cause);
  free(cause_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);

  return data;
}

///////////////////////////Decoding of Handover messages.////////////////////////////////
struct HandoverFields AccessHandoverRequired(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = msg.value.choice.handoverRequired.protocolIEs
                        .cause.value.choice.radioNetwork;

  return decoded;
}

struct HandoverFields AccessHandoverRequest(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = msg.value.choice.handoverRequest.protocolIEs
                        .cause.value.choice.radioNetwork;

  return decoded;
}

struct HandoverFields AccessHandoverRequestAcknowledge(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  SuccessfulOutcome_t msg = pdu->type.choice.successfulOutcome;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = -1;

  return decoded;
}

struct HandoverFields AccessHandoverCommand(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = -1;

  return decoded;
}

struct HandoverFields AccessENBStatusTransfer(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = -1;

  return decoded;
}

struct HandoverFields AccessMMEStatusTransfer(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = -1;

  return decoded;
}

struct HandoverFields AccessHandoverNotify(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = -1;

  return decoded;
}

struct HandoverFields AccessUEContextReleaseCommand(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;

  if (msg.value.choice.ueContextReleaseCommand.protocolIEs
          .cause.value.present == 1)
  {
    decoded.CauseID = msg.value.choice.ueContextReleaseCommand.protocolIEs
                          .cause.value.choice.radioNetwork;
  }
  else if (msg.value.choice.ueContextReleaseCommand.protocolIEs
               .cause.value.present == 3)
  {
    decoded.CauseID = msg.value.choice.ueContextReleaseCommand.protocolIEs
                          .cause.value.choice.nas;
  }

  return decoded;
}

struct HandoverFields AccessUEContextReleaseComplete(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = -1;

  return decoded;
}

struct HandoverFields AccessUEContextReleaseRequest(S1AP_PDU_t *pdu)
{
  struct HandoverFields decoded;

  decoded.S1apPdu = pdu->type.present - 1;
  InitiatingMessage_t msg = pdu->type.choice.initiatingMessage;

  decoded.ProcedureCode = msg.procedureCode;
  decoded.CauseID = msg.value.choice.handoverRequired.protocolIEs
                        .cause.value.choice.radioNetwork;

  return decoded;
}

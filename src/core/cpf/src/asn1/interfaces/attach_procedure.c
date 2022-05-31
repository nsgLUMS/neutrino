#include "../compiled/S1AP-PDU.h"
#include "structs.h"

struct Data create_initial_ue_message(struct InitUe);
struct Data create_downlink_nas_transport(struct DownlinkNas);
struct Data create_uplink_nas_transport(struct UplinkNas);
struct Data create_initial_context_setup_request(struct InitialSetupRequest);
struct Data create_initial_context_setup_response(struct InitialSetupResponse);
struct Data create_ue_capability_info_indication(struct UeCapabilityIndication);

struct Data encode(S1AP_PDU_t *pdu, uint8_t *buffer)
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

//////////////////////Encoding of Attach messages//////////////////////////

struct Data attach_request()
{
  struct InitUe ue;

  ue.enub.id = 8;
  ue.enub.criticality = 0;
  ue.enub.value = 1;

  ue.nas.id = 26;
  ue.nas.criticality = 0;
  char *nas_value = "0741720bf655f531870148e000000102e0e000210201d011d1271a8080211001000010810600000000830600000000000d00000a005255f531b26e9011034f18a6e0";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_NAS_Item, nas_value, strlen(nas_value));
  ue.nas.value = *s;

  char *plmn_identity = "55f531";
  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, plmn_identity, strlen(plmn_identity));
  ue.tai.plmnidentity = *s1;
  char *tac = "45678";
  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, tac, strlen(tac));
  ue.tai.tac = *s2;
  ue.tai.id = 67;
  ue.tai.criticality = 0;

  OCTET_STRING_t *s3 = OCTET_STRING_new_fromBuf(&asn_DEF_EUTRAN_CGI, plmn_identity, strlen(plmn_identity));
  ue.eutran.plmnidentity = *s3;
  ue.eutran.cell_id = "00000101";
  ue.eutran.id = 100;
  ue.eutran.criticality = 1;

  ue.rrc.id = 134;
  ue.rrc.criticality = 1;
  ue.rrc.value = 0;

  ue.s1ap.procedureCode = 12;
  ue.s1ap.criticality = 1;
  ue.s1ap.msgType = 1;

  struct Data data = create_initial_ue_message(ue);

  FREEMEM(s->buf);
  FREEMEM(s);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);
  FREEMEM(s3->buf);
  FREEMEM(s3);

  return data;
}

struct Data authentication_request()
{
  struct DownlinkNas down;

  down.mme.id = 0;
  down.mme.criticality = 0;
  down.mme.value = 0;

  down.enub.id = 8;
  down.enub.criticality = 0;
  down.enub.value = 1;

  down.nas.id = 26;
  down.nas.criticality = 0;
  char *nas_value = "075201b8eeb8eeb8eeb8eeb8eeb8eeb8eeb8ee10b14d41f46e52b9b9e10fa67cbfda8011";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_NAS_Item, nas_value, strlen(nas_value));
  down.nas.value = *s;

  down.s1ap.procedureCode = 11;
  down.s1ap.criticality = 1;
  down.s1ap.msgType = 2;

  struct Data data = create_downlink_nas_transport(down);

  FREEMEM(s->buf);
  FREEMEM(s);

  return data;
}

struct Data authentication_response()
{
  struct UplinkNas up;

  up.mme.id = 0;
  up.mme.criticality = 0;
  up.mme.value = 0;

  up.enub.id = 8;
  up.enub.criticality = 0;
  up.enub.value = 1;

  up.nas.id = 26;
  up.nas.criticality = 0;
  char *nas_value = "075308deba34fd848a4448";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_NAS_Item, nas_value, strlen(nas_value));
  up.nas.value = *s;

  char *plmn_identity = "55f531";
  OCTET_STRING_t *s3 = OCTET_STRING_new_fromBuf(&asn_DEF_EUTRAN_CGI, plmn_identity, strlen(plmn_identity));
  up.eutran.plmnidentity = *s3;
  up.eutran.cell_id = "00000101";
  up.eutran.id = 100;
  up.eutran.criticality = 1;

  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, plmn_identity, strlen(plmn_identity));
  up.tai.plmnidentity = *s1;
  char *tac = "45678";
  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, tac, strlen(tac));
  up.tai.tac = *s2;
  up.tai.id = 67;
  up.tai.criticality = 1;

  up.s1ap.procedureCode = 13;
  up.s1ap.criticality = 1;
  up.s1ap.msgType = 3;

  struct Data data = create_uplink_nas_transport(up);

  FREEMEM(s->buf);
  FREEMEM(s);
  FREEMEM(s3->buf);
  FREEMEM(s3);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);

  return data;
}

struct Data security_mode_command()
{
  struct DownlinkNas down;

  down.mme.id = 0;
  down.mme.criticality = 0;
  down.mme.value = 0;

  down.enub.id = 8;
  down.enub.criticality = 0;
  down.enub.value = 1;

  down.nas.id = 26;
  down.nas.criticality = 0;
  char *nas_value = "37ba9d339400075d010102e0e0c1";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_NAS_Item, nas_value, strlen(nas_value));
  down.nas.value = *s;

  down.s1ap.procedureCode = 11;
  down.s1ap.criticality = 1;
  down.s1ap.msgType = 2;

  struct Data data = create_downlink_nas_transport(down);

  FREEMEM(s->buf);
  FREEMEM(s);

  return data;

}

struct Data security_mode_complete()
{
  struct UplinkNas up;

  up.mme.id = 0;
  up.mme.criticality = 0;
  up.mme.value = 0;

  up.enub.id = 8;
  up.enub.criticality = 0;
  up.enub.value = 1;

  up.nas.id = 26;
  up.nas.criticality = 0;
  char *nas_value = "47bb6fc9ce00075e23093385910614114005f0";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_NAS_Item, nas_value, strlen(nas_value));
  up.nas.value = *s;

  char *plmn_identity = "55f531";
  OCTET_STRING_t *s3 = OCTET_STRING_new_fromBuf(&asn_DEF_EUTRAN_CGI, plmn_identity, strlen(plmn_identity));
  up.eutran.plmnidentity = *s3;
  up.eutran.cell_id = "00000101";
  up.eutran.id = 100;
  up.eutran.criticality = 1;

  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, plmn_identity, strlen(plmn_identity));
  up.tai.plmnidentity = *s1;
  char *tac = "45678";
  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, tac, strlen(tac));
  up.tai.tac = *s2;
  up.tai.id = 67;
  up.tai.criticality = 1;

  up.s1ap.procedureCode = 13;
  up.s1ap.criticality = 1;
  up.s1ap.msgType = 3;

  struct Data data = create_uplink_nas_transport(up);

  FREEMEM(s->buf);
  FREEMEM(s);
  FREEMEM(s3->buf);
  FREEMEM(s3);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);

  return data;
}

struct Data esm_information_request()
{
  struct DownlinkNas down;

  down.mme.id = 0;
  down.mme.criticality = 0;
  down.mme.value = 0;

  down.enub.id = 8;
  down.enub.criticality = 0;
  down.enub.value = 1;

  down.nas.id = 26;
  down.nas.criticality = 0;
  char *nas_value = "277a296668010201d9";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_NAS_Item, nas_value, strlen(nas_value));
  down.nas.value = *s;

  down.s1ap.procedureCode = 11;
  down.s1ap.criticality = 1;
  down.s1ap.msgType = 2;

  struct Data data = create_downlink_nas_transport(down);

  FREEMEM(s->buf);
  FREEMEM(s);

  return data;
}

struct Data esm_information_response()
{
  struct UplinkNas up;

  up.mme.id = 0;
  up.mme.criticality = 0;
  up.mme.value = 0;

  up.enub.id = 8;
  up.enub.criticality = 0;
  up.enub.value = 1;

  up.nas.id = 26;
  up.nas.criticality = 0;
  char *nas_value = "2700d38231010201da2808034e534e03636f6d";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_NAS_Item, nas_value, strlen(nas_value));
  up.nas.value = *s;

  char *plmn_identity = "55f531";
  OCTET_STRING_t *s3 = OCTET_STRING_new_fromBuf(&asn_DEF_EUTRAN_CGI, plmn_identity, strlen(plmn_identity));
  up.eutran.plmnidentity = *s3;
  up.eutran.cell_id = "00000101";
  up.eutran.id = 100;
  up.eutran.criticality = 1;

  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, plmn_identity, strlen(plmn_identity));
  up.tai.plmnidentity = *s1;
  char *tac = "45678";
  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, tac, strlen(tac));
  up.tai.tac = *s2;
  up.tai.id = 67;
  up.tai.criticality = 1;

  up.s1ap.procedureCode = 13;
  up.s1ap.criticality = 1;
  up.s1ap.msgType = 3;

  struct Data data = create_uplink_nas_transport(up);

  FREEMEM(s->buf);
  FREEMEM(s);
  FREEMEM(s3->buf);
  FREEMEM(s3);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);

  return data;
}

struct Data attach_accept()
{
  struct InitialSetupRequest req;

  req.mme.id = 0;
  req.mme.criticality = 0;
  req.mme.value = 0;

  req.enub.id = 8;
  req.enub.criticality = 0;
  req.enub.value = 1;

  req.bitrate.id = 66;
  req.bitrate.criticality = 0;

  req.bitrate.bitratedl = 97000000;
  req.bitrate.bitrateul = 47000000;

  req.sureq.id = 24;
  req.sureq.criticality = 0;
  req.sureq.ie_id = 52;
  req.sureq.ie_criticality = 0;
  req.sureq.erab_id = 5;
  req.sureq.qci = 5;
  req.sureq.priority = 1;
  req.sureq.pre_capability = 1;
  req.sureq.pre_vulnerability = 1;
  req.sureq.transport_address = "10.200.10.254";
  char *gtp = "01000000";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_E_RABToBeSetupItemCtxtSUReqN, gtp, strlen(gtp));
  req.sureq.gtp = *s;
  char *nas_pdu = "2760581d220207420125062055f531b26e00305201c1010508034e534e03636f6d05010a0a0a045e04fefe9b69271480802110030000108106d412102a830600000000500bf655f531870147c00000015312640101";
  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_E_RABToBeSetupItemCtxtSUReqN, nas_pdu, strlen(nas_pdu));
  req.sureq.nas_pdu = *s1;

  req.seccaps.id = 107;
  req.seccaps.criticality = 0;
  req.seccaps.encryption = "c000";
  req.seccaps.integrity = "c000";

  req.seckey.id = 73;
  req.seckey.criticality = 0;
  req.seckey.key = "b317cc65ef950a53aebea20ec04d921bb3dce8832274ff299839fea99bac1ab8";

  req.s1ap.procedureCode = 9;
  req.s1ap.criticality = 0;
  req.s1ap.msgType = 4;

  struct Data data = create_initial_context_setup_request(req);

  FREEMEM(s->buf);
  FREEMEM(s);
  FREEMEM(s1->buf);
  FREEMEM(s1);

  return data;
}

struct Data attach_accept_response()
{
  struct InitialSetupResponse res;

  res.mme.id = 0;
  res.mme.criticality = 1;
  res.mme.value = 0;

  res.enub.id = 8;
  res.enub.criticality = 1;
  res.enub.value = 1;

  res.sures.id = 51;
  res.sures.criticality = 1;
  res.sures.ie_id = 50;
  res.sures.ie_criticality = 1;
  res.sures.erab_id = 5;

  res.sures.transport_address = "10.200.10.37";
  char *gtp = "00000001";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_E_RABSetupListCtxtSUResN, gtp, strlen(gtp));
  res.sures.gtp = *s;

  res.s1ap.procedureCode = 9;
  res.s1ap.criticality = 0;
  res.s1ap.msgType = 5;

  struct Data data = create_initial_context_setup_response(res);

  FREEMEM(s->buf);
  FREEMEM(s);

  return data;
}

struct Data ue_capability_information()
{
  struct UeCapabilityIndication in;

  in.mme.id = 0;
  in.mme.criticality = 0;
  in.mme.value = 0;

  in.enub.id = 8;
  in.enub.criticality = 0;
  in.enub.value = 1;

  in.radio.id = 74;
  in.radio.criticality = 1;
  char *radio = "00c80101681380004001060e4c204f84f84f84f84faf06ec400000";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_UERadioCapability_Item, radio, strlen(radio));
  in.radio.value = *s;

  in.s1ap.procedureCode = 22;
  in.s1ap.criticality = 1;
  in.s1ap.msgType = 6;

  struct Data data = create_ue_capability_info_indication(in);

  FREEMEM(s->buf);
  FREEMEM(s);

  return data;
}

struct Data attach_complete()
{
  struct UplinkNas up;

  up.mme.id = 0;
  up.mme.criticality = 0;
  up.mme.value = 0;

  up.enub.id = 8;
  up.enub.criticality = 0;
  up.enub.value = 1;

  up.nas.id = 26;
  up.nas.criticality = 0;
  char *nas_value = "27db78240402074300035200c2";
  OCTET_STRING_t *s = OCTET_STRING_new_fromBuf(&asn_DEF_NAS_Item, nas_value, strlen(nas_value));
  up.nas.value = *s;

  char *plmn_identity = "55f531";
  OCTET_STRING_t *s3 = OCTET_STRING_new_fromBuf(&asn_DEF_EUTRAN_CGI, plmn_identity, strlen(plmn_identity));
  up.eutran.plmnidentity = *s3;
  up.eutran.cell_id = "00000101";
  up.eutran.id = 100;
  up.eutran.criticality = 1;

  OCTET_STRING_t *s1 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, plmn_identity, strlen(plmn_identity));
  up.tai.plmnidentity = *s1;
  char *tac = "45678";
  OCTET_STRING_t *s2 = OCTET_STRING_new_fromBuf(&asn_DEF_TAI, tac, strlen(tac));
  up.tai.tac = *s2;
  up.tai.id = 67;
  up.tai.criticality = 1;

  up.s1ap.procedureCode = 13;
  up.s1ap.criticality = 1;
  up.s1ap.msgType = 2;

  struct Data data = create_uplink_nas_transport(up);

  FREEMEM(s->buf);
  FREEMEM(s);
  FREEMEM(s3->buf);
  FREEMEM(s3);
  FREEMEM(s1->buf);
  FREEMEM(s1);
  FREEMEM(s2->buf);
  FREEMEM(s2);

  return data;
}

/* Procedures */
struct Data create_initial_ue_message(struct InitUe ue)
{
  uint8_t *buffer;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = ue.enub.id;
  enub_item->criticality = ue.enub.criticality;
  enub_item->value = ue.enub.value;

  NAS_Item_t *nas_item = (NAS_Item_t *)calloc(1, sizeof *nas_item);
  nas_item->id = ue.nas.id;
  nas_item->criticality = ue.enub.criticality;
  nas_item->value = ue.nas.value;

  TAI_t *TAI = (TAI_t *)calloc(1, sizeof *TAI);
  TAI->pLMNidentity = ue.tai.plmnidentity;
  TAI->tAC = ue.tai.tac;
  TAI_Item_t *tai_item = (TAI_Item_t *)calloc(1, sizeof *tai_item);
  tai_item->id = ue.tai.id;
  tai_item->criticality = ue.tai.criticality;
  tai_item->value = *TAI;

  EUTRAN_CGI_t *eutran_cgi = (EUTRAN_CGI_t *)calloc(1, sizeof *eutran_cgi);
  eutran_cgi->pLMNidentity = ue.eutran.plmnidentity;
  eutran_cgi->cell_ID.buf = ue.eutran.cell_id;
  eutran_cgi->cell_ID.size = strlen(ue.eutran.cell_id);
  EUTRAN_Item_t *eutran_item = (EUTRAN_Item_t *)calloc(1, sizeof *eutran_item);
  eutran_item->id = ue.eutran.id;
  eutran_item->criticality = ue.eutran.criticality;
  eutran_item->value = *eutran_cgi;

  RRC_Establishment_Item_t *rrc_estab = (RRC_Establishment_Item_t *)calloc(1, sizeof *rrc_estab);
  rrc_estab->id = ue.rrc.id;
  rrc_estab->criticality = ue.rrc.criticality;
  rrc_estab->value = ue.rrc.value;

  InitialUEMessage_IEs_t *ies = (InitialUEMessage_IEs_t *)calloc(1, sizeof *ies);
  ies->enub = *enub_item;
  ies->nas = *nas_item;
  ies->tai = *tai_item;
  ies->eutran = *eutran_item;
  ies->rrc = *rrc_estab;

  InitialUeMessage_t *msg = (InitialUeMessage_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_initialUeMessage;
  pros->choice.initialUeMessage = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = ue.s1ap.procedureCode;
  init->criticality = ue.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = ue.s1ap.msgType;

  struct Data data = encode(pdu, buffer);

  free(enub_item);
  free(nas_item);
  free(TAI);
  free(tai_item);
  free(eutran_cgi);
  free(eutran_item);
  free(rrc_estab);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);


  return data;
}

struct Data create_downlink_nas_transport(struct DownlinkNas down)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = down.mme.id;
  mme_item->criticality = down.mme.criticality;
  mme_item->value = down.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = down.enub.id;
  enub_item->criticality = down.enub.criticality;
  enub_item->value = down.enub.value;

  NAS_Item_t *nas_item = (NAS_Item_t *)calloc(1, sizeof *nas_item);
  nas_item->id = down.nas.id;
  nas_item->criticality = down.enub.criticality;
  nas_item->value = down.nas.value;

  DownlinkNasTransport_IEs_t *ies = (DownlinkNasTransport_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->nas = *nas_item;

  DownlinkNasTransport_t *msg = (DownlinkNasTransport_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_downlinkNasTransport;
  pros->choice.downlinkNasTransport = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = down.s1ap.procedureCode;
  init->criticality = down.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = down.s1ap.msgType;

  struct Data data = encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(nas_item);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);

  return data;
}

struct Data create_uplink_nas_transport(struct UplinkNas up)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = up.mme.id;
  mme_item->criticality = up.mme.criticality;
  mme_item->value = up.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = up.enub.id;
  enub_item->criticality = up.enub.criticality;
  enub_item->value = up.enub.value;

  NAS_Item_t *nas_item = (NAS_Item_t *)calloc(1, sizeof *nas_item);
  nas_item->id = up.nas.id;
  nas_item->criticality = up.enub.criticality;
  nas_item->value = up.nas.value;

  EUTRAN_CGI_t *eutran_cgi = (EUTRAN_CGI_t *)calloc(1, sizeof *eutran_cgi);
  eutran_cgi->pLMNidentity = up.eutran.plmnidentity;
  eutran_cgi->cell_ID.buf = up.eutran.cell_id;
  eutran_cgi->cell_ID.size = strlen(up.eutran.cell_id);
  EUTRAN_Item_t *eutran_item = (EUTRAN_Item_t *)calloc(1, sizeof *eutran_item);
  eutran_item->id = up.eutran.id;
  eutran_item->criticality = up.eutran.criticality;
  eutran_item->value = *eutran_cgi;

  TAI_t *TAI = (TAI_t *)calloc(1, sizeof *TAI);
  TAI->pLMNidentity = up.tai.plmnidentity;
  TAI->tAC = up.tai.tac;
  TAI_Item_t *tai_item = (TAI_Item_t *)calloc(1, sizeof *tai_item);
  tai_item->id = up.tai.id;
  tai_item->criticality = up.tai.criticality;
  tai_item->value = *TAI;

  UplinkNasTransport_IEs_t *ies = (UplinkNasTransport_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->nas = *nas_item;
  ies->eutran = *eutran_item;
  ies->tai = *tai_item;

  UplinkNasTransport_t *msg = (UplinkNasTransport_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_uplinkNasTransport;
  pros->choice.uplinkNasTransport = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = up.s1ap.procedureCode;
  init->criticality = up.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = up.s1ap.msgType;

  struct Data data = encode(pdu, buffer);
  free(mme_item);
  free(enub_item);
  free(nas_item);
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

struct Data create_initial_context_setup_request(struct InitialSetupRequest req)
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

  UE_Aggregate_Maxbitrate_t *rate = (UE_Aggregate_Maxbitrate_t *)calloc(1, sizeof *rate);
  rate->ue_Aggregate_MaximumBitRateDL = req.bitrate.bitratedl;
  rate->ue_Aggregate_MaximumBitRateUL = req.bitrate.bitrateul;
  UE_Aggregate_Maxbitrate_Item_t *bitrate = (UE_Aggregate_Maxbitrate_Item_t *)calloc(1, sizeof *bitrate);
  bitrate->id = req.bitrate.id;
  bitrate->criticality = req.bitrate.criticality;
  bitrate->value = *rate;

  AllocationAndRetentionPriority_t *priority = (AllocationAndRetentionPriority_t *)calloc(1, sizeof *priority);
  priority->priorityLevel = req.sureq.priority;
  priority->pre_emptionCapability = req.sureq.pre_capability;
  priority->pre_emptionVulnerability = req.sureq.pre_vulnerability;
  E_RABLevelQoSParameters_t *qos = (E_RABLevelQoSParameters_t *)calloc(1, sizeof *qos);
  qos->qCI = req.sureq.qci;
  qos->allocationRetentionPriority = *priority;
  E_RABToBeSetupItemCtxtSUReqN_t *reqn = (E_RABToBeSetupItemCtxtSUReqN_t *)calloc(1, sizeof *reqn);
  reqn->e_RAB_ID = req.sureq.erab_id;
  reqn->e_RABlevelQoSParameters = *qos;
  reqn->transportLayerAddress.buf = req.sureq.transport_address;
  reqn->transportLayerAddress.size = strlen(req.sureq.transport_address);
  reqn->gTP_TEID = req.sureq.gtp;
  reqn->nas_pdu = req.sureq.nas_pdu;
  E_RABToBeSetupItemCtxtSUReqIEs_t *reqie = (E_RABToBeSetupItemCtxtSUReqIEs_t *)calloc(1, sizeof *reqie);
  reqie->id = req.sureq.ie_id;
  reqie->criticality = req.sureq.ie_criticality;
  reqie->value = *reqn;
  E_RABToBeSetupListCtxtSUReq_t *sureq = (E_RABToBeSetupListCtxtSUReq_t *)calloc(1, sizeof *sureq);
  A_SEQUENCE_OF(struct E_RABToBeSetupItemCtxtSUReqIEs) *arr = calloc(1, sizeof *arr);
  asn_sequence_add(arr, reqie);
  sureq->list.array = arr->array;
  sureq->list.count = arr->count;
  sureq->list.size = arr->size;
  sureq->list.free = arr->free;
  E_RABToBeSetupListCtxtSUReq_Item_t *sureq_item = (E_RABToBeSetupListCtxtSUReq_Item_t *)calloc(1, sizeof *sureq_item);
  sureq_item->id = req.sureq.id;
  sureq_item->criticality = req.sureq.criticality;
  sureq_item->value = *sureq;

  UESecurityCapabilities_t *seccaps = (UESecurityCapabilities_t *)calloc(1, sizeof *seccaps);
  seccaps->encryptionAlgorithms.buf = req.seccaps.encryption;
  seccaps->encryptionAlgorithms.size = strlen(req.seccaps.encryption);
  seccaps->integrityProtectionAlgorithms.buf = req.seccaps.integrity;
  seccaps->integrityProtectionAlgorithms.size = strlen(req.seccaps.integrity);
  UESecurityCapabilities_Item_t *seccaps_item = (UESecurityCapabilities_Item_t *)calloc(1, sizeof *seccaps_item);
  seccaps_item->id = req.seccaps.id;
  seccaps_item->criticality = req.seccaps.criticality;
  seccaps_item->value = *seccaps;

  SecurityKey_Item_t *seckey = (SecurityKey_Item_t *)calloc(1, sizeof *seckey);
  seckey->id = req.seckey.id;
  seckey->criticality = req.seckey.criticality;
  seckey->value.buf = req.seckey.key;
  seckey->value.size = strlen(req.seckey.key);

  InitialContextSetupRequest_IEs_t *ies = (InitialContextSetupRequest_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->bitrate = *bitrate;
  ies->sureq = *sureq_item;
  ies->secCap = *seccaps_item;
  ies->secKey = *seckey;

  InitialContextSetupRequest_t *msg = (InitialContextSetupRequest_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_initialContextSetupRequest;
  pros->choice.initialContextSetupRequest = *msg;

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

  struct Data data = encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(rate);
  free(bitrate);
  free(priority);
  free(qos);
  free(reqn);
  free(reqie);
  free(sureq);
  free(arr->array);
  free(arr);
  free(sureq_item);
  free(seccaps);
  free(seccaps_item);
  free(seckey);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);

  return data;
}

struct Data create_initial_context_setup_response(struct InitialSetupResponse res)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = res.mme.id;
  mme_item->criticality = res.mme.criticality;
  mme_item->value = res.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = res.enub.id;
  enub_item->criticality = res.enub.criticality;
  enub_item->value = res.enub.value;

  E_RABSetupListCtxtSUResN_t *resn = (E_RABSetupListCtxtSUResN_t *)calloc(1, sizeof *resn);
  resn->e_RAB_ID = res.sures.erab_id;
  resn->transportLayerAddress.buf = res.sures.transport_address;
  resn->transportLayerAddress.size = strlen(res.sures.transport_address);
  resn->gTP_TEID = res.sures.gtp;
  E_RABSetupItemCtxtSUResIEs_t *resie = (E_RABSetupItemCtxtSUResIEs_t *)calloc(1, sizeof *resie);
  resie->id = res.sures.ie_id;
  resie->criticality = res.sures.ie_criticality;
  resie->value = *resn;
  E_RABSetupListCtxtSURes_t *sures = (E_RABSetupListCtxtSURes_t *)calloc(1, sizeof *sures);
  A_SEQUENCE_OF(struct E_RABSetupItemCtxtSUResIEs) *arr = calloc(1, sizeof *arr);
  asn_sequence_add(arr, resie);
  sures->list.array = arr->array;
  sures->list.count = arr->count;
  sures->list.size = arr->size;
  sures->list.free = arr->free;
  E_RABSetupListCtxtSURes_Item_t *sures_item = (E_RABSetupListCtxtSURes_Item_t *)calloc(1, sizeof *sures_item);
  sures_item->id = res.sures.id;
  sures_item->criticality = res.sures.criticality;
  sures_item->value = *sures;

  InitialContextSetupResponse_IEs_t *ies = (InitialContextSetupResponse_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->sures = *sures_item;

  InitialContextSetupResponse_t *msg = (InitialContextSetupResponse_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_initialContextSetupResponse;
  pros->choice.initialContextSetupResponse = *msg;

  SuccessfulOutcome_t *so = (SuccessfulOutcome_t *)calloc(1, sizeof *so);
  so->procedureCode = res.s1ap.procedureCode;
  so->criticality = res.s1ap.criticality;
  so->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_successfulOutcome;
  pdu_type->choice.successfulOutcome = *so;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = res.s1ap.msgType;

  struct Data data = encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(resn);
  free(resie);
  free(sures);
  free(arr->array);
  free(arr);
  free(sures_item);
  free(ies);
  free(msg);
  free(pros);
  free(so);
  free(pdu_type);
  free(pdu);

  return data;
}

struct Data create_ue_capability_info_indication(struct UeCapabilityIndication in)
{
  uint8_t *buffer;

  MME_Item_t *mme_item = (MME_Item_t *)calloc(1, sizeof *mme_item);
  mme_item->id = in.mme.id;
  mme_item->criticality = in.mme.criticality;
  mme_item->value = in.mme.value;

  ENUB_Item_t *enub_item = (ENUB_Item_t *)calloc(1, sizeof *enub_item);
  enub_item->id = in.enub.id;
  enub_item->criticality = in.enub.criticality;
  enub_item->value = in.enub.value;

  UERadioCapability_Item_t *radio = (UERadioCapability_Item_t *)calloc(1, sizeof *radio);
  radio->id = in.radio.id;
  radio->criticality = in.radio.criticality;
  radio->value = in.radio.value;

  UeCapabilityInfoIndication_IEs_t *ies = (UeCapabilityInfoIndication_IEs_t *)calloc(1, sizeof *ies);
  ies->mme = *mme_item;
  ies->enub = *enub_item;
  ies->radio = *radio;

  UeCapabilityInfoIndication_t *msg = (UeCapabilityInfoIndication_t *)calloc(1, sizeof *msg);
  msg->protocolIEs = *ies;

  S1AP_ELEMENTARY_PROCEDURE_t *pros = (S1AP_ELEMENTARY_PROCEDURE_t *)calloc(1, sizeof *pros);
  pros->present = S1AP_ELEMENTARY_PROCEDURE_PR_ueCapabilityInfoIndication;
  pros->choice.ueCapabilityInfoIndication = *msg;

  InitiatingMessage_t *init = (InitiatingMessage_t *)calloc(1, sizeof *init);
  init->procedureCode = in.s1ap.procedureCode;
  init->criticality = in.s1ap.criticality;
  init->value = *pros;

  S1AP_PDU_Types_t *pdu_type = (S1AP_PDU_Types_t *)calloc(1, sizeof *pdu_type);
  pdu_type->present = S1AP_PDU_Types_PR_initiatingMessage;
  pdu_type->choice.initiatingMessage = *init;

  S1AP_PDU_t *pdu = (S1AP_PDU_t *)calloc(1, sizeof *pdu);
  pdu->type = *pdu_type;
  pdu->msgType = in.s1ap.msgType;

  struct Data data = encode(pdu, buffer);

  free(mme_item);
  free(enub_item);
  free(radio);
  free(ies);
  free(msg);
  free(pros);
  free(init);
  free(pdu_type);
  free(pdu);

  return data;
}

//////////////////////Decoding of Attach messages////////////////////////////////
struct DecodedData access_initial_ue_message(S1AP_PDU_t *pdu)
{
  struct DecodedData decoded;
  char *nas = pdu->type.choice.initiatingMessage.value.choice.initialUeMessage.protocolIEs.nas.value.buf; 
  // memcpy(nas, , sizeof(char*));

  char temp[3];
  if (nas[0] == '0')
  {
    decoded.NASPD = (int)(nas[1] - '0');
    memcpy(temp, &nas[2], 2);
    temp[2] = '\0';
    decoded.MsgCode = (int)strtol(temp, NULL, 16);
  }
  else
  {
    decoded.NASPD = (int)(nas[13] - '0');
    if (decoded.NASPD == 7)
    {
      memcpy(temp, &nas[14], 2);
      temp[2] = '\0';
      decoded.MsgCode = (int)strtol(temp, NULL, 16);
    }
    else
    {
      memcpy(temp, &nas[16], 2);
      temp[2] = '\0';
      decoded.MsgCode = (int)strtol(temp, NULL, 16);
    }
  }

  decoded.ProtocolType = 1;
  return decoded;
}

struct DecodedData access_downlink_nas_transport(S1AP_PDU_t *pdu)
{
  struct DecodedData decoded;
  char *nas = pdu->type.choice.initiatingMessage.value.choice.downlinkNasTransport.protocolIEs.nas.value.buf;

  char temp[3];
  if (nas[0] == '0')
  {
    decoded.NASPD = (int)(nas[1] - '0');
    memcpy(temp, &nas[2], 2);
    temp[2] = '\0';
    decoded.MsgCode = (int)strtol(temp, NULL, 16);
  }
  else
  {
    decoded.NASPD = (int)(nas[13] - '0');
    if (decoded.NASPD == 7)
    {
      memcpy(temp, &nas[14], 2);
      temp[2] = '\0';
      decoded.MsgCode = (int)strtol(temp, NULL, 16);
    }
    else
    {
      memcpy(temp, &nas[16], 2);
      temp[2] = '\0';
      decoded.MsgCode = (int)strtol(temp, NULL, 16);
    }
  }
  decoded.ProtocolType = 1;
  return decoded;
}

struct DecodedData access_uplink_nas_transport(S1AP_PDU_t *pdu)
{
  struct DecodedData decoded;
  char *nas = pdu->type.choice.initiatingMessage.value.choice.uplinkNasTransport.protocolIEs.nas.value.buf;

  char temp[3];
  if (nas[0] == '0')
  {
    decoded.NASPD = (int)(nas[1] - '0');
    memcpy(temp, &nas[2], 2);
    temp[2] = '\0';
    decoded.MsgCode = (int)strtol(temp, NULL, 16);
  }
  else
  {
    decoded.NASPD = (int)(nas[13] - '0');
    if (decoded.NASPD == 7)
    {
      memcpy(temp, &nas[14], 2);
      temp[2] = '\0';
      decoded.MsgCode = (int)strtol(temp, NULL, 16);
    }
    else
    {
      memcpy(temp, &nas[16], 2);
      temp[2] = '\0';
      decoded.MsgCode = (int)strtol(temp, NULL, 16);
    }
  }
  decoded.ProtocolType = 1;
  return decoded;
}

struct DecodedData access_initial_context_setup_request(S1AP_PDU_t *pdu)
{
  struct DecodedData decoded;
  char *nas = (*pdu->type.choice.initiatingMessage.value.choice.initialContextSetupRequest.protocolIEs.sureq.value.list.array)[0].value.nas_pdu.buf;

  char temp[3];
  if (nas[0] == '0')
  {
    decoded.NASPD = (int)(nas[1] - '0');
    memcpy(temp, &nas[2], 2);
    temp[2] = '\0';
    decoded.MsgCode = (int)strtol(temp, NULL, 16);
  }
  else
  {
    decoded.NASPD = (int)(nas[13] - '0');
    if (decoded.NASPD == 7)
    {
      memcpy(temp, &nas[14], 2);
      temp[2] = '\0';
      decoded.MsgCode = (int)strtol(temp, NULL, 16);
    }
    else
    {
      memcpy(temp, &nas[16], 2);
      temp[2] = '\0';
      decoded.MsgCode = (int)strtol(temp, NULL, 16);
    }
  }
  decoded.ProtocolType = 1;

  return decoded;
}

struct DecodedData access_initial_context_setup_response(S1AP_PDU_t *pdu)
{
  struct DecodedData decoded;
  decoded.MsgCode = pdu->type.choice.successfulOutcome.procedureCode;
  decoded.NASPD = -1;
  decoded.ProtocolType = 0;

  return decoded;
}

struct DecodedData access_ue_capability_info_indication(S1AP_PDU_t *pdu)
{
  struct DecodedData decoded;
  decoded.MsgCode = pdu->type.choice.initiatingMessage.procedureCode;
  decoded.NASPD = -1;
  decoded.ProtocolType = 0;

  return decoded;
}

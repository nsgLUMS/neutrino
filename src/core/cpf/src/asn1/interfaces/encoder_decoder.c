#include "encoder_decoder.h"
#include "structs.h"
#include "attach_procedure.h"
#include "handover_procedure.h"
#include "service_request_procedure.h"


/*For Handover messages.*/
/* 
   Input: buffer-> Encoded message.
            size-> Size of encoded message.
 
   Return: HandoverFields structure
                 |
                 v
           S1apPdu field: Type of message(Initiating, successful or unsuccessful)
           CauseID field: If present, otherwise set to -1.
           ProcedureCode: Procedure code of message.

*/
struct HandoverFields DecodeHandover(uint8_t buffer[], int size) {
  S1AP_PDU_t *pdu = 0;
  struct HandoverFields fields;
  const asn_dec_rval_t rval = uper_decode(0, &asn_DEF_S1AP_PDU,
                                          (void **)&pdu,
                                          buffer,
                                          size,
                                          0, 0);

  
  if(pdu->msgType == 1) {
    fields = AccessHandoverRequired(pdu);
  } else if(pdu->msgType == 2) {
    fields = AccessHandoverRequest(pdu);
  } else if(pdu->msgType == 3) {
    fields = AccessHandoverRequestAcknowledge(pdu);
  } else if (pdu->msgType == 4) {
    fields = AccessHandoverCommand(pdu);
  } else if (pdu->msgType == 5) {
    fields = AccessENBStatusTransfer(pdu);
  } else if (pdu->msgType == 6) {
    fields = AccessMMEStatusTransfer(pdu);
  } else if (pdu->msgType == 7) {
    fields = AccessHandoverNotify(pdu);
  } else if (pdu->msgType == 8) {
    fields = AccessUEContextReleaseCommand(pdu);
  } else if (pdu->msgType == 9) {
    fields = AccessUEContextReleaseComplete(pdu);
  } else {
    fields = AccessUEContextReleaseRequest(pdu);
  }

  SEQUENCE_free(&asn_DEF_S1AP_PDU, pdu, ASFM_FREE_EVERYTHING);
  return fields;
}

/*For Attach messages.*/
/* 
   Input: buffer-> Encoded message.
            size-> Size of encoded message.
 
   Return: DecodedData structure
                 |
                 v
           MsgCode field: Procedure Code of a particular message.
           NASPD field: If present, otherwise set to -1.
           ProtocolType: 1 if present, otherwise 0.

*/
struct DecodedData Decode(uint8_t buffer[], int size) {
  S1AP_PDU_t *pdu = 0;
  struct DecodedData data;
  const asn_dec_rval_t rval = uper_decode(0, &asn_DEF_S1AP_PDU,
                                          (void **)&pdu,
                                          buffer,
                                          size,
                                          0, 0);

  
  if(pdu->msgType == 1) {
    data = access_initial_ue_message(pdu);
  } else if(pdu->msgType == 2) {
    data = access_downlink_nas_transport(pdu);
  } else if(pdu->msgType == 3) {
    data = access_uplink_nas_transport(pdu);
  } else if (pdu->msgType == 4) {
    data = access_initial_context_setup_request(pdu);
  } else if (pdu->msgType == 5) {
    data = access_initial_context_setup_response(pdu);
  } else {
    data = access_ue_capability_info_indication(pdu);
  }

  SEQUENCE_free(&asn_DEF_S1AP_PDU, pdu, ASFM_FREE_EVERYTHING);
  return data;
}


uint8_t DecodeServiceRequest(uint8_t buffer[], int size) {
  S1AP_PDU_t *pdu = 0;
  uint8_t msgType;
  const asn_dec_rval_t rval = uper_decode(0, &asn_DEF_S1AP_PDU,
                                          (void **)&pdu,
                                          buffer,
                                          size,
                                          0, 0);

  
  if(pdu->msgType == 1) {
    msgType = access_service_request();
  } else if(pdu->msgType == 2) {
    msgType = access_sr_initial_context_setup_request();
  } else if(pdu->msgType == 3) {
    msgType = access_sr_initial_context_setup_response();
  } else if (pdu->msgType == 4) {
    msgType = access_erab_modify_request();
  } else {
    msgType = access_erab_modify_response();
  } 

  SEQUENCE_free(&asn_DEF_S1AP_PDU, pdu, ASFM_FREE_EVERYTHING);
  return msgType;
}

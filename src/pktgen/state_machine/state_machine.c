#include "state_machine.h"
#include <rte_malloc.h>
#include <rte_memcpy.h>

// ================ ASN1 encoding/decoding ======================
uint16_t asn1_attach_decode(uint8_t* buf, size_t len, States* nxt) {
  struct DecodedData decod = Decode(buf, len);

  uint8_t proc_type = decod.ProtocolType;
  int nas_pdu = decod.NASPD;
  int msg_type = decod.MsgCode;

  // printf("Msgtype: %d | proc_type: %d | NAS_PDU: %d\n", msg_type, proc_type, nas_pdu);

  uint16_t index = 0;
  switch (msg_type) {
    case AUTHENTICATION_REQUEST:
      // printf("Got here\n");
      nxt[index] = AUTHENTICATION_RESPONSE;
      index++;
      break;

    case SECURITY_MODE_COMPLETE:
      nxt[index] = ESM_INFO_REQUEST;
      index++;
      break;

    case ESM_INFO_RESPONSE:
      nxt[index] = ATTACH_ACCEPT;
      index++;
      break;

    case ATTACH_ACCEPT:
      nxt[index] = INITIAL_CONTEXT_SETUP_RESPONSE;
      index++;
      nxt[index] = UE_CAPABILITY_INFO_INDICATION;
      index++;
      nxt[index] = ATTACH_COMPLETE;
      index++;
      break;

    case UE_CAPABILITY_INFO_INDICATION:
      break;

    case ATTACH_COMPLETE:
      break;

    case ATTACH_REQUEST:
      nxt[index] = AUTHENTICATION_REQUEST;
      index++;
      break;

    case AUTHENTICATION_RESPONSE:
      nxt[index] = SECURITY_MODE_COMMAND;
      index++;
      break;

    case SECURITY_MODE_COMMAND:
      nxt[index] = SECURITY_MODE_COMPLETE;
      index++;
      break;

    case ESM_INFO_REQUEST:
      nxt[index] = ESM_INFO_RESPONSE;
      index++;
      break;

    case INITIAL_CONTEXT_SETUP_RESPONSE:
      break;
  }

  return index;
}

size_t asn1_attach_next_msg(States curr, uint8_t* buf) {
  struct Data asn1_msg;
  size_t len;

  switch (curr) {
    case ATTACH_REQUEST:
      asn1_msg = attach_request();
      break;

    case AUTHENTICATION_REQUEST:
      asn1_msg = authentication_request();
      break;

    case AUTHENTICATION_RESPONSE:
      asn1_msg = authentication_response();
      break;

    case SECURITY_MODE_COMMAND:
      asn1_msg = security_mode_command();
      break;

    case SECURITY_MODE_COMPLETE:
      asn1_msg = security_mode_complete();
      break;

    case ESM_INFO_REQUEST:
      asn1_msg = esm_information_request();
      break;

    case ESM_INFO_RESPONSE:
      asn1_msg = esm_information_response();
      break;

    case ATTACH_ACCEPT:
      asn1_msg = attach_accept();
      break;

    case INITIAL_CONTEXT_SETUP_RESPONSE:
      asn1_msg = attach_accept_response();
      break;

    case UE_CAPABILITY_INFO_INDICATION:
      asn1_msg = ue_capability_information();
      break;

    case ATTACH_COMPLETE:
      asn1_msg = attach_complete();
      break;
  }
  rte_memcpy(buf, asn1_msg.buffer, asn1_msg.size);
  len = asn1_msg.size;
  free(asn1_msg.buffer);
  return len;
}

uint16_t asn1_handovr_decode(uint8_t* buf, size_t len, States* nxt) {
  struct HandoverFields decod = DecodeHandover(buf, len);
  uint16_t index = 0;

  States state_id = asn1_handovr_id(&decod);

  switch (state_id) {
    case HANDOVER_REQUIRED:
      nxt[index] = HANDOVER_REQUEST;
      index++;
      break;

    case HANDOVER_REQUEST:
      nxt[index] = HANDOVER_REQUEST_ACK;
      index++;
      break;

    case HANDOVER_REQUEST_ACK:
      nxt[index] = HANDOVER_COMMAND;
      index++;
      break;

    case HANDOVER_COMMAND:
      nxt[index] = ENB_STATUS_TRANSFER;
      index++;
      break;

    case ENB_STATUS_TRANSFER:
      nxt[index] = MME_STATUS_TRANSFER;
      index++;
      break;

    case MME_STATUS_TRANSFER:
      nxt[index] = HANDOVER_NOTIFY;
      index++;
      break;

    case HANDOVER_NOTIFY:
      nxt[index] = UE_CONTEXT_RELEASE_COMMAND;
      index++;
      break;

    case UE_CONTEXT_RELEASE_COMMAND:
      nxt[index] = UE_CONTEXT_RELEASE_COMPLETE;
      index++;
      nxt[index] = UE_CONTEXT_RELEASE_REQUEST;
      index++;
      break;

    case UE_CONTEXT_RELEASE_COMPLETE:
      index++;
      break;

    case UE_CONTEXT_RELEASE_REQUEST:
      nxt[index] = UE_CONTEXT_RELEASE_COMMAND;
      index++;
      break;
  }
  return index;
}

size_t asn1_handovr_next_msg(States curr, uint8_t* buf) {
  struct Data asn1_msg;
  size_t len;

  switch (curr) {
    case HANDOVER_REQUIRED:
      asn1_msg = HandoverRequired();
      break;

    case HANDOVER_REQUEST:
      asn1_msg = HandoverRequest();
      break;

    case HANDOVER_REQUEST_ACK:
      asn1_msg = HandoverRequestAcknowledge();
      break;

    case HANDOVER_COMMAND:
      asn1_msg = HandoverCommand();
      break;

    case ENB_STATUS_TRANSFER:
      asn1_msg = ENBStatusTransfer();
      break;

    case MME_STATUS_TRANSFER:
      asn1_msg = MMEStatusTransfer();
      break;

    case HANDOVER_NOTIFY:
      asn1_msg = HandoverNotify();
      break;
    case UE_CONTEXT_RELEASE_COMMAND:
      asn1_msg = UEContextReleaseCommand();
      break;

    case UE_CONTEXT_RELEASE_COMPLETE:
      asn1_msg = UEContextReleaseComplete();
      break;

    case UE_CONTEXT_RELEASE_REQUEST:
      asn1_msg = UEContextReleaseRequest();
      break;
  }

  rte_memcpy(buf, asn1_msg.buffer, asn1_msg.size);
  len = asn1_msg.size;
  free(asn1_msg.buffer);
  return len;
}

States asn1_handovr_id(struct HandoverFields* decod) {
  if (decod->S1apPdu == 0 && decod->ProcedureCode == 0 && decod->CauseID == 16)
    return HANDOVER_REQUIRED;
  else if (decod->S1apPdu == 0 && decod->ProcedureCode == 1 &&
           decod->CauseID == 33)
    return HANDOVER_REQUEST;
  else if (decod->S1apPdu == 1 && decod->ProcedureCode == 1 &&
           decod->CauseID == -1)
    return HANDOVER_REQUEST_ACK;
  else if (decod->S1apPdu == 1 && decod->ProcedureCode == 0 &&
           decod->CauseID == -1)
    return HANDOVER_COMMAND;
  else if (decod->S1apPdu == 0 && decod->ProcedureCode == 24 &&
           decod->CauseID == -1)
    return ENB_STATUS_TRANSFER;
  else if (decod->S1apPdu == 0 && decod->ProcedureCode == 25 &&
           decod->CauseID == -1)
    return MME_STATUS_TRANSFER;
  else if (decod->S1apPdu == 0 && decod->ProcedureCode == 2 &&
           decod->CauseID == -1)
    return HANDOVER_NOTIFY;
  else if (decod->S1apPdu == 0 && decod->ProcedureCode == 23 &&
           decod->CauseID == 2)
    return UE_CONTEXT_RELEASE_COMMAND;
  else if (decod->S1apPdu == 1 && decod->ProcedureCode == 23 &&
           decod->CauseID == -1)
    return UE_CONTEXT_RELEASE_COMPLETE;
  else if (decod->S1apPdu == 0 && decod->ProcedureCode == 18 &&
           decod->CauseID == 0)
    return UE_CONTEXT_RELEASE_REQUEST;
  else if (decod->S1apPdu == 0 && decod->ProcedureCode == 23 &&
           decod->CauseID == 0)
    return UE_CONTEXT_RELEASE_COMMAND;
}

uint16_t asn1_srvc_req_decode(uint8_t* buf, size_t len, States* nxt) {
  uint8_t msgType = DecodeServiceRequest(buf, len);
  uint16_t index = 0;

  switch (msgType) {
    case SR_INITIAL_CONTEXT_SETUP_REQUEST:
      nxt[index++] = SR_INITIAL_CONTEXT_SETUP_RESPONSE;
      break;

    case E_RAB_MODIFICATION_REQUEST:
      nxt[index++] = E_RAB_MODIFICATION_RESPONSE;
      break;

    default:
      printf("ERROR: Tried to handle unexpected message: %i\n", msgType);
      break;
  }
  return index;
}

size_t asn1_srvc_req_next_msg(States curr, uint8_t* buf) {
  struct Data encoding_data;

  switch (curr) {
    case SERVICE_REQUEST:
      encoding_data = service_request();
      break;

    case SR_INITIAL_CONTEXT_SETUP_RESPONSE:
      encoding_data = sr_initial_context_setup_response();
      break;

    case E_RAB_MODIFICATION_RESPONSE:
      encoding_data = erab_modify_response();
      break;

    default:
      printf("ERROR: Tried to prepare unexpected message\n");
      break;
  }

  rte_memcpy(buf, encoding_data.buffer, encoding_data.size);
  free(encoding_data.buffer);
  return encoding_data.size;
}

uint16_t asn1_dpcm_srvc_decode(uint8_t* buf, size_t len, States* nxt) {
  uint8_t msgType = DecodeServiceRequest(buf, len);
  uint16_t index = 0;

  switch (msgType) {

    case E_RAB_MODIFICATION_REQUEST:
      nxt[index++] = E_RAB_MODIFICATION_RESPONSE;
      break;

    default:
      printf("ERROR: Tried to handle unexpected message: %i\n", msgType);
      break;
  }
  return index;
}

size_t asn1_dpcm_srvc_next_msg(States curr, uint8_t* buf) {
  struct Data encoding_data;

  switch (curr) {
    case SERVICE_REQUEST:
      encoding_data = dpcm_service_request();
      break;

    case E_RAB_MODIFICATION_RESPONSE:
      encoding_data = erab_modify_response();
      break;

    default:
      printf("ERROR: Tried to prepare unexpected message\n");
      break;
  }

  rte_memcpy(buf, encoding_data.buffer, encoding_data.size);
  free(encoding_data.buffer);
  return encoding_data.size;
}

// ================ Flatbuffers encoding/decoding ======================

uint16_t fb_attach_decode(uint8_t* buf, size_t len, States* nxt) {
  struct AttachDecodedData decod = DecoderPartial(buf);

  int proc_code = decod.procedureCode;
  int nasID = decod.protocolDiscriminator;
  int msgType = decod.epsMobilityMessageType;
  uint16_t index = 0;

  switch (msgType) {
    case AUTHENTICATION_REQUEST:
      nxt[index] = AUTHENTICATION_RESPONSE;
      index++;
      break;

    case SECURITY_MODE_COMPLETE:
      nxt[index] = ESM_INFO_REQUEST;
      index++;
      break;

    case ESM_INFO_RESPONSE:
      nxt[index] = ATTACH_ACCEPT;
      index++;
      break;

    case ATTACH_ACCEPT:
      nxt[index] = INITIAL_CONTEXT_SETUP_RESPONSE;
      index++;
      nxt[index] = UE_CAPABILITY_INFO_INDICATION;
      index++;
      nxt[index] = ATTACH_COMPLETE;
      index++;
      break;

    case ATTACH_REQUEST:
      nxt[index] = AUTHENTICATION_REQUEST;
      index++;
      break;

    case AUTHENTICATION_RESPONSE:
      nxt[index] = SECURITY_MODE_COMMAND;
      index++;
      break;

    case SECURITY_MODE_COMMAND:
      nxt[index] = SECURITY_MODE_COMPLETE;
      index++;
      break;

    case ESM_INFO_REQUEST:
      nxt[index] = ESM_INFO_RESPONSE;
      index++;
      break;

    case INITIAL_CONTEXT_SETUP_RESPONSE:
    case UE_CAPABILITY_INFO_INDICATION:
    case ATTACH_COMPLETE:
      break;
  }
  return index;
}

size_t fb_attach_next_msg(States curr, uint8_t* buf) {
  size_t size = 0;
  flatcc_builder_t builder;
  void* buffer;
  flatcc_builder_init(&builder);
  switch (curr) {
    case ATTACH_REQUEST:
      EncodeInitialUEMessage(
          &builder, 3,
          "0741720bf655f531870148e000000102e0e000210201d011d1271a80802110010000"
          "10810600000000830600000000000d00000a005255f531b26e9011034f18a6e0",
          "55f531", "45678", "55f531", "0x00000201");
      break;
    case AUTHENTICATION_REQUEST:
      EncodeDownlinkNASTransport(&builder, 0, 1,
                                 "075201b8eeb8eeb8eeb8eeb8eeb8eeb8eeb8ee10b14d4"
                                 "1f46e52b9b9e10fa67cbfda8011");
      break;
    case AUTHENTICATION_RESPONSE:
      EncodeUplinkNASTransport(&builder, 0, 1, "075308deba34fd848a4448",
                               "55f531", "0x00000101", "55f531", "45678");
      break;
    case SECURITY_MODE_COMMAND:
      EncodeDownlinkNASTransport(&builder, 0, 1,
                                 "37ba9d339400075d010102e0e0c1");
      break;
    case SECURITY_MODE_COMPLETE:
      EncodeUplinkNASTransport(&builder, 0, 1,
                               "47bb6fc9ce00075e23093385910614114005f0",
                               "55f531", "0x00000101", "55f531", "45678");
      break;
    case ESM_INFO_REQUEST:
      EncodeDownlinkNASTransport(&builder, 0, 1, "277a296668010201d9");
      break;
    case ESM_INFO_RESPONSE:
      EncodeUplinkNASTransport(&builder, 0, 1,
                               "2700d38231010201da2808034e534e03636f6d",
                               "55f531", "0x00000101S", "55f531", "45678");
      break;
    case ATTACH_ACCEPT:
      EncodeInitialContextSetupRequest(
          &builder, 0, 1, 97000000, 47000000, 5, "0ac80afe", "01000000",
          "2760581d220207420125062055f531b26e00305201c1010508034e534e03636f6d05"
          "010a0a0a045e04fefe9b69271480802110030000108106d412102a83060000000050"
          "0bf655f531870147c00000015312640101",
          5, 1, "c000", "c000",
          "b317cc65ef950a53aebea20ec04d921bb3dce8832274ff299839fea99bac1ab8");
      break;
    case INITIAL_CONTEXT_SETUP_RESPONSE:
      EncodeInitialContextSetupResponse(&builder, 0, 1, "0ac80a25", "00000001",
                                        5);
      break;
    case UE_CAPABILITY_INFO_INDICATION:
      EncodeUECapabilityInfoIndication(
          &builder, 0, 1,
          "00c80101681380004001060e4c204f84f84f84f84faf06ec400000");
      break;
    case ATTACH_COMPLETE:
      EncodeUplinkNASTransport(&builder, 0, 1, "27db78240402074300035200c2",
                               "55f531", "0x00000101", "55f531", "45678");
      break;
  }
  buffer = flatcc_builder_finalize_buffer(&builder, &size);
  rte_memcpy(buf, buffer, size);
  free(buffer);
  flatcc_builder_clear(&builder);
  return size;
}

uint16_t fb_handovr_decode(uint8_t* buf, size_t len, States* nxt) {
  struct AttachDecodedData decod = DecoderPartial(buf);
  uint16_t index = 0;
  int stateID = fb_handovr_id(&decod);

  switch (stateID) {
    case HANDOVER_REQUIRED:
      nxt[index] = HANDOVER_REQUEST;
      index++;
      break;

    case HANDOVER_REQUEST:
      nxt[index] = HANDOVER_REQUEST_ACK;
      index++;
      break;

    case HANDOVER_REQUEST_ACK:
      nxt[index] = HANDOVER_COMMAND;
      index++;
      break;

    case HANDOVER_COMMAND:
      nxt[index] = ENB_STATUS_TRANSFER;
      index++;
      break;

    case ENB_STATUS_TRANSFER:
      nxt[index] = MME_STATUS_TRANSFER;
      index++;
      break;

    case MME_STATUS_TRANSFER:
      nxt[index] = HANDOVER_NOTIFY;
      index++;
      break;

    case HANDOVER_NOTIFY:
      nxt[index] = UE_CONTEXT_RELEASE_COMMAND;
      index++;
      break;

    case UE_CONTEXT_RELEASE_COMMAND:
      nxt[index] = UE_CONTEXT_RELEASE_COMPLETE;
      index++;
      nxt[index] = UE_CONTEXT_RELEASE_REQUEST;
      index++;
      break;

    case UE_CONTEXT_RELEASE_COMPLETE:
      index++;
      break;

    case UE_CONTEXT_RELEASE_REQUEST:
      nxt[index] = UE_CONTEXT_RELEASE_COMMAND;
      index++;
      break;
  }
  return index;
}

size_t fb_handovr_next_msg(States curr, uint8_t* buf) {
  flatcc_builder_t builder;
  size_t len = 0;
  void* buffer;
  flatcc_builder_init(&builder);

  switch (curr) {
    case HANDOVER_REQUIRED:
      EncodeHandoverRequired(
          &builder, 0, 1, "55f531", "000020", "55f531", "45678",
          "40809b0a101681380004001060e4c204f84f84f84f84faf06ec4000015e800000591"
          "382002900838f0841e1f041c78808081cf80400002008c0240b56ddd5ad813fd9405"
          "c13fd94041f51b582f1f28a2e08c5d04348565caa8430fb380004850d4340007c229"
          "bd96000008155509ec9b8000040605fc60901081816f20f20327630dda980606c091"
          "d018002080240a7da2a827623517260c8591404a845000004e400245000055f53100"
          "0020100055f53100001011800000");
      break;

    case HANDOVER_REQUEST:
      EncodeHandoverRequest(
          &builder, 0, 97000000, 47000000, 5, 1, "0ac80afd", "02000000", 5,
          "40809b0a101681380004001060e4c204f84f84f84f84faf06ec4000015e800000591"
          "382002900838f0841e1f041c78808081cf80400002008c0240b56ddd5ad813fd9405"
          "c13fd94041f51b582f1f28a2e08c5d04348565caa8430fb380004850d4340007c229"
          "bd96000008155509ec9b8000040605fc60901081816f20f20327630dda980606c091"
          "d018002080240a7da2a827623517260c8591404a845000004e400245000055f53100"
          "0020100055f53100001011800000",
          "c000", "c000",
          "92160cdabe2b8ae4f42250dcafea2494a42d0953df715903786734ce0cb9507c",
          2);
      break;

    case HANDOVER_REQUEST_ACK:
      EncodeHandoverRequestAcknowledge(&builder, 0, 1, 5, "0ac80a26",
                                       "00000001", "0ac80a26", "00000002",
                                       "002301091058215b37b281327630dda8181b020"
                                       "0c003f50004869b380004850d0001f09110");
      break;

    case HANDOVER_COMMAND:
      EncodeHandoverCommand(&builder, 0, 1, 5, "0ac80a26", "00000002",
                            "002301091058215b37b281327630dda8181b0200c003f50004"
                            "869b380004850d0001f09110");
      break;

    case ENB_STATUS_TRANSFER:
      EncodeENBStatusTransfer(&builder, 0, 1, 5, 231, 0, 51, 0);
      break;

    case MME_STATUS_TRANSFER:
      EncodeMMEStatusTransfer(&builder, 0, 1, 5, 231, 0, 51, 0);
      break;

    case HANDOVER_NOTIFY:
      EncodeHandoverNotify(&builder, 0, 1, "55f531", "0x00000201", "55f531",
                           "45678");
      break;

    case UE_CONTEXT_RELEASE_COMMAND:
      EncodeUEContextReleaseCommand(&builder, 0, 1);
      break;

    case UE_CONTEXT_RELEASE_COMPLETE:
      EncodeUEContextReleaseComplete(&builder, 0, 1);
      break;

    case UE_CONTEXT_RELEASE_REQUEST:
      EncodeUEContextReleaseRequest(&builder, 0, 1);
      break;
  }

  buffer = flatcc_builder_finalize_buffer(&builder, &len);
  rte_memcpy(buf, buffer, len);
  free(buffer);
  flatcc_builder_clear(&builder);
  return len;
}

States fb_handovr_id(struct AttachDecodedData* decod) {
  if (decod->epsMobilityMessageType == 0 && decod->procedureCode == 0 &&
      decod->protocolDiscriminator == 16)
    return HANDOVER_REQUIRED;
  else if (decod->epsMobilityMessageType == 0 && decod->procedureCode == 1 &&
           decod->protocolDiscriminator == 33)
    return HANDOVER_REQUEST;
  else if (decod->epsMobilityMessageType == 1 && decod->procedureCode == 1 &&
           decod->protocolDiscriminator == -1)
    return HANDOVER_REQUEST_ACK;
  else if (decod->epsMobilityMessageType == 1 && decod->procedureCode == 0 &&
           decod->protocolDiscriminator == -1)
    return HANDOVER_COMMAND;
  else if (decod->epsMobilityMessageType == 0 && decod->procedureCode == 24 &&
           decod->protocolDiscriminator == -1)
    return ENB_STATUS_TRANSFER;
  else if (decod->epsMobilityMessageType == 0 && decod->procedureCode == 25 &&
           decod->protocolDiscriminator == -1)
    return MME_STATUS_TRANSFER;
  else if (decod->epsMobilityMessageType == 0 && decod->procedureCode == 2 &&
           decod->protocolDiscriminator == -1)
    return HANDOVER_NOTIFY;
  else if (decod->epsMobilityMessageType == 0 && decod->procedureCode == 23 &&
           decod->protocolDiscriminator == 2)
    return UE_CONTEXT_RELEASE_COMMAND;
  else if (decod->epsMobilityMessageType == 1 && decod->procedureCode == 23 &&
           decod->protocolDiscriminator == -1)
    return UE_CONTEXT_RELEASE_COMPLETE;
  else if (decod->epsMobilityMessageType == 0 && decod->procedureCode == 18 &&
           decod->protocolDiscriminator == 0)
    return UE_CONTEXT_RELEASE_REQUEST;
  else if (decod->epsMobilityMessageType == 0 && decod->procedureCode == 23 &&
           decod->protocolDiscriminator == 0)
    return UE_CONTEXT_RELEASE_COMMAND;

  return 0;
}

uint16_t fb_srvc_req_decode(uint8_t* buf, size_t len, States* nxt) {
  uint8_t msgType = DecoderFull(buf);
  int index = 0;
  switch (msgType) {
    case SR_INITIAL_CONTEXT_SETUP_REQUEST:
      nxt[index++] = SR_INITIAL_CONTEXT_SETUP_RESPONSE;
      break;

    case E_RAB_MODIFICATION_REQUEST:
      nxt[index++] = E_RAB_MODIFICATION_RESPONSE;
      break;

    default:
      printf("ERROR: Tried to handle unexpected message: %i\n", msgType);
      break;
  }
  return index;
}

size_t fb_srvc_req_next_msg(States curr, uint8_t* buf) {
  size_t size = 0;
  flatcc_builder_t builder;
  void* buffer;
  flatcc_builder_init(&builder);

  switch (curr) {
    case SERVICE_REQUEST:
      EncodeInitialUEMessage(&builder, 3,
                             "0741720bf655f531870148e000000102e0e000210201d011d"
                             "1271a80802110010000108106000000008306000000",
                             "55f531", "45678", "55f531", "0x00000201");
      break;

    case SR_INITIAL_CONTEXT_SETUP_RESPONSE:
      EncodeInitialContextSetupResponse(&builder, 0, 1, "0ac80a25", "00000001",
                                        5);
      break;

    case E_RAB_MODIFICATION_RESPONSE:
      EncodeE_RABModifyResponse(&builder, 0, 1);
      break;

    default:
      printf("ERROR: Tried to prepare unexpected message\n");
      break;
  }
  buffer = flatcc_builder_finalize_buffer(&builder, &size);
  rte_memcpy(buf, buffer, size);
  flatcc_builder_clear(&builder);
  free(buffer);
  return size;
}

uint16_t fb_fast_ho_decode(uint8_t* buf, size_t len, States* nxt) {
  struct AttachDecodedData decod = DecoderPartial(buf);
  uint16_t index = 0;
  int stateID = fb_handovr_id(&decod);

  switch (stateID) {
    case HANDOVER_REQUEST:
      nxt[index] = ENB_STATUS_TRANSFER;
      index++;
      break;

    case HANDOVER_COMMAND:
      nxt[index] = DONE;
      index++;
      break;
  }
  return index;
}

size_t fb_fast_ho_next_msg(States curr, uint8_t* buf) {
  size_t size = 0;
  flatcc_builder_t builder;
  void* buffer;
  flatcc_builder_init(&builder);

  switch (curr) {
    case HANDOVER_REQUEST_ACK:
      EncodeHandoverRequestAcknowledge(&builder, 0, 1, 5, "0ac80a26",
                                       "00000001", "0ac80a26", "00000002",
                                       "002301091058215b37b281327630dda8181b020"
                                       "0c003f50004869b380004850d0001f09110");
      break;

    case ENB_STATUS_TRANSFER:
      EncodeENBStatusTransfer(&builder, 0, 1, 5, 231, 0, 51, 0);
      break;
  }
  buffer = flatcc_builder_finalize_buffer(&builder, &size);
  rte_memcpy(buf, buffer, size);
  flatcc_builder_clear(&builder);
  free(buffer);
  return size;
}
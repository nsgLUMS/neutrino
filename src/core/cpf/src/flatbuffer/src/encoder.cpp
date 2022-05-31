#include "encoder_decoder.h"

void EncodeInitialUEMessage(flatcc_builder_t *B, uint32_t ENB_UE_S1AP_ID_input, char* NAS_PDU_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input, char* EUTRAN_CGI_pLMNidentity_input, char* EUTRAN_CGI_cell_ID_input) {
	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_NAS_PDU = 26;
	pie(NAS_PDU_ref_t) NAS_PDU_val = flatbuffers_string_create_str(B, NAS_PDU_input);
	pie(NAS_PDU_ref_t) NAS_PDU = pie(NAS_PDU_create(B, NAS_PDU_val));
	value = pie(Value_as_NAS_PDU(NAS_PDU));
	pie(S1AP_PROTOCOL_IES_ref_t) id_NAS_PDU_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_NAS_PDU, pie(Criticality_reject), value));

	uint16_t id_TAI = 67;
	pie(TAI_ref_t) TAI_pLMNidentity_val = flatbuffers_string_create_str(B, TAI_pLMNidentity_input);
	pie(TAI_ref_t) TAI_TAC_val = flatbuffers_string_create_str(B, TAI_TAC_input);
	pie(TAI_ref_t) TAI = pie(TAI_create(B, TAI_pLMNidentity_val, TAI_TAC_val));
	value = pie(Value_as_TAI(TAI));
	pie(S1AP_PROTOCOL_IES_ref_t) id_TAI_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_TAI, pie(Criticality_reject), value));

	uint16_t id_EUTRAN_CGI = 100;
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI_pLMNidentity_val = flatbuffers_string_create_str(B, EUTRAN_CGI_pLMNidentity_input);
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI_cell_ID_val = flatbuffers_string_create_str(B, EUTRAN_CGI_cell_ID_input);
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI = pie(EUTRAN_CGI_create(B, EUTRAN_CGI_pLMNidentity_val, EUTRAN_CGI_cell_ID_val));
	value = pie(Value_as_EUTRAN_CGI(EUTRAN_CGI));
	pie(S1AP_PROTOCOL_IES_ref_t) id_EUTRAN_CGI_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_EUTRAN_CGI, pie(Criticality_ignore), value));

	uint16_t id_RRC_Establishment_Cause = 134;
	pie(RRC_Establishment_Cause_ref_t) RRC_Establishment_Cause = pie(RRC_Establishment_Cause_create(B, pie(RRC_Establishment_Cause_enum_mo_Signalling)));
	value = pie(Value_as_RRC_Establishment_Cause(RRC_Establishment_Cause));
	pie(S1AP_PROTOCOL_IES_ref_t) id_RRC_Establishment_Cause_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_RRC_Establishment_Cause, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_NAS_PDU_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_TAI_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_EUTRAN_CGI_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_RRC_Establishment_Cause_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) InitialUEMessage = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 12;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, InitialUEMessage, procedure_code, pdu(Criticality_ignore)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));

	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 0));
}

void EncodeDownlinkNASTransport(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* NAS_PDU_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_NAS_PDU = 26;
	pie(NAS_PDU_ref_t) NAS_PDU_val = flatbuffers_string_create_str(B, NAS_PDU_input);
	pie(NAS_PDU_ref_t) NAS_PDU = pie(NAS_PDU_create(B, NAS_PDU_val));
	value = pie(Value_as_NAS_PDU(NAS_PDU));
	pie(S1AP_PROTOCOL_IES_ref_t) id_NAS_PDU_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_NAS_PDU, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_NAS_PDU_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) DownlinkNASTransport = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 11;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, DownlinkNASTransport, procedure_code, pdu(Criticality_ignore)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));

	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 1));
}

void EncodeUplinkNASTransport(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* NAS_PDU_input, char* EUTRAN_CGI_pLMNidentity_input, char* EUTRAN_CGI_cell_ID_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_NAS_PDU = 26;
	pie(NAS_PDU_ref_t) NAS_PDU_val = flatbuffers_string_create_str(B, NAS_PDU_input);
	pie(NAS_PDU_ref_t) NAS_PDU = pie(NAS_PDU_create(B, NAS_PDU_val));
	value = pie(Value_as_NAS_PDU(NAS_PDU));
	pie(S1AP_PROTOCOL_IES_ref_t) id_NAS_PDU_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_NAS_PDU, pie(Criticality_reject), value));

	uint16_t id_EUTRAN_CGI = 100;
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI_pLMNidentity_val = flatbuffers_string_create_str(B, EUTRAN_CGI_pLMNidentity_input);
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI_cell_ID_val = flatbuffers_string_create_str(B, EUTRAN_CGI_cell_ID_input);
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI = pie(EUTRAN_CGI_create(B, EUTRAN_CGI_pLMNidentity_val, EUTRAN_CGI_cell_ID_val));
	value = pie(Value_as_EUTRAN_CGI(EUTRAN_CGI));
	pie(S1AP_PROTOCOL_IES_ref_t) id_EUTRAN_CGI_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_EUTRAN_CGI, pie(Criticality_ignore), value));

	uint16_t id_TAI = 67;
	pie(TAI_ref_t) TAI_pLMNidentity_val = flatbuffers_string_create_str(B, TAI_pLMNidentity_input);
	pie(TAI_ref_t) TAI_TAC_val = flatbuffers_string_create_str(B, TAI_TAC_input);
	pie(TAI_ref_t) TAI = pie(TAI_create(B, TAI_pLMNidentity_val, TAI_TAC_val));
	value = pie(Value_as_TAI(TAI));
	pie(S1AP_PROTOCOL_IES_ref_t) id_TAI_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_TAI, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_NAS_PDU_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_EUTRAN_CGI_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_TAI_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) UplinkNASTransport = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 13;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, UplinkNASTransport, procedure_code, pdu(Criticality_ignore)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));

	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 2));
}

void EncodeInitialContextSetupRequest(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint64_t uEaggregateMaximumBitRateDL_input, uint64_t uEaggregateMaximumBitRateUL_input, uint32_t e_RAB_ID_input, char* TransportLayerAddress_input, char*  GTP_TEID_input, char* nAS_PDU_input, uint8_t qCI_input, uint8_t priorityLevel_input, char* encryptionAlgorithms_input, char* integrityProtectionAlgorithms_input, char* SecurityKey_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_uEaggregateMaximumBitrate = 66;
	pie(UEAggregateMaximumBitrate_ref_t) UEAggregateMaximumBitrate = pie(UEAggregateMaximumBitrate_create(B, uEaggregateMaximumBitRateDL_input, uEaggregateMaximumBitRateUL_input));
	value = pie(Value_as_UEAggregateMaximumBitrate(UEAggregateMaximumBitrate));
	pie(S1AP_PROTOCOL_IES_ref_t) id_UEAggregateMaximumBitrate_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_uEaggregateMaximumBitrate, pie(Criticality_reject), value));

	pie(E_RABLevelQoSParameters_t) E_RABLevelQoSParameters = { qCI_input, priorityLevel_input, pie(Pre_emptionCapability_may_trigger_pre_emption), pie(Pre_emptionVulnerability_pre_emptable) };
	pie(E_RABToBeSetupItemCtxtSUReq_ref_t) TransportLayerAddress_val = flatbuffers_string_create_str(B, TransportLayerAddress_input);
	pie(E_RABToBeSetupItemCtxtSUReq_ref_t) GTP_TEID_val = flatbuffers_string_create_str(B, GTP_TEID_input);
	pie(E_RABToBeSetupItemCtxtSUReq_ref_t) nAS_PDU_val = flatbuffers_string_create_str(B, nAS_PDU_input);
	uint16_t id_E_RABToBeSetupItemCtxtSUReq = 52;
	pie(E_RABToBeSetupItemCtxtSUReq_ref_t) E_RABToBeSetupItemCtxtSUReq = pie(E_RABToBeSetupItemCtxtSUReq_create(B, e_RAB_ID_input, &E_RABLevelQoSParameters, TransportLayerAddress_val, GTP_TEID_val, nAS_PDU_val));
	pie(Value_union_ref_t) inner_value = pie(Value_as_E_RABToBeSetupItemCtxtSUReq(E_RABToBeSetupItemCtxtSUReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeSetupItemCtxtSUReq_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeSetupItemCtxtSUReq, pie(Criticality_reject), inner_value));
	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeSetupItemCtxtSUReq_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(E_RABToBeSetupListCtxtSUReq_ref_t) E_RABToBeSetupListCtxtSUReq = pie(E_RABToBeSetupListCtxtSUReq_create(B, inner_protocolIEs));
	uint16_t id_E_RABToBeSetupListCtxtSUReq = 24;
	value = pie(Value_as_E_RABToBeSetupListCtxtSUReq(E_RABToBeSetupListCtxtSUReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeSetupListCtxtSUReq_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeSetupListCtxtSUReq, pie(Criticality_reject), value));

	uint16_t id_UESecurityCapabilities = 107;
	pie(UESecurityCapabilities_ref_t) encryptionAlgorithms_val = flatbuffers_string_create_str(B, encryptionAlgorithms_input);
	pie(UESecurityCapabilities_ref_t) integrityProtectionAlgorithms_val = flatbuffers_string_create_str(B, integrityProtectionAlgorithms_input);
	pie(UESecurityCapabilities_ref_t) UESecurityCapabilities = pie(UESecurityCapabilities_create(B, encryptionAlgorithms_val, integrityProtectionAlgorithms_val));
	value = pie(Value_as_UESecurityCapabilities(UESecurityCapabilities));
	pie(S1AP_PROTOCOL_IES_ref_t) id_UESecurityCapabilities_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_UESecurityCapabilities, pie(Criticality_reject), value));

	uint16_t id_SecurityKey = 73;
	pie(SecurityKey_ref_t) SecurityKey_val = flatbuffers_string_create_str(B, SecurityKey_input);
	pie(SecurityKey_ref_t) SecurityKey = pie(SecurityKey_create(B, SecurityKey_val));
	value = pie(Value_as_SecurityKey(SecurityKey));
	pie(S1AP_PROTOCOL_IES_ref_t) id_SecurityKey_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_SecurityKey, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_UEAggregateMaximumBitrate_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeSetupListCtxtSUReq_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_UESecurityCapabilities_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_SecurityKey_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) InitialContextSetupRequest = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 9;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, InitialContextSetupRequest, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 3));
}

void EncodeInitialContextSetupResponse(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* TransportLayerAddress_input, char* GTP_TEID_input, uint32_t e_RAB_ID_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_ignore), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_ignore), value));

	pie(E_RABSetupItemCtxtSURes_ref_t) TransportLayerAddress_val = flatbuffers_string_create_str(B, TransportLayerAddress_input);
	pie(E_RABSetupItemCtxtSURes_ref_t) GTP_TEID_val = flatbuffers_string_create_str(B, GTP_TEID_input);
	uint16_t id_E_RABSetupItemCtxtSURes = 50;
	pie(E_RABSetupItemCtxtSURes_ref_t) E_RABSetupItemCtxtSURes = pie(E_RABSetupItemCtxtSURes_create(B, e_RAB_ID_input, TransportLayerAddress_val, GTP_TEID_val));
	pie(Value_union_ref_t) inner_value = pie(Value_as_E_RABSetupItemCtxtSURes(E_RABSetupItemCtxtSURes));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABSetupItemCtxtSURes_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABSetupItemCtxtSURes, pie(Criticality_ignore), inner_value));
	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABSetupItemCtxtSURes_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(E_RABSetupListCtxtSURes_ref_t) E_RABSetupListCtxtSURes = pie(E_RABSetupListCtxtSURes_create(B, inner_protocolIEs));
	uint16_t id_E_RABSetupListCtxtSURes = 51;
	value = pie(Value_as_E_RABSetupListCtxtSURes(E_RABSetupListCtxtSURes));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABSetupListCtxtSURes_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABSetupListCtxtSURes, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABSetupListCtxtSURes_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) InitialContextSetupResponse = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 9;
	pdu(SuccessfulOutcome_ref_t) SuccessfulOutcome = pdu(SuccessfulOutcome_create(B, InitialContextSetupResponse, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) SuccessfulOutcome_in_union = pdu(S1AP_PDU_as_SuccessfulOutcome(SuccessfulOutcome));
	pdu(S1Message_create_as_root(B, SuccessfulOutcome_in_union, (uint8_t) 4));
}

void EncodeUECapabilityInfoIndication(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* UERadioCapability_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_UERadioCapability = 74;
	pie(UERadioCapability_ref_t) UERadioCapability_val = flatbuffers_string_create_str(B, UERadioCapability_input);
	pie(UERadioCapability_ref_t) UERadioCapability = pie(UERadioCapability_create(B, UERadioCapability_val));
	value = pie(Value_as_UERadioCapability(UERadioCapability));
	pie(S1AP_PROTOCOL_IES_ref_t) id_UERadioCapability_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_UERadioCapability, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_UERadioCapability_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) UECapabilityInfoIndication = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 22;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, UECapabilityInfoIndication, procedure_code, pdu(Criticality_ignore)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 5));
}

// Enum value initializing needs revisit - Usman 
void EncodeHandoverRequired(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* pLMNidentity_input, char* ENB_ID_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input, char* Source_ToTarget_TransparentContainer_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_HandoverType = 8;
	pie(HandoverType_ref_t) HandoverType = pie(HandoverType_create(B, pie(HandoverType_enum_intralte)));
	value = pie(Value_as_HandoverType(HandoverType));
	pie(S1AP_PROTOCOL_IES_ref_t) id_HandoverType_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_HandoverType, pie(Criticality_reject), value));

	uint16_t id_Cause = 2;
	pie(CauseRadioNetwork_ref_t) radioNetwork = pie(CauseRadioNetwork_create(B, pie(CauseRadioNetwork_enum_handover_desirable_for_radio_reason)));
	pie(Cause_ref_t) Cause = pie(Cause_create(B, pie(Cause_union_as_CauseRadioNetwork(radioNetwork))));
	value = pie(Value_as_Cause(Cause));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Cause_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Cause, pie(Criticality_ignore), value));

	uint16_t id_TargetID = 4;
	pie(Global_ENB_ID_ref_t) pLMNidentity_val = flatbuffers_string_create_str(B, pLMNidentity_input);
	pie(Global_ENB_ID_ref_t) ENB_ID_val = flatbuffers_string_create_str(B, ENB_ID_input);
	pie(Global_ENB_ID_ref_t) global_ENB_ID = pie(Global_ENB_ID_create(B, pLMNidentity_val, ENB_ID_val));
	pie(TAI_ref_t) TAI_pLMNidentity_val = flatbuffers_string_create_str(B, TAI_pLMNidentity_input);
	pie(TAI_ref_t) TAI_TAC_val = flatbuffers_string_create_str(B, TAI_TAC_input);
	pie(TAI_ref_t) selected_TAI = pie(TAI_create(B, TAI_pLMNidentity_val, TAI_TAC_val));
	pie(TargeteNB_ID_ref_t) targeteNB_ID = pie(TargeteNB_ID_create(B, global_ENB_ID, selected_TAI));
	pie(TargetID_ref_t) TargetID = pie(TargetID_create(B, pie(TargetID_union_as_TargeteNB_ID(targeteNB_ID))));
	value = pie(Value_as_TargetID(TargetID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_TargetID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_TargetID, pie(Criticality_reject), value));

	uint16_t id_Source_ToTarget_TransparentContainer = 104;
	pie(Source_ToTarget_TransparentContainer_ref_t) Source_ToTarget_TransparentContainer_val = flatbuffers_string_create_str(B, Source_ToTarget_TransparentContainer_input);
	pie(Source_ToTarget_TransparentContainer_ref_t) Source_ToTarget_TransparentContainer = pie(Source_ToTarget_TransparentContainer_create(B, Source_ToTarget_TransparentContainer_val));
	value = pie(Value_as_Source_ToTarget_TransparentContainer(Source_ToTarget_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Source_ToTarget_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Source_ToTarget_TransparentContainer, pie(Criticality_reject), value));


	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_HandoverType_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Cause_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_TargetID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Source_ToTarget_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverRequired = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 0;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, HandoverRequired, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 6));
}

void EncodeHandoverRequest(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint64_t uEaggregateMaximumBitRateDL_input, uint64_t uEaggregateMaximumBitRateUL_input, uint8_t qCI_input, uint8_t priorityLevel_input, char* TransportLayerAddress_input, char* GTP_TEID_input, uint32_t e_RAB_ID_input, char* Source_ToTarget_TransparentContainer_input, char* encryptionAlgorithms_input, char* integrityProtectionAlgorithms_input, char* nextHopParameter_input, uint8_t nextHopChainingCount_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_HandoverType = 1;
	pie(HandoverType_ref_t) HandoverType = pie(HandoverType_create(B, pie(HandoverType_enum_intralte)));
	value = pie(Value_as_HandoverType(HandoverType));
	pie(S1AP_PROTOCOL_IES_ref_t) id_HandoverType_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_HandoverType, pie(Criticality_reject), value));

	uint16_t id_Cause = 2;
	pie(CauseRadioNetwork_ref_t) radioNetwork = pie(CauseRadioNetwork_create(B, pie(CauseRadioNetwork_enum_s1_intra_system_handover_triggered)));
	pie(Cause_ref_t) Cause = pie(Cause_create(B, pie(Cause_union_as_CauseRadioNetwork(radioNetwork))));
	value = pie(Value_as_Cause(Cause));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Cause_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Cause, pie(Criticality_ignore), value));

	uint16_t id_uEaggregateMaximumBitrate = 66;
	pie(UEAggregateMaximumBitrate_ref_t) UEAggregateMaximumBitrate = pie(UEAggregateMaximumBitrate_create(B, uEaggregateMaximumBitRateDL_input, uEaggregateMaximumBitRateUL_input));
	value = pie(Value_as_UEAggregateMaximumBitrate(UEAggregateMaximumBitrate));
	pie(S1AP_PROTOCOL_IES_ref_t) id_UEAggregateMaximumBitrate_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_uEaggregateMaximumBitrate, pie(Criticality_reject), value));

	pie(E_RABLevelQoSParameters_t) E_RABLevelQoSParameters = { qCI_input, priorityLevel_input, pie(Pre_emptionCapability_may_trigger_pre_emption), pie(Pre_emptionVulnerability_pre_emptable) };
	pie(E_RABToBeSetupItemHOReq_ref_t) TransportLayerAddress_val = flatbuffers_string_create_str(B, TransportLayerAddress_input);
	pie(E_RABToBeSetupItemHOReq_ref_t) GTP_TEID_val = flatbuffers_string_create_str(B, GTP_TEID_input);
	uint16_t id_E_RABToBeSetupItemHOReq = 27;
	pie(E_RABToBeSetupItemHOReq_ref_t) E_RABToBeSetupItemHOReq = pie(E_RABToBeSetupItemHOReq_create(B, e_RAB_ID_input, TransportLayerAddress_val, GTP_TEID_val, &E_RABLevelQoSParameters));
	pie(Value_union_ref_t) inner_value = pie(Value_as_E_RABToBeSetupItemHOReq(E_RABToBeSetupItemHOReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeSetupItemHOReq_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeSetupItemHOReq, pie(Criticality_reject), inner_value));
	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeSetupItemHOReq_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(E_RABToBeSetupListHOReq_ref_t) E_RABToBeSetupListHOReq = pie(E_RABToBeSetupListHOReq_create(B, inner_protocolIEs));
	uint16_t id_E_RABToBeSetupListHOReq = 53;
	value = pie(Value_as_E_RABToBeSetupListHOReq(E_RABToBeSetupListHOReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeSetupListHOReq_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeSetupListHOReq, pie(Criticality_reject), value));

	uint16_t id_Source_ToTarget_TransparentContainer = 104;
	pie(Source_ToTarget_TransparentContainer_ref_t) Source_ToTarget_TransparentContainer_val = flatbuffers_string_create_str(B, Source_ToTarget_TransparentContainer_input);
	pie(Source_ToTarget_TransparentContainer_ref_t) Source_ToTarget_TransparentContainer = pie(Source_ToTarget_TransparentContainer_create(B, Source_ToTarget_TransparentContainer_val));
	value = pie(Value_as_Source_ToTarget_TransparentContainer(Source_ToTarget_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Source_ToTarget_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Source_ToTarget_TransparentContainer, pie(Criticality_reject), value));

	uint16_t id_UESecurityCapabilities = 107;
	pie(UESecurityCapabilities_ref_t) encryptionAlgorithms_val = flatbuffers_string_create_str(B, encryptionAlgorithms_input);
	pie(UESecurityCapabilities_ref_t) integrityProtectionAlgorithms_val = flatbuffers_string_create_str(B, integrityProtectionAlgorithms_input);
	pie(UESecurityCapabilities_ref_t) UESecurityCapabilities = pie(UESecurityCapabilities_create(B, encryptionAlgorithms_val, integrityProtectionAlgorithms_val));
	value = pie(Value_as_UESecurityCapabilities(UESecurityCapabilities));
	pie(S1AP_PROTOCOL_IES_ref_t) id_UESecurityCapabilities_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_UESecurityCapabilities, pie(Criticality_reject), value));

	uint16_t id_SecurityContext = 40;
	pie(SecurityContext_ref_t) nextHopParameter_val = flatbuffers_string_create_str(B, nextHopParameter_input);
	pie(SecurityContext_ref_t) SecurityContext = pie(SecurityContext_create(B, nextHopChainingCount_input, nextHopParameter_val));
	value = pie(Value_as_SecurityContext(SecurityContext));
	pie(S1AP_PROTOCOL_IES_ref_t) id_SecurityContext_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_SecurityContext, pie(Criticality_reject), value));


	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_HandoverType_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Cause_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_UEAggregateMaximumBitrate_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeSetupListHOReq_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Source_ToTarget_TransparentContainer_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_UESecurityCapabilities_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_SecurityContext_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverRequest = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 1;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, HandoverRequest, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 7));
}

void EncodeHandoverRequestAcknowledge(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input, char* TransportLayerAddress_input, char* gTP_TEID_input, char* dL_transportLayerAddress_input, char* dL_gTP_TEID_input, char* Target_ToSource_TransparentContainer_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_ignore), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_ignore), value));

	pie(E_RABAdmittedItem_ref_t) TransportLayerAddress_val = flatbuffers_string_create_str(B, TransportLayerAddress_input);
	pie(E_RABAdmittedItem_ref_t) gTP_TEID_val = flatbuffers_string_create_str(B, gTP_TEID_input);
	pie(E_RABAdmittedItem_ref_t) dL_transportLayerAddress_val = flatbuffers_string_create_str(B, dL_transportLayerAddress_input);
	pie(E_RABAdmittedItem_ref_t) dL_gTP_TEID_val = flatbuffers_string_create_str(B, dL_gTP_TEID_input);
	pie(E_RABAdmittedItem_ref_t) E_RABAdmittedItem = pie(E_RABAdmittedItem_create(B, e_RAB_ID_input, TransportLayerAddress_val, gTP_TEID_val, dL_transportLayerAddress_val, dL_gTP_TEID_val));
	uint16_t id_E_RABAdmittedItem = 20;
	pie(Value_union_ref_t) inner_value = pie(Value_as_E_RABAdmittedItem(E_RABAdmittedItem));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABAdmittedItem_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABAdmittedItem, pie(Criticality_ignore), inner_value));
	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABAdmittedItem_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(E_RABAdmittedList_ref_t) E_RABAdmittedList = pie(E_RABAdmittedList_create(B, inner_protocolIEs));
	uint16_t id_E_RABAdmittedList = 18;
	value = pie(Value_as_E_RABAdmittedList(E_RABAdmittedList));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABAdmittedList_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABAdmittedList, pie(Criticality_ignore), value));

	uint16_t id_Target_ToSource_TransparentContainer = 123;
	pie(Target_ToSource_TransparentContainer_ref_t) Target_ToSource_TransparentContainer_val = flatbuffers_string_create_str(B, Target_ToSource_TransparentContainer_input);
	pie(Target_ToSource_TransparentContainer_ref_t) Target_ToSource_TransparentContainer = pie(Target_ToSource_TransparentContainer_create(B, Target_ToSource_TransparentContainer_val));
	value = pie(Value_as_Target_ToSource_TransparentContainer(Target_ToSource_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Target_ToSource_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Target_ToSource_TransparentContainer, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABAdmittedList_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Target_ToSource_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverRequestAcknowledge = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 1;
	pdu(SuccessfulOutcome_ref_t) SuccessfulOutcome = pdu(SuccessfulOutcome_create(B, HandoverRequestAcknowledge, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) SuccessfulOutcome_in_union = pdu(S1AP_PDU_as_SuccessfulOutcome(SuccessfulOutcome));
	pdu(S1Message_create_as_root(B, SuccessfulOutcome_in_union, (uint8_t) 8));
}

void EncodeHandoverCommand(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input, char* dL_transportLayerAddress_input, char* dL_gTP_TEID_input, char* Target_ToSource_TransparentContainer_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_HandoverType = 8;
	pie(HandoverType_ref_t) HandoverType = pie(HandoverType_create(B, pie(HandoverType_enum_intralte)));
	value = pie(Value_as_HandoverType(HandoverType));
	pie(S1AP_PROTOCOL_IES_ref_t) id_HandoverType_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_HandoverType, pie(Criticality_reject), value));

	pie(E_RABDataForwardingItem_ref_t) dL_transportLayerAddress_val = flatbuffers_string_create_str(B, dL_transportLayerAddress_input);
	pie(E_RABDataForwardingItem_ref_t) dL_gTP_TEID_val = flatbuffers_string_create_str(B, dL_gTP_TEID_input);

	
	pie(E_RABDataForwardingItem_ref_t) E_RABDataForwardingItem = pie(E_RABDataForwardingItem_create(B, e_RAB_ID_input, dL_transportLayerAddress_val, dL_gTP_TEID_val));
	uint16_t id_E_RABDataForwardingItem = 14;
	pie(Value_union_ref_t) inner_value = pie(Value_as_E_RABDataForwardingItem(E_RABDataForwardingItem));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABDataForwardingItem_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABDataForwardingItem, pie(Criticality_ignore), inner_value));
	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABDataForwardingItem_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(E_RABSubjecttoDataForwardingList_ref_t) E_RABSubjecttoDataForwardingList = pie(E_RABSubjecttoDataForwardingList_create(B, inner_protocolIEs));
	uint16_t id_E_RABSubjecttoDataForwardingList = 12;
	value = pie(Value_as_E_RABSubjecttoDataForwardingList(E_RABSubjecttoDataForwardingList));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABSubjecttoDataForwardingList_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABSubjecttoDataForwardingList, pie(Criticality_ignore), value));

	uint16_t id_Target_ToSource_TransparentContainer = 123;
	pie(Target_ToSource_TransparentContainer_ref_t) Target_ToSource_TransparentContainer_val = flatbuffers_string_create_str(B, Target_ToSource_TransparentContainer_input);
	pie(Target_ToSource_TransparentContainer_ref_t) Target_ToSource_TransparentContainer = pie(Target_ToSource_TransparentContainer_create(B, Target_ToSource_TransparentContainer_val));
	value = pie(Value_as_Target_ToSource_TransparentContainer(Target_ToSource_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Target_ToSource_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Target_ToSource_TransparentContainer, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_HandoverType_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABSubjecttoDataForwardingList_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Target_ToSource_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverCommand = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 0;
	pdu(SuccessfulOutcome_ref_t) SuccessfulOutcome = pdu(SuccessfulOutcome_create(B, HandoverCommand, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) SuccessfulOutcome_in_union = pdu(S1AP_PDU_as_SuccessfulOutcome(SuccessfulOutcome));
	pdu(S1Message_create_as_root(B, SuccessfulOutcome_in_union, (uint8_t) 9));
}

void EncodeENBStatusTransfer(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input, uint8_t uL_COUNTvalue_pDCP_SN_input, uint32_t uL_COUNTvalue_hFN_input, uint8_t dL_COUNTvalue_pDCP_SN_input, uint32_t dL_COUNTvalue_hFN_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_Bearers_SubjectToStatusTransfer_Item = 89;
	pie(Bearers_SubjectToStatusTransfer_Item_ref_t) Bearers_SubjectToStatusTransfer_Item = pie(Bearers_SubjectToStatusTransfer_Item_create(B, e_RAB_ID_input, uL_COUNTvalue_pDCP_SN_input, uL_COUNTvalue_hFN_input, dL_COUNTvalue_pDCP_SN_input, dL_COUNTvalue_hFN_input));
	pie(Value_union_ref_t) inner_value = pie(Value_as_Bearers_SubjectToStatusTransfer_Item(Bearers_SubjectToStatusTransfer_Item));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Bearers_SubjectToStatusTransfer_Item_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Bearers_SubjectToStatusTransfer_Item, pie(Criticality_ignore), inner_value));
	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_Bearers_SubjectToStatusTransfer_Item_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(Bearers_SubjectToStatusTransferList_ref_t) Bearers_SubjectToStatusTransferList = pie(Bearers_SubjectToStatusTransferList_create(B, inner_protocolIEs));
	uint16_t id_eNB_StatusTransfer_TransparentContainer = 90;
	
	pie(ENB_StatusTransfer_TransparentContainer_ref_t) ENB_StatusTransfer_TransparentContainer = pie(ENB_StatusTransfer_TransparentContainer_create(B, Bearers_SubjectToStatusTransferList));

	value = pie(Value_as_ENB_StatusTransfer_TransparentContainer(ENB_StatusTransfer_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_eNB_StatusTransfer_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_StatusTransfer_TransparentContainer, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_eNB_StatusTransfer_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) ENBStatusTransfer = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 24;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, ENBStatusTransfer, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 10));
}

void EncodeMMEStatusTransfer(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input, uint8_t uL_COUNTvalue_pDCP_SN_input, uint32_t uL_COUNTvalue_hFN_input, uint8_t dL_COUNTvalue_pDCP_SN_input, uint32_t dL_COUNTvalue_hFN_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_Bearers_SubjectToStatusTransfer_Item = 89;
	pie(Bearers_SubjectToStatusTransfer_Item_ref_t) Bearers_SubjectToStatusTransfer_Item = pie(Bearers_SubjectToStatusTransfer_Item_create(B, e_RAB_ID_input, uL_COUNTvalue_pDCP_SN_input, uL_COUNTvalue_hFN_input, dL_COUNTvalue_pDCP_SN_input, dL_COUNTvalue_hFN_input));
	pie(Value_union_ref_t) inner_value = pie(Value_as_Bearers_SubjectToStatusTransfer_Item(Bearers_SubjectToStatusTransfer_Item));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Bearers_SubjectToStatusTransfer_Item_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Bearers_SubjectToStatusTransfer_Item, pie(Criticality_ignore), inner_value));
	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_Bearers_SubjectToStatusTransfer_Item_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(Bearers_SubjectToStatusTransferList_ref_t) Bearers_SubjectToStatusTransferList = pie(Bearers_SubjectToStatusTransferList_create(B, inner_protocolIEs));
	uint16_t id_eNB_StatusTransfer_TransparentContainer = 90;
	
	pie(ENB_StatusTransfer_TransparentContainer_ref_t) ENB_StatusTransfer_TransparentContainer = pie(ENB_StatusTransfer_TransparentContainer_create(B, Bearers_SubjectToStatusTransferList));

	value = pie(Value_as_ENB_StatusTransfer_TransparentContainer(ENB_StatusTransfer_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_eNB_StatusTransfer_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_StatusTransfer_TransparentContainer, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_eNB_StatusTransfer_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) MMEStatusTransfer = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 25;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, MMEStatusTransfer, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 11));
}

void EncodeHandoverNotify(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* EUTRAN_CGI_pLMNidentity_input, char* EUTRAN_CGI_cell_ID_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_EUTRAN_CGI = 100;
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI_pLMNidentity_val = flatbuffers_string_create_str(B, EUTRAN_CGI_pLMNidentity_input);
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI_cell_ID_val = flatbuffers_string_create_str(B, EUTRAN_CGI_cell_ID_input);
	pie(EUTRAN_CGI_ref_t) EUTRAN_CGI = pie(EUTRAN_CGI_create(B, EUTRAN_CGI_pLMNidentity_val, EUTRAN_CGI_cell_ID_val));
	value = pie(Value_as_EUTRAN_CGI(EUTRAN_CGI));
	pie(S1AP_PROTOCOL_IES_ref_t) id_EUTRAN_CGI_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_EUTRAN_CGI, pie(Criticality_ignore), value));

	uint16_t id_TAI = 67;
	pie(TAI_ref_t) TAI_pLMNidentity_val = flatbuffers_string_create_str(B, TAI_pLMNidentity_input);
	pie(TAI_ref_t) TAI_TAC_val = flatbuffers_string_create_str(B, TAI_TAC_input);
	pie(TAI_ref_t) TAI = pie(TAI_create(B, TAI_pLMNidentity_val, TAI_TAC_val));
	value = pie(Value_as_TAI(TAI));
	pie(S1AP_PROTOCOL_IES_ref_t) id_TAI_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_TAI, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_EUTRAN_CGI_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_TAI_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverNotify = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 2;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, HandoverNotify, procedure_code, pdu(Criticality_ignore)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 12));
}

void EncodeUEContextReleaseCommand(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input) {
	uint16_t id_UE_S1AP_IDs = 99;
	pie(UE_S1AP_ID_pair_ref_t) UE_S1AP_ID_pair = pie(UE_S1AP_ID_pair_create(B, MME_UE_S1AP_ID_input, ENB_UE_S1AP_ID_input));
	pie(UE_S1AP_IDs_ref_t) UE_S1AP_IDs = pie(UE_S1AP_IDs_create(B, pie(UE_S1AP_IDs_union_as_UE_S1AP_ID_pair(UE_S1AP_ID_pair))));
	pie(Value_union_ref_t) value = pie(Value_as_UE_S1AP_IDs(UE_S1AP_IDs));
	pie(S1AP_PROTOCOL_IES_ref_t) id_UE_S1AP_IDs_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_UE_S1AP_IDs, pie(Criticality_reject), value));

	uint16_t id_Cause = 2;
	pie(CauseRadioNetwork_ref_t) radioNetwork = pie(CauseRadioNetwork_create(B, pie(CauseRadioNetwork_enum_successful_handover)));
	pie(Cause_ref_t) Cause = pie(Cause_create(B, pie(Cause_union_as_CauseRadioNetwork(radioNetwork))));
	value = pie(Value_as_Cause(Cause));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Cause_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Cause, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_UE_S1AP_IDs_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Cause_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) UEContextReleaseCommand = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 23;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, UEContextReleaseCommand, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 13));
}

void EncodeUEContextReleaseComplete(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_ignore), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) UEContextReleaseComplete = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 23;
	pdu(SuccessfulOutcome_ref_t) SuccessfulOutcome = pdu(SuccessfulOutcome_create(B, UEContextReleaseComplete, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) SuccessfulOutcome_in_union = pdu(S1AP_PDU_as_SuccessfulOutcome(SuccessfulOutcome));
	pdu(S1Message_create_as_root(B, SuccessfulOutcome_in_union, (uint8_t) 14));
}

void EncodeUEContextReleaseRequest(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_Cause = 2;
	pie(CauseTransport_ref_t) transport = pie(CauseTransport_create(B, pie(CauseTransport_enum_transport_resource_unavailable)));
	pie(Cause_ref_t) Cause = pie(Cause_create(B, pie(Cause_union_as_CauseRadioNetwork(transport))));
	value = pie(Value_as_Cause(Cause));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Cause_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Cause, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Cause_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) UEContextReleaseRequest = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 18;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, UEContextReleaseRequest, procedure_code, pdu(Criticality_ignore)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 15));
}

void EncodeHandoverRequiredForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* pLMNidentity_input, char* ENB_ID_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input, char* Source_ToTarget_TransparentContainer_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_HandoverType = 8;
	pie(HandoverType_ref_t) HandoverType = pie(HandoverType_create(B, pie(HandoverType_enum_intralte)));
	value = pie(Value_as_HandoverType(HandoverType));
	pie(S1AP_PROTOCOL_IES_ref_t) id_HandoverType_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_HandoverType, pie(Criticality_reject), value));

	uint16_t id_Cause = 2;
	pie(CauseRadioNetwork_ref_t) radioNetwork = pie(CauseRadioNetwork_create(B, pie(CauseRadioNetwork_enum_s1_intra_system_handover_triggered)));
	pie(Cause_ref_t) Cause = pie(Cause_create(B, pie(Cause_union_as_CauseRadioNetwork(radioNetwork))));
	value = pie(Value_as_Cause(Cause));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Cause_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Cause, pie(Criticality_ignore), value));

	uint16_t id_TargetID = 4;
	pie(Global_ENB_ID_ref_t) pLMNidentity_val = flatbuffers_string_create_str(B, pLMNidentity_input);
	pie(Global_ENB_ID_ref_t) ENB_ID_val = flatbuffers_string_create_str(B, ENB_ID_input);
	pie(Global_ENB_ID_ref_t) global_ENB_ID = pie(Global_ENB_ID_create(B, pLMNidentity_val, ENB_ID_val));
	pie(TAI_ref_t) TAI_pLMNidentity_val = flatbuffers_string_create_str(B, TAI_pLMNidentity_input);
	pie(TAI_ref_t) TAI_TAC_val = flatbuffers_string_create_str(B, TAI_TAC_input);
	pie(TAI_ref_t) selected_TAI = pie(TAI_create(B, TAI_pLMNidentity_val, TAI_TAC_val));
	pie(TargeteNB_ID_ref_t) targeteNB_ID = pie(TargeteNB_ID_create(B, global_ENB_ID, selected_TAI));
	pie(TargetID_ref_t) TargetID = pie(TargetID_create(B, pie(TargetID_union_as_TargeteNB_ID(targeteNB_ID))));
	value = pie(Value_as_TargetID(TargetID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_TargetID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_TargetID, pie(Criticality_reject), value));

	uint16_t id_Source_ToTarget_TransparentContainer = 104;
	pie(Source_ToTarget_TransparentContainer_ref_t) Source_ToTarget_TransparentContainer_val = flatbuffers_string_create_str(B, Source_ToTarget_TransparentContainer_input);
	pie(Source_ToTarget_TransparentContainer_ref_t) Source_ToTarget_TransparentContainer = pie(Source_ToTarget_TransparentContainer_create(B, Source_ToTarget_TransparentContainer_val));
	value = pie(Value_as_Source_ToTarget_TransparentContainer(Source_ToTarget_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Source_ToTarget_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Source_ToTarget_TransparentContainer, pie(Criticality_reject), value));


	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_HandoverType_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Cause_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_TargetID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Source_ToTarget_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverRequired = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 0;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, HandoverRequired, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 6));
}

void EncodeHandoverRequestForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint64_t uEaggregateMaximumBitRateDL_input, uint64_t uEaggregateMaximumBitRateUL_input, uint8_t qCI_input_0, uint8_t priorityLevel_input_0, char* TransportLayerAddress_input_0, char* GTP_TEID_input_0, uint32_t e_RAB_ID_input_0, uint8_t qCI_input_1, uint8_t priorityLevel_input_1, char* TransportLayerAddress_input_1, char* GTP_TEID_input_1, uint32_t e_RAB_ID_input_1, char* Source_ToTarget_TransparentContainer_input, char* encryptionAlgorithms_input, char* integrityProtectionAlgorithms_input, char* nextHopParameter_input, uint8_t nextHopChainingCount_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_HandoverType = 1;
	pie(HandoverType_ref_t) HandoverType = pie(HandoverType_create(B, pie(HandoverType_enum_intralte)));
	value = pie(Value_as_HandoverType(HandoverType));
	pie(S1AP_PROTOCOL_IES_ref_t) id_HandoverType_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_HandoverType, pie(Criticality_reject), value));

	uint16_t id_Cause = 2;
	pie(CauseRadioNetwork_ref_t) radioNetwork = pie(CauseRadioNetwork_create(B, pie(CauseRadioNetwork_enum_s1_intra_system_handover_triggered)));
	pie(Cause_ref_t) Cause = pie(Cause_create(B, pie(Cause_union_as_CauseRadioNetwork(radioNetwork))));
	value = pie(Value_as_Cause(Cause));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Cause_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Cause, pie(Criticality_ignore), value));

	uint16_t id_uEaggregateMaximumBitrate = 66;
	pie(UEAggregateMaximumBitrate_ref_t) UEAggregateMaximumBitrate = pie(UEAggregateMaximumBitrate_create(B, uEaggregateMaximumBitRateDL_input, uEaggregateMaximumBitRateUL_input));
	value = pie(Value_as_UEAggregateMaximumBitrate(UEAggregateMaximumBitrate));
	pie(S1AP_PROTOCOL_IES_ref_t) id_UEAggregateMaximumBitrate_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_uEaggregateMaximumBitrate, pie(Criticality_reject), value));

	pie(E_RABLevelQoSParameters_t) E_RABLevelQoSParameters_0 = { qCI_input_0, priorityLevel_input_0, pie(Pre_emptionCapability_may_trigger_pre_emption), pie(Pre_emptionVulnerability_pre_emptable) };
	pie(E_RABToBeSetupItemHOReq_ref_t) TransportLayerAddress_val = flatbuffers_string_create_str(B, TransportLayerAddress_input_0);
	pie(E_RABToBeSetupItemHOReq_ref_t) GTP_TEID_val = flatbuffers_string_create_str(B, GTP_TEID_input_0);
	uint16_t id_E_RABToBeSetupItemHOReq = 27;
	pie(E_RABToBeSetupItemHOReq_ref_t) E_RABToBeSetupItemHOReq = pie(E_RABToBeSetupItemHOReq_create(B, e_RAB_ID_input_0, TransportLayerAddress_val, GTP_TEID_val, &E_RABLevelQoSParameters_0));
	pie(Value_union_ref_t) inner_value = pie(Value_as_E_RABToBeSetupItemHOReq(E_RABToBeSetupItemHOReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeSetupItemHOReq_protocolIE_0 = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeSetupItemHOReq, pie(Criticality_reject), inner_value));

	pie(E_RABLevelQoSParameters_t) E_RABLevelQoSParameters_1 = { qCI_input_1, priorityLevel_input_1, pie(Pre_emptionCapability_may_trigger_pre_emption), pie(Pre_emptionVulnerability_pre_emptable) };
	TransportLayerAddress_val = flatbuffers_string_create_str(B, TransportLayerAddress_input_1);
	GTP_TEID_val = flatbuffers_string_create_str(B, GTP_TEID_input_1);
	E_RABToBeSetupItemHOReq = pie(E_RABToBeSetupItemHOReq_create(B, e_RAB_ID_input_1, TransportLayerAddress_val, GTP_TEID_val, &E_RABLevelQoSParameters_1));
	inner_value = pie(Value_as_E_RABToBeSetupItemHOReq(E_RABToBeSetupItemHOReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeSetupItemHOReq_protocolIE_1 = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeSetupItemHOReq, pie(Criticality_reject), inner_value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeSetupItemHOReq_protocolIE_0));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeSetupItemHOReq_protocolIE_1));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(E_RABToBeSetupListHOReq_ref_t) E_RABToBeSetupListHOReq = pie(E_RABToBeSetupListHOReq_create(B, inner_protocolIEs));
	uint16_t id_E_RABToBeSetupListHOReq = 53;
	value = pie(Value_as_E_RABToBeSetupListHOReq(E_RABToBeSetupListHOReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeSetupListHOReq_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeSetupListHOReq, pie(Criticality_reject), value));

	uint16_t id_Source_ToTarget_TransparentContainer = 104;
	pie(Source_ToTarget_TransparentContainer_ref_t) Source_ToTarget_TransparentContainer_val = flatbuffers_string_create_str(B, Source_ToTarget_TransparentContainer_input);
	pie(Source_ToTarget_TransparentContainer_ref_t) Source_ToTarget_TransparentContainer = pie(Source_ToTarget_TransparentContainer_create(B, Source_ToTarget_TransparentContainer_val));
	value = pie(Value_as_Source_ToTarget_TransparentContainer(Source_ToTarget_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Source_ToTarget_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Source_ToTarget_TransparentContainer, pie(Criticality_reject), value));

	uint16_t id_UESecurityCapabilities = 107;
	pie(UESecurityCapabilities_ref_t) encryptionAlgorithms_val = flatbuffers_string_create_str(B, encryptionAlgorithms_input);
	pie(UESecurityCapabilities_ref_t) integrityProtectionAlgorithms_val = flatbuffers_string_create_str(B, integrityProtectionAlgorithms_input);
	pie(UESecurityCapabilities_ref_t) UESecurityCapabilities = pie(UESecurityCapabilities_create(B, encryptionAlgorithms_val, integrityProtectionAlgorithms_val));
	value = pie(Value_as_UESecurityCapabilities(UESecurityCapabilities));
	pie(S1AP_PROTOCOL_IES_ref_t) id_UESecurityCapabilities_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_UESecurityCapabilities, pie(Criticality_reject), value));

	uint16_t id_SecurityContext = 40;
	pie(SecurityContext_ref_t) nextHopParameter_val = flatbuffers_string_create_str(B, nextHopParameter_input);
	pie(SecurityContext_ref_t) SecurityContext = pie(SecurityContext_create(B, nextHopChainingCount_input, nextHopParameter_val));
	value = pie(Value_as_SecurityContext(SecurityContext));
	pie(S1AP_PROTOCOL_IES_ref_t) id_SecurityContext_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_SecurityContext, pie(Criticality_reject), value));


	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_HandoverType_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Cause_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_UEAggregateMaximumBitrate_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeSetupListHOReq_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Source_ToTarget_TransparentContainer_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_UESecurityCapabilities_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_SecurityContext_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverRequest = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 1;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, HandoverRequest, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 16));
}

void EncodeHandoverRequestAcknowledgeForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input_0, char* TransportLayerAddress_input_0, char* gTP_TEID_input_0, uint32_t e_RAB_ID_input_1, char* TransportLayerAddress_input_1, char* gTP_TEID_input_1, char* Target_ToSource_TransparentContainer_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_ignore), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_ignore), value));

	LteS1_ProtocolIE_E_RABAdmittedItem_start(B);
	LteS1_ProtocolIE_E_RABAdmittedItem_e_RAB_ID_add(B, e_RAB_ID_input_0);
	LteS1_ProtocolIE_E_RABAdmittedItem_transportLayerAddress_create_str(B, TransportLayerAddress_input_0);
	LteS1_ProtocolIE_E_RABAdmittedItem_gTP_TEID_create_str(B, gTP_TEID_input_0);
	pie(E_RABAdmittedItem_ref_t) E_RABAdmittedItem = LteS1_ProtocolIE_E_RABAdmittedItem_end(B);
	uint16_t id_E_RABAdmittedItem = 20;
	pie(Value_union_ref_t) inner_value = pie(Value_as_E_RABAdmittedItem(E_RABAdmittedItem));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABAdmittedItem_protocolIE_0 = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABAdmittedItem, pie(Criticality_ignore), inner_value));

	LteS1_ProtocolIE_E_RABAdmittedItem_start(B);
	LteS1_ProtocolIE_E_RABAdmittedItem_e_RAB_ID_add(B, e_RAB_ID_input_1);
	LteS1_ProtocolIE_E_RABAdmittedItem_transportLayerAddress_create_str(B, TransportLayerAddress_input_1);
	LteS1_ProtocolIE_E_RABAdmittedItem_gTP_TEID_create_str(B, gTP_TEID_input_1);
	E_RABAdmittedItem = LteS1_ProtocolIE_E_RABAdmittedItem_end(B);
	inner_value = pie(Value_as_E_RABAdmittedItem(E_RABAdmittedItem));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABAdmittedItem_protocolIE_1 = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABAdmittedItem, pie(Criticality_ignore), inner_value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABAdmittedItem_protocolIE_0));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABAdmittedItem_protocolIE_1));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(E_RABAdmittedList_ref_t) E_RABAdmittedList = pie(E_RABAdmittedList_create(B, inner_protocolIEs));
	uint16_t id_E_RABAdmittedList = 18;
	value = pie(Value_as_E_RABAdmittedList(E_RABAdmittedList));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABAdmittedList_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABAdmittedList, pie(Criticality_ignore), value));

	uint16_t id_Target_ToSource_TransparentContainer = 123;
	pie(Target_ToSource_TransparentContainer_ref_t) Target_ToSource_TransparentContainer_val = flatbuffers_string_create_str(B, Target_ToSource_TransparentContainer_input);
	pie(Target_ToSource_TransparentContainer_ref_t) Target_ToSource_TransparentContainer = pie(Target_ToSource_TransparentContainer_create(B, Target_ToSource_TransparentContainer_val));
	value = pie(Value_as_Target_ToSource_TransparentContainer(Target_ToSource_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Target_ToSource_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Target_ToSource_TransparentContainer, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABAdmittedList_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Target_ToSource_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverRequestAcknowledge = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 1;
	pdu(SuccessfulOutcome_ref_t) SuccessfulOutcome = pdu(SuccessfulOutcome_create(B, HandoverRequestAcknowledge, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) SuccessfulOutcome_in_union = pdu(S1AP_PDU_as_SuccessfulOutcome(SuccessfulOutcome));
	pdu(S1Message_create_as_root(B, SuccessfulOutcome_in_union, (uint8_t) 17));
}

void EncodeHandoverCommandForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* Target_ToSource_TransparentContainer_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_HandoverType = 8;
	pie(HandoverType_ref_t) HandoverType = pie(HandoverType_create(B, pie(HandoverType_enum_intralte)));
	value = pie(Value_as_HandoverType(HandoverType));
	pie(S1AP_PROTOCOL_IES_ref_t) id_HandoverType_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_HandoverType, pie(Criticality_reject), value));

	uint16_t id_Target_ToSource_TransparentContainer = 123;
	pie(Target_ToSource_TransparentContainer_ref_t) Target_ToSource_TransparentContainer_val = flatbuffers_string_create_str(B, Target_ToSource_TransparentContainer_input);
	pie(Target_ToSource_TransparentContainer_ref_t) Target_ToSource_TransparentContainer = pie(Target_ToSource_TransparentContainer_create(B, Target_ToSource_TransparentContainer_val));
	value = pie(Value_as_Target_ToSource_TransparentContainer(Target_ToSource_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Target_ToSource_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_Target_ToSource_TransparentContainer, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_HandoverType_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Target_ToSource_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) HandoverCommand = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 0;
	pdu(SuccessfulOutcome_ref_t) SuccessfulOutcome = pdu(SuccessfulOutcome_create(B, HandoverCommand, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) SuccessfulOutcome_in_union = pdu(S1AP_PDU_as_SuccessfulOutcome(SuccessfulOutcome));
	pdu(S1Message_create_as_root(B, SuccessfulOutcome_in_union, (uint8_t) 18));
}

void EncodeENBStatusTransferForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input_0, uint8_t uL_COUNTvalue_pDCP_SN_input_0, uint32_t uL_COUNTvalue_hFN_input_0, uint8_t dL_COUNTvalue_pDCP_SN_input_0, uint32_t dL_COUNTvalue_hFN_input_0, uint32_t e_RAB_ID_input_1, uint8_t uL_COUNTvalue_pDCP_SN_input_1, uint32_t uL_COUNTvalue_hFN_input_1, uint8_t dL_COUNTvalue_pDCP_SN_input_1, uint32_t dL_COUNTvalue_hFN_input_1) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_Bearers_SubjectToStatusTransfer_Item = 89;
	pie(Bearers_SubjectToStatusTransfer_Item_ref_t) Bearers_SubjectToStatusTransfer_Item = pie(Bearers_SubjectToStatusTransfer_Item_create(B, e_RAB_ID_input_0, uL_COUNTvalue_pDCP_SN_input_0, uL_COUNTvalue_hFN_input_0, dL_COUNTvalue_pDCP_SN_input_0, dL_COUNTvalue_hFN_input_0));
	pie(Value_union_ref_t) inner_value = pie(Value_as_Bearers_SubjectToStatusTransfer_Item(Bearers_SubjectToStatusTransfer_Item));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Bearers_SubjectToStatusTransfer_Item_protocolIE_0 = pie(S1AP_PROTOCOL_IES_create(B, id_Bearers_SubjectToStatusTransfer_Item, pie(Criticality_ignore), inner_value));

	Bearers_SubjectToStatusTransfer_Item = pie(Bearers_SubjectToStatusTransfer_Item_create(B, e_RAB_ID_input_1, uL_COUNTvalue_pDCP_SN_input_1, uL_COUNTvalue_hFN_input_1, dL_COUNTvalue_pDCP_SN_input_1, dL_COUNTvalue_hFN_input_1));
	inner_value = pie(Value_as_Bearers_SubjectToStatusTransfer_Item(Bearers_SubjectToStatusTransfer_Item));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Bearers_SubjectToStatusTransfer_Item_protocolIE_1 = pie(S1AP_PROTOCOL_IES_create(B, id_Bearers_SubjectToStatusTransfer_Item, pie(Criticality_ignore), inner_value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_Bearers_SubjectToStatusTransfer_Item_protocolIE_0));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Bearers_SubjectToStatusTransfer_Item_protocolIE_1));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(Bearers_SubjectToStatusTransferList_ref_t) Bearers_SubjectToStatusTransferList = pie(Bearers_SubjectToStatusTransferList_create(B, inner_protocolIEs));
	uint16_t id_eNB_StatusTransfer_TransparentContainer = 90;
	
	pie(ENB_StatusTransfer_TransparentContainer_ref_t) ENB_StatusTransfer_TransparentContainer = pie(ENB_StatusTransfer_TransparentContainer_create(B, Bearers_SubjectToStatusTransferList));

	value = pie(Value_as_ENB_StatusTransfer_TransparentContainer(ENB_StatusTransfer_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_eNB_StatusTransfer_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_StatusTransfer_TransparentContainer, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_eNB_StatusTransfer_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) ENBStatusTransfer = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 0;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, ENBStatusTransfer, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 19));
}

void EncodeMMEStatusTransferForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input_0, uint8_t uL_COUNTvalue_pDCP_SN_input_0, uint32_t uL_COUNTvalue_hFN_input_0, uint8_t dL_COUNTvalue_pDCP_SN_input_0, uint32_t dL_COUNTvalue_hFN_input_0, uint32_t e_RAB_ID_input_1, uint8_t uL_COUNTvalue_pDCP_SN_input_1, uint32_t uL_COUNTvalue_hFN_input_1, uint8_t dL_COUNTvalue_pDCP_SN_input_1, uint32_t dL_COUNTvalue_hFN_input_1) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_Bearers_SubjectToStatusTransfer_Item = 89;
	pie(Bearers_SubjectToStatusTransfer_Item_ref_t) Bearers_SubjectToStatusTransfer_Item = pie(Bearers_SubjectToStatusTransfer_Item_create(B, e_RAB_ID_input_0, uL_COUNTvalue_pDCP_SN_input_0, uL_COUNTvalue_hFN_input_0, dL_COUNTvalue_pDCP_SN_input_0, dL_COUNTvalue_hFN_input_0));
	pie(Value_union_ref_t) inner_value = pie(Value_as_Bearers_SubjectToStatusTransfer_Item(Bearers_SubjectToStatusTransfer_Item));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Bearers_SubjectToStatusTransfer_Item_protocolIE_0 = pie(S1AP_PROTOCOL_IES_create(B, id_Bearers_SubjectToStatusTransfer_Item, pie(Criticality_ignore), inner_value));

	Bearers_SubjectToStatusTransfer_Item = pie(Bearers_SubjectToStatusTransfer_Item_create(B, e_RAB_ID_input_1, uL_COUNTvalue_pDCP_SN_input_1, uL_COUNTvalue_hFN_input_1, dL_COUNTvalue_pDCP_SN_input_1, dL_COUNTvalue_hFN_input_1));
	inner_value = pie(Value_as_Bearers_SubjectToStatusTransfer_Item(Bearers_SubjectToStatusTransfer_Item));
	pie(S1AP_PROTOCOL_IES_ref_t) id_Bearers_SubjectToStatusTransfer_Item_protocolIE_1 = pie(S1AP_PROTOCOL_IES_create(B, id_Bearers_SubjectToStatusTransfer_Item, pie(Criticality_ignore), inner_value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_Bearers_SubjectToStatusTransfer_Item_protocolIE_0));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_Bearers_SubjectToStatusTransfer_Item_protocolIE_1));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(Bearers_SubjectToStatusTransferList_ref_t) Bearers_SubjectToStatusTransferList = pie(Bearers_SubjectToStatusTransferList_create(B, inner_protocolIEs));
	uint16_t id_eNB_StatusTransfer_TransparentContainer = 90;
	
	pie(ENB_StatusTransfer_TransparentContainer_ref_t) ENB_StatusTransfer_TransparentContainer = pie(ENB_StatusTransfer_TransparentContainer_create(B, Bearers_SubjectToStatusTransferList));

	value = pie(Value_as_ENB_StatusTransfer_TransparentContainer(ENB_StatusTransfer_TransparentContainer));
	pie(S1AP_PROTOCOL_IES_ref_t) id_eNB_StatusTransfer_TransparentContainer_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_StatusTransfer_TransparentContainer, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_eNB_StatusTransfer_TransparentContainer_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) MMEStatusTransfer = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 0;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, MMEStatusTransfer, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 20));
}

void EncodeE_RABModifyRequest(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* nAS_PDU_input, uint8_t qCI_input, uint8_t priorityLevel_input, uint32_t e_RAB_ID_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_reject), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_reject), value));

	pie(E_RABLevelQoSParameters_t) E_RABLevelQoSParameters = { qCI_input, priorityLevel_input, pie(Pre_emptionCapability_may_trigger_pre_emption), pie(Pre_emptionVulnerability_pre_emptable) };
	pie(E_RABToBeModifiedItemBearerModReq_ref_t) nAS_PDU_val = flatbuffers_string_create_str(B, nAS_PDU_input);
	uint16_t id_E_RABToBeModifiedItemBearerModReq = 36;
	pie(E_RABToBeModifiedItemBearerModReq_ref_t) E_RABToBeModifiedItemBearerModReq = pie(E_RABToBeModifiedItemBearerModReq_create(B, e_RAB_ID_input, &E_RABLevelQoSParameters, nAS_PDU_val));
	pie(Value_union_ref_t) inner_value = pie(Value_as_E_RABToBeModifiedItemBearerModReq(E_RABToBeModifiedItemBearerModReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeModifiedItemBearerModReq_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeModifiedItemBearerModReq, pie(Criticality_reject), inner_value));
	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeModifiedItemBearerModReq_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) inner_protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	pie(E_RABToBeModifiedListBearerModReq_ref_t) E_RABToBeModifiedListBearerModReq = pie(E_RABToBeModifiedListBearerModReq_create(B, inner_protocolIEs));

	uint16_t id_E_RABToBeModifiedListBearerModReq = 30;
	value = pie(Value_as_E_RABToBeModifiedListBearerModReq(E_RABToBeModifiedListBearerModReq));
	pie(S1AP_PROTOCOL_IES_ref_t) id_E_RABToBeModifiedListBearerModReq_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_E_RABToBeModifiedListBearerModReq, pie(Criticality_reject), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_E_RABToBeModifiedListBearerModReq_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) E_RABModifyRequest = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 6;
	pdu(InitiatingMessage_ref_t) InitiatingMessage = pdu(InitiatingMessage_create(B, E_RABModifyRequest, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) InitiatingMessage_in_union = pdu(S1AP_PDU_as_InitiatingMessage(InitiatingMessage));
	pdu(S1Message_create_as_root(B, InitiatingMessage_in_union, (uint8_t) 21));
}

void EncodeE_RABModifyResponse(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input) {
	uint16_t id_MME_UE_S1AP_ID = 0;
	pie(MME_UE_S1AP_ID_ref_t) MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_create(B, MME_UE_S1AP_ID_input));
	pie(Value_union_ref_t) value = pie(Value_as_MME_UE_S1AP_ID(MME_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_MME_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_MME_UE_S1AP_ID, pie(Criticality_ignore), value));

	uint16_t id_eNB_UE_S1AP_ID = 8;
	pie(ENB_UE_S1AP_ID_ref_t) ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_create(B, ENB_UE_S1AP_ID_input));
	value = pie(Value_as_ENB_UE_S1AP_ID(ENB_UE_S1AP_ID));
	pie(S1AP_PROTOCOL_IES_ref_t) id_ENB_UE_S1AP_ID_protocolIE = pie(S1AP_PROTOCOL_IES_create(B, id_eNB_UE_S1AP_ID, pie(Criticality_ignore), value));

	pie(S1AP_PROTOCOL_IES_vec_start(B));
    pie(S1AP_PROTOCOL_IES_vec_push(B, id_MME_UE_S1AP_ID_protocolIE));
	pie(S1AP_PROTOCOL_IES_vec_push(B, id_ENB_UE_S1AP_ID_protocolIE));
    pie(S1AP_PROTOCOL_IES_vec_ref_t) protocolIEs = pie(S1AP_PROTOCOL_IES_vec_end(B));
	msg(Message_ref_t) E_RABModifyResponse = msg(Message_create(B, protocolIEs));

	uint8_t procedure_code = 6;
	pdu(SuccessfulOutcome_ref_t) SuccessfulOutcome = pdu(SuccessfulOutcome_create(B, E_RABModifyResponse, procedure_code, pdu(Criticality_reject)));
	pdu(S1AP_PDU_union_ref_t) SuccessfulOutcome_in_union = pdu(S1AP_PDU_as_SuccessfulOutcome(SuccessfulOutcome));
	pdu(S1Message_create_as_root(B, SuccessfulOutcome_in_union, (uint8_t) 22));
}
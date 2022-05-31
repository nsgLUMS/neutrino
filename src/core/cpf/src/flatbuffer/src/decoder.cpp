#include "encoder_decoder.h"

/* This function takes as input a FlatBuffer-encoded buffer, partially decodes it. Using the decoded values, it decides which message is encoded in the buffer and hence which decoder function to call. Returns a struct containing the decoded values. For Attach procedure messages, the struct contains: procedureCode, protocolDiscriminator and epsMobilityMessageType and for Handover procedure messages the struct contains: procedureCode, messageType and cause. */
struct AttachDecodedData DecoderPartial(const void* buffer) {
	pdu(S1Message_table_t) S1Message = pdu(S1Message_as_root(buffer));
	uint8_t message_type = pdu(S1Message_message_type(S1Message));
	struct AttachDecodedData decoded;
	switch (message_type) {
		case 0:
			return DecodeInitialUEMessage(S1Message);
		case 1:
			return DecodeDownlinkNASTransport(S1Message);
		case 2:
			return DecodeUplinkNASTransport(S1Message);
		case 3:
			return DecodeInitialContextSetupRequest(S1Message);
		case 4:
			decoded.procedureCode = DecodeInitialContextSetupResponse(S1Message);
			decoded.protocolDiscriminator = -1;
			decoded.epsMobilityMessageType = -1;
			return decoded;
		case 5:
			decoded.procedureCode = DecodeUECapabilityInfoIndication(S1Message);
			decoded.protocolDiscriminator = -1;
			decoded.epsMobilityMessageType = -1;
			return decoded;
		case 6:
			return DecodeHandoverRequired(S1Message);
		case 7:
			return DecodeHandoverRequest(S1Message);
		case 8:
			return DecodeHandoverRequestAcknowledge(S1Message);
		case 9:
			return DecodeHandoverCommand(S1Message);
		case 10:
			return DecodeENBStatusTransfer(S1Message);
		case 11:
			return DecodeMMEStatusTransfer(S1Message);
		case 12:
			return DecodeHandoverNotify(S1Message);
		case 13:
			return DecodeUEContextReleaseCommand(S1Message);
		case 14:
			return DecodeUEContextReleaseComplete(S1Message);
		case 15:
			return DecodeUEContextReleaseRequest(S1Message);
		case 16:
			DecodeHandoverRequestForTAP(S1Message);
			return decoded;
		case 17:
			DecodeHandoverRequestAcknowledgeForTAP(S1Message);
			return decoded;
		case 18:
			DecodeHandoverCommandForTAP(S1Message);
			return decoded;
		case 19:
			DecodeENBStatusTransferForTAP(S1Message);
			return decoded;
		case 20:
			DecodeMMEStatusTransferForTAP(S1Message);
			return decoded;
		default:
			printf("ERROR: Tried to decode unknown message\n");
			break;
	return decoded;
	}
}

/* This function takes as input a FlatBuffer-encoded buffer, partially decodes it. Using the decoded values, it decides which message is encoded in the buffer and hence which decoder function to call. */
uint8_t DecoderFull(const void* buffer) {
	pdu(S1Message_table_t) S1Message = pdu(S1Message_as_root(buffer));
	uint8_t message_type = pdu(S1Message_message_type(S1Message));
	switch (message_type) {
		case 0:
			DecodeInitialUEMessageFully(S1Message);
			break;
		case 1:
			DecodeDownlinkNASTransportFully(S1Message);
			break;
		case 2:
			DecodeUplinkNASTransportFully(S1Message);
			break;
		case 3:
			DecodeInitialContextSetupRequestFully(S1Message);
			break;
		case 4:
			DecodeInitialContextSetupResponseFully(S1Message);
			break;
		case 5:
			DecodeUECapabilityInfoIndicationFully(S1Message);
			break;
		case 6:
			DecodeHandoverRequiredFully(S1Message);
			break;
		case 7:
			DecodeHandoverRequestFully(S1Message);
			break;
		case 8:
			DecodeHandoverRequestAcknowledgeFully(S1Message);
			break;
		case 14:
			DecodeUEContextReleaseCompleteFully(S1Message);
			break;
		case 21:
			DecodeE_RABModifyRequestFully(S1Message);
			break;
		case 22:
			DecodeE_RABModifyResponseFully(S1Message);
			break;
		default:
			printf("ERROR: Tried to decode unknown message\n");
			break;
	}
	return message_type;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded InitialUEMessage, decodes it partially and returns a struct containing the procedureCode, protocolDiscriminator and epsMobilityMessageType. */
struct AttachDecodedData DecodeInitialUEMessage(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) InitialUEMessage = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(InitialUEMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	const char* NAS_PDU = pie(NAS_PDU_NAS_PDU_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));
	struct AttachDecodedData ret;
	ret.procedureCode = procedureCode;
	char temp[3];
	if (NAS_PDU[0] == '0') {
		ret.protocolDiscriminator = (int)(NAS_PDU[1] - '0');
		memcpy(temp, &NAS_PDU[2], 2);
		temp[2] = '\0';
		ret.epsMobilityMessageType = HexToInt(temp);
	} else {
		ret.protocolDiscriminator = (int)(NAS_PDU[13] - '0');
		if (ret.protocolDiscriminator == 7) {
			memcpy(temp, &NAS_PDU[14], 2);
			temp[2] = '\0';
			ret.epsMobilityMessageType = HexToInt(temp);
		} else {
			memcpy(temp, &NAS_PDU[16], 2);
			temp[2] = '\0';
			ret.epsMobilityMessageType = HexToInt(temp);
		}
	}
	return ret;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded InitialUEMessage and decodes it fully. */
void DecodeInitialUEMessageFully(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) InitialUEMessage = pdu(InitiatingMessage_value(InitiatingMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	int criticality = pdu(InitiatingMessage_criticality(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(InitialUEMessage));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_NAS_PDU = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_NAS_PDU = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	const char* NAS_PDU = pie(NAS_PDU_NAS_PDU_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));
	
	uint16_t id_TAI = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_TAI = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	const char* TAI_pLMNidentity = pie(TAI_pLMNidentity(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	const char* TAI_TAC = pie(TAI_TAC(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	
	uint16_t id_EUTRAN_CGI = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	int criticality_EUTRAN_CGI = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	const char* EUTRAN_CGI_pLMNidentity = pie(EUTRAN_CGI_pLMNidentity(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	const char* EUTRAN_CGI_cell_ID = pie(EUTRAN_CGI_cell_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	
	uint16_t id_RRC_Establishment_Cause = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	int criticality_RRC_Establishment_Cause = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	int RRC_Establishment_Cause = pie(RRC_Establishment_Cause_RRC_Establishment_Cause_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded DownlinkNASTransport, decodes it partially and returns a struct containing the procedureCode, protocolDiscriminator and epsMobilityMessageType. */
struct AttachDecodedData DecodeDownlinkNASTransport(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) DownlinkNASTransport = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(DownlinkNASTransport));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	const char* NAS_PDU = pie(NAS_PDU_NAS_PDU_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	struct AttachDecodedData ret;
	ret.procedureCode = procedureCode;
	char temp[3];
	if (NAS_PDU[0] == '0') {
		ret.protocolDiscriminator = (int)(NAS_PDU[1] - '0');
		memcpy(temp, &NAS_PDU[2], 2);
		temp[2] = '\0';
		ret.epsMobilityMessageType = HexToInt(temp);
	} else {
		ret.protocolDiscriminator = (int)(NAS_PDU[13] - '0');
		if (ret.protocolDiscriminator == 7) {
			memcpy(temp, &NAS_PDU[14], 2);
			temp[2] = '\0';
			ret.epsMobilityMessageType = HexToInt(temp);
		} else {
			memcpy(temp, &NAS_PDU[16], 2);
			temp[2] = '\0';
			ret.epsMobilityMessageType = HexToInt(temp);
		}
	}
	return ret;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded DownlinkNASTransport and decodes it fully. */
void DecodeDownlinkNASTransportFully(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) DownlinkNASTransport = pdu(InitiatingMessage_value(InitiatingMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	int criticality = pdu(InitiatingMessage_criticality(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(DownlinkNASTransport));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));

	uint16_t id_NAS_PDU = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_NAS_PDU = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	const char* NAS_PDU = pie(NAS_PDU_NAS_PDU_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded DownlinkNASTransport, decodes it partially and returns a struct containing the procedureCode, protocolDiscriminator and epsMobilityMessageType. */
struct AttachDecodedData DecodeUplinkNASTransport(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) UplinkNASTransport = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(UplinkNASTransport));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	const char* NAS_PDU = pie(NAS_PDU_NAS_PDU_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	struct AttachDecodedData ret;
	ret.procedureCode = procedureCode;
	char temp[3];
	if (NAS_PDU[0] == '0') {
		ret.protocolDiscriminator = (int)(NAS_PDU[1] - '0');
		memcpy(temp, &NAS_PDU[2], 2);
		temp[2] = '\0';
		ret.epsMobilityMessageType = HexToInt(temp);
	} else {
		ret.protocolDiscriminator = (int)(NAS_PDU[13] - '0');
		if (ret.protocolDiscriminator == 7) {
			memcpy(temp, &NAS_PDU[14], 2);
			temp[2] = '\0';
			ret.epsMobilityMessageType = HexToInt(temp);
		} else {
			memcpy(temp, &NAS_PDU[16], 2);
			temp[2] = '\0';
			ret.epsMobilityMessageType = HexToInt(temp);
		}
	}
	return ret;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded UplinkNASTransport and decodes it fully. */
void DecodeUplinkNASTransportFully(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) UplinkNASTransport = pdu(InitiatingMessage_value(InitiatingMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	int criticality = pdu(InitiatingMessage_criticality(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(UplinkNASTransport));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));

	uint16_t id_NAS_PDU = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_NAS_PDU = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	const char* NAS_PDU = pie(NAS_PDU_NAS_PDU_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));

	uint16_t id_EUTRAN_CGI = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	int criticality_EUTRAN_CGI = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	const char* EUTRAN_CGI_pLMNidentity = pie(EUTRAN_CGI_pLMNidentity(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	const char* EUTRAN_CGI_cell_ID = pie(EUTRAN_CGI_cell_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	
	uint16_t id_TAI = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	int criticality_TAI = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	const char* TAI_pLMNidentity = pie(TAI_pLMNidentity(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))));
	const char* TAI_TAC = pie(TAI_TAC(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded InitialContextSetupRequest, decodes it partially and returns a struct containing the procedureCode, protocolDiscriminator and epsMobilityMessageType. */
struct AttachDecodedData DecodeInitialContextSetupRequest(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) InitialContextSetupRequest = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(InitialContextSetupRequest));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABToBeSetupListCtxtSUReq_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	const char* NAS_PDU = pie(E_RABToBeSetupItemCtxtSUReq_nAS_PDU(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	struct AttachDecodedData ret;
	ret.procedureCode = procedureCode;
	char temp[3];
	if (NAS_PDU[0] == '0') {
		ret.protocolDiscriminator = (int)(NAS_PDU[1] - '0');
		memcpy(temp, &NAS_PDU[2], 2);
		temp[2] = '\0';
		ret.epsMobilityMessageType = HexToInt(temp);
	} else {
		ret.protocolDiscriminator = (int)(NAS_PDU[13] - '0');
		if (ret.protocolDiscriminator == 7) {
			memcpy(temp, &NAS_PDU[14], 2);
			temp[2] = '\0';
			ret.epsMobilityMessageType = HexToInt(temp);
		} else {
			memcpy(temp, &NAS_PDU[16], 2);
			temp[2] = '\0';
			ret.epsMobilityMessageType = HexToInt(temp);
		}
	}
	return ret;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded InitialContextSetupRequest and decodes it fully. */
void DecodeInitialContextSetupRequestFully(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) InitialContextSetupRequest = pdu(InitiatingMessage_value(InitiatingMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	int criticality = pdu(InitiatingMessage_criticality(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(InitialContextSetupRequest));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));

	uint16_t id_UEAggregateMaximumBitrate = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_UEAggregateMaximumBitrate = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	uint64_t uEaggregateMaximumBitRateDL = pie(UEAggregateMaximumBitrate_uEaggregateMaximumBitRateDL(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	uint64_t uEaggregateMaximumBitRateUL = pie(UEAggregateMaximumBitrate_uEaggregateMaximumBitRateUL(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));

	uint16_t id_E_RABToBeSetupListCtxtSUReq = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	int criticality_E_RABToBeSetupListCtxtSUReq = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABToBeSetupListCtxtSUReq_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	uint16_t id_E_RABToBeSetupItemCtxtSUReq = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	int criticality_E_RABToBeSetupItemCtxtSUReq = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	uint32_t e_RAB_ID = pie(E_RABToBeSetupItemCtxtSUReq_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* transportLayerAddress = pie(E_RABToBeSetupItemCtxtSUReq_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* gTP_TEID = pie(E_RABToBeSetupItemCtxtSUReq_gTP_TEID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* NAS_PDU = pie(E_RABToBeSetupItemCtxtSUReq_nAS_PDU(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	uint8_t qCI = pie(E_RABLevelQoSParameters_qCI(pie(E_RABToBeSetupItemCtxtSUReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))));
	uint8_t priorityLevel = pie(AllocationAndRetentionPriority_priorityLevel(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeSetupItemCtxtSUReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));
	int pre_emptionCapability = pie(AllocationAndRetentionPriority_pre_emptionCapability(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeSetupItemCtxtSUReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));
	int pre_emptionVulnerability = pie(AllocationAndRetentionPriority_pre_emptionVulnerability(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeSetupItemCtxtSUReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));

	uint16_t id_UESecurityCapabilities = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	int criticality_UESecurityCapabilities = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	const char* encryptionAlgorithms = pie(UESecurityCapabilities_encryptionAlgorithms(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))));
	const char* integrityProtectionAlgorithms = pie(UESecurityCapabilities_integrityProtectionAlgorithms(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))));

	uint16_t id_SecurityKey = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))));
	int criticality_SecurityKey = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))));
	const char* SecurityKey = pie(SecurityKey_SecurityKey_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))))));	
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded InitialContextSetupResponse, decodes it partially and returns a struct containing the procedureCode. */
uint8_t DecodeInitialContextSetupResponse(pdu(S1Message_table_t) S1Message) {
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) InitialContextSetupResponse = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(InitialContextSetupResponse));
	return pdu(SuccessfulOutcome_procedureCode(SuccessfulOutcome));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded InitialContextSetupResponse and decodes it fully. */
void DecodeInitialContextSetupResponseFully(pdu(S1Message_table_t) S1Message) {
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) InitialContextSetupResponse = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	uint8_t procedureCode = pdu(SuccessfulOutcome_procedureCode(SuccessfulOutcome));
	int criticality = pdu(SuccessfulOutcome_criticality(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(InitialContextSetupResponse));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));

	uint16_t id_E_RABSetupListCtxtSURes = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_E_RABSetupListCtxtSURes = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABSetupListCtxtSURes_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	uint16_t id_E_RABSetupItemCtxtSURes = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	int criticality_E_RABSetupItemCtxtSURes = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	uint32_t e_RAB_ID = pie(E_RABSetupItemCtxtSURes_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* transportLayerAddress = pie(E_RABSetupItemCtxtSURes_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* gTP_TEID = pie(E_RABSetupItemCtxtSURes_gTP_TEID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded UECapabilityIndoIndication, decodes it partially and returns a struct containing the procedureCode. */
uint8_t DecodeUECapabilityInfoIndication(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) UECapabilityInfoIndication = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(UECapabilityInfoIndication));
	return pdu(InitiatingMessage_procedureCode(InitiatingMessage));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded UECapabilityInfoIndication and decodes it fully. */
void DecodeUECapabilityInfoIndicationFully(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) UECapabilityInfoIndication = pdu(InitiatingMessage_value(InitiatingMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	int criticality = pdu(InitiatingMessage_criticality(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(UECapabilityInfoIndication));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));

	uint16_t id_UERadioCapability = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_UERadioCapability = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	const char* UERadioCapability = pie(UERadioCapability_UERadioCapability_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverRequired, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeHandoverRequired(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverRequired = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverRequired));
	ret.procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	ret.protocolDiscriminator = pie(CauseRadioNetwork_CauseRadioNetwork_value(pie(Cause_Cause_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))))));
	ret.epsMobilityMessageType = 0;
	return ret;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverRequired and decodes it fully. */
void DecodeHandoverRequiredFully(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverRequired = pdu(InitiatingMessage_value(InitiatingMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	int criticality = pdu(InitiatingMessage_criticality(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverRequired));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));

	uint16_t id_HandoverType = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_HandoverType = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int HandoverType = pie(HandoverType_HandoverType_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));

	uint16_t id_Cause = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	int criticality_Cause = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	int Cause = pie(CauseRadioNetwork_CauseRadioNetwork_value(pie(Cause_Cause_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))))));

	uint16_t id_TargetID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	int criticality_TargetID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	const char* pLMNidentity = pie(Global_ENB_ID_pLMNidentity(pie(TargeteNB_ID_global_ENB_ID(pie(TargetID_TargetID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))))))));
	const char* ENB_ID = pie(Global_ENB_ID_ENB_ID(pie(TargeteNB_ID_global_ENB_ID(pie(TargetID_TargetID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))))))));
	const char* TAI_pLMNidentity = pie(TAI_pLMNidentity(pie(TargeteNB_ID_selected_TAI(pie(TargetID_TargetID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))))))));
	const char* TAI_TAC = pie(TAI_TAC(pie(TargeteNB_ID_selected_TAI(pie(TargetID_TargetID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))))))));

	uint16_t id_Source_ToTarget_TransparentContainer = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))));
	int criticality_Source_ToTarget_TransparentContainer = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))));
	const char* Source_ToTarget_TransparentContainer = pie(Source_ToTarget_TransparentContainer_Source_ToTarget_TransparentContainer_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverRequest, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeHandoverRequest(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverRequest = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverRequest));
	ret.procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	ret.protocolDiscriminator = pie(CauseRadioNetwork_CauseRadioNetwork_value(pie(Cause_Cause_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))))));
	ret.epsMobilityMessageType = 0;
	return ret;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverRequest and decodes it fully. */
void DecodeHandoverRequestFully(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverRequest = pdu(InitiatingMessage_value(InitiatingMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	int criticality = pdu(InitiatingMessage_criticality(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverRequest));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_HandoverType = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_HandoverType = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int HandoverType = pie(HandoverType_HandoverType_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));

	uint16_t id_Cause = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_Cause = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int Cause = pie(CauseRadioNetwork_CauseRadioNetwork_value(pie(Cause_Cause_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))))));

	uint16_t id_UEAggregateMaximumBitrate = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	int criticality_UEAggregateMaximumBitrate = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	uint64_t uEaggregateMaximumBitRateDL = pie(UEAggregateMaximumBitrate_uEaggregateMaximumBitRateDL(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	uint64_t uEaggregateMaximumBitRateUL = pie(UEAggregateMaximumBitrate_uEaggregateMaximumBitRateUL(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));

	uint16_t id_E_RABToBeSetupListHOReq = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	int criticality_E_RABToBeSetupListHOReq = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABToBeSetupListHOReq_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))));
	uint16_t id_E_RABToBeSetupItemHOReq = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	int criticality_E_RABToBeSetupItemHOReq = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	uint32_t e_RAB_ID = pie(E_RABToBeSetupItemHOReq_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* transportLayerAddress = pie(E_RABToBeSetupItemHOReq_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* gTP_TEID = pie(E_RABToBeSetupItemHOReq_gTP_TEID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	uint8_t qCI = pie(E_RABLevelQoSParameters_qCI(pie(E_RABToBeSetupItemHOReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))));
	uint8_t priorityLevel = pie(AllocationAndRetentionPriority_priorityLevel(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeSetupItemHOReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));
	int pre_emptionCapability = pie(AllocationAndRetentionPriority_pre_emptionCapability(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeSetupItemHOReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));
	int pre_emptionVulnerability = pie(AllocationAndRetentionPriority_pre_emptionVulnerability(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeSetupItemHOReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));

	uint16_t id_Source_ToTarget_TransparentContainer = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))));
	int criticality_Source_ToTarget_TransparentContainer = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))));
	const char* Source_ToTarget_TransparentContainer = pie(Source_ToTarget_TransparentContainer_Source_ToTarget_TransparentContainer_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 5))))));

	uint16_t id_UESecurityCapabilities = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 6))));
	int criticality_UESecurityCapabilities = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 6))));
	const char* encryptionAlgorithms = pie(UESecurityCapabilities_encryptionAlgorithms(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 6))))));
	const char* integrityProtectionAlgorithms = pie(UESecurityCapabilities_integrityProtectionAlgorithms(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 6))))));

	uint16_t id_SecurityContext = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 7))));
	int criticality_SecurityContext = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 7))));
	uint8_t nextHopChainingCount = pie(SecurityContext_nextHopChainingCount(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 7))))));
	const char* nextHopParameter = pie(SecurityContext_nextHopParameter(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 7))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverRequestAcknowledge, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeHandoverRequestAcknowledge(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverRequestAcknowledge = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverRequestAcknowledge));
	ret.procedureCode = pdu(SuccessfulOutcome_procedureCode(SuccessfulOutcome));
	ret.protocolDiscriminator = -1;
	ret.epsMobilityMessageType = 1;
	return ret;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverRequestAcknowledge and decodes it fully. */
void DecodeHandoverRequestAcknowledgeFully(pdu(S1Message_table_t) S1Message) {
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverRequestAcknowledge = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	uint8_t procedureCode = pdu(SuccessfulOutcome_procedureCode(SuccessfulOutcome));
	int criticality = pdu(SuccessfulOutcome_criticality(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverRequestAcknowledge));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));

	uint16_t id_E_RABAdmittedList = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_E_RABAdmittedList = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABAdmittedList_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	uint16_t id_E_RABAdmittedItem = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	int criticality_E_RABAdmittedItem = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	uint32_t e_RAB_ID = pie(E_RABAdmittedItem_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* transportLayerAddress = pie(E_RABAdmittedItem_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* gTP_TEID = pie(E_RABAdmittedItem_gTP_TEID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* dL_transportLayerAddress = pie(E_RABAdmittedItem_dL_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* dL_gTP_TEID = pie(E_RABAdmittedItem_dL_gTP_TEID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));

	uint16_t id_Target_ToSource_TransparentContainer = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	int criticality_Target_ToSource_TransparentContainer = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))));
	const char* Target_ToSource_TransparentContainer = pie(Target_ToSource_TransparentContainer_Target_ToSource_TransparentContainer_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverCommand, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeHandoverCommand(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverCommand = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverCommand));
	ret.procedureCode = pdu(SuccessfulOutcome_procedureCode(SuccessfulOutcome));
	ret.protocolDiscriminator = -1;
	ret.epsMobilityMessageType = 1;
	return ret;
	// pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	// msg(Message_table_t) HandoverCommand = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	// pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverCommand));
	// pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABSubjecttoDataForwardingList_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	// const char* dL_transportLayerAddress = pie(E_RABDataForwardingItem_dL_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	// uint16_t id_E_RABDataForwardingItem = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	// const char* Target_ToSource_TransparentContainer = pie(Target_ToSource_TransparentContainer_Target_ToSource_TransparentContainer_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))));
	// printf("%u | %s | %s\n", id_E_RABDataForwardingItem, dL_transportLayerAddress, Target_ToSource_TransparentContainer);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded ENBStatusTransfer, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeENBStatusTransfer(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) ENBStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(ENBStatusTransfer));
	ret.procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	ret.protocolDiscriminator = -1;
	ret.epsMobilityMessageType = 0;
	return ret;
	// pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	// msg(Message_table_t) ENBStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	// pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(ENBStatusTransfer));
	// pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(Bearers_SubjectToStatusTransferList_protocolIEs(pie(ENB_StatusTransfer_TransparentContainer_bearers_SubjectToStatusTransferList_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))))));
	// uint32_t e_RAB_ID = pie(Bearers_SubjectToStatusTransfer_Item_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	// uint8_t pDCP_SN = pie(COUNTvalue_pDCP_SN(pie(Bearers_SubjectToStatusTransfer_Item_uL_COUNTvalue(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))));
	// uint16_t id = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	// printf("%u | %u | %u\n", e_RAB_ID, pDCP_SN, id);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded MMEStatusTransfer, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeMMEStatusTransfer(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) MMEStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(MMEStatusTransfer));
	ret.procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	ret.protocolDiscriminator = -1;
	ret.epsMobilityMessageType = 0;
	return ret;
	// pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	// msg(Message_table_t) MMEStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	// pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(MMEStatusTransfer));
	// pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(Bearers_SubjectToStatusTransferList_protocolIEs(pie(ENB_StatusTransfer_TransparentContainer_bearers_SubjectToStatusTransferList_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))))));
	// uint32_t e_RAB_ID = pie(Bearers_SubjectToStatusTransfer_Item_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	// uint8_t pDCP_SN = pie(COUNTvalue_pDCP_SN(pie(Bearers_SubjectToStatusTransfer_Item_uL_COUNTvalue(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))));
	// uint16_t id = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	// printf("%u | %u | %u\n", e_RAB_ID, pDCP_SN, id);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverNotify, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeHandoverNotify(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) MMEStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(MMEStatusTransfer));
	ret.procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	ret.protocolDiscriminator = -1;
	ret.epsMobilityMessageType = 0;
	return ret;
	// pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	// msg(Message_table_t) MMEStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	// pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(MMEStatusTransfer));
	// uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));
	// const char* EUTRAN_CGI_cell_ID = pie(EUTRAN_CGI_cell_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	// const char* TAI_TAC = pie(TAI_TAC(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));

	// printf("%u | %s | %s\n", ENB_UE_S1AP_ID, EUTRAN_CGI_cell_ID, TAI_TAC);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded UEContextReleaseCommand, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeUEContextReleaseCommand(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) MMEStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(MMEStatusTransfer));
	ret.procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	ret.protocolDiscriminator = pie(CauseNas_CauseNas_value(pie(Cause_Cause_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))))));
	ret.epsMobilityMessageType = 0;
	return ret;
	// pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	// msg(Message_table_t) MMEStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	// pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(MMEStatusTransfer));
	// uint32_t ENB_UE_S1AP_ID = pie(UE_S1AP_ID_pair_ENB_UE_S1AP_ID(pie(UE_S1AP_IDs_UE_S1AP_IDs_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))))));
	// uint32_t MME_UE_S1AP_ID = pie(UE_S1AP_ID_pair_MME_UE_S1AP_ID(pie(UE_S1AP_IDs_UE_S1AP_IDs_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))))));
	// uint16_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	// printf("%u | %u | %u\n", ENB_UE_S1AP_ID, procedureCode, MME_UE_S1AP_ID);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded UEContextReleaseComplete, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeUEContextReleaseComplete(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) UEContextReleaseComplete = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(UEContextReleaseComplete));
	ret.procedureCode = pdu(SuccessfulOutcome_procedureCode(SuccessfulOutcome));
	ret.protocolDiscriminator = -1;
	ret.epsMobilityMessageType = 1;
	return ret;
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded UEContextReleaseComplete and decodes it fully. */
void DecodeUEContextReleaseCompleteFully(pdu(S1Message_table_t) S1Message) {
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) UEContextReleaseComplete = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	uint8_t procedureCode = pdu(SuccessfulOutcome_procedureCode(SuccessfulOutcome));
	int criticality = pdu(SuccessfulOutcome_criticality(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(UEContextReleaseComplete));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded UEContextReleaseRequest, decodes it partially and returns a struct containing the procedureCode, CauseRadioNetwork and PDU Message Type. */
struct AttachDecodedData DecodeUEContextReleaseRequest(pdu(S1Message_table_t) S1Message) {
	struct AttachDecodedData ret;
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) UEContextReleaseRequest = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(UEContextReleaseRequest));
	ret.procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	ret.protocolDiscriminator = pie(CauseTransport_CauseTransport_value(pie(Cause_Cause_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))))));
	ret.epsMobilityMessageType = 0;
	return ret;
	// pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	// msg(Message_table_t) UEContextReleaseRequest = pdu(InitiatingMessage_value(InitiatingMessage));
	// pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(UEContextReleaseRequest));
	// uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));
	// uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));
	// uint32_t Cause = pie(CauseTransport_CauseTransport_value(pie(Cause_Cause_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))))));
	// printf("%u | %u | %u\n", ENB_UE_S1AP_ID, MME_UE_S1AP_ID, Cause);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverRequest for Tracking Area Update, decodes it partially and prints transportLayerAddress0, priorityLevel0, transportLayerAddress1, priorityLevel1, nextHopParameter. */
void DecodeHandoverRequestForTAP(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverRequest = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverRequest));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABToBeSetupListHOReq_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 4))))));
	const char* transportLayerAddress0 = pie(E_RABToBeSetupItemHOReq_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	uint8_t priorityLevel0 = pie(AllocationAndRetentionPriority_priorityLevel(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeSetupItemHOReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));
	const char* nextHopParameter = pie(SecurityContext_nextHopParameter(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 7))))));

	const char* transportLayerAddress1 = pie(E_RABToBeSetupItemHOReq_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 1))))));
	uint8_t priorityLevel1 = pie(AllocationAndRetentionPriority_priorityLevel(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeSetupItemHOReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 1))))))))));
	printf("%s | %u | %s | %u | %s\n", transportLayerAddress0, priorityLevel0, transportLayerAddress1, priorityLevel1, nextHopParameter);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverRequestAcknowledge for Tracking Area Update, decodes it partially and prints id_E_RABAdmittedItem, gTP_TEID_0, transportLayerAddress_0, gTP_TEID_1, transportLayerAddress_1. */
void DecodeHandoverRequestAcknowledgeForTAP(pdu(S1Message_table_t) S1Message) {
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverRequestAcknowledge = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverRequestAcknowledge));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABAdmittedList_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	const char* gTP_TEID_0 = pie(E_RABAdmittedItem_gTP_TEID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* transportLayerAddress_0 = pie(E_RABAdmittedItem_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	const char* gTP_TEID_1 = pie(E_RABAdmittedItem_gTP_TEID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 1))))));
	const char* transportLayerAddress_1 = pie(E_RABAdmittedItem_transportLayerAddress(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 1))))));
	uint16_t id_E_RABAdmittedItem = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	printf("%u | %s | %s | %s | %s\n", id_E_RABAdmittedItem, gTP_TEID_0, transportLayerAddress_0, gTP_TEID_1, transportLayerAddress_1);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded HandoverCommand for Tracking Area Update, decodes it partially and prints MME_UE_S1AP_ID, ENB_UE_S1AP_ID, Target_ToSource_TransparentContainer. */
void DecodeHandoverCommandForTAP(pdu(S1Message_table_t) S1Message) {
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) HandoverCommand = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(HandoverCommand));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));
	const char* Target_ToSource_TransparentContainer = pie(Target_ToSource_TransparentContainer_Target_ToSource_TransparentContainer_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 3))))));
	printf("%u | %u | %s\n", MME_UE_S1AP_ID, ENB_UE_S1AP_ID, Target_ToSource_TransparentContainer);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded ENBStatusTransfer for Tracking Area Update, decodes it partially and prints e_RAB_ID_0, pDCP_SN_0, e_RAB_ID_1, pDCP_SN_1, id. */
void DecodeENBStatusTransferForTAP(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) ENBStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(ENBStatusTransfer));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(Bearers_SubjectToStatusTransferList_protocolIEs(pie(ENB_StatusTransfer_TransparentContainer_bearers_SubjectToStatusTransferList_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))))));
	uint32_t e_RAB_ID_0 = pie(Bearers_SubjectToStatusTransfer_Item_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	uint8_t pDCP_SN_0 = pie(COUNTvalue_pDCP_SN(pie(Bearers_SubjectToStatusTransfer_Item_uL_COUNTvalue(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))));
	uint32_t e_RAB_ID_1 = pie(Bearers_SubjectToStatusTransfer_Item_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 1))))));
	uint8_t pDCP_SN_1 = pie(COUNTvalue_pDCP_SN(pie(Bearers_SubjectToStatusTransfer_Item_uL_COUNTvalue(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 1))))))));
	uint16_t id = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	printf("%u | %u | %u | %u | %u\n", e_RAB_ID_0, pDCP_SN_0, e_RAB_ID_1, pDCP_SN_1, id);
}

/* This function takes as input a pointer to the S1 Message struct containing a Flatbuffer-encoded MMEStatusTransfer for Tracking Area Update, decodes it partially and prints e_RAB_ID_0, pDCP_SN_0, e_RAB_ID_1, pDCP_SN_1, id. */
void DecodeMMEStatusTransferForTAP(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) MMEStatusTransfer = pdu(InitiatingMessage_value(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(MMEStatusTransfer));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(Bearers_SubjectToStatusTransferList_protocolIEs(pie(ENB_StatusTransfer_TransparentContainer_bearers_SubjectToStatusTransferList_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))))));
	uint32_t e_RAB_ID_0 = pie(Bearers_SubjectToStatusTransfer_Item_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	uint8_t pDCP_SN_0 = pie(COUNTvalue_pDCP_SN(pie(Bearers_SubjectToStatusTransfer_Item_uL_COUNTvalue(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))));
	uint16_t id = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	uint32_t e_RAB_ID_1 = pie(Bearers_SubjectToStatusTransfer_Item_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 1))))));
	uint8_t pDCP_SN_1 = pie(COUNTvalue_pDCP_SN(pie(Bearers_SubjectToStatusTransfer_Item_uL_COUNTvalue(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 1))))))));
	printf("%u | %u | %u | %u | %u\n", e_RAB_ID_0, pDCP_SN_0, e_RAB_ID_1, pDCP_SN_1, id);
}

void DecodeE_RABModifyRequestFully(pdu(S1Message_table_t) S1Message) {
	pdu(InitiatingMessage_table_t) InitiatingMessage = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) E_RABModifyRequest = pdu(InitiatingMessage_value(InitiatingMessage));
	uint8_t procedureCode = pdu(InitiatingMessage_procedureCode(InitiatingMessage));
	int criticality = pdu(InitiatingMessage_criticality(InitiatingMessage));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(E_RABModifyRequest));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));
	
	uint16_t id_E_RABToBeModifiedListBearerModReq = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	int criticality_E_RABToBeModifiedListBearerModReq = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))));
	pie(S1AP_PROTOCOL_IES_vec_t) inner_protocolIEs = pie(E_RABToBeModifiedListBearerModReq_protocolIEs(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 2))))));
	uint16_t id_E_RABToBeModifiedItemBearerModReq = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	int criticality_E_RABToBeModifiedItemBearerModReq = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))));
	uint32_t e_RAB_ID = pie(E_RABToBeModifiedItemBearerModReq_e_RAB_ID(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
	uint8_t qCI = pie(E_RABLevelQoSParameters_qCI(pie(E_RABToBeModifiedItemBearerModReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))));
	uint8_t priorityLevel = pie(AllocationAndRetentionPriority_priorityLevel(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeModifiedItemBearerModReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));
	int pre_emptionCapability = pie(AllocationAndRetentionPriority_pre_emptionCapability(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeModifiedItemBearerModReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));
	int pre_emptionVulnerability = pie(AllocationAndRetentionPriority_pre_emptionVulnerability(pie(E_RABLevelQoSParameters_allocationRetentionPriority(pie(E_RABToBeModifiedItemBearerModReq_e_RABlevelQoSParameters(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))))))));
	const char* NAS_PDU = pie(E_RABToBeModifiedItemBearerModReq_nAS_PDU(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(inner_protocolIEs, 0))))));
}

void DecodeE_RABModifyResponseFully(pdu(S1Message_table_t) S1Message) {
	pdu(SuccessfulOutcome_table_t) SuccessfulOutcome = pdu(S1Message_S1AP_PDU_value(S1Message));
	msg(Message_table_t) E_RABModifyResponse = pdu(SuccessfulOutcome_value(SuccessfulOutcome));
	uint8_t procedureCode = pdu(SuccessfulOutcome_procedureCode(SuccessfulOutcome));
	int criticality = pdu(SuccessfulOutcome_criticality(SuccessfulOutcome));
	pie(S1AP_PROTOCOL_IES_vec_t) protocolIEs = msg(Message_protocolIEs(E_RABModifyResponse));

	uint16_t id_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	int criticality_MME_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))));
	uint32_t MME_UE_S1AP_ID = pie(MME_UE_S1AP_ID_MME_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 0))))));

	uint16_t id_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_id(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	int criticality_eNB_UE_S1AP_ID = pie(S1AP_PROTOCOL_IES_criticality(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))));
	uint32_t ENB_UE_S1AP_ID = pie(ENB_UE_S1AP_ID_ENB_UE_S1AP_ID_value(pie(S1AP_PROTOCOL_IES_value(pie(S1AP_PROTOCOL_IES_vec_at(protocolIEs, 1))))));
}
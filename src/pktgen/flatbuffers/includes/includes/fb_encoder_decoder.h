#include "S1AP_PDU_builder.h"
#include "Message_builder.h"
#include "S1AP_Protocol_IEs_builder.h"
#include "stdio.h"

#undef pdu
#undef msg
#undef pie
#define pdu(x) FLATBUFFERS_WRAP_NAMESPACE(LteS1_PDU, x)
#define msg(x) FLATBUFFERS_WRAP_NAMESPACE(LteS1_Message, x)
#define pie(x) FLATBUFFERS_WRAP_NAMESPACE(LteS1_ProtocolIE, x)

struct AttachDecodedData {
	uint8_t procedureCode;
	int protocolDiscriminator;
	int epsMobilityMessageType;
};

struct HandoverDecodedData {
	uint8_t procedureCode;
	int messageType;
	int cause;
};

void EncodeInitialUEMessage(flatcc_builder_t *B, uint32_t ENB_UE_S1AP_ID_input, char* NAS_PDU_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input, char* EUTRAN_CGI_pLMNidentity_input, char* EUTRAN_CGI_cell_ID_input);
void EncodeDownlinkNASTransport(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* NAS_PDU_input);
void EncodeUplinkNASTransport(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* NAS_PDU_input, char* EUTRAN_CGI_pLMNidentity_input, char* EUTRAN_CGI_cell_ID_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input);
void EncodeInitialContextSetupRequest(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint64_t uEaggregateMaximumBitRateDL_input, uint64_t uEaggregateMaximumBitRateUL_input, uint32_t e_RAB_ID_input, char* TransportLayerAddress_input, char*  GTP_TEID_input, char* nAS_PDU_input, uint8_t qCI_input, uint8_t priorityLevel_input, char* encryptionAlgorithms_input, char* integrityProtectionAlgorithms_input, char* SecurityKey_input);
void EncodeInitialContextSetupResponse(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* TransportLayerAddress_input, char* GTP_TEID_input, uint32_t e_RAB_ID_input);
void EncodeUECapabilityInfoIndication(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* UERadioCapability_input);
void EncodeHandoverRequired(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* pLMNidentity_input, char* ENB_ID_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input, char* Source_ToTarget_TransparentContainer_input);
void EncodeHandoverRequest(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint64_t uEaggregateMaximumBitRateDL_input, uint64_t uEaggregateMaximumBitRateUL_input, uint8_t qCI_input, uint8_t priorityLevel_input, char* TransportLayerAddress_input, char* GTP_TEID_input, uint32_t e_RAB_ID_input, char* Source_ToTarget_TransparentContainer_input, char* encryptionAlgorithms_input, char* integrityProtectionAlgorithms_input, char* nextHopParameter_input, uint8_t nextHopChainingCount_input);
void EncodeHandoverRequestAcknowledge(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input, char* TransportLayerAddress_input, char* gTP_TEID_input, char* dL_transportLayerAddress_input, char* dL_gTP_TEID_input, char* Target_ToSource_TransparentContainer_input);
void EncodeHandoverCommand(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input, char* dL_transportLayerAddress_input, char* dL_gTP_TEID_input, char* Target_ToSource_TransparentContainer_input);
void EncodeENBStatusTransfer(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input, uint8_t uL_COUNTvalue_pDCP_SN_input, uint32_t uL_COUNTvalue_hFN_input, uint8_t dL_COUNTvalue_pDCP_SN_input, uint32_t dL_COUNTvalue_hFN_input);
void EncodeMMEStatusTransfer(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input, uint8_t uL_COUNTvalue_pDCP_SN_input, uint32_t uL_COUNTvalue_hFN_input, uint8_t dL_COUNTvalue_pDCP_SN_input, uint32_t dL_COUNTvalue_hFN_input);
void EncodeHandoverNotify(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* EUTRAN_CGI_pLMNidentity_input, char* EUTRAN_CGI_cell_ID_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input);
void EncodeUEContextReleaseCommand(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input);
void EncodeUEContextReleaseComplete(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input);
void EncodeUEContextReleaseRequest(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input);
void EncodeHandoverRequiredForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* pLMNidentity_input, char* ENB_ID_input, char* TAI_pLMNidentity_input, char* TAI_TAC_input, char* Source_ToTarget_TransparentContainer_input);
void EncodeHandoverRequestForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint64_t uEaggregateMaximumBitRateDL_input, uint64_t uEaggregateMaximumBitRateUL_input, uint8_t qCI_input_0, uint8_t priorityLevel_input_0, char* TransportLayerAddress_input_0, char* GTP_TEID_input_0, uint32_t e_RAB_ID_input_0, uint8_t qCI_input_1, uint8_t priorityLevel_input_1, char* TransportLayerAddress_input_1, char* GTP_TEID_input_1, uint32_t e_RAB_ID_input_1, char* Source_ToTarget_TransparentContainer_input, char* encryptionAlgorithms_input, char* integrityProtectionAlgorithms_input, char* nextHopParameter_input, uint8_t nextHopChainingCount_input);
void EncodeHandoverRequestAcknowledgeForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input_0, char* TransportLayerAddress_input_0, char* gTP_TEID_input_0, uint32_t e_RAB_ID_input_1, char* TransportLayerAddress_input_1, char* gTP_TEID_input_1, char* Target_ToSource_TransparentContainer_input);
void EncodeHandoverCommandForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* Target_ToSource_TransparentContainer_input);
void EncodeENBStatusTransferForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input_0, uint8_t uL_COUNTvalue_pDCP_SN_input_0, uint32_t uL_COUNTvalue_hFN_input_0, uint8_t dL_COUNTvalue_pDCP_SN_input_0, uint32_t dL_COUNTvalue_hFN_input_0, uint32_t e_RAB_ID_input_1, uint8_t uL_COUNTvalue_pDCP_SN_input_1, uint32_t uL_COUNTvalue_hFN_input_1, uint8_t dL_COUNTvalue_pDCP_SN_input_1, uint32_t dL_COUNTvalue_hFN_input_1);
void EncodeMMEStatusTransferForTAP(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, uint32_t e_RAB_ID_input_0, uint8_t uL_COUNTvalue_pDCP_SN_input_0, uint32_t uL_COUNTvalue_hFN_input_0, uint8_t dL_COUNTvalue_pDCP_SN_input_0, uint32_t dL_COUNTvalue_hFN_input_0, uint32_t e_RAB_ID_input_1, uint8_t uL_COUNTvalue_pDCP_SN_input_1, uint32_t uL_COUNTvalue_hFN_input_1, uint8_t dL_COUNTvalue_pDCP_SN_input_1, uint32_t dL_COUNTvalue_hFN_input_1);
void EncodeE_RABModifyRequest(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input, char* nAS_PDU_input, uint8_t qCI_input, uint8_t priorityLevel_input, uint32_t e_RAB_ID_input);
void EncodeE_RABModifyResponse(flatcc_builder_t *B, uint32_t MME_UE_S1AP_ID_input, uint32_t ENB_UE_S1AP_ID_input);


struct AttachDecodedData DecoderPartial(const void* buffer);
struct AttachDecodedData DecodeInitialUEMessage(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeDownlinkNASTransport(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeUplinkNASTransport(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeInitialContextSetupRequest(pdu(S1Message_table_t) S1Message);
uint8_t DecodeInitialContextSetupResponse(pdu(S1Message_table_t) S1Message);
uint8_t DecodeUECapabilityInfoIndication(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeHandoverRequired(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeHandoverRequest(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeHandoverRequestAcknowledge(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeHandoverCommand(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeENBStatusTransfer(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeMMEStatusTransfer(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeHandoverNotify(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeUEContextReleaseCommand(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeUEContextReleaseComplete(pdu(S1Message_table_t) S1Message);
struct AttachDecodedData DecodeUEContextReleaseRequest(pdu(S1Message_table_t) S1Message);
void DecodeHandoverRequestForTAP(pdu(S1Message_table_t) S1Message);
void DecodeHandoverRequestAcknowledgeForTAP(pdu(S1Message_table_t) S1Message);
void DecodeHandoverCommandForTAP(pdu(S1Message_table_t) S1Message);
void DecodeENBStatusTransferForTAP(pdu(S1Message_table_t) S1Message);
void DecodeMMEStatusTransferForTAP(pdu(S1Message_table_t) S1Message);

uint8_t DecoderFull(const void* buffer);
void DecodeInitialUEMessageFully(pdu(S1Message_table_t) S1Message);
void DecodeDownlinkNASTransportFully(pdu(S1Message_table_t) S1Message);
void DecodeUplinkNASTransportFully(pdu(S1Message_table_t) S1Message);
void DecodeInitialContextSetupRequestFully(pdu(S1Message_table_t) S1Message);
void DecodeInitialContextSetupResponseFully(pdu(S1Message_table_t) S1Message);
void DecodeUECapabilityInfoIndicationFully(pdu(S1Message_table_t) S1Message);
void DecodeHandoverRequiredFully(pdu(S1Message_table_t) S1Message);
void DecodeHandoverRequestFully(pdu(S1Message_table_t) S1Message);
void DecodeHandoverRequestAcknowledgeFully(pdu(S1Message_table_t) S1Message);
void DecodeUEContextReleaseCompleteFully(pdu(S1Message_table_t) S1Message);
void DecodeE_RABModifyRequestFully(pdu(S1Message_table_t) S1Message);
void DecodeE_RABModifyResponseFully(pdu(S1Message_table_t) S1Message);

int HexToInt(char* input);
void* Uint8ToVoidArray(const uint8_t *buffer, int size);
void VoidToUint8Array(void * buffer, int size);
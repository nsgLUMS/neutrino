#ifndef HANDOVER_PROCEDURE_H_
#define HANDOVER_PROCEDURE_H_
#include "structs.h"
#include "../compiled/S1AP-PDU.h"

/* Functions to decode messages of Handover procedures. */
struct HandoverFields AccessHandoverRequired(S1AP_PDU_t*);
struct HandoverFields AccessHandoverRequest(S1AP_PDU_t*);
struct HandoverFields AccessHandoverRequestAcknowledge(S1AP_PDU_t*);
struct HandoverFields AccessHandoverCommand(S1AP_PDU_t*);
struct HandoverFields AccessENBStatusTransfer(S1AP_PDU_t*);
struct HandoverFields AccessMMEStatusTransfer(S1AP_PDU_t*);
struct HandoverFields AccessHandoverNotify(S1AP_PDU_t*);
struct HandoverFields AccessUEContextReleaseCommand(S1AP_PDU_t*);
struct HandoverFields AccessUEContextReleaseComplete(S1AP_PDU_t*);
struct HandoverFields AccessUEContextReleaseRequest(S1AP_PDU_t*);
#endif

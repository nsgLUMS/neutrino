#ifndef ATTACH_PROCEDURE_H_
#define ATTACH_PROCEDURE_H_
#include "structs.h"
#include "../compiled/S1AP-PDU.h"

/* Functions to decode message of Attach procedure */
struct DecodedData access_initial_ue_message(S1AP_PDU_t*);
struct DecodedData access_downlink_nas_transport(S1AP_PDU_t*);
struct DecodedData access_uplink_nas_transport(S1AP_PDU_t*);
struct DecodedData access_initial_context_setup_request(S1AP_PDU_t*);
struct DecodedData access_initial_context_setup_response(S1AP_PDU_t*);
struct DecodedData access_ue_capability_info_indication(S1AP_PDU_t*);
#endif

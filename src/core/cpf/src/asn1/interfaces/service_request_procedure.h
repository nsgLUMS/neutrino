#ifndef SERVICE_REQUEST_PROCEDURE_H_
#define SERVICE_REQUEST_PROCEDURE_H_
#include "structs.h"
#include "../compiled/S1AP-PDU.h"

/* Functions to decode message of Service Request procedure */
uint8_t access_service_request();
uint8_t access_sr_initial_context_setup_request();
uint8_t access_sr_initial_context_setup_response();
uint8_t access_erab_modify_request();
uint8_t access_erab_modify_response();
#endif

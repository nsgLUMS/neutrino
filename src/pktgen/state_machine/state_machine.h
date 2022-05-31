#ifndef __state_machine__
#define __state_machine__

#include <stddef.h>
#include <stdint.h>
#include "asn1_encoder_decoder.h"
#include "fb_encoder_decoder.h"
#include "structs.h"

// This enum defines the state ids for different
// states of a UE for attach procedure
typedef enum States {
  // States for attach procedure ...
  INITIAL_CONTEXT_SETUP_RESPONSE = 9,
  UE_CAPABILITY_INFO_INDICATION = 22,
  AUTHENTICATION_REQUEST = 82,
  AUTHENTICATION_RESPONSE = 83,
  SECURITY_MODE_COMPLETE = 94,
  SECURITY_MODE_COMMAND = 93,
  ESM_INFO_RESPONSE = 218,
  ESM_INFO_REQUEST = 217,
  ATTACH_COMPLETE = 67,
  ATTACH_REQUEST = 65,
  ATTACH_ACCEPT = 66,

  // States for handover procedure ...
  HANDOVER_REQUIRED = 100,
  HANDOVER_REQUEST,
  HANDOVER_REQUEST_ACK,
  HANDOVER_COMMAND,
  ENB_STATUS_TRANSFER,
  MME_STATUS_TRANSFER,
  HANDOVER_NOTIFY,
  UE_CONTEXT_RELEASE_COMMAND,
  UE_CONTEXT_RELEASE_COMPLETE,
  UE_CONTEXT_RELEASE_REQUEST,

  // State to represent complete procedure
  DONE

} States;

enum ServiceRequestStates {

  // States for service request procedure ....
  SERVICE_REQUEST = 0,
  SR_INITIAL_CONTEXT_SETUP_REQUEST = 3,
  SR_INITIAL_CONTEXT_SETUP_RESPONSE = 4,
  E_RAB_MODIFICATION_REQUEST = 21,
  E_RAB_MODIFICATION_RESPONSE = 22

} ServiceRequestStates;

enum DPCMServiceRequestStates {

  // States for service request procedure ....
  DPCM_SERVICE_REQUEST = 0,
  DPCM_E_RAB_MODIFICATION_REQUEST = 21,
  DPCM_E_RAB_MODIFICATION_RESPONSE = 22

} DPCMServiceRequestStates;

// ================ ASN1 encoding/decoding ======================
// asn1_attach_decode decodes an ASN1 encoded message
// and based on the decoded fields, it populates the
// next state ids in the third argument of the funtion ...
uint16_t asn1_attach_decode(uint8_t *buf, size_t len, States *nxt);

// asn1_attach_next_msg populates the bytes of next
// message in second argument provided current state
// in the first argument. It returns the number of
// bytes in the populated message ...
size_t asn1_attach_next_msg(States curr, uint8_t *buf);

// asn1_handovr_decode decodes an ASN1 encoded message
// and based on the decoded fields, it populates the
// next state ids in the third argument of the funtion ...
uint16_t asn1_handovr_decode(uint8_t *buf, size_t len, States *nxt);

// asn1_handovr_next_msg populates the bytes of next
// message in second argument provided current state
// in the first argument. It returns the number of
// bytes in the populated message ...
size_t asn1_handovr_next_msg(States curr, uint8_t *buf);

// asn1_handovr_id returns the message id of the state
// from the decoded buffer of ASN1 ...
States asn1_handovr_id(struct HandoverFields *decod);

// asn1_srvc_req_decode decodes an ASN1 encoded message
// and based on the decoded fields, it populates the
// next state ids in the third argument of the funtion ...
uint16_t asn1_srvc_req_decode(uint8_t *buf, size_t len, States *nxt);

// asn1_srvc_req_next_msg populates the bytes of next
// message in second argument provided current state
// in the first argument. It returns the number of
// bytes in the populated message ...
size_t asn1_srvc_req_next_msg(States curr, uint8_t *buf);

// asn1_dpcm_srvc_req_decode decodes an ASN1 encoded message
// and based on the decoded fields, it populates the
// next state ids in the third argument of the funtion ...
uint16_t asn1_dpcm_srvc_decode(uint8_t *buf, size_t len, States *nxt);

// asn1_srvc_req_next_msg populates the bytes of next
// message in second argument provided current state
// in the first argument. It returns the number of
// bytes in the populated message ...
size_t asn1_dpcm_srvc_next_msg(States curr, uint8_t *buf);


// ================ Flatbuffers encoding/decoding ======================

// fb_attach_decode decodes an FB encoded message
// and based on the decoded fields, it populates the
// next state ids in the third argument of the funtion ...
uint16_t fb_attach_decode(uint8_t *, size_t, States *);

// fb_attach_next_msg populates the bytes of next
// message in second argument provided current state
// in the first argument. It returns the number of
// bytes in the populated message ...
size_t fb_attach_next_msg(States curr, uint8_t *buf);

// fb_handovr_decode decodes an FB encoded message
// and based on the decoded fields, it populates the
// next state ids in the third argument of the funtion ...
uint16_t fb_handovr_decode(uint8_t *buf, size_t len, States *nxt);

// fb_handovr_next_msg populates the bytes of next
// message in second argument provided current state
// in the first argument. It returns the number of
// bytes in the populated message ...
size_t fb_handovr_next_msg(States curr, uint8_t *buf);

// fb_handovr_id returns the message id of the state
// from the decoded buffer of FB ...
States fb_handovr_id(struct AttachDecodedData *decod);

// fb_srvc_req_decode decodes an FB encoded message
// and based on the decoded fields, it populates the
// next state ids in the third argument of the funtion ...
uint16_t fb_srvc_req_decode(uint8_t *buf, size_t len, States *nxt);

// fb_srvc_req_next_msg populates the bytes of next
// message in second argument provided current state
// in the first argument. It returns the number of
// bytes in the populated message ...
size_t fb_srvc_req_next_msg(States curr, uint8_t *buf);

// fb_fast_ho_decode decodes an FB encoded message
// and based on the decoded fields, it populates the
// next state ids in the third argument of the funtion ...
uint16_t fb_fast_ho_decode(uint8_t* buf, size_t len, States* nxt);

// fb_fast_ho_next_msg populates the bytes of next
// message in second argument provided current state
// in the first argument. It returns the number of
// bytes in the populated message ...
size_t fb_fast_ho_next_msg(States curr, uint8_t* buf);

#endif
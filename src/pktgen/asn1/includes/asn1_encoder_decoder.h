#include <stdbool.h>
#include "S1AP-PDU.h"

#ifndef ENCODER_DECODER_H_
#define ENCODER_DECODER_H_

/* Encoding methods for Handover Procedure */
/* Once called, these functions return the encoded message
   as well as its size confined in a structure.
*/
struct Data HandoverRequired();
struct Data HandoverRequest();
struct Data HandoverRequestAcknowledge();
struct Data HandoverCommand();
struct Data ENBStatusTransfer();
struct Data MMEStatusTransfer();
struct Data HandoverNotify();
struct Data UEContextReleaseCommand();
struct Data UEContextReleaseComplete();
struct Data UEContextReleaseRequest();
struct Data UEContextReleaseCommandNas();
struct Data UEContextReleaseCompleteFin();

/* Decoding for Handover Procedure */
/* Input to this function is the encoded message and
   its size. It returns three fields after decoding them 
   in a structure.     
*/
struct HandoverFields DecodeHandover(uint8_t[], int);

/* Encoding of Attach Procedure */
/* Once called, these functions return the encoded message
   as well as its size confined in a structure.
*/
struct Data attach_request();
struct Data authentication_request();
struct Data authentication_response();
struct Data security_mode_command();
struct Data security_mode_complete();
struct Data esm_information_request();
struct Data esm_information_response();
struct Data attach_accept();
struct Data attach_accept_response();
struct Data ue_capability_information();
struct Data attach_complete();

/* Decoding for Attach Procedure */
/* Input to this function is the encoded message and
   its size. It returns three fields after decoding them 
   in a structure.     
*/
struct DecodedData Decode(uint8_t[], int);


/* Encoding of Service Reqest Procedure */
/* Once called, these functions return the encoded message
   as well as its size confined in a structure.
*/
struct Data dpcm_service_request();
struct Data service_request();
struct Data sr_initial_context_setup_response();
struct Data sr_initial_context_setup_request();
struct Data erab_modify_request();
struct Data erab_modify_response();

/* Decoding for Service Request Procedure */
/* Input to this function is the encoded message and
   its size. It returns one field after decoding them 
   in a structure.     
*/
 uint8_t DecodeServiceRequest(uint8_t[], int);



#endif

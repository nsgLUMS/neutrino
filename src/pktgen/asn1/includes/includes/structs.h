#include <stdbool.h>
#include "../compiled/S1AP-PDU.h"

#ifndef STRUCTS_H_
#define STRUCTS_H_

/* Structure used in encoding message.
   The buffer field is set to the actual encoded message.
   While the size is set to its length. */
struct Data {
  uint8_t *buffer;
  size_t size;
};

/* Structure used in decoding attach procedure. */
struct DecodedData {
  int MsgCode;
  int NASPD;
  bool ProtocolType;
};

/* Structure used in decoding handover procedure. */
struct HandoverFields {
  int S1apPdu;
  int ProcedureCode;
  int CauseID;
};

///////////////////////For intermediate processing////////////////////////////
struct Enub {
  int id;
  int criticality;
  int value;
};

struct Nas {
  int id;
  int criticality;
  OCTET_STRING_t value;
};

struct Tai {  
  OCTET_STRING_t plmnidentity;
  OCTET_STRING_t tac;
  int id;
  int criticality;
};

struct Eutran {
  OCTET_STRING_t plmnidentity;
  char* cell_id;
  int id;
  int criticality;
};

struct Rrc {
  int id;
  int criticality;
  int value;
};

struct Mme {
  int id;
  int criticality;
  int value;
};

struct Bitrate {
  int id;
  int criticality;
  int bitratedl;
  int bitrateul;
};

struct Sureq {
  int id;
  int criticality;
  int ie_id;
  int ie_criticality;
  int erab_id;
  int qci;
  int priority;
  int pre_capability;
  int pre_vulnerability;
  char* transport_address;
  OCTET_STRING_t gtp;
  OCTET_STRING_t nas_pdu;
};

struct SecCaps {
  int id;
  int criticality;
  char* encryption;
  char* integrity;
};

struct SecKey {
  int id;
  int criticality;
  char* key;
};

struct Sures {
  int id;
  int criticality;
  int ie_id;
  int ie_criticality;
  int erab_id;
  char* transport_address;
  OCTET_STRING_t gtp;
};

struct Radio {
  int id;
  int criticality;
  OCTET_STRING_t value;
};

/* Handover structs*/

struct HandType {
  int id;
  int criticality;
  int type;
};

struct CauseItem {
  int id;
  int criticality;
  int radioNetwork;

  int transport;
  int nas;
  int protocol;
  int misc;
};

struct Target {
  int id;
  int criticality;
  OCTET_STRING_t plmnidentity;
  char* macroenb;
  char* homeenb;
  OCTET_STRING_t plmnidentity_tai;
  OCTET_STRING_t tac;

  OCTET_STRING_t plmnidentity_lai;
  OCTET_STRING_t lac;
  int rnc_id;
  OCTET_STRING_t plmnidentity_cgi;
  OCTET_STRING_t lac_cgi;
  OCTET_STRING_t ci;
};

struct SourceToTarget {
  int id;
  int criticality;
  OCTET_STRING_t value;
};

struct Horeq {
  int id;
  int criticality;

  int horeq_id;
  int horeq_criticality;
  int erab_id;
  char* transport_layer;
  OCTET_STRING_t gtpe_id;
  int qci;
  int priority_level;
  int preemption_cap;
  int preemption_vul; 
};

struct SecContext {
  int id;
  int criticality;

  int next_hop;
  char* next_hop_param;
};

struct ErabAdmitted {
  int id;
  int criticality;
  
  int id_erab;
  int criticality_erab;
  int erab_id;
  char* transport_layer;
  OCTET_STRING_t gtpe_id;
};

struct TargetToSource {
  int id;
  int criticality;
  OCTET_STRING_t value;
};

struct Forwarding {
  int id;
  int criticality;
  
  int id_for;
  int criticality_for;
  int erab_id;
};

struct StatusTransfer {
  int id;
  int criticality;

  int bearer_id;
  int bearer_criticality;
  int erab_id;
  int ul_pdcp;
  int ul_hfn;
  int dl_pdcp;
  int dl_hfn;
};

struct UE_IDs {
  int id;
  int criticality;

  int mme_pair;
  int enub_pair;
};

/*Service request*/

struct ModreqList {
  int id;
  int criticality;

  int modreq_item_id;
  int modreq_criticality;

  int erab_id;
  int qci;
  int priority;
  int emption_capability;
  int emption_vulnerability;

  OCTET_STRING_t nas_pdu;
};

struct S1AP {
  int procedureCode;
  int criticality;
  int msgType;
};

/* Structures */
/* Attach procedure */
struct InitUe {
  
  struct Enub enub;
  struct Nas nas;
  struct Tai tai;
  struct Eutran eutran;
  struct Rrc rrc;
  struct S1AP s1ap;

};

struct DownlinkNas {
  struct Mme mme;
  struct Enub enub;
  struct Nas nas;
  struct S1AP s1ap;
};

struct UplinkNas {
  
  struct Mme mme;
  struct Enub enub;
  struct Nas nas;
  struct Eutran eutran;
  struct Tai tai;
  struct S1AP s1ap;

};

struct InitialSetupRequest {

  struct Mme mme;
  struct Enub enub; 
  struct Bitrate bitrate;
  struct Sureq sureq;
  struct SecCaps seccaps;
  struct SecKey seckey;
  struct S1AP s1ap;

};

struct InitialSetupResponse {
  struct Mme mme;
  struct Enub enub;
  struct Sures sures;
  struct S1AP s1ap;
};

struct UeCapabilityIndication {
  struct Mme mme;
  struct Enub enub;
  struct Radio radio;
  struct S1AP s1ap;
};

/* Handover procedure */

struct HandRequired {
  struct Mme mme;
  struct Enub enub;
  struct HandType type;
  struct CauseItem cause;
  struct Target target;
  struct SourceToTarget stt;
  struct S1AP s1ap;
};

struct HandRequest {
  struct Mme mme;
  struct HandType type;
  struct CauseItem cause;
  struct Bitrate bitrate;
  struct Horeq horeq;
  struct SourceToTarget stt;
  struct SecCaps seccap;
  struct SecContext sec_context;

  struct S1AP s1ap;
};

struct HandRequestAck {
  struct Mme mme;
  struct Enub enub;
  struct ErabAdmitted admitted;
  struct TargetToSource tts;

  struct S1AP s1ap;

};

struct HandCommand {
  struct Mme mme;
  struct Enub enub;
  struct HandType type;
  struct Forwarding forwarding;
  struct TargetToSource tts;

  struct S1AP s1ap;

};

struct ENBStatus {
  struct Mme mme;
  struct Enub enub;
  struct StatusTransfer status;
  struct S1AP s1ap;

};

struct HandNotify {
  
  struct Mme mme;
  struct Enub enub;
  struct Eutran eutran;
  struct Tai tai;
  struct S1AP s1ap;

};

struct ReleaseCommand {
  
  struct UE_IDs ids;
  struct CauseItem cause;
  int choice;
  struct S1AP s1ap;

};

struct ReleaseComplete {
  
  struct Mme mme;
  struct Enub enub;
  struct S1AP s1ap;

};

struct ReleaseRequest {
  
  struct Mme mme;
  struct Enub enub;
  struct CauseItem cause;
  struct S1AP s1ap;

};

struct ErabToBeModified {
  
  struct Mme mme;
  struct Enub enub;
  struct ModreqList list;

  struct S1AP s1ap;

};

struct ErabResponse {
  
  struct Mme mme;
  struct Enub enub;

  struct S1AP s1ap;

};

#endif

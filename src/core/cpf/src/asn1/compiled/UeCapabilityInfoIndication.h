/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Messages"
 * 	found in "../asn1_schema/messages.asn"
 */

#ifndef	_UeCapabilityInfoIndication_H_
#define	_UeCapabilityInfoIndication_H_


#include <asn_application.h>

/* Including external dependencies */
#include "UeCapabilityInfoIndication-IEs.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UeCapabilityInfoIndication */
typedef struct UeCapabilityInfoIndication {
	UeCapabilityInfoIndication_IEs_t	 protocolIEs;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UeCapabilityInfoIndication_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UeCapabilityInfoIndication;
extern asn_SEQUENCE_specifics_t asn_SPC_UeCapabilityInfoIndication_specs_1;
extern asn_TYPE_member_t asn_MBR_UeCapabilityInfoIndication_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _UeCapabilityInfoIndication_H_ */
#include <asn_internal.h>

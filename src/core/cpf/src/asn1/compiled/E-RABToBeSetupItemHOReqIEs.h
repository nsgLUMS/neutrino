/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#ifndef	_E_RABToBeSetupItemHOReqIEs_H_
#define	_E_RABToBeSetupItemHOReqIEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtocolIE-ID.h"
#include "Criticality.h"
#include "E-RABToBeSetupItemHOReq.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* E-RABToBeSetupItemHOReqIEs */
typedef struct E_RABToBeSetupItemHOReqIEs {
	ProtocolIE_ID_t	 id;
	Criticality_t	 criticality;
	E_RABToBeSetupItemHOReq_t	 type;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E_RABToBeSetupItemHOReqIEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E_RABToBeSetupItemHOReqIEs;
extern asn_SEQUENCE_specifics_t asn_SPC_E_RABToBeSetupItemHOReqIEs_specs_1;
extern asn_TYPE_member_t asn_MBR_E_RABToBeSetupItemHOReqIEs_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _E_RABToBeSetupItemHOReqIEs_H_ */
#include <asn_internal.h>

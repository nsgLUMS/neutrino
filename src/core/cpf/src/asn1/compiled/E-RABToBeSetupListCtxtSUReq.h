/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#ifndef	_E_RABToBeSetupListCtxtSUReq_H_
#define	_E_RABToBeSetupListCtxtSUReq_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct E_RABToBeSetupItemCtxtSUReqIEs;

/* E-RABToBeSetupListCtxtSUReq */
typedef struct E_RABToBeSetupListCtxtSUReq {
	A_SEQUENCE_OF(struct E_RABToBeSetupItemCtxtSUReqIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E_RABToBeSetupListCtxtSUReq_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E_RABToBeSetupListCtxtSUReq;
extern asn_SET_OF_specifics_t asn_SPC_E_RABToBeSetupListCtxtSUReq_specs_1;
extern asn_TYPE_member_t asn_MBR_E_RABToBeSetupListCtxtSUReq_1[1];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "E-RABToBeSetupItemCtxtSUReqIEs.h"

#endif	/* _E_RABToBeSetupListCtxtSUReq_H_ */
#include <asn_internal.h>

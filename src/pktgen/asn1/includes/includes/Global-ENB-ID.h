/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#ifndef	_Global_ENB_ID_H_
#define	_Global_ENB_ID_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PLMNidentity.h"
#include "ENB-ID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Global-ENB-ID */
typedef struct Global_ENB_ID {
	PLMNidentity_t	 pLMNidentity;
	ENB_ID_t	 eNB_ID;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Global_ENB_ID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Global_ENB_ID;
extern asn_SEQUENCE_specifics_t asn_SPC_Global_ENB_ID_specs_1;
extern asn_TYPE_member_t asn_MBR_Global_ENB_ID_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _Global_ENB_ID_H_ */
#include <asn_internal.h>

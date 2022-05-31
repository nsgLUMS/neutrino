/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#ifndef	_UE_S1AP_IDs_H_
#define	_UE_S1AP_IDs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "UE-S1AP-ID-pair.h"
#include "MME-UE-S1AP-ID.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum UE_S1AP_IDs_PR {
	UE_S1AP_IDs_PR_NOTHING,	/* No components present */
	UE_S1AP_IDs_PR_uE_S1AP_ID_pair,
	UE_S1AP_IDs_PR_mme
} UE_S1AP_IDs_PR;

/* UE-S1AP-IDs */
typedef struct UE_S1AP_IDs {
	UE_S1AP_IDs_PR present;
	union UE_S1AP_IDs_u {
		UE_S1AP_ID_pair_t	 uE_S1AP_ID_pair;
		MME_UE_S1AP_ID_t	 mme;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UE_S1AP_IDs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UE_S1AP_IDs;
extern asn_CHOICE_specifics_t asn_SPC_UE_S1AP_IDs_specs_1;
extern asn_TYPE_member_t asn_MBR_UE_S1AP_IDs_1[2];
extern asn_per_constraints_t asn_PER_type_UE_S1AP_IDs_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _UE_S1AP_IDs_H_ */
#include <asn_internal.h>

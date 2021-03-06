/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Messages"
 * 	found in "../asn1_schema/messages.asn"
 */

#ifndef	_UeCapabilityInfoIndication_IEs_H_
#define	_UeCapabilityInfoIndication_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MME-Item.h"
#include "ENUB-Item.h"
#include "UERadioCapability-Item.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UeCapabilityInfoIndication-IEs */
typedef struct UeCapabilityInfoIndication_IEs {
	MME_Item_t	 mme;
	ENUB_Item_t	 enub;
	UERadioCapability_Item_t	 radio;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UeCapabilityInfoIndication_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UeCapabilityInfoIndication_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_UeCapabilityInfoIndication_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_UeCapabilityInfoIndication_IEs_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _UeCapabilityInfoIndication_IEs_H_ */
#include <asn_internal.h>

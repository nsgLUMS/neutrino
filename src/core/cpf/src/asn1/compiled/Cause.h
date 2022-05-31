/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#ifndef	_Cause_H_
#define	_Cause_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CauseRadioNetwork.h"
#include "CauseTransport.h"
#include "CauseNas.h"
#include "CauseProtocol.h"
#include "CauseMisc.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Cause_PR {
	Cause_PR_NOTHING,	/* No components present */
	Cause_PR_radioNetwork,
	Cause_PR_transport,
	Cause_PR_nas,
	Cause_PR_protocol,
	Cause_PR_misc
} Cause_PR;

/* Cause */
typedef struct Cause {
	Cause_PR present;
	union Cause_u {
		CauseRadioNetwork_t	 radioNetwork;
		CauseTransport_t	 transport;
		CauseNas_t	 nas;
		CauseProtocol_t	 protocol;
		CauseMisc_t	 misc;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Cause_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Cause;
extern asn_CHOICE_specifics_t asn_SPC_Cause_specs_1;
extern asn_TYPE_member_t asn_MBR_Cause_1[5];
extern asn_per_constraints_t asn_PER_type_Cause_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _Cause_H_ */
#include <asn_internal.h>

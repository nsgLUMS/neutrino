/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Messages"
 * 	found in "../asn1_schema/messages.asn"
 */

#ifndef	_HandoverCommand_H_
#define	_HandoverCommand_H_


#include <asn_application.h>

/* Including external dependencies */
#include "HandoverCommand-IEs.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HandoverCommand */
typedef struct HandoverCommand {
	HandoverCommand_IEs_t	 protocolIEs;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} HandoverCommand_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HandoverCommand;
extern asn_SEQUENCE_specifics_t asn_SPC_HandoverCommand_specs_1;
extern asn_TYPE_member_t asn_MBR_HandoverCommand_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _HandoverCommand_H_ */
#include <asn_internal.h>

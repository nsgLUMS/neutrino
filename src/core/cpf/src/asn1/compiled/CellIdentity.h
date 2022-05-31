/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#ifndef	_CellIdentity_H_
#define	_CellIdentity_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CellIdentity */
typedef BIT_STRING_t	 CellIdentity_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CellIdentity;
asn_struct_free_f CellIdentity_free;
asn_struct_print_f CellIdentity_print;
asn_constr_check_f CellIdentity_constraint;
ber_type_decoder_f CellIdentity_decode_ber;
der_type_encoder_f CellIdentity_encode_der;
xer_type_decoder_f CellIdentity_decode_xer;
xer_type_encoder_f CellIdentity_encode_xer;
oer_type_decoder_f CellIdentity_decode_oer;
oer_type_encoder_f CellIdentity_encode_oer;
per_type_decoder_f CellIdentity_decode_uper;
per_type_encoder_f CellIdentity_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _CellIdentity_H_ */
#include <asn_internal.h>

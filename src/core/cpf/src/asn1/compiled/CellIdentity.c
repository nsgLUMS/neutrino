/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#include "CellIdentity.h"

/*
 * This type is implemented using BIT_STRING,
 * so here we adjust the DEF accordingly.
 */
static const ber_tlv_tag_t asn_DEF_CellIdentity_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (3 << 2))
};
asn_TYPE_descriptor_t asn_DEF_CellIdentity = {
	"CellIdentity",
	"CellIdentity",
	&asn_OP_BIT_STRING,
	asn_DEF_CellIdentity_tags_1,
	sizeof(asn_DEF_CellIdentity_tags_1)
		/sizeof(asn_DEF_CellIdentity_tags_1[0]), /* 1 */
	asn_DEF_CellIdentity_tags_1,	/* Same as above */
	sizeof(asn_DEF_CellIdentity_tags_1)
		/sizeof(asn_DEF_CellIdentity_tags_1[0]), /* 1 */
	{ 0, 0, BIT_STRING_constraint },
	0, 0,	/* No members */
	&asn_SPC_BIT_STRING_specs	/* Additional specs */
};


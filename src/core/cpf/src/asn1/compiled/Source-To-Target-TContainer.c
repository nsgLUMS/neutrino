/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#include "Source-To-Target-TContainer.h"

/*
 * This type is implemented using OCTET_STRING,
 * so here we adjust the DEF accordingly.
 */
static const ber_tlv_tag_t asn_DEF_Source_To_Target_TContainer_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (4 << 2))
};
asn_TYPE_descriptor_t asn_DEF_Source_To_Target_TContainer = {
	"Source-To-Target-TContainer",
	"Source-To-Target-TContainer",
	&asn_OP_OCTET_STRING,
	asn_DEF_Source_To_Target_TContainer_tags_1,
	sizeof(asn_DEF_Source_To_Target_TContainer_tags_1)
		/sizeof(asn_DEF_Source_To_Target_TContainer_tags_1[0]), /* 1 */
	asn_DEF_Source_To_Target_TContainer_tags_1,	/* Same as above */
	sizeof(asn_DEF_Source_To_Target_TContainer_tags_1)
		/sizeof(asn_DEF_Source_To_Target_TContainer_tags_1[0]), /* 1 */
	{ 0, 0, OCTET_STRING_constraint },
	0, 0,	/* No members */
	&asn_SPC_OCTET_STRING_specs	/* Additional specs */
};


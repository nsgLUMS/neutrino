/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Messages"
 * 	found in "../asn1_schema/messages.asn"
 */

#include "HandoverRequestAcknowledge-IEs.h"

asn_TYPE_member_t asn_MBR_HandoverRequestAcknowledge_IEs_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequestAcknowledge_IEs, mme),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MME_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mme"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequestAcknowledge_IEs, enub),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ENUB_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"enub"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequestAcknowledge_IEs, erab),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_E_RABAdmittedList_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"erab"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequestAcknowledge_IEs, container),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Target_To_Source_TContainer_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"container"
		},
};
static const ber_tlv_tag_t asn_DEF_HandoverRequestAcknowledge_IEs_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_HandoverRequestAcknowledge_IEs_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* mme */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* enub */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* erab */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* container */
};
asn_SEQUENCE_specifics_t asn_SPC_HandoverRequestAcknowledge_IEs_specs_1 = {
	sizeof(struct HandoverRequestAcknowledge_IEs),
	offsetof(struct HandoverRequestAcknowledge_IEs, _asn_ctx),
	asn_MAP_HandoverRequestAcknowledge_IEs_tag2el_1,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_HandoverRequestAcknowledge_IEs = {
	"HandoverRequestAcknowledge-IEs",
	"HandoverRequestAcknowledge-IEs",
	&asn_OP_SEQUENCE,
	asn_DEF_HandoverRequestAcknowledge_IEs_tags_1,
	sizeof(asn_DEF_HandoverRequestAcknowledge_IEs_tags_1)
		/sizeof(asn_DEF_HandoverRequestAcknowledge_IEs_tags_1[0]), /* 1 */
	asn_DEF_HandoverRequestAcknowledge_IEs_tags_1,	/* Same as above */
	sizeof(asn_DEF_HandoverRequestAcknowledge_IEs_tags_1)
		/sizeof(asn_DEF_HandoverRequestAcknowledge_IEs_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_HandoverRequestAcknowledge_IEs_1,
	4,	/* Elements count */
	&asn_SPC_HandoverRequestAcknowledge_IEs_specs_1	/* Additional specs */
};

/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Messages"
 * 	found in "../asn1_schema/messages.asn"
 */

#include "HandoverRequest-IEs.h"

asn_TYPE_member_t asn_MBR_HandoverRequest_IEs_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequest_IEs, mme),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MME_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mme"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequest_IEs, type),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_HandoverType_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"type"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequest_IEs, cause),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Cause_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cause"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequest_IEs, bitrate),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UE_Aggregate_Maxbitrate_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"bitrate"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequest_IEs, horeq),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_E_RABToBeSetupListHOReq_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"horeq"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequest_IEs, container),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Source_To_Target_TContainer_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"container"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequest_IEs, secCap),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UESecurityCapabilities_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"secCap"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverRequest_IEs, secContext),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SecurityContext_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"secContext"
		},
};
static const ber_tlv_tag_t asn_DEF_HandoverRequest_IEs_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_HandoverRequest_IEs_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* mme */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* type */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* cause */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* bitrate */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* horeq */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* container */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* secCap */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 } /* secContext */
};
asn_SEQUENCE_specifics_t asn_SPC_HandoverRequest_IEs_specs_1 = {
	sizeof(struct HandoverRequest_IEs),
	offsetof(struct HandoverRequest_IEs, _asn_ctx),
	asn_MAP_HandoverRequest_IEs_tag2el_1,
	8,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_HandoverRequest_IEs = {
	"HandoverRequest-IEs",
	"HandoverRequest-IEs",
	&asn_OP_SEQUENCE,
	asn_DEF_HandoverRequest_IEs_tags_1,
	sizeof(asn_DEF_HandoverRequest_IEs_tags_1)
		/sizeof(asn_DEF_HandoverRequest_IEs_tags_1[0]), /* 1 */
	asn_DEF_HandoverRequest_IEs_tags_1,	/* Same as above */
	sizeof(asn_DEF_HandoverRequest_IEs_tags_1)
		/sizeof(asn_DEF_HandoverRequest_IEs_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_HandoverRequest_IEs_1,
	8,	/* Elements count */
	&asn_SPC_HandoverRequest_IEs_specs_1	/* Additional specs */
};

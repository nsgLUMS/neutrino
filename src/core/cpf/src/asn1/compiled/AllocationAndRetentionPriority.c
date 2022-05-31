/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Structures"
 * 	found in "../asn1_schema/structures.asn"
 */

#include "AllocationAndRetentionPriority.h"

asn_TYPE_member_t asn_MBR_AllocationAndRetentionPriority_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct AllocationAndRetentionPriority, priorityLevel),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PriorityLevel,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"priorityLevel"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AllocationAndRetentionPriority, pre_emptionCapability),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Pre_emptionCapability,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pre-emptionCapability"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AllocationAndRetentionPriority, pre_emptionVulnerability),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Pre_emptionVulnerability,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pre-emptionVulnerability"
		},
};
static const ber_tlv_tag_t asn_DEF_AllocationAndRetentionPriority_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_AllocationAndRetentionPriority_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* priorityLevel */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* pre-emptionCapability */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* pre-emptionVulnerability */
};
asn_SEQUENCE_specifics_t asn_SPC_AllocationAndRetentionPriority_specs_1 = {
	sizeof(struct AllocationAndRetentionPriority),
	offsetof(struct AllocationAndRetentionPriority, _asn_ctx),
	asn_MAP_AllocationAndRetentionPriority_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_AllocationAndRetentionPriority = {
	"AllocationAndRetentionPriority",
	"AllocationAndRetentionPriority",
	&asn_OP_SEQUENCE,
	asn_DEF_AllocationAndRetentionPriority_tags_1,
	sizeof(asn_DEF_AllocationAndRetentionPriority_tags_1)
		/sizeof(asn_DEF_AllocationAndRetentionPriority_tags_1[0]), /* 1 */
	asn_DEF_AllocationAndRetentionPriority_tags_1,	/* Same as above */
	sizeof(asn_DEF_AllocationAndRetentionPriority_tags_1)
		/sizeof(asn_DEF_AllocationAndRetentionPriority_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_AllocationAndRetentionPriority_1,
	3,	/* Elements count */
	&asn_SPC_AllocationAndRetentionPriority_specs_1	/* Additional specs */
};

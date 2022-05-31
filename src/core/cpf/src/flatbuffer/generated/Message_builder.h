#ifndef MESSAGE_BUILDER_H
#define MESSAGE_BUILDER_H

/* Generated by flatcc 0.6.0-pre FlatBuffers schema compiler for C by dvide.com */

#ifndef MESSAGE_READER_H
#include "Message_reader.h"
#endif
#ifndef FLATBUFFERS_COMMON_BUILDER_H
#include "flatbuffers_common_builder.h"
#endif
#ifndef S1AP_PROTOCOL_IES_BUILDER_H
#include "S1AP_Protocol_IEs_builder.h"
#endif
#include "flatcc/flatcc_prologue.h"
#ifndef flatbuffers_identifier
#define flatbuffers_identifier 0
#endif
#ifndef flatbuffers_extension
#define flatbuffers_extension ".bin"
#endif

static const flatbuffers_voffset_t __LteS1_Message_Message_required[] = { 0 };
typedef flatbuffers_ref_t LteS1_Message_Message_ref_t;
static LteS1_Message_Message_ref_t LteS1_Message_Message_clone(flatbuffers_builder_t *B, LteS1_Message_Message_table_t t);
__flatbuffers_build_table(flatbuffers_, LteS1_Message_Message, 1)

#define __LteS1_Message_Message_formal_args , LteS1_ProtocolIE_S1AP_PROTOCOL_IES_vec_ref_t v0
#define __LteS1_Message_Message_call_args , v0
static inline LteS1_Message_Message_ref_t LteS1_Message_Message_create(flatbuffers_builder_t *B __LteS1_Message_Message_formal_args);
__flatbuffers_build_table_prolog(flatbuffers_, LteS1_Message_Message, LteS1_Message_Message_identifier, LteS1_Message_Message_type_identifier)

__flatbuffers_build_table_vector_field(0, flatbuffers_, LteS1_Message_Message_protocolIEs, LteS1_ProtocolIE_S1AP_PROTOCOL_IES, LteS1_Message_Message)

static inline LteS1_Message_Message_ref_t LteS1_Message_Message_create(flatbuffers_builder_t *B __LteS1_Message_Message_formal_args)
{
    if (LteS1_Message_Message_start(B)
        || LteS1_Message_Message_protocolIEs_add(B, v0)) {
        return 0;
    }
    return LteS1_Message_Message_end(B);
}

static LteS1_Message_Message_ref_t LteS1_Message_Message_clone(flatbuffers_builder_t *B, LteS1_Message_Message_table_t t)
{
    __flatbuffers_memoize_begin(B, t);
    if (LteS1_Message_Message_start(B)
        || LteS1_Message_Message_protocolIEs_pick(B, t)) {
        return 0;
    }
    __flatbuffers_memoize_end(B, t, LteS1_Message_Message_end(B));
}

#include "flatcc/flatcc_epilogue.h"
#endif /* MESSAGE_BUILDER_H */

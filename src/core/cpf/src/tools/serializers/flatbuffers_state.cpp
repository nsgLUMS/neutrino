#include "../../flatbuffer/src/encoder_decoder.h"
#include "../state.h"

#include <iostream>
using namespace std;

FlatbuffersState::FlatbuffersState(guti_t guti, guti_t hash,
                                   size_t payload_length)
    : guti(guti), hash(hash) {
  state_machine = StateMachine();
  last_msg_count = 0;

  if (payload_length == 408) {
    state_machine.setProcedure(0);
  } else if (payload_length == 700) {
    state_machine.setProcedure(1);
  } else if (payload_length == 390) {
    state_machine.setProcedure(2);
  } else if (payload_length == 368) {
    state_machine.setProcedure(3);
  } else {
    cout << "Wrong payload length " << payload_length << " (Flatbuffers)"
         << endl;
  }
}

size_t FlatbuffersState::PrepareAttachMsg(int state, uint8_t msg[]) {
  flatcc_builder_t builder;
  size_t size = 0;
  void *buffer;
  flatcc_builder_init(&builder);

  if (state == AttachStates::ATTACH_REQUEST) {
    EncodeInitialUEMessage(
        &builder, 3,
        "0741720bf655f531870148e000000102e0e000210201d011d1271a8080211001000010"
        "810600000000830600000000000d00000a005255f531b26e9011034f18a6e0",
        "55f531", "45678", "55f531", "0x00000201");
  }
  if (state == AttachStates::AUTHENTICATION_REQUEST) {
    EncodeDownlinkNASTransport(&builder, 0, 1,
                               "075201b8eeb8eeb8eeb8eeb8eeb8eeb8eeb8ee10b14d41f"
                               "46e52b9b9e10fa67cbfda8011");
  }
  if (state == AttachStates::AUTHENTICATION_RESPONSE) {
    EncodeUplinkNASTransport(&builder, 0, 1, "075308deba34fd848a4448", "55f531",
                             "0x00000101", "55f531", "45678");
  }
  if (state == AttachStates::SECURITY_MODE_COMMAND) {
    EncodeDownlinkNASTransport(&builder, 0, 1, "37ba9d339400075d010102e0e0c1");
  }
  if (state == AttachStates::SECURITY_MODE_COMPLETE) {
    EncodeUplinkNASTransport(&builder, 0, 1,
                             "47bb6fc9ce00075e23093385910614114005f0", "55f531",
                             "0x00000101", "55f531", "45678");
  }
  if (state == AttachStates::ESM_INFO_REQUEST) {
    EncodeDownlinkNASTransport(&builder, 0, 1, "277a296668010201d9");
  }
  if (state == AttachStates::ESM_INFO_RESPONSE) {
    EncodeUplinkNASTransport(&builder, 0, 1,
                             "2700d38231010201da2808034e534e03636f6d", "55f531",
                             "0x00000101S", "55f531", "45678");
  }
  if (state == AttachStates::ATTACH_ACCEPT) {
    EncodeInitialContextSetupRequest(
        &builder, 0, 1, 97000000, 47000000, 5, "0ac80afe", "01000000",
        "2760581d220207420125062055f531b26e00305201c1010508034e534e03636f6d0501"
        "0a0a0a045e04fefe9b69271480802110030000108106d412102a830600000000500bf6"
        "55f531870147c00000015312640101",
        5, 1, "c000", "c000",
        "b317cc65ef950a53aebea20ec04d921bb3dce8832274ff299839fea99bac1ab8");
  }
  if (state == AttachStates::INITIAL_CONTEXT_SETUP_RESPONSE) {
    EncodeInitialContextSetupResponse(&builder, 0, 1, "0ac80a25", "00000001",
                                      5);
  }
  if (state == AttachStates::UE_CAPABILITY_INFO_INDICATION) {
    EncodeUECapabilityInfoIndication(
        &builder, 0, 1,
        "00c80101681380004001060e4c204f84f84f84f84faf06ec400000");
  }
  if (state == AttachStates::ATTACH_COMPLETE) {
    EncodeUplinkNASTransport(&builder, 0, 1, "27db78240402074300035200c2",
                             "55f531", "0x00000101", "55f531", "45678");
  }

  buffer = flatcc_builder_finalize_buffer(&builder, &size);
  memcpy(msg, buffer, size);
  flatcc_builder_clear(&builder);
  free(buffer);
  return size;
}

size_t FlatbuffersState::PrepareServiceRequestMsg(int state, uint8_t msg[]) {
  flatcc_builder_t builder;
  size_t size = 0;
  void *buffer;
  flatcc_builder_init(&builder);

  switch (state) {
    case ServiceRequestStates::SR_INITIAL_CONTEXT_SETUP_REQUEST:
      EncodeInitialContextSetupRequest(
          &builder, 0, 1, 97000000, 47000000, 5, "0ac80afe", "01000000",
          "2760581d220207420125062055f531b26e00305201c1010508034e534e03636f6d05"
          "010a0a0a045e04fefe9b69271480802110030000108106d412102a83060000000050"
          "0bf655f531870147c00000015312640101",
          5, 1, "c000", "c000",
          "b317cc65ef950a53aebea20ec04d921bb3dce8832274ff299839fea99bac1ab8");
      break;
    case ServiceRequestStates::E_RAB_MODIFICATION_REQUEST:
      EncodeE_RABModifyRequest(&builder, 0, 1, "hello", 2, 3, 4);
      break;
    default:
      printf("ERROR: Tried to prepare unexpected message: %u\n", state);
      break;
  }

  buffer = flatcc_builder_finalize_buffer(&builder, &size);
  memcpy(msg, buffer, size);
  flatcc_builder_clear(&builder);
  free(buffer);
  return size;
}

std::vector<int> FlatbuffersState::HandleServiceRequest(void *buff,
                                                        size_t len) {
  uint8_t msgType = DecoderFull(buff);
  std::vector<int> next_states;

  if (state_machine.service_request_states[state_machine.state_index + 1] ==
      msgType) {
    switch (msgType) {
      case ServiceRequestStates::SERVICE_REQUEST:
        next_states.push_back(
            ServiceRequestStates::SR_INITIAL_CONTEXT_SETUP_REQUEST);
        state_machine.state_index++;
        break;
      case ServiceRequestStates::SR_INITIAL_CONTEXT_SETUP_RESPONSE:
        next_states.push_back(ServiceRequestStates::E_RAB_MODIFICATION_REQUEST);
        state_machine.state_index++;
        break;
      case ServiceRequestStates::E_RAB_MODIFICATION_RESPONSE:
        next_states.push_back(99);
        state_machine.state_index++;
        break;
      default:
        printf("ERROR: Tried to handle unexpected message: %u\n", msgType);
        break;
    }
  } else {
    printf("Expected ---- Message: %i\n",
           state_machine.service_request_states[state_machine.state_index]);
    printf("Received ---- Message: %u\n", msgType);
    printf("Something Fishy for GUTI %d\n", this->getHash());
  }
  return next_states;
}

std::vector<int> FlatbuffersState::HandleAttach(void *buff, size_t len) {

  AttachDecodedData decod = DecoderPartial(buff);
  int proto = (int)ProtocolType::NAS;
  int proc_code = decod.procedureCode;
  int nasID = decod.protocolDiscriminator;  // protocol Discriminator
  int msgType = decod.epsMobilityMessageType;
  std::vector<int> next_states;

  if (nasID == -1) {
    proto = (int)ProtocolType::S1AP;
    msgType = proc_code;
  }

  if (msgType == AttachStates::UE_CAPABILITY_INFO_INDICATION ||
		msgType == AttachStates::INITIAL_CONTEXT_SETUP_RESPONSE || 
		msgType == AttachStates::ATTACH_COMPLETE)
	{
		last_msg_count++;
		
		if (last_msg_count == 3){
			state_machine.procedure = ProcedureType::HANDOVER;
			state_machine.state_index = -1;
            next_states.push_back(99);
		}

		return next_states;
	}

  // if (msgType == AttachStates::UE_CAPABILITY_INFO_INDICATION) {
  //   return next_states;
  // } else if (msgType == AttachStates::INITIAL_CONTEXT_SETUP_RESPONSE) {
  //   return next_states;
  // } else if (msgType == AttachStates::ATTACH_COMPLETE) {
  //   state_machine.procedure = ProcedureType::DETACH;
  //   state_machine.state_index = -1;
  //   next_states.push_back(99);
  //   return next_states;
  // }

  if (state_machine.nas_pd == static_cast<NASProtocolDiscriminator>(nasID) &&
      state_machine.attach_states[state_machine.state_index + 1] == msgType) {
    if (msgType == AttachStates::AUTHENTICATION_REQUEST) {
      state_machine.state_index++;
      next_states.push_back(AttachStates::AUTHENTICATION_RESPONSE);
    }
    if (msgType == AttachStates::SECURITY_MODE_COMPLETE) {
      state_machine.state_index++;
      state_machine.nas_pd =
          NASProtocolDiscriminator::EPS_session_management_messages;
      next_states.push_back(AttachStates::ESM_INFO_REQUEST);
    }
    if (msgType == AttachStates::ESM_INFO_RESPONSE) {
      state_machine.state_index++;
      state_machine.nas_pd = NASProtocolDiscriminator::ERROR;
      next_states.push_back(AttachStates::ATTACH_ACCEPT);
    }
    if (msgType == AttachStates::ATTACH_ACCEPT) {
      state_machine.state_index++;
      state_machine.protocol = ProtocolType::S1AP;
      state_machine.nas_pd = NASProtocolDiscriminator::ERROR;
      next_states.push_back(AttachStates::INITIAL_CONTEXT_SETUP_RESPONSE);
      next_states.push_back(AttachStates::UE_CAPABILITY_INFO_INDICATION);
      next_states.push_back(AttachStates::ATTACH_COMPLETE);
    }
    if (msgType == AttachStates::ATTACH_COMPLETE) {
      state_machine.procedure = ProcedureType::HANDOVER;
      state_machine.state_index = -1;
      next_states.push_back(99);
    }
    if (msgType == AttachStates::ATTACH_REQUEST) {
      state_machine.state_index++;
      next_states.push_back(AttachStates::AUTHENTICATION_REQUEST);
    }
    if (msgType == AttachStates::AUTHENTICATION_RESPONSE) {
      state_machine.state_index++;
      next_states.push_back(AttachStates::SECURITY_MODE_COMMAND);
    }
    if (msgType == AttachStates::SECURITY_MODE_COMMAND) {
      state_machine.state_index++;
      state_machine.nas_pd =
          NASProtocolDiscriminator::EPS_session_management_messages;
      next_states.push_back(AttachStates::SECURITY_MODE_COMPLETE);
    }
    if (msgType == AttachStates::ESM_INFO_REQUEST) {
      state_machine.state_index++;
      state_machine.nas_pd =
          NASProtocolDiscriminator::EPS_mobility_management_messages;
      next_states.push_back(AttachStates::ESM_INFO_RESPONSE);
    }
    if (msgType == AttachStates::INITIAL_CONTEXT_SETUP_RESPONSE) {
      state_machine.state_index++;
    }
  } else {
    printf("Expected ---- NAS Protocol Disc %d, StateMachine Value %d\n",
           state_machine.nas_pd,
           state_machine.attach_states[state_machine.state_index + 1]);
    printf("Received ---- NAS Protocol Disc %d, StateMachine Value %d\n", nasID,
           msgType);
    printf("Something Fishy for GUTI %lu\n", this->getHash());

    uint8_t *ptr = (uint8_t *)buff;
    for (size_t i = 0; i < len; i++) {
      printf("%u, ", ptr[i]);
    }
    printf("\n");
    fflush(stdout);
  }
  return next_states;
}

size_t FlatbuffersState::PrepareHandoverMsg(int state, uint8_t msg[]) {
  flatcc_builder_t builder;
  size_t size = 0;
  void *buffer;
  flatcc_builder_init(&builder);

  switch (state) {
    case HandoverStates::HANDOVER_REQUIRED:
      EncodeHandoverRequired(
          &builder, 0, 1, "55f531", "000020", "55f531", "45678",
          "40809b0a101681380004001060e4c204f84f84f84f84faf06ec4000015e800000591"
          "382002900838f0841e1f041c78808081cf80400002008c0240b56ddd5ad813fd9405"
          "c13fd94041f51b582f1f28a2e08c5d04348565caa8430fb380004850d4340007c229"
          "bd96000008155509ec9b8000040605fc60901081816f20f20327630dda980606c091"
          "d018002080240a7da2a827623517260c8591404a845000004e400245000055f53100"
          "0020100055f53100001011800000");
      break;
    case HandoverStates::HANDOVER_REQUEST:
      EncodeHandoverRequest(
          &builder, 0, 97000000, 47000000, 5, 1, "0ac80afd", "02000000", 5,
          "40809b0a101681380004001060e4c204f84f84f84f84faf06ec4000015e800000591"
          "382002900838f0841e1f041c78808081cf80400002008c0240b56ddd5ad813fd9405"
          "c13fd94041f51b582f1f28a2e08c5d04348565caa8430fb380004850d4340007c229"
          "bd96000008155509ec9b8000040605fc60901081816f20f20327630dda980606c091"
          "d018002080240a7da2a827623517260c8591404a845000004e400245000055f53100"
          "0020100055f53100001011800000",
          "c000", "c000",
          "92160cdabe2b8ae4f42250dcafea2494a42d0953df715903786734ce0cb9507c",
          2);
      break;
    case HandoverStates::HANDOVER_REQUEST_ACK:
      EncodeHandoverRequestAcknowledge(&builder, 0, 1, 5, "0ac80a26",
                                       "00000001", "0ac80a26", "00000002",
                                       "002301091058215b37b281327630dda8181b020"
                                       "0c003f50004869b380004850d0001f09110");
      break;
    case HandoverStates::HANDOVER_COMMAND:
      EncodeHandoverCommand(&builder, 0, 1, 5, "0ac80a26", "00000002",
                            "002301091058215b37b281327630dda8181b0200c003f50004"
                            "869b380004850d0001f09110");
      break;
    case HandoverStates::ENB_STATUS_TRANSFER:
      EncodeENBStatusTransfer(&builder, 0, 1, 5, 231, 0, 51, 0);
      break;
    case HandoverStates::MME_STATUS_TRANSFER:
      EncodeMMEStatusTransfer(&builder, 0, 1, 5, 231, 0, 51, 0);
      break;
    case HandoverStates::HANDOVER_NOTIFY:
      EncodeHandoverNotify(&builder, 0, 1, "55f531", "0x00000201", "55f531",
                           "45678");
      break;
    case HandoverStates::UE_CONTEXT_RELEASE_COMMAND:
      EncodeUEContextReleaseCommand(&builder, 0, 1);
      break;
    case HandoverStates::UE_CONTEXT_RELEASE_COMPLETE:
      EncodeUEContextReleaseComplete(&builder, 0, 1);
      break;
    case HandoverStates::UE_CONTEXT_RELEASE_REQUEST:
      EncodeUEContextReleaseRequest(&builder, 0, 1);
      break;
  }
  buffer = flatcc_builder_finalize_buffer(&builder, &size);
  memcpy(msg, buffer, size);
  flatcc_builder_clear(&builder);
  free(buffer);
  return size;
}

std::vector<int> FlatbuffersState::HandleFastHandover(void *buff, size_t len) {
  return HandleHandover(buff, len);
}

std::vector<int> FlatbuffersState::HandleHandover(void *buff, size_t len) {
  AttachDecodedData decod = DecoderPartial(buff);
  std::vector<int> next_states;

  struct HandoverCompareable expectedState =
      state_machine.handover_states[state_machine.state_index + 1];

  {
    switch (expectedState.stateID) {
      case HandoverStates::HANDOVER_REQUIRED:
        state_machine.state_index++;
        next_states.push_back(HandoverStates::HANDOVER_REQUEST);
        break;
      case HandoverStates::HANDOVER_REQUEST:
        state_machine.state_index++;
        next_states.push_back(HandoverStates::HANDOVER_REQUEST_ACK);
        break;
      case HandoverStates::HANDOVER_REQUEST_ACK:
        state_machine.state_index++;
        next_states.push_back(HandoverStates::HANDOVER_COMMAND);
        break;
      case HandoverStates::HANDOVER_COMMAND:
        state_machine.state_index++;
        next_states.push_back(HandoverStates::ENB_STATUS_TRANSFER);
        break;
      case HandoverStates::ENB_STATUS_TRANSFER:
        state_machine.state_index++;
        next_states.push_back(HandoverStates::MME_STATUS_TRANSFER);
        break;
      case HandoverStates::MME_STATUS_TRANSFER:
        state_machine.state_index++;
        next_states.push_back(HandoverStates::HANDOVER_NOTIFY);
        break;
      case HandoverStates::HANDOVER_NOTIFY:
        state_machine.state_index++;
        next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_COMMAND);
        break;
      case HandoverStates::UE_CONTEXT_RELEASE_COMMAND:
        state_machine.state_index++;
        if (expectedState.causeID == 0) {
          next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_COMPLETE);
        } else {
          next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_COMPLETE);
          next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_REQUEST);
        }
        break;
      case HandoverStates::UE_CONTEXT_RELEASE_COMPLETE:
        state_machine.state_index++;
        if (state_machine.state_index == 6) {
          next_states.push_back(99);
          state_machine.procedure = ProcedureType::ERROR;
        }
        break;
      case HandoverStates::UE_CONTEXT_RELEASE_REQUEST:
        if (state_machine.state_index < 6) {
          state_machine.state_index++;
          next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_COMMAND);
        }

        break;
    }
  }
  return next_states;
}

size_t FlatbuffersState::PrepareMsg(int state, uint8_t msg[]) {
  switch (state_machine.procedure) {
    case ProcedureType::ATTACH:
      return PrepareAttachMsg(state, msg);
    case ProcedureType::DETACH:
      return 0;
    case ProcedureType::HANDOVER:
    case ProcedureType::FAST_HANDOVER:
      return PrepareHandoverMsg(state, msg);
    case ProcedureType::SERVICE_REQUEST:
      return PrepareServiceRequestMsg(state, msg);
  }
  return 0;
}

std::vector<int> FlatbuffersState::UpdateState(void *buff, size_t len) {
  switch (state_machine.procedure) {
    case ProcedureType::ATTACH:
      return HandleAttach(buff, len);
    case ProcedureType::DETACH:
      return std::vector<int>();
    case ProcedureType::HANDOVER:
      return HandleHandover(buff, len);
    case ProcedureType::FAST_HANDOVER:
      return HandleFastHandover(buff, len);
    case ProcedureType::SERVICE_REQUEST:
      return HandleServiceRequest(buff, len);
    case ProcedureType::ERROR:
      return std::vector<int>();
  }
}

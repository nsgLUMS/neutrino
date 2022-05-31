#include "../state.h"
#include <iostream>

using namespace std;

extern "C"
{
#include "../../asn1/interfaces/encoder_decoder.h"
#include "../../asn1/interfaces/structs.h"
}

Asn1State::Asn1State(){
	state_machine = StateMachine();
}

Asn1State::Asn1State(guti_t guti, int procedure_type): guti(guti){
	state_machine = StateMachine();

	setGuti(guti);
	if (procedure_type == 2)
	{
		state_machine.setProcedure(1);
	}
	else if (procedure_type == 4){
		state_machine.setProcedure(3);	
	}
}


Asn1State::Asn1State(guti_t guti, guti_t hash, size_t payload_length) : guti(guti), hash(hash)
{
	state_machine = StateMachine();
	last_msg_count = 0;
	if (payload_length == 182)
	{
		state_machine.setProcedure(0);
	}
	else if (payload_length == 423)
	{
		state_machine.setProcedure(1);
	}
	else if (payload_length == 121)
	{
		state_machine.setProcedure(2);
	}
	else if (payload_length == 141)
	{
		state_machine.setProcedure(3);
	}
	else if (payload_length == 234)
	{
		state_machine.setProcedure(4);
	}
	else
	{

		cout << "Wrong payload length " << payload_length << " (ASN1)" << endl;
	}
}

size_t Asn1State::PrepareAttachMsg(int state, uint8_t msg[])
{
	struct Data encoding_data;

	switch (state)
	{
	case AttachStates::ATTACH_REQUEST:
		encoding_data = attach_request();
		break;
	case AttachStates::AUTHENTICATION_REQUEST:
		encoding_data = authentication_request();
		break;
	case AttachStates::AUTHENTICATION_RESPONSE:
		encoding_data = authentication_response();
		break;
	case AttachStates::SECURITY_MODE_COMMAND:
		encoding_data = security_mode_command();
		break;
	case AttachStates::SECURITY_MODE_COMPLETE:
		encoding_data = security_mode_complete();
		break;
	case AttachStates::ESM_INFO_REQUEST:
		encoding_data = esm_information_request();
		break;
	case AttachStates::ESM_INFO_RESPONSE:
		encoding_data = esm_information_response();
		break;
	case AttachStates::ATTACH_ACCEPT:
		encoding_data = attach_accept();
		break;
	case AttachStates::INITIAL_CONTEXT_SETUP_RESPONSE:
		encoding_data = attach_accept_response();
		break;
	case AttachStates::UE_CAPABILITY_INFO_INDICATION:
		encoding_data = ue_capability_information();
		break;
	case AttachStates::ATTACH_COMPLETE:
		encoding_data = attach_complete();
		break;
	}

	memcpy(msg, encoding_data.buffer, encoding_data.size);
	free(encoding_data.buffer);
	return encoding_data.size;
}

std::vector<int> Asn1State::HandleAttach(void *buff, size_t len)
{
	DecodedData decod = Decode(buff, len);
	int proto = (int)ProtocolType::NAS;
	int proc_code = decod.ProtocolType;
	int nasID = decod.NASPD; // protocol Discriminator
	int msgType = decod.MsgCode;
	std::vector<int> next_states;

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

	if (state_machine.nas_pd == static_cast<NASProtocolDiscriminator>(nasID) && state_machine.attach_states[state_machine.state_index + 1] == msgType)
	{
		switch (msgType)
		{
		case AttachStates::AUTHENTICATION_REQUEST:
			state_machine.state_index++;
			next_states.push_back(AttachStates::AUTHENTICATION_RESPONSE);
			break;
		case AttachStates::SECURITY_MODE_COMPLETE:
			state_machine.state_index++;
			state_machine.nas_pd =
				NASProtocolDiscriminator::EPS_session_management_messages;
			next_states.push_back(AttachStates::ESM_INFO_REQUEST);
			break;
		case AttachStates::ESM_INFO_RESPONSE:
			state_machine.state_index++;
			state_machine.nas_pd = NASProtocolDiscriminator::ERROR;
			next_states.push_back(AttachStates::ATTACH_ACCEPT);
			break;
		case AttachStates::ATTACH_ACCEPT:
			state_machine.state_index++;
			state_machine.protocol = ProtocolType::S1AP;
			state_machine.nas_pd = NASProtocolDiscriminator::ERROR;
			next_states.push_back(AttachStates::INITIAL_CONTEXT_SETUP_RESPONSE);
			next_states.push_back(AttachStates::UE_CAPABILITY_INFO_INDICATION);
			next_states.push_back(AttachStates::ATTACH_COMPLETE);
			break;
		case AttachStates::ATTACH_COMPLETE:
			state_machine.procedure = ProcedureType::HANDOVER;
			state_machine.state_index = -1;
			next_states.push_back(99);
			break;
		case AttachStates::ATTACH_REQUEST:

			// int ret = rand() % 100;
			// if (ret == 1 || ret == 2){
				// usleep(68.90);
			// }
			// if (counter == 49){
			// 	usleep(68.90);
			// 	counter = 0;
			// }
			state_machine.state_index++;
			next_states.push_back(AttachStates::AUTHENTICATION_REQUEST);
			break;
		case AttachStates::AUTHENTICATION_RESPONSE:
			state_machine.state_index++;
			next_states.push_back(AttachStates::SECURITY_MODE_COMMAND);
			break;
		case AttachStates::SECURITY_MODE_COMMAND:
			state_machine.state_index++;
			state_machine.nas_pd =
				NASProtocolDiscriminator::EPS_session_management_messages;
			next_states.push_back(AttachStates::SECURITY_MODE_COMPLETE);
			break;
		case AttachStates::ESM_INFO_REQUEST:
			state_machine.state_index++;
			state_machine.nas_pd =
				NASProtocolDiscriminator::EPS_mobility_management_messages;
			next_states.push_back(AttachStates::ESM_INFO_RESPONSE);
			break;
		case AttachStates::INITIAL_CONTEXT_SETUP_RESPONSE:
			state_machine.state_index++;
			break;
		}
	}
	else
	{
		printf("(ATTACH)Expected ---- NAS Protocol Disc %d, StateMachine Value %d\n", state_machine.nas_pd, state_machine.attach_states[state_machine.state_index + 1]);
		printf("Received ---- NAS Protocol Disc %d, StateMachine Value %d\n", nasID, msgType);
		printf("Something Fishy for GUTI %d\n", this->getHash());
	}

	return next_states;
}

size_t Asn1State::PrepareHandoverMsg(int state, uint8_t msg[])
{
	struct Data encoding_data;

	switch (state)
	{
	case HandoverStates::HANDOVER_REQUIRED:
		encoding_data = HandoverRequired();
		break;
	case HandoverStates::HANDOVER_REQUEST:
		encoding_data = HandoverRequest();
		break;
	case HandoverStates::HANDOVER_REQUEST_ACK:
		encoding_data = HandoverRequestAcknowledge();
		break;
	case HandoverStates::HANDOVER_COMMAND:
		encoding_data = HandoverCommand();
		break;
	case HandoverStates::ENB_STATUS_TRANSFER:
		encoding_data = ENBStatusTransfer();
		break;
	case HandoverStates::MME_STATUS_TRANSFER:
		encoding_data = MMEStatusTransfer();
		break;
	case HandoverStates::HANDOVER_NOTIFY:
		encoding_data = HandoverNotify();
		break;
	case HandoverStates::UE_CONTEXT_RELEASE_COMMAND:
		encoding_data = UEContextReleaseCommand();
		break;
	case HandoverStates::UE_CONTEXT_RELEASE_COMPLETE:
		encoding_data = UEContextReleaseComplete();
		break;
	case HandoverStates::UE_CONTEXT_RELEASE_REQUEST:
		encoding_data = UEContextReleaseRequest();
		break;
	}
	memcpy(msg, encoding_data.buffer, encoding_data.size);
	free(encoding_data.buffer);
	return encoding_data.size;
}

std::vector<int> Asn1State::HandleHandover(void *buff, size_t len)
{
	HandoverFields fields = DecodeHandover(buff, len);
	int proc_code = fields.ProcedureCode;
	std::vector<int> next_states;

	struct HandoverCompareable expectedState = state_machine.handover_states[state_machine.state_index + 1];
	{
		switch (expectedState.stateID)
		{
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
			if (expectedState.causeID == 0)
			{
				next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_COMPLETE);
			}
			else
			{
				next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_COMPLETE);
				next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_REQUEST);
			}
			break;
		case HandoverStates::UE_CONTEXT_RELEASE_COMPLETE:
			state_machine.state_index++;
			if (state_machine.state_index == 6)
			{
				next_states.push_back(99);
				state_machine.procedure = ProcedureType::ERROR;
			}
			break;
		case HandoverStates::UE_CONTEXT_RELEASE_REQUEST:
			if (state_machine.state_index < 6)
			{
				state_machine.state_index++;
				next_states.push_back(HandoverStates::UE_CONTEXT_RELEASE_COMMAND);
			}

			break;
		}
	}
	return next_states;
}

size_t Asn1State::PrepareServiceRequestMsg(int state, uint8_t msg[])
{
	struct Data encoding_data;

	switch (state)
	{
	case ServiceRequestStates::SR_INITIAL_CONTEXT_SETUP_REQUEST:
		encoding_data = sr_initial_context_setup_request();
		break;
	case ServiceRequestStates::E_RAB_MODIFICATION_REQUEST:
		encoding_data = erab_modify_request();
		break;
	default:
		printf("ERROR: Tried to prepare unexpected message: %u\n", state);
		break;
	}

	memcpy(msg, encoding_data.buffer, encoding_data.size);
	free(encoding_data.buffer);
	return encoding_data.size;
}

std::vector<int> Asn1State::HandleServiceRequest(void *buff, size_t len)
{
	uint8_t msgType = DecodeServiceRequest(buff, len);
	std::vector<int> next_states;

	if (state_machine.service_request_states[state_machine.state_index + 1] == msgType)
	{
		switch (msgType)
		{

		case ServiceRequestStates::SERVICE_REQUEST:
			next_states.push_back(ServiceRequestStates::SR_INITIAL_CONTEXT_SETUP_REQUEST);
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
	}
	else
	{
		printf("Expected ---- Message: %i\n", state_machine.service_request_states[state_machine.state_index]);
		printf("Received ---- Message: %u\n", msgType);
		printf("Something Fishy for GUTI %d\n", this->getHash());
	}

	return next_states;
}

std::vector<int> Asn1State::HandleDPCMServiceRequest(void *buff, size_t len)
{
	uint8_t msgType = DecodeServiceRequest(buff, len);
	std::vector<int> next_states;
	if (state_machine.dpcm_service_request_states[state_machine.state_index + 1] == msgType)
	{
		switch (msgType)
		{
		case DPCMServiceRequestStates::DPCM_SERVICE_REQUEST:
			DecodeServiceRequest(buff, len);
			service_request();
			next_states.push_back(DPCMServiceRequestStates::DPCM_E_RAB_MODIFICATION_REQUEST);
			state_machine.state_index++;
			break;
		case DPCMServiceRequestStates::DPCM_E_RAB_MODIFICATION_RESPONSE:
			next_states.push_back(99);
			state_machine.state_index++;
			break;
		default:
			printf("ERROR: Tried to handle unexpected message: %u\n", msgType);
			break;
		}
	}
	else
	{
		printf("Expected ---- Message: %i\n", state_machine.service_request_states[state_machine.state_index]);
		printf("Received ---- Message: %u\n", msgType);
		printf("Something Fishy for GUTI %d\n", this->getHash());
	}
	return next_states;
}

size_t Asn1State::PrepareMsg(int state, uint8_t msg[])
{

	switch (state_machine.procedure)
	{
	case ProcedureType::ATTACH:
		return PrepareAttachMsg(state, msg);
	case ProcedureType::DETACH:
		return 0;
	case ProcedureType::HANDOVER:
	case ProcedureType::FAST_HANDOVER:
		return PrepareHandoverMsg(state, msg);
	case ProcedureType::SERVICE_REQUEST:
	case ProcedureType::DPCM_SERVICE_REQUEST:
		return PrepareServiceRequestMsg(state, msg);
	}
	return 0;
}

std::vector<int> Asn1State::HandleFastHandover(void *buff, size_t len)
{
	return HandleHandover(buff, len);
}

std::vector<int> Asn1State::UpdateState(void *buff, size_t len)
{
	switch (state_machine.procedure)
	{
	case ProcedureType::ATTACH:
		return HandleAttach(buff, (int)len);
	case ProcedureType::DETACH:
		return std::vector<int>();
	case ProcedureType::HANDOVER:
		return HandleHandover(buff, len);
	case ProcedureType::FAST_HANDOVER:
		return HandleFastHandover(buff, len);
	case ProcedureType::SERVICE_REQUEST:
		return HandleServiceRequest(buff, len);
	case ProcedureType::DPCM_SERVICE_REQUEST:
		return HandleDPCMServiceRequest(buff, len);
	case ProcedureType::ERROR:
		return std::vector<int>();
	}
}

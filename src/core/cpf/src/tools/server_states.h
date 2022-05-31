#ifndef STATES_H_
#define STATES_H_

struct HandoverCompareable
{
	int s1PDU;
	int procedureCode;
	int causeID;
	int stateID;
};

enum AttachStates
{
	ATTACH_REQUEST = 65,
	AUTHENTICATION_REQUEST = 82,
	AUTHENTICATION_RESPONSE = 83,
	SECURITY_MODE_COMMAND = 93,
	SECURITY_MODE_COMPLETE = 94,
	ESM_INFO_REQUEST = 217,  // Session variable
	ESM_INFO_RESPONSE = 218, // Session variable
	ATTACH_ACCEPT = 66,
	INITIAL_CONTEXT_SETUP_RESPONSE = 9,
	UE_CAPABILITY_INFO_INDICATION = 22,
	ATTACH_COMPLETE = 67,

};

enum HandoverStates
{
	HANDOVER_REQUIRED,
	HANDOVER_REQUEST,
	HANDOVER_REQUEST_ACK,
	HANDOVER_COMMAND,
	ENB_STATUS_TRANSFER,
	MME_STATUS_TRANSFER,
	HANDOVER_NOTIFY,
	UE_CONTEXT_RELEASE_COMMAND,
	UE_CONTEXT_RELEASE_COMPLETE,
	UE_CONTEXT_RELEASE_REQUEST,
};

enum ServiceRequestStates
{
	SERVICE_REQUEST = 0,
	SR_INITIAL_CONTEXT_SETUP_REQUEST = 3,
	SR_INITIAL_CONTEXT_SETUP_RESPONSE = 4,
	E_RAB_MODIFICATION_REQUEST = 21,
	E_RAB_MODIFICATION_RESPONSE = 22
};

enum DPCMServiceRequestStates
{
	DPCM_SERVICE_REQUEST = 0,
	DPCM_E_RAB_MODIFICATION_REQUEST = 21,
	DPCM_E_RAB_MODIFICATION_RESPONSE = 22
};

enum class ProtocolType
{
	S1AP,
	NAS,
};

enum class ProcedureType
{
	ATTACH,
	DETACH,
	HANDOVER,
	FAST_HANDOVER,
	SERVICE_REQUEST,
	DPCM_SERVICE_REQUEST,
	ERROR
};

enum class NASProtocolDiscriminator
{
	group_call_control,
	broadcast_call_control,
	EPS_session_management_messages,
	call_control,
	GPRS_Transparent_Transport_Protocol,
	mobility_management_messages,
	radio_resources_management_messages,
	EPS_mobility_management_messages,
	GPRS_mobility_management_messages,
	SMS_messages,
	GPRS_session_management_messages,
	non_call_related_SS_messages,
	ERROR = -1
};

class StateMachine
{
public:
	ProcedureType procedure;
	ProtocolType protocol;
	NASProtocolDiscriminator nas_pd;

	int state_index;

	int attach_states[5] = {AttachStates::ATTACH_REQUEST, AttachStates::AUTHENTICATION_RESPONSE,
							AttachStates::SECURITY_MODE_COMPLETE, AttachStates::ESM_INFO_RESPONSE,
							AttachStates::ATTACH_COMPLETE};

	int service_request_states[3] = {ServiceRequestStates::SERVICE_REQUEST, ServiceRequestStates::SR_INITIAL_CONTEXT_SETUP_RESPONSE, ServiceRequestStates::E_RAB_MODIFICATION_RESPONSE};
	int dpcm_service_request_states[2] = {ServiceRequestStates::SERVICE_REQUEST, ServiceRequestStates::E_RAB_MODIFICATION_RESPONSE};
	HandoverCompareable handover_states[7] = {{0, 0, 16, HANDOVER_REQUIRED}, {1, 1, -1, HANDOVER_REQUEST_ACK}, {0, 24, -1, ENB_STATUS_TRANSFER}, {0, 2, -1, HANDOVER_NOTIFY}, {1, 23, -1, UE_CONTEXT_RELEASE_COMPLETE}, {0, 18, 0, UE_CONTEXT_RELEASE_REQUEST}, {1, 23, -1, UE_CONTEXT_RELEASE_COMPLETE}};

	StateMachine()
	{
		nas_pd = NASProtocolDiscriminator::EPS_mobility_management_messages;

		state_index = -1;
	}

	enum class ProcedureType getProcedureType(){
		return procedure;
	}

	void setProcedure(uint8_t proc)
	{
		if (proc == 0)
		{
			protocol = ProtocolType::NAS;
			procedure = ProcedureType::ATTACH;
		}
		else if (proc == 1)
		{
			protocol = ProtocolType::S1AP;
			procedure = ProcedureType::HANDOVER;
		}
		else if (proc == 2)
		{
			protocol = ProtocolType::S1AP;
			procedure = ProcedureType::FAST_HANDOVER;
		}
		else if (proc == 3)
		{
			protocol = ProtocolType::S1AP;
			procedure = ProcedureType::SERVICE_REQUEST;
		}
		else if (proc == 4)
		{
			protocol = ProtocolType::S1AP;
			procedure = ProcedureType::DPCM_SERVICE_REQUEST;
		}
		else
		{
			printf("Undefined Procedure type\n");
			exit(1);
		}
	}
};

#endif

#ifndef ENUM_PHONECALLBEHAVIOR_H
#define ENUM_PHONECALLBEHAVIOR_H

enum PhoneCallBehavior 
{
	ePhoneCallBehavior_none = -1,
	ePhoneCallBehavior_ringPhone = 0,
	ePhoneCallBehavior_tripsMomMsgInterrupted,
	ePhoneCallBehavior_tripsMomOnPhone,
	ePhoneCallBehavior_gracesDadMsgInterrupted,
	ePhoneCallBehavior_gracesDadOnPhone,
	ePhoneCallBehavior_answeringMachineOutgoing,
	ePhoneCallBehavior_answeringMachineMessage,
	ePhoneCallBehavior_tripsMomAnsweringMachine,
	ePhoneCallBehavior_gracesDadAnsweringMachine
};

#endif // ENUM_PHONECALLBEHAVIOR_H
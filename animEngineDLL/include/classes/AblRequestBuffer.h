#ifndef ABLREQUESTBUFFER_H
#define ABLREQUESTBUFFER_H

#include "facadecommon.h"

struct AblRequest
{
	int active;
	EAblRequestType type;
	char name[32];
	uint32_t requesttime;
	ESpriteID unknown000;
	ESpriteID unknown001;
	int unknown002;
	int unknown003;
	int unknown004;
	int unknown005;
	EAnimLayer unknown006;
	int unknown007;
	int unknown008;
	float unknown009;
	float unknown010;
	float unknown011;
	int unknown012;
	float unknown013;
	float unknown014;
	float unknown015;
	float unknown016;
	int unknown017;
	int unknown018;
	int unknown019;
	int unknown020;
	int unknown021;
	int unknown022;
	int unknown023;
	int unknown024;
	int unknown025;
	int unknown026;
	int unknown027;
	int unknown028;
	int unknown029;
	int unknown030;
	int unknown031;
	int unknown032;
	int unknown033;
	int unknown034;
	int unknown035;
	int unknown036;
	int unknown037;
	int unknown038;
	int unknown039;
	int unknown040;
	int unknown041;
	int unknown042;
	int unknown043;
	int unknown044;
	int unknown045;
	int unknown046;
	int unknown047;
	int unknown048;
	int unknown049;
	int unknown050;
	int unknown051;
	int unknown052;
	int unknown053;
	int unknown054;
	int unknown055;
	int unknown056;
	int unknown057;
	int unknown058;
	int unknown059;
	int unknown060;
	int unknown061;
	int unknown062;
	int unknown063;
	int unknown064;
	int unknown065;
	int unknown066;
	int unknown067;
	int unknown068;
	int unknown069;
	int unknown070;
	int unknown071;
	int unknown072;
	int unknown073;
	int unknown074;
	int unknown075;
	int unknown076;
	int unknown077;
	int unknown078;
	int unknown079;
	int unknown080;
	int unknown081;
	int unknown082;
	int unknown083;
	int unknown084;
	int unknown085;
	int unknown086;
	int unknown087;
	int unknown088;
	int unknown089;
	int unknown090;
	int unknown091;
	int unknown092;
	int unknown093;
	int unknown094;
	int unknown095;
	int unknown096;
	int unknown097;
	int unknown098;
	int unknown099;
	int unknown100;
	int unknown101;
	int unknown102;
	int unknown103;
	int unknown104;
	int unknown105;
	ObjSensePacket unknown106[eSpriteID_NUMOBJECTS+1];
	ObjStateSensePacket unknown302[eSpriteID_NUMOBJECTS + 1];
	HeldObjectSensePacket unknown400[eSpriteID_NUMOBJECTS + 1];
	AnimCueSensePacket unknown596;
	ObjectActivationSensePacket unknown620[eSpriteID_NUMOBJECTS + 1];
	int unknown718;
	int unknown719;
	int unknown720;
	int unknown721;
	int unknown722;
	int unknown723;
	int unknown724;
	int unknown725;
	int unknown726;
	int unknown727;
	int unknown728;
	int unknown729;
	int unknown730;
	int unknown731;
	int unknown732;
	int unknown733;
	int unknown734;
	int unknown735;
	int unknown736;
	int unknown737;
	int unknown738;
	int unknown739;
	int unknown740;
	int unknown741;
	int unknown742;
	int unknown743;
	int unknown744;
	int unknown745;
	int unknown746;
	int unknown747;
	int unknown748;
	int unknown749;
	int unknown750;
	int unknown751;
	int unknown752;
	int unknown753;
	int unknown754;
	int unknown755;
	int unknown756;
	int unknown757;
	int unknown758;
	int unknown759;
	int unknown760;
	int unknown761;
	int unknown762;
	int unknown763;
	int unknown764;
	int unknown765;
	int unknown766;
	int unknown767;
	int unknown768;
	int unknown769;
	int unknown770;
	int unknown771;
	int unknown772;
	int unknown773;
	int unknown774;
	int unknown775;
	int unknown776;
	int unknown777;
	int unknown778;
	int unknown779;
	int unknown780;
	int unknown781;
	int unknown782;
	int unknown783;
	int unknown784;
	int unknown785;
	int unknown786;
	int unknown787;
	int unknown788;
	int unknown789;
	int unknown790;
	int unknown791;
	int unknown792;
	int unknown793;
	int unknown794;
	int unknown795;
	int unknown796;
	int unknown797;
	int unknown798;
	int unknown799;
	int unknown800;
	int unknown801;
	int unknown802;
};

struct unknownablstructure1
{
	char name[100];
	int unknown[7];
};

struct unknownablstructure2
{
	char name[64];
	int unknown001;
	int unknown002;
	int unknown003;
	int unknown004;
	int unknown005;
	int unknown006;
	int unknown007;
	int unknown008;
	int unknown009;
	int unknown010;
	int unknown011;
	int unknown012;
	int unknown013;
	int unknown014;
	int unknown015;
	int unknown016;
	int unknown017;
	int unknown018;
	int unknown019;
	int unknown020;
	int unknown021;
	int unknown022;
	int unknown023;
	int unknown024;
	int unknown025;
	int unknown026;
	int unknown027;
	int unknown028;
	int unknown029;
	int unknown030;
	int unknown031;
	int unknown032;
	int unknown033;
	int unknown034;
	int unknown035;
	int unknown036;
	int unknown037;
	int unknown038;
	int unknown039;
	int unknown040;
	int unknown041;
	int unknown042;
	int unknown043;
	int unknown044;
	int unknown045;
	int unknown046;
	int unknown047;
	int unknown048;
	int requesttime;
	int spriteid1;
	int spriteid2;
	int unknown052;
	int unknown053;
	int unknown054;
	int unknown055;
	int unknown056;
	int unknown057;
};

static_assert(sizeof(AblRequest) == 3256, "");

#define REQUESTBUFFERSIZE 100

class AblRequestBuffer
{
public:
	virtual ~AblRequestBuffer() = default;

	void InitBuffer(void* mappedmemptr, bool resetmemory);

	int PerformAllRequests();


	bool CheckForBaseAnimRequestsQueuedUp(int unknown);
	bool CheckForCallbacksPending(EAblRequestType type, int unknown1, int unknown2);
	bool DoesAnimNeedToWaitForLocomotionToFinish(ESpriteID sprite, int unknown);

	int GetFreeRequestPtr(AblRequest*& out);
	void GetRequestPtrByIndex(int i, AblRequest*& out);
	void DeactiveWalkstepCallbacks(ESpriteID spriteid);

	void PerformAnyRequestedCueTriggeredPerformance(ESpriteID spriteid);
	int PerformRequest(int reqindex, bool abortonly);

	void PrintTraceBuffer();
	void SaveRequestInTrace(AblRequest* req);

	void QueueUpRequestedPerformance(ESpriteID spriteid, /*PerformanceInfo*/void* performanceinfo, int randomnum);
	void TickDownAndPerformRequestedPerformances(ESpriteID spriteid);


	unknownablstructure1* unknown001;

private:
	AblRequest* m_pAblRequests;
	unknownablstructure2* unknown002;
	int unknown003;
	int unknown004;
	int unknown005;
	int unknown006;
	int unknown007;
	int unknown008;
	int unknown009;
	int unknown010;
};

static_assert(sizeof(AblRequestBuffer) == 48, "");


extern AblRequestBuffer* g_ablrequestbuffer;

#endif // ABLREQUESTBUFFER_H
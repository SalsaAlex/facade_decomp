#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller
{
public:
	virtual ~Controller() = default;

	void InitController(float* valptr, float valmin, float valmax, bool b1);

	void SetTargetByFrames(float target, int numframes, EControllerType controltype);
	void SetTargetByRate(float target, float rate, EControllerType controltype, float f1);
	void GetRateByFrames(float f1, int numframes);


	void SetVal(float value, bool keepMoving);
	float GetVal();

	bool GetIsMoving();
	void StopMoving();

	void UpdateTarget();
	void UpdateController();
	void Offset();



private:
	float* m_valPtr = nullptr;
	float m_valMin;
	float m_valMax;
	bool m_bIsMoving = false;
	EControllerType m_eControllerType;
	float m_valTarget;
	int unknown6;
	float m_fLastVal;
	int unknown8;
	int unknown9;
	int unknown10;
	bool unknown11;
	int unknown12;
	int unknown13;
	int unknown14;
protected:

};

static_assert(sizeof(Controller) == 64, "");						//proof: animEngineDLL.dll:.text:1004D4D8 (g_autoTurnController is 64 bytes long)

#endif // CONTROLLER_H
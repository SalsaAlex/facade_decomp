#ifndef ANIMMGR_H
#define ANIMMGR_H

class AnimMgr
{
public:
	virtual ~AnimMgr() = default;

	void PushLayeredScript(int i1, int i2, int i3, int i4);

	bool IsLayerDone(int layer);

	int unknown[80935];

private:
protected:

};

static_assert(sizeof(AnimMgr) == 323744, ""); //estimate

#endif // ANIMMGR_H
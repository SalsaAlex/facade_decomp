#ifndef ANIMMGR_H
#define ANIMMGR_H

class AnimMgr
{
public:
	virtual ~AnimMgr() = default;

	int unknown[80935];

private:
protected:

};

static_assert(sizeof(AnimMgr) == 323744, ""); //estimate

#endif // ANIMMGR_H
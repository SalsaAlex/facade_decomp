#ifndef SPRITE_H
#define SPRITE_H

class Sprite
{
public:
	virtual void InitSprite(ESpriteID spriteid, const char* name, bool hasmodel, bool drawable, int nummodelpoints = -1);
	virtual void UpdateSprite();
	virtual bool GetIsAnimatingSprite();
	virtual bool GetIsCharacterSprite();
	virtual bool GetIsHeadSprite();
	virtual const char* GetName();
	virtual bool GetUpdateMe();
	virtual void SetUpdateMe(bool option);
	virtual bool GetDrawMe();
	virtual void SetDrawMe(bool option);
	virtual XYZPointf GetPosition();
	virtual XYZPointf GetRotation();
	virtual float GetScale();
	virtual XYZPointf GetVolume();
	virtual void SetPosition(XYZPointf pos, float);
	virtual void SetRotation(XYZPointf rot);
	virtual void SetScale(float scale);
	virtual void SetVolume(XYZPointf vol);
	virtual XYZPointf GetAttachPosition();
	virtual void SetAttachPosition(XYZPointf attachpos);
	virtual void GetPickupPositionAndRotation(XZPointf& pos, float& rot);
	virtual void GetSurfaceDropPositionAndRotation(int, XZPointf& pos, float& rot);
	virtual void AddSurfaceObjectPositionAndRotationOffset(int, XZPointf& pos, float& rot);
	virtual XYZPointf GetModelPointPosition(int);
	virtual ETextureFilmstrip GetPanelSpriteTextureFilmstrip();
	virtual void SetPanelSpriteTextureFilmstrip(ETextureFilmstrip filmstrip);
	virtual void SetTextureFrameAnimation(int, int, int, bool);
	virtual void SetScreenText(const char* text, bool);
	virtual bool GetGrabable();
	virtual void SetGrabable(bool option);
	virtual bool GetClickable();
	virtual void SetClickable(bool option);
	virtual bool GetClickedThisFrame();
	virtual void SetClickedThisFrame(bool option);
	virtual ECollidableType GetCollidableType();
	virtual void SetCollidableType(ECollidableType collidetype);
	virtual bool GetAmIHoldingSprite();
	virtual void* GetHeldSpritesList();
	virtual int GetNumHeldSprites();
	virtual Sprite* GetFirstSpriteAttachedToModelPoint(int);
	//virtual GetSpriteListAttachedToModelPoint(int, Sprite**, int*);
	virtual Sprite* GetNthHeldSpriteAttachModelPoint(int);
	virtual void SetSpriteToHold(Sprite* sprite, int, int);
	virtual void DropSprite(Sprite* sprite);
	virtual void DropAllSprites();
	virtual Sprite* GetSpriteHoldingMe();
	virtual void SetSpriteHoldingMe(Sprite* sprite);
	virtual bool GetAmIStillGrabDropAnimating();
	//virtual Get2DBoundingBox(
	//virtual Set2DBoundingBox(
	virtual void AddSurface(int i1, int i2, int i3, int i4, ESurfacePickupAngle surfpickangle);
	//virtual GetSurfaceListPtr(
	//virtual GetNumSurfaces(
	//virtual AddSpriteToSurface(
	//virtual RemoveSpriteFromSurface(
	//virtual UpdateSurfaces(
	//virtual FindSurfaceIAmOn(
	//virtual RemoveFromAnySurfaceIMayBeOn(
	//virtual GetModelPointCursorIsOver(
	//virtual GetModelPointXRect(

	int unknown001;
	char name[64];
	void* modelframe;
	void* modelinfo;
	void* modelquadlines;
	int unknown002[2];
	void* modelquadpoly;
	int unknown003[21005];

private:

protected:

};

static_assert(sizeof(Sprite) == 84116, "");						//proof: animEngineDLL.dll:.text:1007739D

#endif // SPRITE_H
#pragma once

class Actor : public IGameObject
{
protected:
	Actor() {};
	virtual ~Actor() {};


protected:
	virtual bool Start() override{ return true; }
	virtual void Update() override{}
	virtual void Render(RenderContext& rc) override{}
};
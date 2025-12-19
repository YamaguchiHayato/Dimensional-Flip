#pragma once
#include "Src/Actor/Actor.h"


class Character : public Actor
{
protected:
    Character() {};
    virtual ~Character() {};


protected:
	virtual bool Start() override{ return true; }
	virtual void Update() override{}
	virtual void Render(RenderContext& rc) override{}

};

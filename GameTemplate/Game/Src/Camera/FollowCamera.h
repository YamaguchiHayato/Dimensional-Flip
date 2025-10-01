#pragma once
#include "ICameraStrategy.h"
class FollowCamera : public ICameraStrategy
{
public:
	void Update(GameCamera* owner) override;
};


#pragma once
#include "Src/camera/ICameraStrategy.h"

class Player;
class FollowStrategy : public ICameraStrategy
{
public:
    // 暗黙的な型変換を禁止する。
	explicit FollowStrategy(Player* pPlayer);
	virtual ~FollowStrategy() {};


public:
	bool Start() override;
	void Update() override;
};

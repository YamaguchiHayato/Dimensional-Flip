/*
 * @file ICameraStrategy.h
 * @brief カメラのStrategyパターンの基底クラス。
 */

#pragma once
class GameCamera;
class ICameraStrategy 
{
public:
	virtual ~ICameraStrategy() = default;

	/*
	 * @brief カメラの毎フレーム更新。 
	 */
	virtual void Update(GameCamera* owner) = 0;

};


#pragma once
#include "Src/Production/AttackMarker.h"

namespace app{
    namespace production{
        class AttackMarker;
    }
}

class Player;
class CutInView;

class Star : public IGameObject
{
public:
	Star() = default;
	virtual ~Star() = default;


public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

        // 移動処理全体を更新。
    void UpdateMovement(float deltaTime);

    // 演出終了時の処理。
    void OnEffectFinished();

private:
    // 回転処理。
	void Rotation();



// セッター。
public:
    // 座標をセット。
	inline void SetStarPosition(const Vector3& starPosition)
	{
		m_starPosition = starPosition;
	}

    // 演出をする時間をセット。
    inline void SetDisplayDuration(float duration)
    {
        displayDuration_ = duration;
    }

// ゲッター。
public:
	inline const Vector3& GetStarPosition()
	{
		return m_starPosition;
	}


private:
	Player* m_player = nullptr;
    CutInView* pCutInView_ = nullptr;


private:
	ModelRender m_starRender;

	Vector3 m_starPosition = Vector3::Zero;

    Quaternion m_starRotation = Quaternion::Identity;
    Quaternion currentRotation_ = Quaternion::Identity;

    bool m_isGoal = false;
    bool isCollected_ = false;

    float disPlayTimer_ = 0.0f;
    float displayDuration_ = 0.0f;
};


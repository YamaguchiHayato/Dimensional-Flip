#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

enum class FoolState : uint8_t
{
    UP,     // 上へ移動中。
    TOP,    // 上で停止中。
    DOWN,   // 下へ移動中。
    BOTTOM  // 下で停止中。
};

class RotationFool : public IGimmic 
{
public:
    RotationFool() {};
    virtual ~RotationFool();

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline const std::string InitGimmick(const std::string& gimmickname) override
    {
        return IGimmic::InitGimmick(gimmickname);
    };

private:
    void Move(float deltatTime);

    // セッター。
public:
    // ギミックの停止時間を設定。
    inline const void SetStopTime(const float& stopTime)
    {
        stopTime_ = stopTime;
    }
    // initPosを取得。
    inline const void SetInitPos(const Vector3& initPos)
    {
        initGimmickPos_ = initPos;
    }
    // ギミックの上限座標を取得。
    inline const void SetTopPos(const Vector3& topPos)
    {
        topPos_ = topPos;
    }
    // ギミックの座標を設定。
    inline const void SetPos(const Vector3& rotFoolPos)
	{
		gimmickPos_ = rotFoolPos;
	}
    // ギミックの大きさを設定。
    inline void SetScale(const Vector3& scale)
    {
        scale_ = scale;
	}
    // ギミックの移動速度を取得。
    inline const void SetMoveSpeed(const float& moveSpeed)
    {
        moveSpeed_ = moveSpeed;
    }

// ゲッター。
public:
    // ギミックの座標を取得。
    inline const Vector3 GetPos() const
    {
        return gimmickPos_;
    }
    // ギミックの座標を取得。
    inline const Vector3 GetTopPos() const
    {
        return topPos_;
    }
    // initPosを取得。
    inline const Vector3 GetInitPos() const
    {
        return initGimmickPos_;
    }

private:
    Vector3 scale_ = Vector3::One;  // ギミックの大きさ。
    Vector3 pos_ = Vector3::Zero;   // ギミックの座標。
    Vector3 topPos_ = Vector3::Zero;// ギミックの上の座標。  

private:
    Player* pPlayer = nullptr;

private:
    float moveSpeed_ = 100.0f;             // 移動速度。
    float stopTime_ = 2.0f;               // 停止時間。
    FoolState foolState_ = FoolState::TOP; // ギミックステートの初期値。

};

#pragma once


class IState
{
public:
    IState() =default;
    virtual ~IState() = default;


public:
    // 1回処理される処理。
    virtual void Enter() = 0;
    // 更新処理。
    virtual void Update() = 0;
    // 終了判定。
    virtual void Exit() = 0;
    // ステート遷移要求処理。
    virtual bool RequestID(uint8_t& request) = 0;

public:
    // モデルのパスを取得。
    virtual const std::string InitStage(const std::string& stagename)
    {
        std::string Stagepath = "Assets/stage/" + stagename + ".tkm";
        return Stagepath;
    };

// ゲッター。
public:
    virtual Vector3 GetStageStartPos() const
    {
        return startPos_;
    }


protected:
    ModelRender render_;

    Vector3 startPos_ = Vector3(0.0f, -3.0f, 0.0f);
    Vector3 initPos_ = Vector3::Zero;
    PhysicsStaticObject physics_;

};


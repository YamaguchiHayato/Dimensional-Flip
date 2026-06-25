#pragma once
#include "Src/StateMachine/IActorState.h"

/**
 * @brief 既存キャラクターステートの基底クラス。
 * @note RequestTransition は RequestID に委譲し、新 StateMachine と互換にする。
 */
class IState : public nsApp::nsFunction::IActorState
{
public:
    IState() = default;
    virtual ~IState() = default;

public:
    /**
     * @brief IActorState 互換の遷移要求。
     * @param[out] request 遷移先ステート ID。
     * @return 遷移する場合 true。
     */
    bool RequestTransition(uint8_t& request) override { return RequestID(request); }

    /** @brief ステート開始時の処理。 */
    virtual void Enter() = 0;

    /** @brief ステート更新処理。 */
    virtual void Update() = 0;

    /** @brief ステート終了時の処理。 */
    virtual void Exit() = 0;

    /**
     * @brief ステート遷移要求処理。
     * @param[out] request 遷移先ステート ID。
     * @return 遷移する場合 true。
     */
    virtual bool RequestID(uint8_t& request) = 0;

public:
    /**
     * @brief モデルのパスを取得。
     * @param[in] stagename ステージ名。
     * @return ステージモデルのパス。
     */
    virtual const std::string InitStage(const std::string& stagename)
    {
        std::string Stagepath = "Assets/stage/" + stagename + ".tkm";
        return Stagepath;
    }

public:
    /**
     * @brief ステージ開始座標を取得する。
     * @return 開始座標。
     */
    virtual Vector3 GetStageStartPos() const { return startPos_; }

protected:
    ModelRender render_;                            ///< モデルレンダラ。
    Vector3 startPos_ = Vector3(0.0f, -3.0f, 0.0f); ///< 開始座標。
    Vector3 initPos_ = Vector3::Zero;               ///< 初期座標。
    PhysicsStaticObject physics_;                   ///< 物理オブジェクト。
};

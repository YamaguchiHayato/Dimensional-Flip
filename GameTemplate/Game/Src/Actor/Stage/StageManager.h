#pragma once
#include "Src/Actor/Stage/IStage.h"

class StageManager : public IStage
{
private:
    IStage* pCurrentStage_ = nullptr;
    StageID currentID_s_ = StageID::sStage1;
    StageID requestID_s_ = StageID::sInvalid;

public:
    // 初期化処理。
    bool Start() override;
    // 更新処理。
    void Update() override;
    // 描画処理。
    void Render(RenderContext& rc) override;
    // ステージの生成処理。
    IStage* CreateStage(StageID id);

// セッター。
public:
    // ステージの変更処理。
    inline void ChangeStage(StageID newStageID)
    {
        requestID_s_ = newStageID;
    }

// ゲッター。
public:
    // ステージ開始位置の取得。
    inline Vector3 GetStageStartPos() const override
    {
        if (pCurrentStage_)
        {
            return pCurrentStage_->GetStageStartPos();
        }
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    // 現在のステージIDを取得。
    // ステージ変更検知で使用。
    inline StageID GetCurrentStageID() const
    {
        return currentID_s_;
    }
private:
    StageManager() {};
    virtual ~StageManager() {};

// static メンバー。
private:
    static StageManager* pStageManger_;

public:
    // シングルトンインスタンスの生成。
    inline static void CreateInstance()
    {
        if (!pStageManger_)
            pStageManger_ = new StageManager();
    }

    // シングルトンインスタンスを取得
    inline static StageManager* GetInstance()
    {
        return pStageManger_;
    }

    // ステージの解放処理。
    inline static void DeleteInstance()
    {
        delete pStageManger_;
        pStageManger_ = nullptr;
    }
};

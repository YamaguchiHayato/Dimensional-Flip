#pragma once
#include "Src/Actor/Stage/IStage.h"
#include "Src/Actor/Actor.h"

namespace StageStartPos
{
//    const Vector3 stage2(0.0f, 100.0f, 0.0f);
    const Vector3 stage2(3000.0f, 800.0f, -60.0f);
}

class LoadingScene;
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
    void Update()override;
    // 描画処理。
    void Render(RenderContext& rc) override;
    // ステージの生成処理。
    IStage* CreateStage(StageID id);
    // ステージの同期的変更処理。
    void ChangeStageSync(StageID newStageID);
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
        return StageStartPos::stage2;
    }
    // 現在のステージIDを取得。
    // ステージ変更検知で使用。
    inline StageID GetCurrentStageID() const
    {
        return currentID_s_;
    }

    StageManager() {};
    virtual ~StageManager();


private:
    // static メンバー。
    static StageManager* pStageManger_;

public:
    // シングルトンインスタンスの生成。
    inline static void CreateInstance()
    {
        if (!pStageManger_)
            pStageManger_ = NewGO<StageManager>(0, "stagemanager");
    }

    // シングルトンインスタンスの破棄。
    inline static void DeleteInstance()
    {
         DeleteGO(pStageManger_);
        pStageManger_ = nullptr;
    }

    // シングルトンインスタンスを取得
    inline static StageManager* GetInstance()
    {
        return pStageManger_;
    }

};

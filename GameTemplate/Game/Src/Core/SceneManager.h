#pragma once
#include "Src/Scene/Scene.h"

// フェード処理の列挙型
// ロードシーンはステージの描画を待つため細分化。
enum class SceneTransitionState : uint8_t
{
    None,            // 通常状態。
    FadeOut,         // フェードアウト中。
    Load,            // ロード準備。
    Load_Render,     // ローディング画面の準備。
    Load_Wait,       // 同期ロード実行。
    Load_WaitFinish, // ロード完了待ち。
    FadeIn,          // フェードイン中。
    FadeIn_Wait,     // フェードイン完了待ち。
};

class Fade;
class SceneManager : public IScene
{
private:
    IScene* pCurrentScene_ = nullptr;
    SceneID currentID_ = SceneID::sTitle;
    SceneID requestID_ = SceneID::sInvalid;
    SceneID targetID = SceneID::sInvalid;

public:
    // 初期化処理。
    bool Start() override;
    // 更新処理。
    void Update() override;
        IScene* CreateScene(SceneID id);

// セッター。
public:
    // FadeSceneから呼ばれる内部用のシーン変更
    inline void ChangeSceneInternal(SceneID newID)
    {
        requestID_ = newID;
    }

    // シーンの変更処理。
    inline void ChangeScene(SceneID newID)
    {
        requestID_ = newID;
        //requestID_ = SceneID::sFade;
    }


private:
    SceneManager(){};
    virtual ~SceneManager();

private:
    static SceneManager* pSceneManger_;
    Fade* pFade_ = nullptr;

public:
    // シングルトンインスタンスの生成。
    inline static void CreateInstance()
    {
        if (!pSceneManger_)
            pSceneManger_ = new SceneManager();
    }
    // Fadeシーンの生成管理。
    inline Fade* GetFade()
    {
        return pFade_;
    }

    // ターゲットシーンIDの取得。
    SceneID GetTargetSceneID() const
    {
        return targetID;
    }

    // シングルトンインスタンスを取得
    inline static SceneManager* GetInstance()
    {
        return pSceneManger_;
    }

    // シーンの解放処理。
    inline static void DeleteInstance()
    {
        delete pSceneManger_;
        pSceneManger_ = nullptr;
    }
};

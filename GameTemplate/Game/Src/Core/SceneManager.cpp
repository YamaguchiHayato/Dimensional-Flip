#include "stdafx.h"
#include "Src/Production/Fade.h"

#include "Src/Scene/TitleScene.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/GameClear.h"
#include "Src/Scene/GameClearScene.h"
#include "Src/Scene/GameOverScene.h"
#include "Src/Scene/WorldSelectScene.h"
#include "Src/Scene/LoadingScene.h"
#include "Src/Scene/EndRollScene.h"
#include "Src/UI/Select/WorldSelectUI.h"

SceneManager* SceneManager::pSceneManger_ = nullptr;


SceneManager::~SceneManager()
{
    if (pCurrentScene_ != nullptr)
    {
        delete pCurrentScene_;
        pCurrentScene_ = nullptr;
    }

    if (pFade_ != nullptr)
    {
        DeleteGO(pFade_);
        pFade_ = nullptr;
    }
}


bool SceneManager::Start()
{
    // フェードシーンの生成。
    pFade_ = NewGO<Fade>(2, "fade");
    if (pFade_ == nullptr)
    {
        return false;
    }
    // 最初のシーンを生成。
    pCurrentScene_ = CreateScene(SceneID::sTitle);
    if (pCurrentScene_)
    {
        pCurrentScene_->Start();
        // シーンの初期値を設定する。
        currentID_ = SceneID::sTitle;
        return true;
    }
    return false;
} 


void SceneManager::Update()
{
    if (isAutoLoadingEnabled_ && pFade_ && pFade_->GetFadeState() == FadeState::Fade_Out)
    {
        if (pFade_->GetFadeSprite().GetWipeSize() < 300.0f)
        {
            ShowLoading();
        }
    }

    // 2. シーン遷移リクエストがあった場合の初期化処理
    if (requestID_ != SceneID::sInvalid && !isLoadingSceneActive_)
    {
        // ロード画面を表示
        ShowLoading();

        // フラグとタイマーのリセット
        isLoadingSceneActive_ = true;
        isSceneControler_ = false;
        minLoadingTime_ = 0.0f;

        return; // ロード画面を描画するために一旦リターン
    }

    // 3. ロード中の処理
    if (isLoadingSceneActive_)
    {
        // A. まだ次のシーンを作っていないなら、作る（重い処理）
        if (!isSceneControler_)
        {
            IScene* nextScene = CreateScene(requestID_);

            if (nextScene != nullptr)
            {
                nextScene->Start();

                // 古いシーンを削除
                if (pCurrentScene_)
                {
                    delete pCurrentScene_;
                }

                // 新しいシーンをセット
                pCurrentScene_ = nextScene;
                currentID_ = requestID_;
            }

            // 生成完了フラグを立てる
            isSceneControler_ = true;
        }

        // B. 待機時間の更新（ここが修正のキモです！）
        float dt = g_gameTime->GetFrameDeltaTime();

        // ★重要修正: ロード処理(CreateScene)で止まっていた時間を無視する。
        // これがないと、ロードにかかった時間(例:1.0秒)がそのまま足されてしまい、
        // 待機時間(0.5秒)を一瞬で超えて即座に消えてしまいます。
        if (dt > 0.1f)
        {
            dt = 0.1f;
        }

        minLoadingTime_ += dt; // 補正した時間を足す

        // C. 指定時間が経過したらロード画面を消す
        // 点滅を見せたい場合は 0.5f ～ 1.0f くらいで調整してください
        if (minLoadingTime_ >= 0.5f)
        {
            // リクエスト情報をクリア
            requestID_ = SceneID::sInvalid;
            isLoadingSceneActive_ = false;

            // ロード画面を非表示にする
            HideLoading();
        }
    }

    // 4. 現在のシーンの更新
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Update();
    }
}


void SceneManager::ShowLoading()
{
    if (pLoadingScene_ == nullptr)
    {
        // 優先度3（Fadeより手前）で生成
        pLoadingScene_ = NewGO<LoadingScene>(3, "loading");
    }
}


void SceneManager::HideLoading()
{
    if (pLoadingScene_ != nullptr)
    {
        DeleteGO(pLoadingScene_);
        pLoadingScene_ = nullptr;
    }
}



IScene* SceneManager::CreateScene(SceneID id)
{
    IScene* newScene = nullptr;
    switch (id)
    {
        // ケース: タイトルシーン。
        case SceneID::sTitle:
             newScene= new TitleScene();
             break;

        case SceneID::sWorldSelect:
             newScene = new WorldSelectScene();
             break;

        case SceneID::sInGame:
             newScene = new InGameScene();
             break;

        case SceneID::sResult:
             newScene = new GameClearScene();
             break
                 ;
        case SceneID::sGameOver:
             newScene = new app::scene::GameOverScene();
             break;

        case SceneID::sEndRoll:
             newScene = new app::production::EndRollScene();
             break;

        default:
             break;
    }

    return newScene;
 }

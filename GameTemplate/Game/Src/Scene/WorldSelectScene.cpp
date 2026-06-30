#include "stdafx.h"

#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Core/StageManager.h"

#include "Src/Production/Fade.h"


#include "WorldSelectScene.h"

#include "Src/UI/Select/WorldSelectUI.h"



namespace
{
    const float RADIUS = 25.0f;


    struct StageData
    {
        const char* path;
        StageID id;
        const wchar_t* name;
    };


    std::vector<StageData> dataList =
    {
        // ステージモデル、ステージID、字幕をセット。
        {"Assets/stage/Stage1/Stage1.tkm", StageID::sStage1, L"Stage1"},
        {"Assets/stage/tutorialStage.tkm", StageID::sTutorialStage, L"Tutorial Stage"},
        {"Assets/stage/WorldSelect/BossStage.tkm", StageID::sStageEX, L"Boss Stage"},
    };
} 


WorldSelectScene::~WorldSelectScene()
{
    // ステージアイコンモデルの削除。
    for (auto* icon : icons_)
    {
        DeleteGO(icon);
    }
    // アイコンリストをクリア。
    icons_.clear();

    // UIの削除。
    if (pSelectUI_)
    {
        DeleteGO(pSelectUI_);
        pSelectUI_ = nullptr;
    }

    // SkyCubeの削除。
    if (pSkyCube_)
    {
        DeleteGO(pSkyCube_);
        pSkyCube_ = nullptr;
    }
}


bool WorldSelectScene::Start()
{
    pFade_ = SceneManager::GetInstance()->GetFade();
    if (pFade_)
        pFade_->StartFadeIn();

    if (g_camera3D)
    {
        g_camera3D->SetPosition({0.0f, 20.0f, -40.0f});
        g_camera3D->SetTarget({0.0f, 0.0f, 0.0f});
    }

    app::core::SoundManager::GetInstance()->PlayBGM(GameSoundList_BGM_StageSelect);

    // ① 描画状態を先にリセット
    if (g_renderingEngine)
    {
        g_renderingEngine->EnableCompositeBackground(false);
        g_renderingEngine->SetStageBackGroundRenderer(nullptr);

        /* IBLなしのWorldSelectScene用のライティング。*/
        g_renderingEngine->SetAmbient(Vector3(0.4f, 0.40f, 0.5f));
    }

    CreateSkyCube();

    // ③ その後に ModelRender を持つオブジェクト
    for (const auto& data : dataList)
    {
        auto* icon = NewGO<app::nsUI::StageIcon>(0, "StageIcon");
        icon->Init(data.path, data.id);
        icons_.push_back(icon);
    }

    pSelectUI_ = NewGO<app::nsUI::WorldSelectUI>(1, "WorldSelectUI");
    pSelectUI_->Init();

    return true;
}


void WorldSelectScene::Update()
{
    // フェード中は操作させない
    if (!isDecided_ && pFade_ && pFade_->IsFadeOutEnd() == false)
    {
        // 右入力。
        bool isRight = g_pad[0]->IsTrigger(enButtonRight) || g_pad[0]->IsTrigger(enButtonRight);
        if (isRight)
        {
            currentIndex_++;
            if (currentIndex_ >= icons_.size())
                currentIndex_ = 0;

            // カーソル移動音SE。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_CursorMove,3.0f);
        }

        // 左入力。
        bool isLeft = g_pad[0]->IsTrigger(enButtonLeft) || g_pad[0]->IsTrigger(enButtonLeft);
        if (isLeft)
        {
            currentIndex_--;
            if (currentIndex_ < 0)
                currentIndex_ = (int) icons_.size() - 1;
        }

        // 決定。
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            isDecided_ = true;
            pFade_->StartFadeOut();

            // 決定音SE。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Button, 3.0f);
        }

        // キャンセル。
        if (g_pad[0]->IsTrigger(enButtonB))
        {
            isButtonB_ = true;
            pFade_->StartFadeOut();

            // キャンセル音SE。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SelectScreen_Cancel);
        }
    }

    // UIを更新する。
    if (pSelectUI_)
        pSelectUI_->SetStageName(dataList[currentIndex_].name);

    // --- 回転計算 ---
    float angleStep = 360.0f / icons_.size();  // 1個あたりの角度
    targetAngle_ = -currentIndex_ * angleStep; // 目標角度

    // 滑らかに回転させる（線形補間）
    currentAngle_ += (targetAngle_ - currentAngle_) * 0.1f;

    // --- 各アイコンの座標更新 ---
    for (int i = 0; i < icons_.size(); i++)
    {
        float deg = currentAngle_ + (i * angleStep);
        float rad = Math::DegToRad(deg);

        // 円周上に配置
        Vector3 pos;
        pos.x = sinf(rad) * RADIUS;
        pos.y = 0.0f;
        pos.z = cosf(rad) * RADIUS;

        icons_[i]->SetPosition(pos);
    }

    // --- 遷移処理 ---
    if (pFade_ && pFade_->IsFadeOutEnd())
    {
        // 遷移する瞬間にBGMを停止する。
        app::core::SoundManager::GetInstance()->StopBGM(GameSoundList_BGM_StageSelect);

        // 決定時: ゲーム画面へ遷移。
        if (isDecided_)
        {
            // 念のため処理を呼ぶ。
            SceneManager::GetInstance()->ShowLoading();

            // 選んだステージIDを取得してセット
            StageID selectID = dataList[currentIndex_].id;

            // ステージ管理クラスに選択されたステージIDをセット。
            nsApp::nsStage::StageManager::SetNextInitStageID(selectID);

            // ゲーム画面へ
            SceneManager::GetInstance()->ChangeScene(SceneID::sInGame);
        }

        // キャンセル時: Bボタンアクションでタイトル画面へ遷移する。
        else if (isButtonB_)
        {
            SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);
        }
    }
}


void WorldSelectScene::CreateSkyCube()
{
    pSkyCube_ = NewGO<SkyCube>(0, "skycube");
    pSkyCube_->SetScale(Vector3::One * 100.0f);

    pSkyCube_->SetType(EnSkyCubeType::enSkyCubeType_Night);                           // 先にタイプ
}

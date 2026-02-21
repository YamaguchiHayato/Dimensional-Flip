#include "stdafx.h"

#include "Src/Core/Game.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Production/Fade.h"
#include "Src/Scene/title/TitleView.h"



TitleView::~TitleView()
{
    if (g_pad[0])
    {
        g_pad[0]->SetVibration(0, 0);
    }

    DeleteGO(pBackgroundLayer_);
    DeleteGO(pLogoLayer_);
    if (pTitleMenu_)
    {
        DeleteGO(pTitleMenu_);
        pTitleMenu_ = nullptr;
    }
}

bool TitleView::Start()
{
    pBackgroundLayer_ = NewGO<TitleBackgroundLayer>(0, "titleBackgroundLayer");
    pLogoLayer_ = NewGO<TitleLogoLayer>(1, "titlelogolayer");

    pTitleMenu_ = NewGO<app::nsUI::TitleMenu>(1, "TitleMenu");
    pTitleMenu_->Init();

    // ★ フェードオブジェクトをマネージャーから取得しておく
    m_pFade = SceneManager::GetInstance()->GetFade();

    return true;
}

void TitleView::Update()
{
    if (m_isStartedFadeOut)
    {
        if (m_pFade && m_pFade->IsFadeOutEnd())
        {
            // フェードが終わったので、実際にシーンを切り替える
            switch (m_selectedMenu)
            {
            case TitleMenuType::GameStart:
                app::core::StageManager::SetNextInitStageID(StageID::sTutorialStage);
                SceneManager::GetInstance()->ChangeScene(SceneID::sInGame);
                break;

            case TitleMenuType::Manual:
                // Manual画面への遷移など
                break;
            case TitleMenuType::GameEnd:
                PostQuitMessage(0);
                break;
            }
        }
        return; // フェード中はメニュー操作を受け付けない
    }

    // 入力取得
    bool isUp = g_pad[0]->IsTrigger(enButtonUp);
    bool isDown = g_pad[0]->IsTrigger(enButtonDown);
    bool isDecide = g_pad[0]->IsTrigger(enButtonA);

    // メニューに入力を渡して更新
    if (pTitleMenu_->Update(isUp, isDown, isDecide))
    {
        // --- 決定された瞬間の処理 ---

        // ★1. コントローラーの振動を即座に止める
        // 強力な振動(左)と微細な振動(右)を両方 0 に設定
        g_pad[0]->SetVibration(0, 0);

        // 2. 選択された項目を保存
        m_selectedMenu = pTitleMenu_->GetCurrentSelect();

        // 3. フェードアウトを開始
        if (m_pFade)
        {
            m_pFade->StartFadeOut();
            m_isStartedFadeOut = true;
        }
    }
}

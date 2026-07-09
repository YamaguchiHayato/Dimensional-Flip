#include "stdafx.h"

#include "InGameBuildHelper.h"
#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"
#include "Src/Parameter/Stage/StageMasterTable.h"
#include "Src/Presentation/UI/Screens/BossHubScreenHost.h"
#include "Src/Presentation/UI/Screens/BossHubScreen.h"
#include "Src/Presentation/UI/Screens/GameplayHudScreenHost.h"   
#include "Src/Presentation/UI/Screens/GameplayHudScreen.h"       
#include "Src/UI/Pause/PauseMenuUI.h"
#include "Src/Presentation/UI/Screens/SoundSettingScreenHost.h"
#include "Src/Presentation/UI/Screens/SoundSettingScreen.h"

namespace nsApp
{
    void InGameBuildHelper::Initialize(nsStage::StageID stageID)
    {
        /* 初期化処理。*/
        stageID_ = stageID;
        pBackGround_ = nullptr;
        buildFunctions_.clear();
        currentBuildIndex_ = 0;
        isFinished_ = false;
        isLoadSuccess_ = true;

        /* ビルド関数の初期化。*/
        InitializeBuildFunctions();
    }

    void InGameBuildHelper::InitializeBuildFunctions()
    {
        buildFunctions_.clear();
        buildFunctions_.push_back([this]() { BuildParameters(); });
        buildFunctions_.push_back([this]() { BuildBossHudUi(); });
        buildFunctions_.push_back([this]() { BuildGameplayHudUiStep(); });
        buildFunctions_.push_back([this]() { BuildPauseMenuUiStep(); });
        buildFunctions_.push_back([this]() { BuildSoundSettingUiStep(); });
        buildFunctions_.push_back([this]() { FinishBuild(); });
    }


    void InGameBuildHelper::BuildParameters()
    {
        /* パラメータシステムをロード。*/
        isLoadSuccess_ = parameterSystem_.LoadAll();
        if (!isLoadSuccess_)
            OutputDebugStringA("InGameBuildHelper::BuildParameters - LoadAll failed.\n");
    }


    void InGameBuildHelper::BuildBackGroundStep()
    {
        /* ステージ背景を生成。*/
        pBackGround_ = CreateBackGround(stageID_);
        if (pBackGround_ == nullptr)
            OutputDebugStringA("InGameBuildHelper::BuildBackGroundStep - CreateBackGround returned nullptr.\n");
    }


    void InGameBuildHelper::FinishBuild()
    {
        isFinished_ = true;
    }


    void InGameBuildHelper::BuildBossHudUi()
    {
        /* 既存 Host が死んでいれば作り直す。 */
        if (pBossHudHost_ != nullptr && FindGO<nsUI::BossHudScreenHost>("BossHudScreenHost") == nullptr)
            pBossHudHost_ = nullptr;

        /* 二重生成防止。 */
        if (pBossHudHost_ != nullptr)
            return;

        /* BossHudScreenHost を生成する（接続は ConnectBossHudData で行う）。 */
        pBossHudHost_ = NewGO<nsUI::BossHudScreenHost>(1, "BossHudScreenHost");
    }


    float InGameBuildHelper::GetProgress() const
    {
        /* ビルド関数が空の場合、進捗は 0 または 1 とする。*/
        if (buildFunctions_.empty())
            return isFinished_ ? 1.0f : 0.0f;

        /* 現在のビルド関数のインデックスを進捗として計算。*/
        float progress = static_cast<float>(currentBuildIndex_) / static_cast<float>(buildFunctions_.size());
        if (progress < 0.0f)
            return 0.0f;
        if (progress > 1.0f)
            return 1.0f;
        return progress;
    }


    void InGameBuildHelper::BuildGameplayHudUiStep()
    {
        /* 二重生成防止 */
        if (pGameplayHudScreenHost_ != nullptr)
            return;

        /* GameplayHudScreenHost を生成。*/
        pGameplayHudScreenHost_ = NewGO<nsUI::GameplayHudScreenHost>(1, "GameplayHudScreenHost");
        if (pGameplayHudScreenHost_ == nullptr)
            return;

        /* Host::Start 内でも接続するが、念のためここでも */
        ConnectGameplayHudData();
    }


    void InGameBuildHelper::BuildPauseMenuUiStep()
    {
        /* 二重生成防止 */
        if (pPauseMenuUI_ != nullptr)
            return;

        /* PauseMenuUI を生成。*/
        pPauseMenuUI_ = NewGO<nsUI::PauseMenuUI>(1, "PauseMenuUI");
    }


    void InGameBuildHelper::DestroyPauseMenuUi()
    {
        /* NewGO で生成した PauseMenuUI は DeleteGO で破棄する（delete 禁止）。 */
        if (pPauseMenuUI_ != nullptr)
        {
            DeleteGO(pPauseMenuUI_);
            pPauseMenuUI_ = nullptr;
        }
        else if (auto* pPause = FindGO<nsUI::PauseMenuUI>("PauseMenuUI"))
            DeleteGO(pPause);
    }

    void InGameBuildHelper::ConnectGameplayHudData()
    {
        /* 二重接続防止 */
        if (pGameplayHudScreenHost_ == nullptr)
            return;

        /* GameplayHudScreenHost から GameplayHudScreen を取得。*/
        auto* pScreen = pGameplayHudScreenHost_->GetGameplayHudScreen();
        if (pScreen == nullptr)
            return;

        /* Game 内の GameplayHudData と Screen を接続 */
        pScreen->ConnectToData(&gameplayHudData_);
    }


    void InGameBuildHelper::ConnectBossHudData()
    {
        /* 念のため FindGO で取り直す。 */
        if (pBossHudHost_ == nullptr)
            pBossHudHost_ = FindGO<nsUI::BossHudScreenHost>("BossHudScreenHost");

        /* 二重接続防止 */
        if (pBossHudHost_ == nullptr)
            return;

        auto* pUiScreen = pBossHudHost_->GetScreen();
        if (pUiScreen == nullptr)
            return;

        auto* pScreen = static_cast<nsUI::BossHudScreen*>(pUiScreen);

        /* ボスステージ開始時だけ表示。 */
        const bool isBossStage = (stageID_ == nsStage::StageID::sStageEX);
        pScreen->SetVisible(isBossStage);

        /* BossHudData と Screen を接続する。 */
        bossHudData_.SetScreen(pScreen);
        pScreen->Bind(&bossHudData_);
    }


    void InGameBuildHelper::DestroyGameplayHud()
    {
        /* 二重破棄防止 */
        if (pGameplayHudScreenHost_ != nullptr)
        {
            DeleteGO(pGameplayHudScreenHost_);
            pGameplayHudScreenHost_ = nullptr;
        }

        /* 念のため、FindGO で探して破棄する */
        else if (auto* pHud = FindGO<nsUI::GameplayHudScreenHost>("GameplayHudScreenHost"))
            DeleteGO(pHud);
    }


    void InGameBuildHelper::BuildSoundSettingUiStep()
    {
        /* 二重生成防止 */
        if (pSoundSettingHost_ != nullptr)
            return;

        /* SoundSettingScreenHost を生成。 */
        pSoundSettingHost_ = NewGO<nsUI::SoundSettingScreenHost>(1, "SoundSettingScreenHost");

        /* NewGO 直後は Start 前のため、同フレーム接続できるように即時初期化する。 */
        if (pSoundSettingHost_ != nullptr)
            pSoundSettingHost_->StartWrapper();
    }


    void InGameBuildHelper::ConnectSoundSettingData()
    {
        if (pSoundSettingHost_ == nullptr)
            pSoundSettingHost_ = FindGO<nsUI::SoundSettingScreenHost>("SoundSettingScreenHost");

        if (pSoundSettingHost_ == nullptr)
            return;

        auto* pUiScreen = pSoundSettingHost_->GetScreen();
        if (pUiScreen == nullptr)
            return;

        auto* pScreen = static_cast<nsUI::SoundSettingScreen*>(pUiScreen);

        /* 起動時は非表示。 */
        pScreen->SetVisible(false);

        /* Data と Screen を接続する。 */
        soundSettingData_.SetScreen(pScreen);
        pScreen->Bind(&soundSettingData_);
    }


    void InGameBuildHelper::DestroySoundSettingUi()
    {
        if (pSoundSettingHost_ != nullptr)
        {
            DeleteGO(pSoundSettingHost_);
            pSoundSettingHost_ = nullptr;
        }
        else if (auto* pHost = FindGO<nsUI::SoundSettingScreenHost>("SoundSettingScreenHost"))
            DeleteGO(pHost);
    }


    void InGameBuildHelper::ExecuteNextBuildFunction()
    {
        /* ビルド関数を順番に実行。*/
        if (isFinished_)
            return;

        /* 現在のビルド関数を実行。*/
        if (currentBuildIndex_ >= static_cast<int>(buildFunctions_.size()))
        {
            isFinished_ = true;
            return;
        }

        /* 現在のビルド関数を実行。*/
        buildFunctions_[currentBuildIndex_]();
        ++currentBuildIndex_;
    }


    nsStage::nsBackGround::IBackGround* InGameBuildHelper::CreateBackGround(nsStage::StageID stageID)
    {
        /* ステージマスターテーブルからステージ情報を取得。*/
        const auto& master = nsSystem::StageMasterTable::Get(stageID);
        if (master.stageID == nsStage::StageID::sInvalid)
            return nullptr;

        /* スクロール背景またはボス背景以外は生成しない。*/
        if (master.backgroundType != "Scroll" && master.backgroundType != "Boss")
            return nullptr;

        /* スクロール背景のステージIDを設定。*/
        nsStage::nsScrollBackGround::ScrollStageBackGround::SetPendingStageID(stageID);

        /* スクロール背景を生成。*/
        auto* bg = NewGO<nsStage::nsScrollBackGround::ScrollStageBackGround>(0, "BackGround");
        if (bg != nullptr)
            bg->SetStageID(stageID);

        return bg;
    }
} // namespace nsApp

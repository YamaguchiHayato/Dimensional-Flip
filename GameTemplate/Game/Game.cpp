#include "stdafx.h"
#include "Game.h"
#include "Fade.h"
#include "Src/WallActor.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"
#include "Src//Actor/Stage/Gimmick/RotationFool.h"
// UI。
#include "Src/UI/TimerUI.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/HPbarUI.h"
// マネージャー。
#include "Src/Camera/CameraManager.h"
#include "Src/Scene/SceneManager.h"
#include "Src/Actor/Stage/StageManager.h"

namespace EnemyPosition
{
	const Vector3 Pos1(600.0f, 0.0f, 0.0f);
}

namespace GameParameter
{
	const float scale = 1.0f;

}



void Game::InitSkyCube()
{
	DeleteGO(pSkyCube_);
	SkyCube* m_SkyCube = NewGO<SkyCube>(0, "skycube");


	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_SkyCube->GetTextureFilePath(), 1.0f);
	// 環境日光の影響が分かりやすいように、ディレクションライトはオフに。
	g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);

}

bool Game::Start()
{

    // ステージマネージャーの生成。
    StageManager::CreateInstance();
    StageManager::GetInstance()->Start();

    FadeStart();

	InitSkyCube();

//	EnemyNewGO_Tracking();
    PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::Update()
{
    // ステージマネージャーの更新。
    StageManager::GetInstance()->Update();

    //
    uint8_t newStageNum = (uint8_t) StageManager::GetInstance()->GetCurrentStageID();

    if (newStageNum != currentStageNum_)
    {
        // 変更時に処理を呼び出す。
        OnStageChange(newStageNum);
        // 現在のステージ番号を更新。
        currentStageNum_ = newStageNum;
    }

    // カメラマネージャーの更新。
    if (pCameraManager_)
        pCameraManager_->Update();

 //   StageManager::DeleteInstance();
}

void Game::OnStageChange(uint8_t newStageNum)
{
    // カメラとプレイヤーの移動。
    Vector3 stageStartPos = StageManager::GetInstance()->GetStageStartPos();

    // カメラの移動。
    if (pCameraManager_)
    {
        // カメラを2D(デフォルト)に戻す。
        pCameraManager_->Request2DMode();
        // カメラの角度をリセット。
        pCameraManager_->Request2DRotation(0.0f);
    }

    // ステージ固有の処理を呼び出す。
    ApplyStageSpecifics(newStageNum);

    // 各UIのリセット処理。
    if (pScoreUI_)
    {
        pScoreUI_->ResetScore();
    }
}

void Game::ApplyStageSpecifics(uint8_t newStageNum)
{

    // 各ステージがロードされた時の処理。
    switch (newStageNum)
    {
    case (uint8_t) StageID::sStage1:
    {
        // ステージ1固有の処理。
        // もしこの敵クラスが存在すれば削除。
        if (pTrackingEnemy_)
        {
            DeleteGO(pTrackingEnemy_);
            pTrackingEnemy_ = nullptr;
        }
        break;
    }
    case (uint8_t) StageID::sStage2:
    {
        // もし敵クラスがいなければ生成。
        if (pTrackingEnemy_ == nullptr)
        {
            EnemyNewGO_Tracking();
        }
        break;
    }
    default:
        // 念のため定義されていないステージの場合は敵を削除。
        if (pTrackingEnemy_)
        {
            DeleteGO(pTrackingEnemy_);
            pTrackingEnemy_ = nullptr;
        }
        break;
    }
}

void Game::Render(RenderContext& rc)
{
    StageManager::GetInstance()->Render(rc);
}


void Game::FadeStart()
{
    //フェードアウトがおわったかの処理をかく。
    //終わってたら下のTrans実行を
    pFade_ = FindGO<Fade>("fade");
    if (pFade_->IsFadeInEnd())
    {
        pFade_->FadeTransition(FadeState::FadeStart);
    }
}

void Game::EnemyNewGO_Tracking()
{
	pTrackingEnemy_ = NewGO<TrackingEnemy>(0, "TrackingEnemy");
	pTrackingEnemy_->enemyPosition_ = {EnemyPosition::Pos1};
	pTrackingEnemy_->enemyFP_ = pTrackingEnemy_->enemyPosition_;
}


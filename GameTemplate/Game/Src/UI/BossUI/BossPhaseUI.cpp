#include "stdafx.h"
#include "BossPhaseUI.h"

namespace
{
    // 解像度。
    const auto WIDTH = 1920.0f;  // 幅。
    const auto HEIGHT = 1080.0f; // 高さ。

    // 拡大率。
    const auto MAGNIFICATION = 0.25f; 

    // 画像ごとの設定データ。
    struct PhaseDef
    {
        app::nsUI::BossPhaseKind kind;
        const char* assetName;
        const float originWIDTH;
        const float originHEIGHT;
    };


    // 構造体を用いて一括定義。
    const PhaseDef PHASE_DEFS[] =
    {
        // フェーズ1。
        {app::nsUI::BossPhaseKind::Phase1, "phase1", WIDTH, HEIGHT},
        // フェーズ2。
        {app::nsUI::BossPhaseKind::Phase2, "phase2", WIDTH, HEIGHT},
        // フェーズ3。
        {app::nsUI::BossPhaseKind::Phase3, "phase3", WIDTH, HEIGHT},
        // GO。
        {app::nsUI::BossPhaseKind::GO, "GO", WIDTH, HEIGHT}
    };


    // 演出時間の設定。
    const auto TIME_STAY_CENTER = 2.5f;       // 中央に留まる時間。
    const auto TIME_MOVE = 0.8f;              // 移動時間。
    const auto DRAWING_TIME = 3.5f;           // GOを表示する時間。
    const Vector3 CENTER_POS = Vector3::Zero; // 画面中央座標。


    // 親クラスから見たローカル座標。
    // 画面中央座標。
    const Vector3 LOCAL_START_POS = Vector3(-400.0f, 100.0f ,0.0f);
    // 目標座標。
    const Vector3 LOCAL_TARGET_POS = Vector3(850.0f, 250.0f, 0.0f);

    // 中央での横並び距離
    const Vector3 OFFSET_HORIZONTAL = Vector3(150.0f, 0.0f, 0.0f);

    // 右下での縦並び距離
    const Vector3 OFFSET_VERTICAL = Vector3(0.0f, -120.0f, 0.0f);  

    // 初期演出のサイズ。
    const Vector3 SCALE_BIG = Vector3::One;
    const Vector3 SCALE_SMALL = Vector3(0.25f, 0.25f, 0.25f);

    // フェーズ数画像のYオフセット。
    const float OFFSET_Y_NUMBER = -200.0f;
}

namespace nsApp
{
    namespace nsUI
    {
        void BossPhaseUI::Initialize()
        {
            for (const auto& def : PHASE_DEFS)
            {
                int index = (int) def.kind;

                // 画像を初期化。
                phaseIcons_[index].Init(FetchUIName(def.assetName).c_str(), WIDTH, HEIGHT);

                // 画像の拡大率。
                phaseIcons_[index].SetScale({MAGNIFICATION, MAGNIFICATION, 1.0f});

                // 画像の中心を中央に設定。
                phaseIcons_[index].SetPivot({0.5f, 0.5f});
            }

            // ボスアイコン(Phase.Ver)を初期化。
            phaseIconRender_.Init(FetchUIName("bossPhaseIcon").c_str(), WIDTH, HEIGHT);
            phaseIconRender_.SetPivot({0.5f, 0.5f});


            // ボスアイコン(固定テキスト)を初期化。
            currentPosition_ = LOCAL_START_POS;
            currentNumberOffset_ - OFFSET_HORIZONTAL;
            currentScale_ = SCALE_BIG;
        }


        void BossPhaseUI::Update()
        {
            // 行列更新。
            UpdateMatrix();

            // 演出全体を更新。
            UpdatePhaseAnim();


            // フェーズ数によって配置を切り替える。
            if (currentPhase_ == BossPhaseKind::Phase1)
                // フェーズ1用の全体更新。
                UpdateLayOut();

            else
                // フェーズ数のみ更新。
                UpdateLayOutLater();


            // GO演出の更新。
            const Matrix& worldMatrix = GetWorldMatrix();
            Vector3 basePos = {worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2]};

            // 演出時の座標更新。
            UpdateGOTimer(basePos);
        }


        void BossPhaseUI::Draw(RenderContext& rc)
        {
            int index = (int) currentPhase_;

            if (index >= 0 && index < (int) BossPhaseKind::GO)
            {
                phaseIconRender_.Draw(rc);   // ボス
                phaseIcons_[index].Draw(rc); // フェーズ画像
            }

            // GO演出中なら描画
            if (isGOPlaying_)
                phaseIcons_[(int) BossPhaseKind::GO].Draw(rc);
        }


        void BossPhaseUI::PlayGOSprite()
        {
            isGOPlaying_ = true;
            goTimer_ = DRAWING_TIME;
        }


        void BossPhaseUI::StartPhaseAninm(BossPhaseKind phase)
        {
            currentPhase_ = phase;
            phaseState_ = PhaseAnimState::Phase;
            animTimer_ = 0.0f;

            // 画面中央に大きく配置。
            currentPosition_ = LOCAL_START_POS;
            currentScale_ = SCALE_BIG;

        }


        void BossPhaseUI::UpdateGOTimer(Vector3& basePos)
        {
            if (!isGOPlaying_)
                return;

            // 経過時間を取得する。
            goTimer_ -= g_gameTime->GetFrameDeltaTime();

            if (goTimer_ <= 0.0f)
                isGOPlaying_ = false;


            // GOは常に画面中央付近に表示。
            phaseIcons_[(int) BossPhaseKind::GO].SetPosition(CENTER_POS);
            phaseIcons_[(int) BossPhaseKind::GO].SetScale(Vector3::One);
            phaseIcons_[(int) BossPhaseKind::GO].Update();
        }


        void BossPhaseUI::UpdatePhaseAnim()
        {
            auto deltaTime = g_gameTime->GetFrameDeltaTime();

            switch (phaseState_)
            {
            // フェーズ表示中。
            case app::nsUI::PhaseAnimState::Phase:
                 UpdatePhaseState(deltaTime);
                 break;


            // 移動中。
            case app::nsUI::PhaseAnimState::Moving:
                 UpdateMovingState(deltaTime, basePosition_);
                 break;


            // GO表示中。
            case app::nsUI::PhaseAnimState::GO:
                 UpdateGOState();
                 break;


            // 演出終了中。
            case app::nsUI::PhaseAnimState::Finish:
                 UpdateFinishState(basePosition_);
                 break;


            // ステート後。
            default:
                UpdateAfterState(basePosition_);
                break;
            }


        }


        void BossPhaseUI::UpdatePhaseState(float deltaTime)
        {
            // フェーズ表示中は中央に固定。
            animTimer_ += deltaTime;

            // このステート時は画面中央に大きく描画を維持する。
            currentPosition_ = LOCAL_START_POS;
            currentScale_ = SCALE_BIG;
            currentNumberOffset_ = OFFSET_HORIZONTAL;

            // 一定時間経過したら移動へ。
            if (animTimer_ >= TIME_STAY_CENTER)
            {
                animTimer_ = 0.0f;
                phaseState_ = PhaseAnimState::Moving;
            }
        }


        void BossPhaseUI::UpdateMovingState(float deltaTime, const Vector3& basePos)
        {
            animTimer_ += deltaTime;
            auto late = min(animTimer_ / TIME_MOVE, 1.0f);

            // イージング。
            auto t = 1.0f - powf(1.0f - late, 3.0f);

            // 線形補間（Lerp）。
            // 中央位置からオフセットへ移動。
            currentPosition_ = LOCAL_START_POS + (LOCAL_TARGET_POS - LOCAL_START_POS) * t;
            currentScale_ = SCALE_BIG + (SCALE_SMALL - SCALE_BIG) * t;

            // 大きさを補間。
            currentNumberOffset_ = OFFSET_HORIZONTAL + (OFFSET_VERTICAL - OFFSET_HORIZONTAL) * t;

             // 割合を補正して遷移。
            if (late >= 1.0f)
            {
                animTimer_ = 0.0f;
                phaseState_ = PhaseAnimState::GO;
                PlayGOSprite();
            }
        }


        void BossPhaseUI::UpdateGOState()
        {
            if (!isGOPlaying_)
                phaseState_ = PhaseAnimState::Finish;
        }


        void BossPhaseUI::UpdateFinishState(const Vector3& basePos)
        {
            // 定位置に固定。
            currentPosition_  = LOCAL_TARGET_POS;
            currentScale_ = SCALE_SMALL;
        }


        void BossPhaseUI::UpdateAfterState(const Vector3& basePos)
        {
            // 演出終了後も定位置を維持。
            currentPosition_ = LOCAL_TARGET_POS;
            currentScale_ = SCALE_SMALL;
        }


        void BossPhaseUI::UpdateLayOut()
        {
            int index = (int) currentPhase_;

            // 看板（PHASE）
            phaseIconRender_.SetPosition(currentPosition_);
            phaseIconRender_.SetScale(currentScale_);
            phaseIconRender_.Update();

            // 数字（ロゴからオフセットした位置）
            Vector3 numPos = currentPosition_ + currentNumberOffset_ * (currentScale_.x / SCALE_SMALL.x);
            phaseIcons_[index].SetPosition(numPos);
            phaseIcons_[index].SetScale(currentScale_);
            phaseIcons_[index].Update();
        }


        void BossPhaseUI::UpdateLayOutLater()
        {
            int index = (int) currentPhase_;

            phaseIconRender_.SetPosition(LOCAL_TARGET_POS);
            phaseIconRender_.SetScale(SCALE_SMALL);

            if (phaseState_ == PhaseAnimState::Phase)
            {
                phaseIconRender_.SetPosition(LOCAL_START_POS);
                phaseIconRender_.SetScale(SCALE_BIG);
            }

            phaseIconRender_.Update();

            Vector3 numPos = currentPosition_ + currentNumberOffset_ * (currentScale_.x / SCALE_SMALL.x);
            phaseIcons_[index].SetPosition(numPos);
            phaseIcons_[index].SetScale(currentScale_);
            phaseIcons_[index].Update();
        }
    }
}

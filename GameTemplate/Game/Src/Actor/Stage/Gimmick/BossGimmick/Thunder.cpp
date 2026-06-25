#include "stdafx.h"
#include "Thunder.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Actor/Character/Player/Player.h"


namespace
{
    const auto WARNING_TIME = 1.5f;    // 警告UIの描画時間。
    const auto FALLING_SPEED = 50.0f;   // 雷エフェクトの落下速度。
    const auto HIT_RADIUS = 0.5f;      // 当たり判定。

    const auto WARNING_UI_WIDHT = 128.0f;
    const auto WARNING_UI_HEIGHT = 128.0f;
    const auto UI_OFFSET_Y = 350.0f;

}

namespace app
{
    namespace gimmick
    {
        Thunder::~Thunder()
        {
            if (pThunderEffect_)
                pThunderEffect_ = nullptr;
        }


        bool Thunder::Start()
        {
            // プレイヤークラスを探索。
            pPlayer_ = FindGO<Player>("player");

            // 警告UIを描画する。
            CreateWarningUI();
            return true;
        }


        void Thunder::Update()
        {
            // ステート全体を更新する。
            UpdateAttackState();
        }


        void Thunder::Render(RenderContext& rc)
        {
            // 攻撃フェーズ時のみ描画する。
            if (isWarningVisible_ || currentStep_ == ThunderStep::Falling)
            {
                warnigUIRender_.Draw(rc);
            }
        }


        void Thunder::UpdateAttackState()
        {
            auto deltaTime = g_gameTime->GetFrameDeltaTime();

            switch (currentStep_)
            {
            case ThunderStep::Warning:
                 UpdateWarningUI(deltaTime);
                 break;

            case ThunderStep::Falling:
                 UpdateStrike(deltaTime);
                 break;

            default:
                // デフォルトはディアクティブ状態。
                DeleteGO(this);
                break;
            }
        }


        void Thunder::UpdateWarningUI(float deltaTime)
        {
            timer_ += deltaTime;

            // 左から右へ移動（X軸）
            if (currentMode_ == ThunderMode::Moving)
            {
                pos_.x += FALLING_SPEED * deltaTime;
            }

            // --- ここで座標変換を行う ---
            Vector2 screenPos; // 変換後の座標を受け取る変数
            // 3Dカメラ(g_camera3D)を使ってワールド座標(pos_)をスクリーン座標に変換
            g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, pos_);

            // UIの位置をスクリーン座標に更新 (Vector2をVector3としてセット)
            warnigUIRender_.SetPosition({screenPos.x, screenPos.y + UI_OFFSET_Y, 0.0f});

            // スケール計算（warningDuration_を使用）
            auto progress = timer_ / warningDuration_;
            float currentScale = baseScale_ * (1.0f - (progress * 0.5f));

            warnigUIRender_.SetScale(Vector3::One * currentScale);
            warnigUIRender_.Update();

            if (timer_ >= warningDuration_)
            {
                CreateThunderEffect();
                currentStep_ = ThunderStep::Falling;
                timer_ = 0.0f;
            }
        }


        void Thunder::UpdateStrike(float deltaTime)
        {
            if (currentMode_ == ThunderMode::Moving)
            {
                // 攻撃中も右へ移動し続ける
                pos_.x += FALLING_SPEED * deltaTime;

                // 雷エフェクトを追従させる
                if (pThunderEffect_)
                    pThunderEffect_->SetPosition(pos_);

                Vector2 screenPos;
                g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, pos_);
                warnigUIRender_.SetPosition({screenPos.x, screenPos.y + UI_OFFSET_Y, 0.0f});
                warnigUIRender_.Update();
            }

            if (!hasDamaged_)
                CheckCollision();

            // 画面外（右端）に行き過ぎた場合などの終了判定をX軸に変更
            if (!pThunderEffect_ || !pThunderEffect_->IsPlay() || pos_.x > 100.0f)
            {
                isWarningVisible_ = false;
                DeleteGO(this);
            }
        }


        void Thunder::CheckCollision()
        {
            if (!pPlayer_)
                return;

            // Playerクラスの座標を取得。
            Vector3 playerPos = pPlayer_->GetPlayerPos();

            // 水平距離のみで距離判定を取る。
            Vector3 diff = playerPos - pos_;
            diff.y = 0.0f;

            // 攻撃半径の計算。
            auto radius = HIT_RADIUS * baseScale_;
            if (diff.LengthSq() <= radius * radius)
            {
                // Playerに1ダメージを付与。
                pPlayer_->OnDamage(1);

                // ダメージの重複を防ぐ。
                hasDamaged_ = true;
            }
        }


        void Thunder::CreateThunderEffect()
        {
            pThunderEffect_ = NewGO<EffectEmitter>(0);

            // 雷エフェクトのIDを指定。
            pThunderEffect_->Init(app::enemyStatus::EffectID::effect_thunder);
            pThunderEffect_->SetPosition(pos_);
            pThunderEffect_->SetScale(Vector3::One * baseScale_ * 1.0f);
            pThunderEffect_->Play();
        }


        void Thunder::CreateWarningUI()
        {
            warnigUIRender_.Init("Assets/UI/BossUI/warning.DDS", WARNING_UI_WIDHT, WARNING_UI_HEIGHT);
            warnigUIRender_.SetPosition(pos_);
            warnigUIRender_.SetScale(scale_);
            isWarningVisible_ = true;
        }
    }
}

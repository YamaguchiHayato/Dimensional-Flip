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
            if (isWarningVisible_)
                warnigUIRender_.Draw(rc);
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

            // カウントダウン演出。
            auto scaleProgress = 1.0f - (timer_ / WARNING_TIME);
            float currentScale = baseScale_ * scaleProgress;
            warnigUIRender_.SetScale(Vector3::One * currentScale);


            // UIをエフェクトを生成する位置にあわせて表示する。
            Vector2 screenPos;
            g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, pos_);


            warnigUIRender_.SetPos(screenPos);
            warnigUIRender_.Update();


            // 描画設定時間だけ描画を行う(UI)。
            if (timer_ >= WARNING_TIME)
            {
                // 警告エフェクトの描画を終了する。
                isWarningVisible_ = false;

                // 雷エフェクトを描画。
                CreateThunderEffect();

                // ステートを更新。
                currentStep_ = ThunderStep::Falling;
                timer_ = 0.0f;
            }
        }


        void Thunder::UpdateStrike(float deltaTime)
        {
            // 移動処理。
            // 移動ステートの時。
            if (currentMode_ == ThunderMode::Moving)
            {
                pos_.z -= FALLING_SPEED * deltaTime;
                if (pThunderEffect_)
                    pThunderEffect_->SetPosition(pos_);
            }

            // 当たり判定(必要な時だけ計算を行う。)
            if (!hasDamaged_)
                CheckCollision();

            // エフェクトを終了、画面外に移動すると削除するように指定。
            if (!pThunderEffect_ || !pThunderEffect_->IsPlay() || pos_.z < -100.0f)
                DeleteGO(this);
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

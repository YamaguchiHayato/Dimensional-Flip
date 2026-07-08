#include "stdafx.h"

#include "BossStatusHudComponent.h"

namespace nsApp
{
    namespace nsUI
    {
        void BossStatusHudComponent::SetHpPercent(float percent)
        {
            /* パーセントを 0.0f から 1.0f の範囲に制限する。*/
            if (percent < 0.0f)
                percent = 0.0f;
            if (percent > 1.0f)
                percent = 1.0f;

            /* 現在 HP パーセントを更新する。*/
            currentPercent_ = percent;
        }


        void BossStatusHudComponent::OnBuild()
        {
            /* スプライトのパスを取得する。*/
            /* ボスアイコン。*/
            bossIcon_.Init(pathBossIcon_, iconW_, iconH_);
            bossIcon_.SetPivot({0.5f, 0.5f});

            /* HP フレーム。*/
            hpFrame_.Init(pathHpFrame_, frameW_, frameH_);
            hpFrame_.SetPivot({0.5f, 0.5f});

            /* 現在 HP とダメージ HP。*/
            hpCurrent_.Init(pathHpCurrent_, barW_, barH_);
            hpCurrent_.SetPivot({0.0f, 0.5f});
            hpDamage_.Init(pathHpDamage_, dmgW_, dmgH_);
            hpDamage_.SetPivot({0.5f, 0.5f});

            /* ビルド完了フラグを立てる。*/
            isBuilt_ = true;

            /* スプライトをリフレッシュする。*/
            RefreshSprites();
        }

        void BossStatusHudComponent::OnUpdate(float deltaTime)
        {
            /* ダメージバーの減少速度を設定する。*/
            (void) deltaTime;

            /* ダメージバーのパーセントを更新する。*/
            if (damagePercent_ > currentPercent_)
            {
                /* ダメージバーのパーセントを減少させる。*/
                damagePercent_ -= damageBarLerpSpeed_;

                /* ダメージバーのパーセントが現在 HP パーセントを下回らないようにする。*/
                if (damagePercent_ < currentPercent_)
                    damagePercent_ = currentPercent_;
            }
            else
                /* ダメージバーのパーセントが現在 HP パーセントを下回った場合、現在 HP パーセントに合わせる。*/
                damagePercent_ = currentPercent_;

            /* スプライトをリフレッシュする。*/
            RefreshSprites();
        }


        void BossStatusHudComponent::RefreshSprites()
        {
            /* ビルドが完了していない場合、スプライトの更新を行わない。*/
            if (!isBuilt_)
                return;

            /* ダメージ HP のスプライトを更新する。*/
            hpDamage_.SetPosition(basePos_);
            hpDamage_.SetScale(Vector3::One);
            hpDamage_.Update();

            /* 現在 HP のスプライトを更新する。*/
            hpCurrent_.SetPosition(currentPos_);
            hpCurrent_.SetScale(Vector3(currentPercent_, 1.0f, 1.0f));
            hpCurrent_.Update();

            /* HP フレームのスプライトを更新する。*/
            hpFrame_.SetPosition(basePos_);
            hpFrame_.SetScale(Vector3::One);
            hpFrame_.Update();

            /* ボスアイコンのスプライトを更新する。*/
            bossIcon_.SetPosition(iconPos_);
            bossIcon_.SetScale(Vector3::One);
            bossIcon_.Update();
        }


        void BossStatusHudComponent::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            /* ownerWorld は使用しないが、関数シグネチャの一部として受け取る。*/
            (void) ownerWorld;

            /* ビルドが完了していない場合、描画を行わない。*/
            if (!isBuilt_)
                return;

            /* スプライトを描画する。*/
            hpDamage_.Draw(rc);
            hpCurrent_.Draw(rc);
            hpFrame_.Draw(rc);
            bossIcon_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp

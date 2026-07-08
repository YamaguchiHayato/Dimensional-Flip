#include "stdafx.h"

#include "BossStatusHudComponent.h"
#include "Src/Presentation/UI/BossHudUiNames.h"

namespace
{
    const float ICON_W = 350.0f;
    const float ICON_H = 250.0f;
    const float FRAME_W = 600.0f;
    const float FRAME_H = 600.0f;
    const float BAR_W = 520.0f;
    const float BAR_H = 95.0f;
    const float DMG_W = 550.0f;
    const float DMG_H = 445.0f;

    /* 旧 BossUIManager と同じ基準位置（画面座標）。 */
    const Vector3 BASE_POS(650.0f, -400.0f, 0.0f);
    const Vector3 ICON_POS(320.0f, -400.0f, 0.0f);    /* BASE + (-330, 0, 0) */
    const Vector3 CURRENT_POS(370.0f, -400.0f, 0.0f); /* BASE + (-280, 0, 0) */

    const float DAMAGE_BAR_LERP_SPEED = 0.005f;
} // namespace


namespace nsApp
{
    namespace nsUI
    {
        void BossStatusHudComponent::SetHpPercent(float percent)
        {
            /* 0〜1 にクランプする。 */
            if (percent < 0.0f)
                percent = 0.0f;
            if (percent > 1.0f)
                percent = 1.0f;

            currentPercent_ = percent;
        }


        void BossStatusHudComponent::OnBuild()
        {
            /* ボスアイコン。 */
            bossIcon_.Init(FetchUIName("bossIcon").c_str(), ICON_W, ICON_H);
            bossIcon_.SetPivot({0.5f, 0.5f});

            /* HP 枠。 */
            hpFrame_.Init(FetchUIName("hpBar_flame").c_str(), FRAME_W, FRAME_H);
            hpFrame_.SetPivot({0.5f, 0.5f});

            /* 現在 HP バー。 */
            hpCurrent_.Init(FetchUIName("hpBar_current").c_str(), BAR_W, BAR_H);
            hpCurrent_.SetPivot({0.5f, 0.5f});

            /* ダメージバー。 */
            hpDamage_.Init(FetchUIName("hpBar_damage").c_str(), DMG_W, DMG_H);
            hpDamage_.SetPivot({0.5f, 0.5f});

            isBuilt_ = true;
        }


        void BossStatusHudComponent::OnUpdate(float deltaTime)
        {
            (void) deltaTime;

            /* ダメージバーを current へゆっくり追従させる。 */
            if (damagePercent_ > currentPercent_)
            {
                damagePercent_ -= DAMAGE_BAR_LERP_SPEED;
                if (damagePercent_ < currentPercent_)
                    damagePercent_ = currentPercent_;
            }
            else
                damagePercent_ = currentPercent_;
        }


        void BossStatusHudComponent::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            (void) ownerWorld;

            if (!isBuilt_)
                return;

            bossIcon_.SetPosition(ICON_POS);
            bossIcon_.SetScale(Vector3::One);
            bossIcon_.Update();
            bossIcon_.Draw(rc);

            hpFrame_.SetPosition(BASE_POS);
            hpFrame_.SetScale(Vector3::One);
            hpFrame_.Update();
            hpFrame_.Draw(rc);

            hpCurrent_.SetPosition(CURRENT_POS);
            hpCurrent_.SetScale(Vector3(currentPercent_, 1.0f, 1.0f));
            hpCurrent_.Update();
            hpCurrent_.Draw(rc);

            hpDamage_.SetPosition(BASE_POS);
            hpDamage_.SetScale(Vector3(damagePercent_, 1.0f, 1.0f));
            hpDamage_.Update();
            hpDamage_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp

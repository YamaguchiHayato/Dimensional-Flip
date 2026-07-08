#include "stdafx.h"

#include "BossHubScreen.h"
#include "Src/Actor/Stage/StageSetup.h"
#include "Src/Presentation/UI/BossHudUiNames.h"

namespace
{
    const Vector3 BASE_POS(650.0f, -400.0f, 0.0f);
    const Vector3 ICON_POS(320.0f, -400.0f, 0.0f);
    const Vector3 CURRENT_POS(370.0f, -400.0f, 0.0f);
    const Vector3 ATTACK_ICON_POS(0.0f, -425.0f, 0.0f);
    const float ATTACK_MAG = 0.25f;

    struct BossHudSprites
    {
        SpriteRender bossIcon;
        SpriteRender hpFrame;
        SpriteRender hpCurrent;
        SpriteRender hpDamage;
        SpriteRender attackIcons[8]; // BossAttackKind::Num == 8
        bool inited = false;
    };

    bool IsBossCutInPlaying()
    {
        return nsApp::nsStage::StageSetup::ShouldKeepPlayerPaused();
    }

    void InitSprites(BossHudSprites& s)
    {
        s.bossIcon.Init("Assets/UI/BossUI/bossIcon.DDS", 350.0f, 250.0f);
        s.bossIcon.SetPivot({0.5f, 0.5f});

        s.hpFrame.Init("Assets/UI/BossUI/hpBar_flame.DDS", 600.0f, 600.0f);
        s.hpFrame.SetPivot({0.5f, 0.5f});

        s.hpCurrent.Init("Assets/UI/BossUI/hpBar_current.DDS", 520.0f, 95.0f);
        s.hpCurrent.SetPivot({0.0f, 0.5f});

        s.hpDamage.Init("Assets/UI/BossUI/hpBar_damage.DDS", 550.0f, 445.0f);
        s.hpDamage.SetPivot({0.5f, 0.5f});

        struct AttackDef
        {
            int kind;
            const char* path;
        };
        const AttackDef defs[] = {
            {1, "Assets/UI/BossUI/fireBall.DDS"},   {2, "Assets/UI/BossUI/jumpIcon.DDS"},
            {3, "Assets/UI/BossUI/meteorIcon.DDS"}, {4, "Assets/UI/BossUI/roarIcon.DDS"},
            {5, "Assets/UI/BossUI/spearIcon.DDS"},  {6, "Assets/UI/BossUI/tumblerIcon.DDS"},
        };
        for (const auto& d : defs)
        {
            s.attackIcons[d.kind].Init(d.path, 1920.0f, 1080.0f);
            s.attackIcons[d.kind].SetPivot({0.5f, 0.5f});
            s.attackIcons[d.kind].SetScale(Vector3(ATTACK_MAG, ATTACK_MAG, 1.0f));
        }

        s.inited = true;
    }

    void DrawSprite(SpriteRender& sp, const Vector3& pos, const Vector3& scale, RenderContext& rc)
    {
        sp.SetPosition(pos);
        sp.SetScale(scale);
        sp.Update();
        sp.Draw(rc);
    }
} // namespace


namespace nsApp
{
    namespace nsUI
    {
        void BossHudScreen::Update(float deltaTime)
        {
            SyncFromDataSource();
            UIScreen::Update(deltaTime);
        }


        void BossHudScreen::Draw(RenderContext& rc)
        {
            if (IsBossCutInPlaying())
                return;

            static BossHudSprites sprites;
            if (!sprites.inited)
                InitSprites(sprites);

            float hpRatio = 1.0f;
            BossAttackKind attackKind = BossAttackKind::None;

            if (pHudData_ != nullptr)
            {
                hpRatio = pHudData_->GetHpRatio();
                attackKind = pHudData_->GetAttackKind();
            }

            /* ダメージ（黒）: 常に等倍・スケールしない */
            DrawSprite(sprites.hpDamage, BASE_POS, Vector3::One, rc);

            /* 現在 HP（赤）: 左基準で幅だけ縮む（pivot は Init で 0,0.5 済み） */
            DrawSprite(sprites.hpCurrent, CURRENT_POS, Vector3(hpRatio, 1.0f, 1.0f), rc);
            DrawSprite(sprites.hpFrame, BASE_POS, Vector3::One, rc);
            DrawSprite(sprites.bossIcon, ICON_POS, Vector3::One, rc);

            const int atkIndex = static_cast<int>(attackKind);
            if (attackKind != BossAttackKind::None && atkIndex > 0 && atkIndex < 8)
                DrawSprite(sprites.attackIcons[atkIndex], ATTACK_ICON_POS, Vector3(ATTACK_MAG, ATTACK_MAG, 1.0f), rc);
        }


        void BossHudScreen::Build()
        {
            CreateRootEntity();
            pStatusComponent_ = nullptr;
            pAttackComponent_ = nullptr;
        }


        void BossHudScreen::Bind(nsPresentation::IBossHudData* pData)
        {
            pHudData_ = pData;
            SyncFromDataSource();
        }


        void BossHudScreen::SyncFromDataSource()
        {
            /* Component を使わないので何もしない（Draw で直接読む） */
        }
    } // namespace nsUI
} // namespace nsApp

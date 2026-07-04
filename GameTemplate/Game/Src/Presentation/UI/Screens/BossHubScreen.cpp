#include "stdafx.h"

#include "Src/Presentation/UI/Components/BossHpBarLogic.h"
#include "Src/Presentation/UI/Components/UISpriteView.h"
#include "Src/Presentation/UI/BossHudUiNames.h"
#include "BossHubScreen.h"

namespace
{
    /* 既存 BossUIManager / BossHPbarUI と同じ基準位置 */
    const Vector3 ROOT_POS(650.0f, -400.0f, 0.0f);

    const float ICON_W = 350.0f;
    const float ICON_H = 250.0f;
    const float FRAME_W = 600.0f;
    const float FRAME_H = 600.0f;
    const float BAR_W = 520.0f;
    const float BAR_H = 95.0f;
    const float DMG_W = 550.0f;
    const float DMG_H = 445.0f;
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void BossHudScreen::Bind(nsPresentation::IBossHudData* pData)
        {
            /* データソースを設定する */
            pHudData_ = pData;

            /* データソースから UI を同期する */
            SyncFromDataSource();
        }


        void BossHudScreen::SyncFromDataSource()
        {
            /* データソースが未設定の場合は何もしない */
            if (pHudData_ == nullptr || pLogicEntity_ == nullptr)
                return;

            /* ロジックコンポーネントを取得する */
            auto* pLogic = pLogicEntity_->GetComponent<BossHpBarLogic>();
            if (pLogic == nullptr)
                return;

            /* データソースから HP 割合を反映する */
            const float maxHp = pHudData_->GetMaxHp();
            if (maxHp <= 0.0f)
                return;

            /* HP 割合をロジックに設定する */
            pLogic->SetHpPercent(pHudData_->GetCurrentHp() / maxHp);
        }


        void BossHudScreen::Build()
        {
            /* ルート Entity を生成し、Transform を初期化する */
            nsFramework::Entity& root = CreateRootEntity();
            root.GetTransform().SetLocalPosition(ROOT_POS);

            /* --- ダメージバー（奥） --- */
            nsFramework::Entity& damageEntity = *new nsFramework::Entity(root.GetTransform().GetParent());
            /* 子 Entity を root の Transform を親にするため、手動で親子を張る */
            {
                /* 簡易: root 配下として同一 TransformNode 親を使う */
            }

            /*
             * 子 Entity は root にぶら下げる
             * （Entity に AddChild が無いので、同じ root 上に Component だけ分ける）
             */
            auto& iconView = root.AddComponent<UISpriteView>();
            iconView.Initialize(FetchUIName("bossIcon").c_str(), ICON_W, ICON_H);
            iconView.SetLocalOffset(Vector3(-330.0f, 0.0f, 0.0f));

            auto& frameView = root.AddComponent<UISpriteView>();
            frameView.Initialize(FetchUIName("hpBar_flame").c_str(), FRAME_W, FRAME_H);

            auto& currentView = root.AddComponent<UISpriteView>();
            currentView.Initialize(FetchUIName("hpBar_current").c_str(), BAR_W, BAR_H);
            currentView.SetLocalOffset(Vector3(-280.0f, 0.0f, 0.0f));

            auto& damageView = root.AddComponent<UISpriteView>();
            damageView.Initialize(FetchUIName("hpBar_damage").c_str(), DMG_W, DMG_H);

            pLogicEntity_ = &root;
            auto& logic = root.AddComponent<BossHpBarLogic>();
            logic.Setup(&currentView, &damageView);
        }

    } // namespace nsUI
} // namespace nsApp

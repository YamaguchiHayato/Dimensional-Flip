#include "stdafx.h"

#include "BossAttackIndicatorHudComponent.h"

namespace
{
    /* 攻撃アイコンの定義構造体。 */
    struct Def
    {
        BossAttackKind kind; //! < 攻撃アイコンの種類 >
        const char* path;    //! < 攻撃アイコンのテクスチャパス >
    };

    /**
     * @brief 攻撃アイコンの定義配列。
     */
    const Def defs[] = {
        {BossAttackKind::FireBall, "Assets/UI/BossUI/fireBall.DDS"},
        {BossAttackKind::Jump, "Assets/UI/BossUI/jumpIcon.DDS"},
        {BossAttackKind::Meteor, "Assets/UI/BossUI/meteorIcon.DDS"},
        {BossAttackKind::Roar, "Assets/UI/BossUI/roarIcon.DDS"},
        {BossAttackKind::Spear, "Assets/UI/BossUI/spearIcon.DDS"},
        {BossAttackKind::Tumbler, "Assets/UI/BossUI/tumblerIcon.DDS"},
    };



}

namespace nsApp
{
    namespace nsUI
    {
        void BossAttackIndicatorHudComponent::SetAttackKind(BossAttackKind kind)
        {
            /* 攻撃アイコンの種類を設定する。 */
            currentKind_ = kind;
        }


        void BossAttackIndicatorHudComponent::OnBuild()
        {
            /* 攻撃アイコンのスプライトを初期化する。 */
            for (const auto& def : defs)
            {
                /* 攻撃アイコンの種類に応じてスプライトを初期化する。 */
                const int index = static_cast<int>(def.kind);
                attackIcons_[index].Init(def.path, texW_, texH_);
                attackIcons_[index].SetPivot({0.5f, 0.5f});
                attackIcons_[index].SetScale(Vector3(magnification_, magnification_, 1.0f));
            }

            /* UI コンポーネントが構築済みであることを示すフラグを設定する。 */
            isBuilt_ = true;

            /* 現在の攻撃アイコンを更新する。 */
            RefreshActiveIcon();
        }


        void BossAttackIndicatorHudComponent::OnUpdate(float deltaTime)
        {
            /* 前フレームからの経過時間を使用して UI コンポーネントを更新する。 */
            (void) deltaTime;
            RefreshActiveIcon();
        }


        void BossAttackIndicatorHudComponent::RefreshActiveIcon()
        {
            /* 現在アクティブな攻撃アイコンのインデックスをリセットする。 */
            activeIconIndex_ = -1;

            /* UI コンポーネントが構築済みであり、現在の攻撃アイコンの種類が None でない場合にのみ処理を行う。 */
            if (!isBuilt_ || currentKind_ == BossAttackKind::None)
                return;

            /* 現在の攻撃アイコンの種類に応じてアクティブな攻撃アイコンのインデックスを設定する。 */
            const int index = static_cast<int>(currentKind_);
            if (index <= 0 || index >= static_cast<int>(BossAttackKind::Num))
                return;

            /* アクティブな攻撃アイコンのインデックスを設定し、位置とスケールを更新する。 */
            activeIconIndex_ = index;
            attackIcons_[index].SetPosition(attackIconPos_);
            attackIcons_[index].SetScale(Vector3(magnification_, magnification_, 1.0f));
            attackIcons_[index].Update();
        }


        void BossAttackIndicatorHudComponent::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            /* レンダリングコンテキストと親のワールド行列を使用して UI コンポーネントを描画する。 */
            (void) ownerWorld;

            /* UI コンポーネントが構築済みであり、アクティブな攻撃アイコンのインデックスが有効である場合にのみ描画を行う。 */
            if (!isBuilt_ || activeIconIndex_ < 0)
                return;

            /* アクティブな攻撃アイコンを描画する。 */
            attackIcons_[activeIconIndex_].Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp

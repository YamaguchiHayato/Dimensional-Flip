#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"
#include "Src/UI/BossUI/BossAttackIndicatorUI.h"

namespace nsApp
{
    namespace nsUI
    {
        class BossAttackIndicatorHudComponent : public IUIComponent
        {
        public:
            /**
             * @brief 攻撃アイコンの種類を設定する。
             * @param kind 攻撃アイコンの種類。
             */
            void SetAttackKind(BossAttackKind kind);

            /**
             * @brief 攻撃アイコンの種類を取得する。
             */
            void OnBuild() override;

            /**
             * @brief UI コンポーネントを更新する。
             * @param deltaTime 前フレームからの経過時間（秒）。
             */
            void OnUpdate(float deltaTime) override;

            /**
             * @brief UI コンポーネントを描画する。
             * @param rc レンダリングコンテキスト。
             * @param ownerWorld 親のワールド行列。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;


        private:
            /**
             * @brief 現在の攻撃アイコンを更新する。
             */
            void RefreshActiveIcon();


        private:
            SpriteRender attackIcons_[(int) BossAttackKind::Num]; //! < 攻撃アイコンのスプライト配列 >
            BossAttackKind currentKind_ = BossAttackKind::None;   //! < 現在の攻撃アイコンの種類 >
            bool isBuilt_ = false;                                //! < UI コンポーネントが構築済みかどうか >

            Vector3 attackIconPos_{0.0f, -425.0f, 0.0f}; //! < 攻撃アイコンの位置 >
            float texW_ = 1920.0f;                       //! < テクスチャの幅 >
            float texH_ = 1080.0f;                       //! < テクスチャの高さ >
            float magnification_ = 0.25f;                //! < 拡大率 >
            int activeIconIndex_ = -1; //! < 現在アクティブな攻撃アイコンのインデックス。-1 は非表示を意味する。 >
        };
    } // namespace nsUI
} // namespace nsApp

#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"

namespace nsApp
{
    namespace nsUI
    {
        class BossStatusHudComponent : public IUIComponent
        {
        public:
            /**
             * @brief ボスの HP パーセントを設定する。
             * @param percent HP パーセント（0.0f から 1.0f の範囲）。
             */
            void SetHpPercent(float percent);

            /**
             * @brief ビルド処理。
             */
            void OnBuild() override;

            /**
             * @brief 更新処理。
             * @param deltaTime 
             */
            void OnUpdate(float deltaTime) override;

            /**
             * @brief 描画処理。
             * @param rc レンダリングテキスト。
             * @param ownerWorld ワールドマトリックス。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;

        private:
            /**
             * @brief スプライトをリフレッシュする。
             */
            void RefreshSprites();


        private:
            SpriteRender bossIcon_; //! < ボスアイコン >
            SpriteRender hpFrame_;  //! < HP フレーム >
            SpriteRender hpCurrent_; //! < 現在 HP >
            SpriteRender hpDamage_;  //! < ダメージ HP >

            float currentPercent_ = 1.0f; //! < 現在 HP パーセント >
            float damagePercent_ = 1.0f;  //! < ダメージ HP パーセント >
            bool isBuilt_ = false;

            /* レイアウト（旧 BossUIManager 相当） */
            Vector3 basePos_{650.0f, -400.0f, 0.0f}; //! < HP フレームの基準座標 >
            Vector3 iconPos_{320.0f, -400.0f, 0.0f}; //! < ボスアイコンの座標 >
            Vector3 currentPos_{370.0f, -400.0f, 0.0f}; //! < 現在 HP の座標 >

            /* スプライトサイズ */
            float iconW_ = 350.0f; //! < ボスアイコンの幅 >
            float iconH_ = 250.0f; //! < ボスアイコンの高さ >
            float frameW_ = 600.0f; //! < HP フレームの幅 >
            float frameH_ = 600.0f; //! < HP フレームの高さ >
            float barW_ = 520.0f;   //! < 現在 HP の幅 >
            float barH_ = 95.0f;    //! < 現在 HP の高さ >
            float dmgW_ = 550.0f;   //! < ダメージ HP の幅 >
            float dmgH_ = 445.0f;   //! < ダメージ HP の高さ >

            /* アセットパス */
            const char* pathBossIcon_ = "Assets/UI/BossUI/bossIcon.DDS";
            const char* pathHpFrame_ = "Assets/UI/BossUI/hpBar_flame.DDS";
            const char* pathHpCurrent_ = "Assets/UI/BossUI/hpBar_current.DDS";
            const char* pathHpDamage_ = "Assets/UI/BossUI/hpBar_damage.DDS";

            float damageBarLerpSpeed_ = 0.005f; // < ダメージバーの補間速度 >
        };
    } // namespace nsUI
} // namespace nsApp

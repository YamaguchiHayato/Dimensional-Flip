#pragma once
#include "Src/Production/CutIn/CutInBase.h"

namespace nsApp
{
    namespace nsProduction
    {
        class CutInScrollLayer : public CutInBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            CutInScrollLayer() = default;
            virtual ~CutInScrollLayer() = default;


        public:
            /**
             * @brief 初期化処理。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 更新処理。
             */
            void Update() override;

            /**
             * @brief 描画処理。
             * @param rc レンダーリングコンテキスト。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief レイヤー DDS のパスを組み立てる。
             * @param layerName ファイル名（拡張子なし）。
             * @return Assets 配下のフルパス。
             */
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }


        private:
            /**
             * @brief スクロール速度をセットする。
             * @param speed スクロール速度のベクトル。
             */
            void SetScrollSpeed(const Vector3& speed) { speed_ = speed; }

            /**
             * @brief UV オフセットを更新する。
             */
            void Scroll();

            /**
             * @brief 振動アニメーションを適用する。
             */
            void Vibration();


        private:
            Vector3 speed_ = Vector3::Zero;
            Vector2 uvOffset_ = Vector2::Zero;
        };
    } // namespace nsProduction
} // namespace nsApp

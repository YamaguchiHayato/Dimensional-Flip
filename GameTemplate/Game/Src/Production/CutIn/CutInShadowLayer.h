#pragma once
#include "Src/Production/CutIn/CutInBase.h"

namespace nsApp
{
    namespace nsProduction
    {
        class CutInSlideLayer;

        class CutInShadowLayer : public CutInBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            CutInShadowLayer() = default;
            virtual ~CutInShadowLayer() = default;


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
             * @param rc 
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


        public:
            /**
             * @brief ターゲットの CutInSlideLayer をセットする。
             * @param target ターゲットの CutInSlideLayer。
             */
            inline void SetTarget(CutInSlideLayer* target) { pTargetLayer_ = target; }

            /**
             * @brief オフセットをセットする。
             * @param offset ターゲットからのオフセット座標。
             */
            inline void SetOffSet(const Vector3& offset) { offset_ = offset; }

            /**
             * @brief ターゲットの現在座標を取得する。
             * @return ターゲットの現在座標。
             */
            inline const Vector3& GetCurrentPos() const { return currentPos_; }


        private:
            CutInSlideLayer* pTargetLayer_ = nullptr; //! < ターゲットの CutInSlideLayer。
            Vector3 offset_ = Vector3::Zero;          //! < ターゲットからのオフセット座標。
            Vector3 currentPos_ = Vector3::Zero;      //! < ターゲットの現在座標。
            Vector3 ejectStartPos_ = Vector3::Zero;   //! < ターゲットの飛び出す開始座標。
            float ejectTimer_ = 0.0f;                 //! < 飛び出すアニメーションのタイマー。
            bool isEjecting_ = false;                 //! < 飛び出すアニメーション中かどうか。
        };
    } // namespace nsProduction
} // namespace nsApp

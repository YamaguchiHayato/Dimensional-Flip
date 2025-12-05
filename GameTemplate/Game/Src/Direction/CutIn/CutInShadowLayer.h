#pragma once
#include "Src/Direction/CutIn/CutInBase.h"


namespace app
{
    namespace cutIn
    {
        class CutInSlideLayer;

        class CutInShadowLayer : public CutInBase
        {
        public:
            CutInShadowLayer() = default;
            virtual ~CutInShadowLayer() = default;

            // 初期化処理。
            bool Start() override;


            // 更新処理。
            void Update() override;


            // 描画処理。
            void Render(RenderContext& rc) override;


            // パス取得処理。
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }
        // セッター。
        public:
            // 追従ターゲットをセットする。
            inline void SetTarget(CutInSlideLayer* target)
            {
                pTargetLayer_ = target;
            }


            // 影用の画像のズレ幅をセットする。
            inline void SetOffSet(const Vector3& offset)
            {
                offset_ = offset;
            }


        // ゲッター。
        public:
            inline const Vector3& GetCurrentPos() const
            {
                return currentPos_;
            }

        private:
            // 飛び出し開始位置を取得する。

        private:
            CutInSlideLayer* pTargetLayer_ = nullptr; // 追従ターゲットレイヤー。
            Vector3 offset_ = Vector3::Zero;          // 影用の画像のズレ幅。
            Vector3 currentPos_ = Vector3::Zero;      // 現在の位置。

            // 影としての処理用のパラメータ。
            Vector3 ejectStartPos_ = Vector3::Zero; // 飛び出し開始位置
            float ejectTimer_ = 0.0f;               // 経過時間
            bool isEjecting_ = false;               // 飛び出し済みかどうかのフラグ
        };

    }
}


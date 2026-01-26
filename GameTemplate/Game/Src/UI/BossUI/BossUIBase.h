#pragma once
#include "math/Matrix.h"


namespace app
{
    namespace nsUI
    {
        class BossUIBase
        {
        protected:
            // 座標と行列関係をもつ構造体。
            struct UIMatrix
            {
                // ローカル座標。
                Vector3 localPosition_ = Vector3::Zero; // ローカル座標。
                Vector3 localScale_ = Vector3::One;     // ローカルスケール。

                // 最終的な計算結果。(描画時に実装)
                Matrix warldMatrix_ = Matrix::Identity; // ワールド行列。
            };


        protected:
            UIMatrix transform_;
            BossUIBase* pParent_ = nullptr;

            // 各UIクラスが持つスプライトレンダラー。
            SpriteRender bossUIRender_;


        protected:
            // スプライト名からUI名を取得する。
            inline const std::string FetchUIName(const std::string spriteName)
            {
                // パス取得。
                std::string path = "Assets/UI/BossUI/" + spriteName + ".DDS";
                return path;
            }


        public:
            // 外部から親を設定する。
            inline void SetParent(BossUIBase* pParent)
            {
                pParent_ = pParent;
            }

            // 行列更新処理。
            virtual void UpdateMatrix();

            // 更新処理。
            virtual void Update() {};

            // 描画処理。
            virtual void Draw(RenderContext& rc) = 0;


        public:
            // スケール行列を取得。
            inline const UIMatrix& GetTransform() const
            {
                return transform_;
            }

            // 計算後のワールド行列を取得。
            inline const Matrix& GetWorldMatrix() const
            {
                return transform_.warldMatrix_;
            }  

            // 現在のローカ座標を取得、
            inline const Vector3& GetLocalPosition() const
            {
                return transform_.localPosition_;
            }


        private:
            Matrix scale_;
            Matrix translation_;
            Matrix localMatrix_;


        };

    }
}

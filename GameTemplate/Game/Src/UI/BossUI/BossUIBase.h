#pragma once

#include "math/Matrix.h"

/**
 * @file   BossUIBase.h
 * @brief  ボス戦 HUD パーツの基底クラス。親子行列とスプライト描画の共通処理。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class BossUIBase
         * @brief ローカル座標・スケールからワールド行列を計算する UI 基底。
         */
        class BossUIBase
        {
        protected:
            /**
             * @struct UIMatrix
             * @brief ローカル変換と計算済みワールド行列を保持する。
             */
            struct UIMatrix
            {
                Vector3 localPosition_ = Vector3::Zero; //!< 親からのローカル座標。
                Vector3 localScale_ = Vector3::One;     //!< ローカルスケール。
                Matrix warldMatrix_ = Matrix::Identity; //!< 描画用ワールド行列。
            };

        protected:
            UIMatrix transform_;            //!< この UI パーツの変換。
            BossUIBase* pParent_ = nullptr; //!< 親 UI（BossUIManager 等）。
            SpriteRender bossUIRender_;     //!< 共通スプライト（派生で未使用のことも）。

        protected:
            /**
             * @brief DDS パスを組み立てる。
             * @param spriteName アセット名（拡張子なし）。
             * @return `Assets/UI/BossUI/<name>.DDS` 形式のパス。
             */
            inline const std::string FetchUIName(const std::string spriteName)
            {
                return "Assets/UI/BossUI/" + spriteName + ".DDS";
            }

        public:
            /**
             * @brief 親 UI を設定する。子は親のワールド行列に連動する。
             * @param pParent 親 BossUIBase。nullptr でルート扱い。
             */
            inline void SetParent(BossUIBase* pParent) { pParent_ = pParent; }

            /**
             * @brief ローカル行列を合成し、親があればワールド行列に乗算する。
             */
            virtual void UpdateMatrix();

            /**
             * @brief 毎フレームの更新（派生でオーバーライド）。
             */
            virtual void Update() {}

            /**
             * @brief 描画処理（派生で実装）。
             * @param rc レンダリングコンテキスト。
             */
            virtual void Draw(RenderContext& rc) = 0;

            /**
             * @brief ローカル変換情報を取得する。
             */
            inline const UIMatrix& GetTransform() const { return transform_; }

            /**
             * @brief 計算済みワールド行列を取得する。
             */
            inline const Matrix& GetWorldMatrix() const { return transform_.warldMatrix_; }

            /**
             * @brief ローカル座標を取得する。
             */
            inline const Vector3& GetLocalPosition() const { return transform_.localPosition_; }

        private:
            Matrix scale_;       //!< スケール行列（UpdateMatrix 用）。
            Matrix translation_; //!< 移動行列（UpdateMatrix 用）。
            Matrix localMatrix_; //!< ローカル合成行列（UpdateMatrix 用）。
        };
    } // namespace nsUI
} // namespace nsApp

using BossUIBase = nsApp::nsUI::BossUIBase;

namespace app
{
    namespace nsUI
    {
        using BossUIBase = nsApp::nsUI::BossUIBase;
    }
} // namespace app

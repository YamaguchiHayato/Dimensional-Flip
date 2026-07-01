#pragma once

#include "Transform.h"

/**
 * @file   UITransform.h
 * @brief  UI 専用 Transform。ピボット・アンカー・矩形サイズを追加する。
 * @details
 *  Unity の RectTransform のうち、まず必要な部分だけを実装する。
 *  画面中央原点の 2D UI 座標系を前提とする。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @enum UIAnchorPreset
         * @brief よく使うアンカー位置のプリセット。
         */
        enum class UIAnchorPreset : uint8_t
        {
            Center,       //!< 画面中央
            TopLeft,      //!< 左上
            TopCenter,    //!< 上中央
            TopRight,     //!< 右上
            MiddleLeft,   //!< 左中央
            MiddleRight,  //!< 右中央
            BottomLeft,   //!< 左下
            BottomCenter, //!< 下中央
            BottomRight,  //!< 右下
        };

        /**
         * @class UITransform
         * @brief UI 要素の配置・サイズ・ピボットを管理する。
         */
        class UITransform : public Transform
        {
        public:
            UITransform() = default;
            virtual ~UITransform() = default;

            /**
             * @brief 参照解像度を設定する（アンカー計算用）。
             * @param width  画面幅（例: 1920）。
             * @param height 画面高さ（例: 1080）。
             */
            static void SetReferenceResolution(float width, float height);

            /**
             * @brief アンカープリセットを設定する。
             * @param preset アンカー位置。
             */
            void SetAnchorPreset(UIAnchorPreset preset);

            /**
             * @brief アンカー基準点からの追加オフセットを設定する。
             * @param offset アンカーからのずれ（ピクセル相当）。
             */
            void SetAnchoredPosition(const Vector2& offset);

            /** @brief アンカーオフセットを取得する。 */
            const Vector2& GetAnchoredPosition() const { return anchoredPosition_; }

            /**
             * @brief UI 矩形のサイズを設定する。
             * @param size 幅・高さ（SpriteRender::Init の w/h に対応）。
             */
            void SetSize(const Vector2& size);

            /**
             * @brief UI 矩形のサイズを取得する。
             * @return 幅・高さ（SpriteRender::Init の w/h に対応）。
             */
            const Vector2& GetSize() const { return size_; }

            /**
             * @brief ピボットを設定する（0〜1、Sprite と同じ）。
             * @param pivot ピボット。
             */
            void SetPivot(const Vector2& pivot);

            /**
             * @brief ピボットを取得する（0〜1、Sprite と同じ）。
             * @return ピボット。
             */
            const Vector2& GetPivot() const { return pivot_; }

            /**
             * @brief アンカーとオフセットからローカル座標を再計算する。
             * @details Update 時やレイアウト変更時に呼ぶ。
             */
            void RebuildLayout();

            /**
             * @brief 計算済み Transform を SpriteRender に反映する。
             * @param sprite 反映先。
             */
            void ApplyTo(SpriteRender& sprite) const;

            /**
             * @brief 計算済み Transform を FontRender に反映する。
             * @param font 反映先。
             */
            void ApplyTo(FontRender& font) const;

        private:
            /**
             * @brief アンカー位置を計算する。
             * @return アンカー位置（参照解像度に対するピクセル座標）。
             */
            Vector2 CalcAnchorPoint() const;


        private:
            static float referenceWidth_;  //!< 参照画面幅。
            static float referenceHeight_; //!< 参照画面高さ。

            UIAnchorPreset anchorPreset_ = UIAnchorPreset::Center; //!< 現在のアンカー。
            Vector2 anchoredPosition_ = Vector2::Zero;             //!< アンカーからのオフセット。
            Vector2 size_ = Vector2(100.0f, 100.0f);               //!< UI 矩形サイズ。
            Vector2 pivot_ = {0.5f, 0.5f};                         //!< 回転・拡大の基準点。
        };
    } // namespace nsUI
} // namespace nsApp

using UITransform = nsApp::nsUI::UITransform;
using UIAnchorPreset = nsApp::nsUI::UIAnchorPreset;

namespace app
{
    namespace nsUI
    {
        using UITransform = nsApp::nsUI::UITransform;
        using UIAnchorPreset = nsApp::nsUI::UIAnchorPreset;
    } // namespace nsUI
} // namespace app

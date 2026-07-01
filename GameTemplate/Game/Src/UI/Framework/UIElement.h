#pragma once

#include <memory>
#include <string>
#include <vector>

#include "UIImage.h"

/**
 * @file   UIElement.h
 * @brief  UI ツリーのノード。子要素を持てる。
 * @details
 *  パッチノート画面の「Window」「ListItem」などを
 *  親子関係で組み立てるための薄いラッパー。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIElement
         * @brief 名前付き UI ノード。UIImage を 0〜1 個持てる。
         */
        class UIElement
        {
        public:
            /* コンストラクタとデストラクタ。*/
            explicit UIElement(const char* name = "UIElement");
            virtual ~UIElement() = default;


        public:
            /**
             * @brief このノードの名前を取得する。
             * @return ノード名。
             */
            const std::string& GetName() const { return name_; }

            /**
             * @brief このノードの Transform を取得する。
             * @return ノードの Transform。
             */
            UITransform& GetTransform() { return transform_; }

            /**
             * @brief このノードの Transform を取得する（const）。
             * @return ノードの Transform。
             */
            const UITransform& GetTransform() const { return transform_; }

            /**
             * @brief 子ノードを追加する。
             * @param child 子。親は自動設定される。
             */
            void AddChild(std::shared_ptr<UIElement> child);

            /**
             * @brief 紐づく UIImage を設定する。
             * @param image UIImage。nullptr で解除。
             */
            void SetImage(std::shared_ptr<UIImage> image) { image_ = image; }

            /**
             * @brief 紐づく UIImage を取得する。
             * @return UIImage。nullptr なら未設定。
             */
            UIImage* GetImage() const { return image_.get(); }

            /**
             * @brief 表示・非表示を切り替える。
             * @param active true なら表示、false なら非表示。
             */
            void SetActive(bool active);

            /**
             * @brief 表示・非表示を取得する。
             */
            void UpdateRecursive();

            /**
             * @brief 表示・非表示を取得する。
             * @param rc 描画コンテキスト。
             */
            void DrawRecursive(RenderContext& rc) const;


        private:
            std::string name_;                                 //!< デバッグ用ノード名。
            UITransform transform_;                            //!< このノードの Transform。
            std::shared_ptr<UIImage> image_;                   //!< 任意の画像。
            std::vector<std::shared_ptr<UIElement>> children_; //!< 子ノード。
            bool isActive_ = true;                             //!< 表示フラグ。
        };
    } // namespace nsUI
} // namespace nsApp

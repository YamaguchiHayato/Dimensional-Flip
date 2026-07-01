#pragma once

#include <string>

#include "UITransform.h"

/**
 * @file   UIImage.h
 * @brief  UI 用の静止画像コンポーネント。
 * @details
 *  DDS パス・表示サイズ・初期化をここに集約する。
 *  PatchNote 用画像は Assets/UI/patcNote/ 以下を参照。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @struct UIImageAssetDef
         * @brief 画像1枚分のパスと表示サイズ。
         */
        struct UIImageAssetDef
        {
            const char* path; //!< DDS フルパス。
            float width;      //!< 表示幅。
            float height;     //!< 表示高さ。
        };

        /**
         * @namespace UIImageAsset
         * @brief プロジェクト内 UI 画像の定義一覧。
         */
        namespace UIImageAsset
        {
            //! PatchNote（Assets/UI/patcNote/）
            const UIImageAssetDef PatchNoteWindow = {"Assets/UI/patcNote/note.DDS", 1024.0f, 1024.0f};
            const UIImageAssetDef PatchNoteBanner = {"Assets/UI/patcNote/banner.DDS", 520.0f, 96.0f};
            const UIImageAssetDef PatchNoteListSelect = {"Assets/UI/patcNote/select.DDS", 360.0f, 56.0f};
            const UIImageAssetDef PatchNoteListUnselect = {"Assets/UI/patcNote/unselect.DDS", 360.0f, 56.0f};
            const UIImageAssetDef PatchNoteBadge = {"Assets/UI/patcNote/badge.DDS", 96.0f, 96.0f};
            const UIImageAssetDef ButtonMenu = {"Assets/UI/patcNote/Menu.DDS", 1920.0f, 1080.0f};

            //! 共通 UI
            const UIImageAssetDef ButtonB = {"Assets/UI/button/ButtonB.DDS", 1280.0f, 706.0f};
            const UIImageAssetDef TitleBlack = {"Assets/title/Black.DDS", 512.0f, 512.0f};
        } // namespace UIImageAsset


        /**
         * @class UIImage
         * @brief 1 枚の UI スプライトを表示する。
         */
        class UIImage
        {
        public:
            /* コンストラクタとデストラクタ。*/
            UIImage() = default;
            virtual ~UIImage() = default;


        public:
            /**
             * @brief UIImage を初期化する。
             * @param texturePath DDSのファイルパス。
             * @param width 横幅。
             * @param height 縦幅。
             */
            void Init(const char* texturePath, float width, float height);

            /**
             * @brief UIImage を初期化する。
             */
            void InitFromUIAsset(const std::string& uiName, float width, float height);

            /**
             * @brief UIImage を初期化する。
             * @param assetDef UIImageAssetDef 構造体の参照。
             */
            void InitFromAssetDef(const UIImageAssetDef& assetDef);

            //! --- PatchNote 用ショートカット ---
            void InitPatchNoteWindow();
            void InitPatchNoteBanner();
            void InitPatchNoteListSelect();
            void InitPatchNoteListUnselect();
            void InitPatchNoteBadge();
            void InitDimOverlay();
            void InitButtonB();
            void InitButtonMenu();

            /**
             * @brief UIImage を更新する。Transform の更新を行う。
             */
            void Update();

            /**
             * @brief UIImage を描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);

            /**
             * @brief UIImage の表示・非表示を切り替える。
             * @param visible 表示フラグ。
             */
            void SetVisible(bool visible) { isVisible_ = visible; }

            /**
             * @brief UIImage の表示状態を返す。
             * @return true なら表示中、false なら非表示。
             */
            bool IsVisible() const { return isVisible_; }

            /**
             * @brief UIImage の Transform を返す。
             * @return UIImage の Transform への参照。
             */
            UITransform& GetTransform() { return transform_; }

            /**
             * @brief UIImage の Transform を返す（const 版）。
             * @return UIImage の Transform への const 参照。
             */
            const UITransform& GetTransform() const { return transform_; }

            /**
             * @brief UIImage の SpriteRender を返す。
             * @return UIImage の SpriteRender への参照。
             */
            SpriteRender& GetSprite() { return sprite_; }

            /**
             * @brief UIImage の SpriteRender を返す（const 版）。
             * @return UIImage の SpriteRender への const 参照。
             */
            const SpriteRender& GetSprite() const { return sprite_; }

            /**
             * @brief UIImage の色を設定する。
             * @param color RGBA の色。0.0f～1.0f。
             */
            void SetColor(const Vector4& color);


        private:
            SpriteRender sprite_; //! < DDS スプライト描画。
            UITransform transform_; //! < Transform。位置・拡大率・回転を保持。
            bool isVisible_ = true; //! < 表示フラグ。
            bool isInitialized_ = false; //! < 初期化済みフラグ。Init() で true になる。
        };
    } // namespace nsUI
} // namespace nsApp

using UIImage = nsApp::nsUI::UIImage;

namespace app
{
    namespace nsUI
    {
        using UIImage = nsApp::nsUI::UIImage;
    } // namespace nsUI
} // namespace app

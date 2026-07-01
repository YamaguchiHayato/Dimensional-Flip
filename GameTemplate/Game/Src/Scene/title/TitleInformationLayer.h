#pragma once
#include "Src/Scene/title/TitleLayerBase.h"
#include "Src/UI/PatchNote/TitlePatchNoteHintUI.h"


namespace nsApp
{
    namespace nsTitle
    {
        class TitleInformationLayer : public TitleLayerBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TitleInformationLayer() = default;
            virtual ~TitleInformationLayer() = default;


        public:
            /**
             * @brief シーン開始時の初期化処理。
             * @return true: 初期化成功、false: 初期化失敗
             */
            bool Start() override;

            /**
             * @brief シーン更新処理。
             */
            void Update() override {};

            /**
             * @brief シーン描画処理。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;


        private:
            /**
             * @brief Version表記テキストを初期化する。
             */
            void InitVersionText();

            /**
             * @brief 権利表記テキストを初期化する。
             */
            void InitCopyRightText();


        // ゲッター。
        public:
            /**
             * @brief レイヤー名を初期化する。
             * @param layerName レイヤー名。
             * @return 初期化後のレイヤー名。
             */
            inline const std::string Init(const std::string layerName) override
            {
                return TitleLayerBase::Init(layerName);
            }

            /**
             * @brief レイヤーの表示状態を取得する。
             * @param isVisble 表示状態。
             */
            inline void SetVisible(bool isVisble)
            {
                isVisible_ = isVisble;
                patchNoteHint_.SetVisible(isVisble);
            }


        private:
            nsUI::TitlePatchNoteHintUI patchNoteHint_; //! パッチノート操作ヒントUI。

            FontRender versionText_; //! Ver 表記テキスト。
            FontRender copyRightText_; //! 権利表記テキスト。

            bool isVisible_ = true; //! < レイヤーの表示状態。
        };
    }
}

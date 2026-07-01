#pragma once
#include "Src/UI/Framework/UITransform.h"    
#include "Src/UI/Framework/UIImage.h"    


namespace nsApp
{
    namespace nsUI
    {
        /**
         * @file   TitlePatchNoteHintUI.h
         * @brief  タイトル画面の Menu 画像と "Information" 表記。
         * @details
         *  Start / Select でパッチノートを開く操作ヒント（表示のみ）。
         *  TitleInformationLayer から使う。
         */
        class TitlePatchNoteHintUI
        {
        public:
            /**
             * @brief TitlePatchNoteHintUI を初期化する。
             * @param parent 親 Transform。nullptr の場合は画面座標。
             */
            void Init(UITransform* parent);

            /**
             * @brief TitlePatchNoteHintUI を更新する。
             */
            void Update();

            /**
             * @brief TitlePatchNoteHintUI を描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);

            /**
             * @brief TitlePatchNoteHintUI の表示・非表示を切り替える。
             * @param visible 表示フラグ。
             */
            void SetVisible(bool visible);


        private:
            UIImage menu_;        //! Menu.DDS。
            FontRender infoText_; //! L"Information"。
            bool isVisible_ = false; //! < 表示フラグ。
        };
    }
}

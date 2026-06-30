#pragma once

// レイヤー分けしたタイトル画面クラス。
#include "Src/Scene/title/TitleBackgroundLayer.h"
#include "Src/Scene/title/TitleLayerBase.h"
#include "Src/Scene/title/TitleLogoLayer.h"
#include "Src/UI/Select/TitleMenu.h" 

#include "Src/Scene/title/TitleInformationLayer.h"

namespace nsApp
{
    namespace nsTitle
    {
        class TitleInformationLayer;

        class TitleView : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TitleView() = default;
            virtual ~TitleView();


        public:
            /**
             * @brief タイトル画面の初期化処理。
             * @return 成功時 true。
             */
            bool Start();


        public:
            /**
             * @brief ロゴの表示切替。
             * @param isShow true: 表示する、false: 非表示にする。
             */
            inline void SetShowLogo(bool isShow)
            {
                if (pLogoLayer_)
                    pLogoLayer_->SetVisible(isShow);
            }

            /**
             * @brief マニュアルモードの切り替え。
             * @param enable true: マニュアルモード有効、false: マニュアルモード無効。
             */
            inline void SetManualMode(bool enable)
            {
                if (pBackgroundLayer_)
                    pBackgroundLayer_->SetManualMode(enable);
            }

            /**
             * @brief 情報表示の切替。
             * @param isShow true: 表示する、false: 非表示にする。
             */
            inline void SetShowInformation(bool isShow)
            {
                if (pInformationLayer_)
                    pInformationLayer_->SetVisible(isShow);
            }


        private:
            TitleBackgroundLayer* pBackgroundLayer_ = nullptr;
            TitleLogoLayer* pLogoLayer_ = nullptr;
            TitleInformationLayer* pInformationLayer_ = nullptr;


        private:
            bool gameLoadFlag = false;
        };

    }
}

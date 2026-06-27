#pragma once
#include "Src/Actor/Stage/BackGround/ScrollLayerConfig.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            /**
             * @struct ScrollBackGroundLayerParameter 
             * @brief スクロール背景のレイヤー定義パラメータ。
             */
            struct ScrollBackGroundLayerParameter
            {
                std::string stageId;                              //! ステージID。
                ScrollLayerKind layerKind = ScrollLayerKind::Sky; //! < レイヤー種別。
                std::string texturePath;                          //!< DDS ファイルパス。
                float parallax = 0.0f;                            //! カメラ追従率。
                int tileCount = 1;                                //! タイル枚数。
                int renderOrder = 0;                              //! 描画順。
                float screenHeightRatio = 1.0f;                   //! 画面高さに対する表示比率（0〜1）。
                float screenCenterY = 0.0f;                       //! 画面中心原点 Y。Ground は 0 で下端揃え計算。
                float parallaxPixelScale = 1.0f;                  //! ワールド X → スクロール px 変換係数（微調整用）。
                bool anchorBottom = false;                        //! true なら screenCenterY を無視して下端揃え。
            };
        }
    }
}

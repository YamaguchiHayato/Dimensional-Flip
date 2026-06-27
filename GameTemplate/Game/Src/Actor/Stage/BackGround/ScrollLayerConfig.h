#pragma once

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            /**
             * @brief スクロール背景のレイヤー種別。
             */
            enum class ScrollLayerKind
            {
                Sky,      //!< 最奥の空。
                Mountain, //!< 中景の山。
                Ground,   //!< 手前の地面。
            };

            /**
             * @brief レイヤー1枚分の定義データ。
             */
            struct ScrollLayerDefinition
            {
                ScrollLayerKind kind = ScrollLayerKind::Sky; //!< レイヤー種別。
                const char* texturePath = nullptr;           //!< DDS ファイルパス。
                const char* fallbackTexturePath = nullptr;   //!< DDS 失敗時の WIC 用パス。
                float parallax = 0.0f;                       //!< カメラ追従率。
                float worldY = 0.0f;                         //!< ワールド Y。
                float worldZ = 0.0f;                         //!< ワールド Z。
                float drawScale = 1.0f;                      //!< 表示スケール。
                float tileWorldWidth = 512.0f;               //!< タイル横幅。
                int tileCount = 4;                           //!< タイル枚数。
                int renderOrder = 0;                         //!< 描画順。

                float screenY = 0.0f;                        //!< 3D 画面 Y（中心原点。Font と同じ）。
                float screenScale = 0.5f;                    //!< 3D 表示スケール。
                float screenTileWidth = 960.0f;              //!< 3D タイル横幅（px 相当）。0 なら texW*screenScale。

                float screenHeightRatio = 1.0f;              //!< 画面高さに対する表示比率（0〜1）。
                float screenCenterY = 0.0f;                  //!< 画面中心原点 Y。Ground は 0 で下端揃え計算。
                float parallaxPixelScale = 1.0f;             //!< ワールド X → スクロール px 変換係数（微調整用）。
                bool anchorBottom = false;                   //!< true なら screenCenterY を無視して下端揃え。
            };

            /**
             * @brief 通常ステージ背景のレイヤー定義配列を取得する。
             * @return 定義配列の先頭ポインタ。失敗時は nullptr。
             */
            const ScrollLayerDefinition* GetNormalStageLayerDefinitions();

            /**
             * @brief 通常ステージ背景のレイヤー定義配列の要素数を取得する。
             * @return 要素数。失敗時は 0。
             */
            int GetNormalStageLayerDefinitionCount();

            /**
             * @brief 指定したレイヤー種別に対応する定義を取得する。
             * @param kind レイヤー種別。
             * @return 対応する定義。見つからない場合は最初の定義を返す。
             */
            const ScrollLayerDefinition& FindLayerDefinition(ScrollLayerKind kind);

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp

#pragma once

/**
 * @file   CutInBase.h
 * @brief  カットイン各レイヤーの基底クラス。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @class CutInBase
         * @brief  カットインレイヤー共通の SpriteRender とパス解決。
         */
        class CutInBase : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            CutInBase() = default;
            virtual ~CutInBase() = default;

            /**
             * @brief 初期化処理。
             * @return 成功時 true。
             */
            virtual bool Start() = 0;

            /**
             * @brief 更新処理。
             */
            virtual void Update() = 0;

            /**
             * @brief 描画処理。
             * @param rc レンダリングコンテキスト。
             */
            virtual void Render(RenderContext& rc) = 0;

            /**
             * @brief レイヤー DDS のパスを組み立てる。
             * @param layerName ファイル名（拡張子なし）。
             * @return Assets 配下のフルパス。
             */
            virtual inline const std::string FindLayerPath(const std::string layerName) const = 0
            {
                const std::string layerPath = "Assets/sprite/CutIn/" + layerName + ".DDS";
                return layerPath;
            }


        protected:
            SpriteRender layer_; //!< レイヤー用スプライト
        };
    } // namespace nsProduction
} // namespace nsApp

using CutInBase = nsApp::nsProduction::CutInBase;

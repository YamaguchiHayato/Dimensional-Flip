#pragma once


namespace nsApp
{
    namespace nsTitle
    {
        class TitleLayerBase : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TitleLayerBase() {};
            virtual ~TitleLayerBase() {};

            /**
             * @brief シーン入場時の初期化。必要な GO を NewGO する。
             * @return 成功時 true。
             */
            virtual bool Start() = 0 { return true; };

            /**
             * @brief 毎フレームのシーン更新（遷移判定・入力など）。
             */
            virtual void Update() = 0 {};

            /**
             * @brief 毎フレームのシーン描画。
             * @param rc レンダリングコンテキスト。
             */
            virtual void Render(RenderContext& rc) = 0 {};

            /**
             * @brief タイトルレイヤーの初期化。
             * @param layerNane レイヤー名。
             * @return ファイルパス。
             */
            inline virtual const std::string Init(const std::string layerNane) = 0
            {
                std::string titleLayerPath = "Assets/title/" + layerNane + ".DDS";
                return titleLayerPath;
            }

        protected:
            SpriteRender titleLayerRender_;
        };
    }
}

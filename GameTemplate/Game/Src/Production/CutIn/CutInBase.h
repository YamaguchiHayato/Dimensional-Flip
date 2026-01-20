#pragma once

// カットイン機能の基底クラス。
namespace app
{
    namespace cutIn
    {
        class CutInBase : public IGameObject
        {
        public:
            CutInBase() = default;
            virtual ~CutInBase() = default;

            // 初期化処理。
            virtual bool Start() = 0;


            // 更新処理。
            virtual void Update() = 0;


            // 描画処理。
            virtual void Render(RenderContext& rc) = 0;


            // レイヤーのパスを取得する。
            virtual inline const std::string FindLayerPath(const std::string layerName) const = 0
            {
                const std::string layerPath = "Assets/sprite/CutIn/" + layerName + ".DDS";
                return layerPath;
            }


        protected:
            SpriteRender layer_;           // 基底レイヤー。
            
        };

    }
}

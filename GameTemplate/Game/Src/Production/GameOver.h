#pragma once

namespace app
{
    namespace direction
    {
        class GameOver : public IGameObject
        {
        public:
            GameOver() = default;
            virtual ~GameOver() = default;

            bool Start();
            void Update();
            void Render(RenderContext& rc);

        private:
            SpriteRender sprite_;
        };

    }
}

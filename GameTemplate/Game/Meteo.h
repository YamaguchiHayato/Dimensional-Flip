#pragma once

namespace app
{
    namespace meteo
    {
        class Meteo : public IGameObject
        {
        public:
            Meteo() = default;
            virtual ~Meteo() = default;

            bool Start();
            void Update();
            void Render(RenderContext& rc);


        };

    }
}


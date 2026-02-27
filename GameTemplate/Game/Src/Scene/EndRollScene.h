#pragma once
#include "Src/Scene/Scene.h"
#include <vector>

namespace nsK2EngineLow
{
    class Camera;
}


namespace app
{
    namespace production
    {
        class EndRollManager;

        class EndRollScene : public IScene
        {
        public:
            EndRollScene() = default;
            virtual ~EndRollScene();


        public:
            // 初期化処理。
            bool Start() override;
            // 更新処理。
            void Update() override;


        public:
            // 終了判定。
            bool IsEnd() const;


        private:
            // エンドロールの管理クラス。
            EndRollManager* pEndRollManager_ = nullptr;

            // カメラ。
            nsK2EngineLow::Camera* pCamera_ = nullptr;
        };
    }
}

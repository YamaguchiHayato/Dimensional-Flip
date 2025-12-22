#pragma once
#include "Src/Actor/Actor.h"
class TrackingEnemy;


namespace app
{
    namespace enemy
    {
        class IEnemy : public Actor
        {
        public:
            IEnemy() {};
            virtual ~IEnemy() {};

        public:
            // 基本関数。
            virtual bool Start()override = 0;
            virtual void Update()override = 0;
            virtual void Render(RenderContext& rc)override = 0;

        // モデルパスの取得。
        public:
            inline const virtual std::string InitModel(const std::string& enemyName)
            {
                std::string enemyPath = "Assets/stage/" + enemyName + ".tkm";
                return enemyPath;
            };

        // セッター。
        public:
            // 座標をセット。
            inline virtual  void SetPos(const Vector3& pos) = 0;
            // 踏みつけて倒せるかどうか。
            inline virtual  void SetStompable(bool enable) = 0;
            // 大きさ。
            inline virtual  void SetScale(const Vector3& scale) = 0;
        // ゲッター。
        public:
            inline bool IsDead() const {}

        };

    }
}



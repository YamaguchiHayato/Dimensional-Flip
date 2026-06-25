#pragma once
#include "ResultData.h"
#include "Src/Actor/Character/PlayerForward.h"

enum class StarGetPhase
{
    Approach, // ズーム & 接近
    Orbit,    // スターの周りを回転
    Finish,   // 左側で固定 & スター上昇開始
    FadeOut,  // フェードアウト
    End,      // 終了（遷移）
    Num       // 初期化用
};

class Star;
class GameClear;

// スターを取った時の演出を行うクラス。
namespace app
{
    namespace production
    {
        class StarGetProduction : public IGameObject
        {
        public:
            StarGetProduction() = default;
            virtual ~StarGetProduction() = default;


        public:
            // 演出を開始する
            void StartSequence(Player* pPlayer, Star* pStar);

            // 更新処理。
            void Update() override;


        private:
            // 演出を開始する。
            void UpdateApproach(float deltaTime);
            void UpdateOrbit(float deltaTime);
            void UpdateFinish(float deltaTime);

            // リザルトデータを収集してStageManagerクラスに送信。
            void CollectAndSendResultData();


        private:
            Player* pPlayer_ = nullptr;
            Star* pStar_ = nullptr;
            GameClear* pGameClear_ = nullptr;

        private:
            StarGetPhase currentPhase_ = StarGetPhase::Num;
            StageResultData resultData_;

            SpriteRender stegeClearSprite_; 

            float timer_ = 0.0f;
            float duration_ = 0.0f;

            // 演出開始時の各座標を保存。
            Vector3 starPos_ = Vector3::Zero;
            Vector3 playerStartPos_ = Vector3::Zero;
            float initialCameraHeight_ = 0.0f;
        };

    }
}


#pragma once

#include "ResultData.h"
#include "Src/Actor/Character/PlayerForward.h"
#include "Src/Production/GameClear.h"
/**
 * @file   StarGetProduction.h
 * @brief  スター取得時のカメラ演出とリザルト遷移。
 */

/**
 * @enum StarGetPhase
 * @brief スター取得演出のフェーズ。
 */
enum class StarGetPhase
{
    Approach, //!< ズーム＆接近
    Orbit,    //!< スター周回
    Finish,   //!< 左固定＆スター上昇
    FadeOut,  //!< フェードアウト
    End,      //!< 終了（遷移）
    Num
};

class Star;

namespace app
{
    namespace production
    {
        /**
         * @class StarGetProduction
         * @brief スター取得時のカメラ演出と sResult シーン遷移を制御する。
         */
        class StarGetProduction : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            StarGetProduction() = default;
            virtual ~StarGetProduction() = default;

            /**
             * @brief 演出シーケンスを開始する。
             * @param pPlayer 操作プレイヤー。
             * @param pStar   取得対象スター。
             */
            void StartSequence(Player* pPlayer, Star* pStar);

            /**
             * @brief シーン更新処理。
             */
            void Update() override;


        private:
            /**
             * @brief スター取得演出の各フェーズを更新する。
             * @param deltaTime 前フレームからの経過時間。
             */
            void UpdateApproach(float deltaTime);

            /**
             * @brief スター周回フェーズを更新する。
             * @param deltaTime 前フレームからの経過時間。
             */
            void UpdateOrbit(float deltaTime);

            /**
             * @brief スター上昇フェーズを更新する。
             * @param deltaTime 
             */
            void UpdateFinish(float deltaTime);

            /**
             * @brief タイム・スコアを集計し static な StageResultData に保存する。
             * @note  ChangeScene(sResult) より前に呼ぶこと。
             */
            void CollectAndSendResultData();


        private:
            Player* pPlayer_ = nullptr; //!< 操作プレイヤー。
            Star* pStar_ = nullptr;     //!< 取得対象スター。
            GameClear* pGameClear_ = nullptr; //! < リザルト演出。

            StarGetPhase currentPhase_ = StarGetPhase::Num; //!< 現在の演出フェーズ。
            nsApp::nsStage::StageResultData resultData_;    //!< ステージ結果データ。
            
            SpriteRender stegeClearSprite_; //!< ステージクリア表示用スプライト。

            float timer_ = 0.0f; //!< フェーズ経過時間。
            float duration_ = 0.0f; //!< フェーズ継続時間。
            float initialCameraHeight_ = 0.0f; //!< フェーズ開始時のカメラ高さ。

            Vector3 starPos_ = Vector3::Zero; //!< スターのワールド座標。
            Vector3 playerStartPos_ = Vector3::Zero; //!< プレイヤーのフェーズ開始時のワールド座標。
        };
    } // namespace production
} // namespace app

#pragma once

/**
 * @file   StageManager.h
 * @brief  ステージの生成・切替・更新を管理するクラス。
 */

#include "ResultData.h"
#include "Src/Actor/Actor.h"
#include "Src/Actor/Stage/Stage.h"
#include "Src/Actor/Stage/StageID.h"
#include "Src/Actor/Stage/StageObjectSpawner.h"
#include "Src/Actor/Stage/StageSetup.h"


namespace nsApp
{
    namespace nsStage
    {
        /**
         * @class StageManager
         * @brief  ステージ本体・TSV スポーン・ボス戦ルールを統括する Actor。
         */
        class StageManager : public Actor
        {
        private:
            static StageManager* pStageManger_;          //! < StageManager のシングルトンインスタンス >
            static StageID nextInitStageID_;             //! < 次回 InGame 開始時に読み込むステージ ID（WorldSelect から設定）>
            static StageResultData stageResultData_;     //!< リザルト画面用（static で InGame 破棄後も保持）


        private:
            Stage* pCurrentStage_ = nullptr;             //!< 現在の Stage GO
            StageID stageCurrentID_ = StageID::sStage1;  //!< 現在のステージ ID
            StageID stageRequestID_ = StageID::sInvalid; //!< 非同期切替リクエスト

            StageObjectSpawner spawner_;                 //!< TSV から Star / Boss / CutIn などを生成
            StageSetup stageSetup_;                      //!< ボス戦カメラ・カットイン・Pause など


        public:
            /**
             * @brief ステージマネージャーを初期化する。
             * @return 初期化に成功した場合は true、失敗した場合は false。
             */
            bool Start() override;

            /**
             * @brief ステージマネージャーを更新する。
             */
            void Update() override;

            /**
             * @brief ステージマネージャーを描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief 指定 ID の Stage GO を生成する。
             * @param id ステージ ID。
             * @return 生成した Stage。失敗時は nullptr。
             */
            Stage* CreateStage(StageID id);

            /**
             * @brief ステージを同期的に切り替える。
             * @param newStageID 切替先ステージ ID。
             */
            void ChangeStageSync(StageID newStageID);

            /**
             * @brief 次フレームでステージ切替を行う。
             * @param newStageID 切替先ステージ ID。
             */
            inline void ChangeStage(StageID newStageID) { stageRequestID_ = newStageID; }

            /**
             * @brief リザルトデータを static 領域に保存する。
             * @param data クリア演出で集計したデータ。
             */
            static inline void SetStageResult(const StageResultData& data) { stageResultData_ = data; }

            /**
             * @brief 現在ステージのプレイヤー開始位置を返す。
             * @return ワールド座標。
             */
            Vector3 GetStageStartPos() const;

            /**
             * @brief 現在のステージ ID を返す。
             * @return 現在のステージ ID。
             */
            inline StageID GetCurrentStageID() const { return stageCurrentID_; }

            /**
             * @brief リザルト画面が参照するクリアデータ。
             * @return static な StageResultData への参照。
             */
            static inline StageResultData& GetStageResultData() { return stageResultData_; }


        public:
            /* コンストラクタとデストラクタ。*/
            StageManager() = default;
            virtual ~StageManager();


        public:
            /**
             * @brief StageManager のシングルトンインスタンスを生成する。
             */
            inline static void CreateInstance()
            {
                if (!pStageManger_)
                    pStageManger_ = NewGO<StageManager>(0, "stagemanager");
            }

            /**
             * @brief StageManager のシングルトンインスタンスを破棄する。
             */
            inline static void DeleteInstance()
            {
                DeleteGO(pStageManger_);
                pStageManger_ = nullptr;
            }

            /**
             * @brief StageManager のシングルトンインスタンスを取得する。
             * @return StageManager のシングルトンインスタンス。存在しない場合は nullptr。
             */
            inline static StageManager* GetInstance() { return pStageManger_; }

            /**
             * @brief 次回 InGame 開始時に読み込むステージ ID を指定する。
             * @param id ステージ ID（WorldSelect から設定）。
             */
            inline static void SetNextInitStageID(StageID id) { nextInitStageID_ = id; }

            /**
             * @brief 次回 InGame 開始時に読み込むステージ ID を取得する。
             * @return ステージ ID（WorldSelect から設定）。
             */
            inline static StageID GetNextInitStageID() { return nextInitStageID_; }

            /**
             * @brief スポーン管理リストからオブジェクトを外す。
             * @param obj 自己削除（DeleteGO）する前に呼ぶ。Clear() のダングリング防止。
             */
            void UnregisterSpawnedObject(IGameObject* obj) { spawner_.Unregister(obj); }
        };
    } // namespace nsStage
} // namespace nsApp

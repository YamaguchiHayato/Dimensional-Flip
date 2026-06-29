#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Stage/StageID.h"

namespace nsApp
{
    namespace nsStage
    {
        class Stage : public Actor
        {
        public:
            /* コンストラクタとデストラクタ。*/
            Stage() = default;
            virtual ~Stage() = default;


        public:
            /**
			 * @brief ステージIDを設定する。
			 * @param id 設定するステージID。
             */
            inline void SetStageID(StageID id) 
            {
                stageID_ = id; 
            }

            /**
			 * @brief ステージIDを取得する。
			 * @return 現在のステージID。
             */
            inline StageID GetStageID() const 
            {
                return stageID_; 
            }

            /**
			 * @brief ステージの開始位置を取得する。
			 * @return ステージの開始位置を表すVector3。
             */
            Vector3 GetPlayerStartPos() const;

            /**
			 * @brief ステージの開始処理を行う。
			 * @return ステージの開始が成功した場合はtrue、失敗した場合はfalse。
             */
            bool Start() override;

            /**
			 * @brief ステージの更新処理を行う。
             */
            void Update() override;
            
            /**
			 * @brief ステージの描画処理を行う。
			 * @param rc 描画コンテキスト。
             */
            void Render(RenderContext& rc) override;


        private:
			StageID stageID_ = StageID::sInvalid; //! 現在のステージID。
			ModelRender stageRender_;             //! ステージのモデルレンダラー。
			PhysicsStaticObject stagePhysics_;    //! ステージの物理オブジェクト。
			Vector3 stagePos_ = Vector3::Zero;    //! ステージの位置。
        };
    }
}

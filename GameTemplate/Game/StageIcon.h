#pragma once
#include "Src/Actor/Stage/StageID.h"

namespace app
{
    namespace nsUI
    {
        class StageIcon : public IGameObject
        {
        public:
            StageIcon() = default;
            virtual ~StageIcon() = default;

        public:
            // 初期化：モデルのパスと、対応するステージIDを受け取る
            void Init(const char* modelPath, StageID stageID);
            void Update() override;
            void Render(RenderContext& rc) override;

            // ゲッター。
        public:
            // ゲッター：このアイコンがどのステージ用か
            inline StageID GetStageID() const { return stageID_; }

            // セッター。
        public:
            // 座標セット（シーンから計算された位置をもらう）
            inline void SetPosition(const Vector3& pos) { position_ = pos; }

        private:
            ModelRender model_; // 3Dモデル
            Vector3 position_ = Vector3::Zero;
            StageID stageID_ = StageID::sInvalid;
        };
    }
}


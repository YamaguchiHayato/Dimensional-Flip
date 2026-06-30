#pragma once

#include "Src/Actor/Stage/StageID.h"

/**
 * @file   StageIcon.h
 * @brief  ワールドSelect用ステージプレビューモデル。
 */

namespace app
{
    namespace nsUI
    {
        /**
         * @class StageIcon
         * @brief ステージ tkm を小さく表示する選択アイコン。
         */
        class StageIcon : public IGameObject
        {
        public:
            StageIcon() = default;
            virtual ~StageIcon() = default;

            /**
             * @brief プレビューモデルを初期化する。
             * @param modelPath ステージ tkm パス。
             * @param stageID   対応ステージ ID。
             */
            void Init(const char* modelPath, StageID stageID);

            /**
             * @brief シーン更新処理。
             */
            void Update() override;

            /**
             * @brief シーン描画処理。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;

            /** @brief このアイコンが表すステージ ID。 */
            inline StageID GetStageID() const { return stageID_; }

            /**
             * @brief ワールド座標を設定する（シーンが回転配置を計算）。
             * @param pos 表示位置。
             */
            inline void SetPosition(const Vector3& pos) { position_ = pos; }


        private:
            ModelRender model_;                   //! < プレビューモデルレンダラー
            Vector3 position_ = Vector3::Zero;    //! < ワールド座標
            StageID stageID_ = StageID::sInvalid; //! < このアイコンが表すステージ ID
        };
    } // namespace nsUI
} // namespace app

#pragma once

#include "Src/Actor/Stage/StageID.h"

/**
 * @file   StageIcon.h
 * @brief  ワールド Select 用ステージプレビューモデル。
 */

namespace nsApp
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
            /* コンストラクタとデストラクタ。*/
            StageIcon() = default;
            virtual ~StageIcon() = default;

            /**
             * @brief プレビューモデルを初期化する。
             * @param modelPath ステージ tkm パス。
             * @param stageID   対応ステージ ID。
             */
            void Init(const char* modelPath, StageID stageID);

            /**
             * @brief プレビューモデルの回転を更新する。
             */
            void Update() override;

            /**
             * @brief プレビューモデルを描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;

            /** @brief このアイコンが表すステージ ID。 */
            inline StageID GetStageID() const { return stageID_; }

            /**
             * @brief ワールド座標を設定する。
             * @param pos WorldSelectScene が計算した配置位置。
             */
            inline void SetPosition(const Vector3& pos) { position_ = pos; }


        private:
            ModelRender model_;                   //!< プレビューモデル
            Vector3     position_ = Vector3::Zero; //!< ワールド座標
            StageID     stageID_  = StageID::sInvalid; //!< 対応ステージ ID
        };
    } // namespace nsUI
} // namespace nsApp

using StageIcon = nsApp::nsUI::StageIcon;

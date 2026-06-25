#pragma once
#include "Src/Actor/Character/Player/Character2DRender.h"

class CameraManager;

namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                /**
                 * @brief Player の描画・モデル表示を担当するコンポーネント。
                 *
                 * 2D モデルの index / scale / 座標同期 / 回転を集約する。
                 * 実データ（pRender_ / currentIndex 等）は Player が所有し、Bind で接続する。
                 */
                class PlayerPresentation
                {
                public:
                    PlayerPresentation() = default;

                    /**
                     * @brief Player 側のメンバをバインドする（Start 時に1回呼ぶ）。
                     * @param[in,out] pRender      2D 描画コンポーネントへの参照
                     * @param[in,out] currentIndex   現在のモデルインデックス
                     * @param[in,out] rot            プレイヤー回転
                     * @param[in,out] offsetRot      カメラオフセット回転
                     * @param[in]     modelScale     モデル表示スケール
                     */
                    void Bind(Character2DRender*& pRender, uint8_t& currentIndex, Quaternion& rot,
                              Quaternion& offsetRot, const Vector3& modelScale);

                    /**
                     * @brief 現在のモデルインデックスをセット。
                     * @param[in] index モデル配列のインデックス
                     */
                    void SetCurrentIndex(uint8_t index);

                    /**
                     * @brief 座標と描画の同期。
                     * @param[in] pos 反映する座標
                     * @note 各 State 末尾の pRender_->SetPosition + Update をこれ1つに置き換える。
                     */
                    void SyncView(const Vector3& pos);

                    /**
                     * @brief Player::Update 末尾の描画更新（index / scale / Update）。
                     */
                    void PostFrameUpdate();

                    /**
                     * @brief カメラの状態に応じて Offset 回転を更新する。
                     * @param[in] pCameraManager カメラマネージャ
                     */
                    void UpdateOffsetRotation(CameraManager* pCameraManager);

                    /**
                     * @brief プレイヤーの回転をセット。
                     * @note offsetRot_ を rot_ に反映し、モデルへ設定する。
                     */
                    void ApplyRotation();

                    /**
                     * @brief キャラモデルを描画する。
                     * @param[in] rc レンダーコンテキスト
                     */
                    void Render(RenderContext& rc);

                    /**
                     * @brief 2D 描画コンポーネントを取得する。
                     * @return Character2DRender へのポインタ（未設定なら nullptr）
                     */
                    Character2DRender* GetRender();

                private:
                    Character2DRender** ppRender_ = nullptr; //! 2D 描画コンポーネントへのポインタの参照。
                    uint8_t* pCurrentIndex_ = nullptr;       //! 現在のモデルインデックス。
                    Quaternion* pRot_ = nullptr;             //! プレイヤー回転。
                    Quaternion* pOffsetRot_ = nullptr;       //! カメラオフセット回転。
                    Vector3 modelScale_ = Vector3::One;      //! モデル表示スケール。
                };
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor
} // namespace nsApp

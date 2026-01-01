#include "stdafx.h"

#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Gimmick/Wall.h"
// #include "Src/Input/InputManager.h"

namespace app
{
    namespace stage
    {
        Wall::~Wall()
        {
            // ポインタなので、オブジェクトが削除される時に中身も消す必要があります
            if (pPhysics_ != nullptr)
            {
                delete pPhysics_;
            }
        }

        bool Wall::Start()
        {
            pPlayer_ = FindGO<Player>("player");

            // 初期状態設定
            is2DMode_ = false;
            currentScale_ = scale_3D_Normal;

            // --- 見た目 ---
            render_.Init("Assets/stage/Stage1/Wall.tkm");
            render_.SetScale(currentScale_);
            render_.SetPosition(pos_);
            render_.SetRotation(rot_);
            render_.Update(); // ★ここで行列を確定

            // --- 物理 ---
            RefreshPhysics();

            return true;
        }

        void Wall::Update()
        {
            // ボタンでモード切替
            if (g_pad[0]->IsTrigger(enButtonB))
            {
                // フラグを反転
                is2DMode_ = !is2DMode_;

                Vector3 nextPos = pos_;

                if (is2DMode_)
                {
                    // もしここで「3Dになってしまっていた」なら、薄くする設定をここに書けばOKです
                    // [3D設定 (薄く)]
                    currentScale_ = scale_3D_Normal;
                    nextPos = pos_;         // 位置もそのまま
                    render_.SetAlpha(0.5f); // 半透明
                }
                else
                {
                    // [2D設定 (分厚く)]
                    currentScale_ = scale_2D_Wide;
                    nextPos.z = 0.0f;       // 通せんぼ用にZ=0固定
                    render_.SetAlpha(1.0f); // くっきり
                }
                // --- 変更を適用して物理を作り直す ---

                // 1. 見た目の更新
                render_.SetScale(currentScale_);
                render_.SetPosition(nextPos);
                render_.Update(); // ★重要: ここでモデルの大きさを確定させる

                // 2. 物理を作り直す
                RefreshPhysics();
            }
        }

        void Wall::RefreshPhysics()
        {
            // 1. 古い物理があれば削除 (DeleteGO)
            if (pPhysics_ != nullptr)
            {
                delete pPhysics_;
            }

            pPhysics_ = new PhysicsStaticObject();

            // 3. 確定したモデルの形に合わせて当たり判定を生成
            // (render_.Update()された後のモデル情報を使います)
            pPhysics_->CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());
           
        }

        void Wall::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    } // namespace stage
} // namespace app

#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Wall.h"
// #include "Src/Input/InputManager.h"

namespace app
{
    namespace stage
    {
        Wall::~Wall()
        {
            if (pPhysics_ != nullptr)
                delete pPhysics_;
        }


        bool Wall::Start()
        {
            pPlayer_ = FindGO<Player>("player");
            pDrawTiming_ = FindGO<app::ui::ButtonActionUI>("actionUI");
            

            // 初期状態設定
            is2DMode_ = false;
            currentScale_ = scale_3D_Normal;

            // モデルの初期設定。
            render_.Init("Assets/stage/Stage1/Wall.tkm");
            render_.SetScale(currentScale_);
            render_.SetPosition(pos_);
            render_.SetRotation(rot_);
            render_.Update(); 

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
                    // [3D設定 (薄く)]
                    currentScale_ = scale_3D_Normal;
                    nextPos = pos_;         // 位置もそのまま
                    render_.SetAlpha(0.5f); // 半透明
                }
                else
                {
                    currentScale_ = scale_2D_Wide;
                    nextPos.z = 0.0f;      
                    render_.SetAlpha(1.0f); 
                }

                render_.SetScale(currentScale_);
                render_.SetPosition(nextPos);
                render_.Update(); 

                // 2. 物理を作り直す
                RefreshPhysics();
            }


            // 距離判定とUIの描画。
            if (pPlayer_ && pDrawTiming_)
            {
                Vector3 diff = pPlayer_->GetPlayerPos() - this->pos_;
                float distance = diff.Length();
                float triggerDistance = 20.0f;

                if (distance < triggerDistance && !is2DMode_)
                {
                    Vector3 uiPos = this->pos_;
                    uiPos.y += 50.0f; // 少し上に表示
                    pDrawTiming_->ShowAt(pPlayer_->GetPlayerPos());

                }

                else
                {
                    pDrawTiming_->Hide();
                }
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

            pPhysics_->CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());
           
        }


        void Wall::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    } 
} // namespace app

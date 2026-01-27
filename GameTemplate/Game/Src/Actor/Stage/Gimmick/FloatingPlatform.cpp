#include "stdafx.h"

#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"

namespace
{
    const auto SPAWN_DEPTH = -300.0f;                // 地中の深さ
    const Vector3 SCALE = Vector3(0.1f, 0.1f, 0.1f); // モデルのスケール

    // Start時の待機場所
    const Vector3 SAFE_INIT_POS = Vector3(0.0f, -1000.0f, 0.0f);
} // namespace

namespace app
{
    namespace gimmick
    {
        FloatingPlatform::~FloatingPlatform()
        {
            // メモリリーク防止のため削除
            if (pPhysics_)
            {
                delete pPhysics_;
                pPhysics_ = nullptr;
            }
        }


        bool FloatingPlatform::Start()
        {
            // 1. モデル読み込み
            const std::string path = InitGimmick("StageEX/pole");
            render_.Init(path.c_str());

            // 初期パラメータ設定
            isActive_ = false;
            currentPos_ = SAFE_INIT_POS;
            targetPos_ = SAFE_INIT_POS;

            // モデルの初期配置
            render_.SetScale(SCALE);
            render_.SetPosition(currentPos_);
            render_.Update();

            // Start時はまだ物理判定を作らない（Activateされたら作る）
            // または、安全な場所に作っておく
            RefreshPhysics();

            return true;
        }


        void FloatingPlatform::Update()
        {
            if (!isActive_)
                return;

            bool isMoving = false;

            // 移動処理
            if (currentPos_.y < targetPos_.y)
            {
                currentPos_.y += 2.0f; // 上昇速度
                if (currentPos_.y > targetPos_.y)
                    currentPos_.y = targetPos_.y;

                isMoving = true;
            }

            // モデル座標更新
            render_.SetScale(currentScale_);
            render_.SetPosition(currentPos_);
            render_.Update();

            if (isMoving || pPhysics_ == nullptr)
                RefreshPhysics();
        }


        void FloatingPlatform::Render(RenderContext& rc)
        {
            if (!isActive_)
                return;
            render_.Draw(rc);
        }


        void FloatingPlatform::RefreshPhysics()
        {
            // 古い物理があれば削除
            if (pPhysics_ != nullptr)
            {
                delete pPhysics_;
                pPhysics_ = nullptr;
            }

            // 新しい物理を作成
            // PhysicsStaticObject は通常、モデルのワールド行列を使ってメッシュコライダーを作ります
            pPhysics_ = new PhysicsStaticObject();
            pPhysics_->CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());
        }


        void FloatingPlatform::Activate(const Vector3& pos, const Vector3& scale)
        {
            isActive_ = true;
            targetPos_ = pos;
            currentScale_ = scale;

            // 出現位置（地中）
            currentPos_ = pos;
            currentPos_.y += SPAWN_DEPTH;

            // 描画位置更新
            render_.SetPosition(currentPos_);
            render_.Update();

            // 出現したので物理判定を作成
            RefreshPhysics();
        }


        void FloatingPlatform::Deactivate()
        {
            isActive_ = false;

            // 物理判定を削除して無効化 (プレイヤーが落ちるようにする)
            if (pPhysics_ != nullptr)
            {
                delete pPhysics_;
                pPhysics_ = nullptr;
            }
        }

    } // namespace gimmick
} // namespace app

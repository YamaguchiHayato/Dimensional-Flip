#include "stdafx.h"

#include "TargetWall.h"

namespace app
{
    namespace gimmick
    {
        bool TargetWall::Start()
        {
            render_.Init("Assets/stage/Stage1/Wall.tkm");

            // 1. まず「見た目」の情報をすべてセットします
            render_.SetScale(Vector3(0.125f, 0.125f, 0.125f));

            // 回転をセット
            Quaternion rot;
            rot.SetRotationDegY(90.0f); // 90度回転
            render_.SetRotation(rot);

            // 座標をセット
            render_.SetPosition(gimmickPos_);

            // 2. ★超重要★ ここで Update を呼んで、設定を行列に反映させます
            // これをしないと、この後の CreateFromModel が古い情報（回転なし・原点）で作られてしまいます
            render_.Update();

            // 3. 確定した見た目（行列）を使って、当たり判定を作ります
            // これで見た目とピッタリ一致するはずです
            gimmickPhysics_.CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());

            isActive_ = true;
            return true;
        }

        void TargetWall::Update()
        {
            // 消えていたら更新処理もしない
            if (!isActive_)
                return;

            // Startで座標・回転は確定しているので、ここでの再設定は本来不要ですが、
            // 念のため描画更新だけ行います
            render_.Update();
        }

        void TargetWall::Render(RenderContext& rc)
        {
            // 消えているなら描画しない
            if (!isActive_)
                return;

            render_.Draw(rc);
        }

        void TargetWall::Vanish()
        {
            if (!isActive_)
                return;

            isActive_ = false;

            // 物理判定を消す
            gimmickPhysics_.Release();

            printf("TargetWall: Vanished!\n");
        }
    } // namespace gimmick
} // namespace app

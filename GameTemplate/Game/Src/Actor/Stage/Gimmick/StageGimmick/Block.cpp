#include "stdafx.h"

#include "Block.h"

namespace app
{
    namespace gimmick
    {
        bool Block::Start()
        {
            // 1. モデル読み込み
            render_.Init("Assets/stage/stageBlock.tkm");

            // 2. 見た目の設定 (スイッチの下にちょうどいい大きさに)
            render_.SetScale(Vector3(0.015f, 0.015f, 0.015f));
            render_.SetPosition(gimmickPos_);

            // ★重要：物理判定を作る前にUpdateして確定させる
            render_.Update();

            // 3. 物理判定の作成（これでプレイヤーが上に乗れるようになります）
            // モデルの形に合わせてカチカチの判定を作ります
            gimmickPhysics_.CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());

            return true;
        }

        void Block::Update()
        {
            // 動かない床なので、描画更新だけでOK
            render_.Update();
        }

        void Block::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    } // namespace gimmick
} // namespace app

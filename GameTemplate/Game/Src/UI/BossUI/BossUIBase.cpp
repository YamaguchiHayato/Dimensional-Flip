#include "stdafx.h"
#include "BossUIBase.h"


namespace app
{
    namespace nsUI
    {
        void BossUIBase::UpdateMatrix()
        {
            // 行列を作成。
            // 移動行列を作成。
            scale_.MakeScaling(transform_.localScale_);              // スケール行列作成
            translation_.MakeTranslation(transform_.localPosition_); // 移動行列作成


            // ローカル行列を合成。
            // 移動行列を作成。
            // 大きさ × 移動行列の順で乗算。
            localMatrix_.Multiply(scale_, translation_);

            // 親が存在する場合、親の行列として扱い、一括での移動を行う。
            if (pParent_)
                transform_.warldMatrix_.Multiply(localMatrix_, pParent_->transform_.warldMatrix_);

            // 存在しない場合。
            else
                transform_.warldMatrix_ = localMatrix_;
        }


    }
}

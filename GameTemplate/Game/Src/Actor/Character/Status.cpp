#include "stdafx.h"
#include "Src/Actor/Character/Status.h"

namespace app
{
    namespace status
    {
        void Status::Initial(uint8_t maxHP, const float speed, const float power)
        {
            maxHP_ = maxHP;
            currentHP_ = maxHP_;
            speed_ = speed;
            power_ = power;
        }
    }
}

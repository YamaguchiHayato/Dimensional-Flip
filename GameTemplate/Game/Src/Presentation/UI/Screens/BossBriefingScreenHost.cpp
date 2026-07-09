#include "stdafx.h"

#include "BossBriefingScreenHost.h"
#include "Src/Presentation/Data/BossBriefingData.h"

namespace nsApp
{
    namespace nsUI
    {
        bool BossBriefingScreenHost::Start()
        {
            /* UIScreenHost の Start() を呼ぶ。 */
            if (!UIScreenHost::Start())
                return false;

            /* @TODO: StageSetup から Data を渡して Bind する。今は Screen だけ生成。 */
            if (auto* pScreen = GetBossBriefingScreen())
                pScreen->SetVisible(false);

            return true;
        }


        BossBriefingScreen* BossBriefingScreenHost::GetBossBriefingScreen()
        {
            /* UIScreenHost から UIScreen を取得して、BossBriefingScreen にキャストする。 */
            if (screen_ == nullptr)
                return nullptr;

            /* dynamic_cast ではなく static_cast を使う。UIScreenHost で生成する
               UIScreen は BossBriefingScreen であることが保証されているため。 */
            return static_cast<BossBriefingScreen*>(screen_.get());
        }


        std::unique_ptr<UIScreen> BossBriefingScreenHost::CreateScreen()
        {
            /* UIScreenHost から UIScreen を生成する。BossBriefingScreen を生成する。 */
            return std::make_unique<BossBriefingScreen>();
        }
    } // namespace nsUI
} // namespace nsApp

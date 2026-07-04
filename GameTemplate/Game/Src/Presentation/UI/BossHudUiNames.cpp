#include "stdafx.h"

#include <unordered_map>

#include "BossHudUiNames.h"

namespace nsApp
{
    namespace nsUI
    {
        namespace
        {
            /*
             * BossHPbarUI と同じキー → 同じパス
             * パスが違う場合は Assets 内の .DDS を探して差し替える
             */
            const std::unordered_map<std::string, const char*> kBossHudUiTable = {
                {"bossIcon", "Assets/UI/Boss/bossIcon.DDS"},
                {"hpBar_flame", "Assets/UI/Boss/hpBar_flame.DDS"},
                {"hpBar_current", "Assets/UI/Boss/hpBar_current.DDS"},
                {"hpBar_damage", "Assets/UI/Boss/hpBar_damage.DDS"},
            };
        } // namespace

        std::string FetchUIName(const char* uiName)
        {
            /* 登録以外のUIが無いのなら処理をしない。*/
            if (uiName == nullptr)
                return std::string();

            /* 探索。*/
            const auto it = kBossHudUiTable.find(uiName);
            if (it != kBossHudUiTable.end())
                return std::string(it->second);

            /* 未知のキーは空を返す（Init 失敗で気づける） */
            return std::string();
        }
    } // namespace nsUI
} // namespace nsApp

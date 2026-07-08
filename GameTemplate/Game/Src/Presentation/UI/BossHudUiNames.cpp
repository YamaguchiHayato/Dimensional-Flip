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
             * 旧 BossUIBase::FetchUIName と同じ Assets/UI/BossUI/*.DDS。
             * キー名は BossAttackIndicatorHudComponent / BossStatusHudComponent と一致させる。
             */
            const std::unordered_map<std::string, const char*> kBossHudUiTable = {
                /* HP */
                {"bossIcon", "Assets/UI/BossUI/bossIcon.DDS"},
                {"hpBar_flame", "Assets/UI/BossUI/hpBar_flame.DDS"},
                {"hpBar_current", "Assets/UI/BossUI/hpBar_current.DDS"},
                {"hpBar_damage", "Assets/UI/BossUI/hpBar_damage.DDS"},

                /* 攻撃アイコン */
                {"fireBall", "Assets/UI/BossUI/fireBall.DDS"},
                {"jumpIcon", "Assets/UI/BossUI/jumpIcon.DDS"},
                {"meteorIcon", "Assets/UI/BossUI/meteorIcon.DDS"},
                {"roarIcon", "Assets/UI/BossUI/roarIcon.DDS"},
                {"spearIcon", "Assets/UI/BossUI/spearIcon.DDS"},
                {"tumblerIcon", "Assets/UI/BossUI/tumblerIcon.DDS"},
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

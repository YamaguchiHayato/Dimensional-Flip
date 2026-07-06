#pragma once

namespace nsApp
{
    namespace nsPresentation
    {
        /**
         * @class IGameplayHudData
         * @brief インゲーム HUD（Timer / Score / HP）のデータ IF。
         */
        class IGameplayHudData
        {
        public:
            /** @brief デストラクタ。 */
            virtual ~IGameplayHudData() = default;

            /** @brief 残り時間（秒）。 */
            virtual int GetTimerSeconds() const = 0;

            /** @brief スコア。 */
            virtual int GetScore() const = 0;

            /** @brief プレイヤー HP 比率 (0.0〜1.0)。 */
            virtual float GetPlayerHpRatio() const = 0;
        };
    } // namespace nsPresentation
} // namespace nsApp

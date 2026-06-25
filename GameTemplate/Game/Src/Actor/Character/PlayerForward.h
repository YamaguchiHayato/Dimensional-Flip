#pragma once

namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                class Player; 
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor
} // namespace nsApp


/**
 * @brief プロジェクト全体で使う Player 型エイリアス。
 * @note 実体は nsApp::nsActor::nsCharacter::nsPlayer::Player。
 */
using Player = nsApp::nsActor::nsCharacter::nsPlayer::Player;

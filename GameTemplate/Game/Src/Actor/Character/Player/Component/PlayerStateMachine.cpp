#include "stdafx.h"

#include "PlayerStateMachine.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"


namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                PlayerStateMachine::PlayerStateMachine
                ( nsFunction::StateMachineCommand<EnPlayerState, enState_Num>& command, Player& owner) : StateMachine(command), owner_(owner)
                {
                }


                bool PlayerStateMachine::CanUpdate() const
                {
                    /* 特に何もない場合はリターン。*/
                    Fade* fade = SceneManager::GetInstance()->GetFade();
                    if (fade != nullptr && fade->GetFadeState() == FadeState::Fade_Out)
                        return false;

                    /* チュートリアル中は更新をしない。*/
                    if (owner_.IsPaused() && !IsInState(enState_TutorialPause))
                        return false;

                    return true;
                }
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor
} // namespace nsApp

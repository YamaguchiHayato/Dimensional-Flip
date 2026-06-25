#pragma once
#include "Src/StateMachine/StateMachine.h"
#include "Src/Actor/Character/PlayerForward.h"

namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                /**
                 * @brief Player 用ステートID列挙。
                 */
                enum EnPlayerState : uint8_t
                {
                    enState_Idle,          ///< 待機。
                    enState_Run,           ///< 走行。
                    enState_Jump,          ///< ジャンプ。
                    enState_Fall,          ///< 落下。
                    enState_TutorialPause, ///< チュートリアル用一時停止ステート。
                    enState_Num,           ///< ステート数。
                };

                /**
                 * @brief Player 専用ステートマシン。
                 *
                 * フェードアウト中・ポーズ中の更新制御を担当する。
                 * 遷移ロジック本体は StateMachine テンプレートに委譲する。
                 */
                class PlayerStateMachine : public nsApp::nsFunction::StateMachine<EnPlayerState, enState_Num>
                {
                public:
                    /**
                     * @brief コンストラクタ。
                     * @param[in] command ステートレジストリ
                     * @param[in] owner   所有 Player
                     */
                    PlayerStateMachine(nsApp::nsFunction::StateMachineCommand<EnPlayerState, enState_Num>& command, Player& owner);


                protected:
                    /**
                     * @brief フェード・ポーズ時は State 更新を止める。
                     * @return 更新可能なら true
                     */
                    bool CanUpdate() const override;


                private:
                    Player& owner_; //! 所有 Player への参照。
                };
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor
} // namespace nsApp

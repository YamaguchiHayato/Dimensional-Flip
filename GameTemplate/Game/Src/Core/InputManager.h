#pragma once

#include "Src/Collision/NonCopyable.h"
#include "Src/Core/CameraManager.h"

/**
 * @file   InputManager.h
 * @brief  2D/3D 次元反転入力を処理するシングルトン。
 */

namespace nsApp
{
    namespace nsCore
    {
        /**
         * @class InputManager
         * @brief パッド入力に応じて Game::ChangeDimension を呼び出す。
         * @note  ボス戦などで一時的に無効化できる。
         */
        class InputManager : public NonCopyable
        {
        public:
            /**
             * @brief シングルトンインスタンスを取得する。
             * @return InputManager のインスタンス。未生成なら new する。
             */
            inline static InputManager* GetInstance()
            {
                if (instance_ == nullptr)
                    instance_ = new InputManager();
                return instance_;
            }

            /**
             * @brief シングルトンインスタンスを破棄する。
             */
            inline static void DeleteInstance()
            {
                if (instance_)
                {
                    delete instance_;
                    instance_ = nullptr;
                }
            }

            /**
             * @brief 次元反転入力の有効/無効を設定する。
             * @param flag true で有効、false で無効。
             */
            inline void SetDimensionFlipFlag(bool flag) { isDimensionFlip_ = flag; }

            /**
             * @brief 現在のカメラモードに応じて 2D⇔3D を切り替える。
             * @param pCamera 操作対象の CameraManager。
             * @note  "game" 名の Game GO が存在しない場合は何もしない。
             */
            void FlipDimension(CameraManager* pCamera);

        private:
            /* コンストラクタとデストラクタ。*/
            InputManager() = default;

        private:
            static InputManager* instance_; //!< シングルトンインスタンス。
            bool isDimensionFlip_ = true;   //!< 次元反転入力が有効かどうか。
        };
    } // namespace nsCore
} // namespace nsApp

using InputManager = nsApp::nsCore::InputManager;

namespace app
{
    namespace core
    {
        using InputManager = nsApp::nsCore::InputManager;
    }
} // namespace app

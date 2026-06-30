#pragma once

/**
 * @file   NonCopyable.h
 * @brief  コピー・代入を禁止するミックスイン基底クラス。
 */

namespace nsApp
{
    namespace nsCollision
    {
        /**
         * @class NonCopyable
         * @brief コピーコンストラクタと代入演算子を delete する基底。
         * @note  k2Engine 内蔵の Noncopyable とは別の自作クラス。
         */
        class NonCopyable
        {
        protected:
            /* コンストラクタとデストラクタ。*/
            NonCopyable() = default;
            virtual ~NonCopyable() = default;

            NonCopyable(const NonCopyable&) = delete;
            NonCopyable& operator=(const NonCopyable&) = delete;
        };
    } // namespace nsCollision
} // namespace nsApp

using NonCopyable = nsApp::nsCollision::NonCopyable;

namespace app
{
    namespace collision
    {
        using NonCopyable = nsApp::nsCollision::NonCopyable;
    }
} // namespace app

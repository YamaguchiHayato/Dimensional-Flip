#pragma once
// コピー禁止クラス

namespace app
{
    namespace collision
    {
        class NonCopyable
        {
        protected:
            NonCopyable() = default;
            virtual ~NonCopyable() = default;

            // コピーと代入演算子を禁止する。
            NonCopyable(const NonCopyable&) = delete;
            NonCopyable& operator=(const NonCopyable&) = delete;
        };

    }
}


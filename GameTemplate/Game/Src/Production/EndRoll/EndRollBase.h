#pragma once


namespace app
{
    namespace production
    {
        class EndRollBase : public IGameObject
        {
        public:
            EndRollBase() = default;
            virtual ~EndRollBase() = default;


        public:
            // 初期化処理。
            virtual bool Start() = 0;
                
            // 更新処理。
            virtual void Update() = 0;

            // 描画処理。
            virtual void Render(RenderContext& rc) = 0;

        // ゲッター。
        public:
            // 終了判定。
            inline bool IsEnd() const
            {
                return isEnd_;
            }


        protected:
            // 終了フラグ。
            bool isEnd_ = false;

            
        };

    }
}

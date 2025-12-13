#pragma once
#include "stdint.h"

namespace nsK2Engine
{

    // リニアワイプ描画モード
    enum LinearWipeDrawingMode
    {
        LinearWipeDrawingMode_Normal,       // 通常ワイプ
        LinearWipeDrawingMode_Direction,    // 方向ワイプ
        LinearWipeDrawingMode_Round,        // 円形ワイプ
        LinearWipeDrawingMode_Vertical,     // 縦じまワイプ
        LinearWipeDrawingMode_Horizontal,   // 横じまワイプ
        LinearWipeDrawingMode_CheckerBoard, // チェッカーボードワイプ
        LinearWipeDrawingMode_None          // 描画しない
    };

    // 画像加工描画モード
    enum ScreenDrawingMode
    {
        ScreenDrawingMode_Monochrome, // モノクロ
        ScreenDrawingMode_Sepia,      // セピア
        ScreenDrawingMode_Nega,       // ネガポジ反転
        ScreenDrawingMode_Noise,      // ノイズ
        ScreenDrawingMode_None,       // 描画しない
    };

    /*
     * @enum フェードの状態を管理。
     *
     * @dote 2025/10/21 eunm追加日。
     */
    enum FadeState
    {
        Fade_In, // フェードイン。
        Loading,   // ローディング中。
        Fade_Out,   // フェードアウト。
        StateNum   // 状態数。
    };

    /// <summary>
    /// スプライトレンダラー。
    /// </summary>
    class SpriteRender : public IRenderer
    {
    public:
        /// <summary>
        /// 初期化。
        /// </summary>
        /// <param name="filePath">ファイルパス。</param>
        /// <param name="w">画像の横幅。</param>
        /// <param name="h">画像の縦幅。</param>
        /// <param name="alphaBlendMode">デフォルトは半透明合成。</param>
        void Init(const char* filePath, const float w, const float h,
                  AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
        /// <summary>
        /// 座標を設定。zは0.0fで。
        /// </summary>
        /// <param name="pos">座標。</param>
        void SetPosition(const Vector3& pos) { m_position = pos; }
        /// <summary>
        /// 座標を取得。
        /// </summary>
        /// <returns>座標。</returns>
        const Vector3& GetPosition() const { return m_position; }
        
		/// <summary>
		/// 2D座標を設定。
        /// </summary>
        inline const void SetPos(const Vector2& pos)
        {
			pos_ = pos;
        }

		/// <summary>
		/// 2D座標を取得。
        /// </summary>
        inline const Vector2& GetPos() const
        {
			return pos_;
        }
        
        // 透明度
        inline void SetAlpha(const float alpha)
        {
            m_alpha = alpha;
        }


        /// <summary>
        /// 大きさを設定。zは1.0fで。
        /// </summary>
        /// <param name="scale">大きさ。</param>
        void SetScale(const Vector3& scale) { m_scale = scale; }
        /// <summary>
        /// 大きさを取得。
        /// </summary>
        /// <returns>大きさ。</returns>
        const Vector3& GetScale() const { return m_scale; }
        /// <summary>
        /// 回転を設定。
        /// </summary>
        /// <param name="rot">回転。</param>
        void SetRotation(const Quaternion& rot) { m_rotation = rot; }
        /// <summary>
        /// 回転を取得。
        /// </summary>
        /// <returns>回転。</returns>
        const Quaternion& GetRotation() const { return m_rotation; }
        /// <summary>
        /// ピボットを設定。
        /// </summary>
        /// <param name="pivot">ピボット。</param>
        void SetPivot(const Vector2& pivot) { m_pivot = pivot; }
        /// <summary>
        /// ピボットを取得。
        /// </summary>
        /// <returns>ピボット。</returns>
        const Vector2& GetPivot() const { return m_pivot; }
        /// <summary>
        /// 乗算カラーを設定。
        /// </summary>
        /// <param name="mulColor">乗算カラー。</param>
        void SetMulColor(const Vector4& mulColor) { m_sprite.SetMulColor(mulColor); }

        /// <summary>
        /// 乗算カラーを取得。
        /// </summary>
        /// <returns></returns>
        const Vector4& GetMulColor() const { return m_sprite.GetMulColor(); }
        /// <summary>
        /// 更新処理。
        /// </summary>
        void Update() 
        {
            if (m_spriteRenderConstantBuffer.linearWipeDrawingMode!= LinearWipeDrawingMode_None)
            {
                LinearWipeUpdate();
            }
            m_sprite.Update(m_position, m_rotation, m_scale, m_pivot); }
        /// <summary>
        /// 描画処理。
        /// </summary>
        /// <param name="rc">レンダーコンテキスト。</param>
        void Draw(RenderContext& rc);

        /// <summary>
        /// 2D描画パスから呼ばれる処理。
        /// </summary>
        /// <param name="rc"></param>
        void OnRender2D(RenderContext& rc) override { m_sprite.Draw(rc); }

        struct LinearWipe
        {
            Vector2 direction; // 方向
            float size = 0.0f; // ワイプサイズ
        };

        // スプライトレンダー用の定数バッファ
        struct SpriteRenderConstantBuffer
        {
            LinearWipe linearWipe; // リニアワイプ
            int linearWipeDrawingMode = LinearWipeDrawingMode_None;
            float drawingRate = 0.0f;                       // 画像加工用イージング割合
            int screenDrawingMode = ScreenDrawingMode_None; // 画像加工の描画モード
        };

        /// <summary>
        ///	リニアワイプの描画モードを設定
        /// </summary>
        /// <param
        /// name="linearWipeMode">描画モード　LinearWipeMode_Directionを設定する場合はSetLinearWipeDirection()で方向を設定して下さい</param>
        void SetLinearWipeDrawingMode(LinearWipeDrawingMode linearWipeMode)
        {
            m_spriteRenderConstantBuffer.linearWipeDrawingMode = linearWipeMode;
            m_spriteRenderConstantBuffer.linearWipe.size = 0.0f;
        }

        /// <summary>
        /// ワイプサイズを設定
        /// </summary>
        /// <param name="wipeSize">ワイプサイズ</param>
        void SetWipeSize(float wipeSize) { m_spriteRenderConstantBuffer.linearWipe.size = wipeSize; }

        /// <summary>
        /// ワイプサイズの取得
        /// </summary>
        /// <returns>ワイプサイズ</returns>
        float GetWipeSize() const { return m_spriteRenderConstantBuffer.linearWipe.size; }

        /// <summary>
        /// リニアワイプの速度を設定
        /// </summary>
        /// <param name="wipeScroolSpeed">速度</param>
        void SetWipeScrollSpeed(float wipeScroolSpeed) { 
            wipeScrollSpeed_ = wipeScroolSpeed; 
        }

        /// <summary>
        /// リニアワイプ(方向)
        /// </summary>
        /// <param name="x">x軸方向(1.0f~0.0f)</param>
        /// <param name="y">y軸方向(1.0f~0.0f)</param>
        void SetLinearWipeDirection(float x, float y)
        {
            m_spriteRenderConstantBuffer.linearWipe.direction.Set(x, y);
            m_spriteRenderConstantBuffer.linearWipe.direction.Normalize();
        }
        /// <summary>
        /// 画像加工の描画モードを設定
        /// </summary>
        /// <param name="screenMode">描画モード</param>
        void SetScreenDrawingMode(ScreenDrawingMode screenDrawingMode)
        {
            m_spriteRenderConstantBuffer.screenDrawingMode = screenDrawingMode;
            m_spriteRenderConstantBuffer.drawingRate = 0.0f;
        }

        /// <summary>
        /// 画像加工をイージングする速度を設定
        /// </summary>
        /// <param name="screenDrawingEasingSpeed">速度</param>
        void SetScreenDrawingEasingSpeed(float screenDrawingSpeed) { screenDrawingEasingSpeed_ = screenDrawingSpeed; }

        /// <summary>
        /// フェードステートの切り替え
        /// </summary>
        /// <param name="enFadeState">フェードステート</param>
        void SetFadeTransition(FadeState enFadeState) { fadeState_ = enFadeState; }

        /// <summary>
        /// スプライトレンダー用の定数バッファを取得
        /// </summary>
        /// <returns></returns>
        SpriteRenderConstantBuffer& GetSpriteRenderConstantBuffer() { return m_spriteRenderConstantBuffer; }

    private:
		/// <summary>
		/// リニアワイプの更新処理
		/// </summary>
		void LinearWipeUpdate()
		{
			////フェードステートがフェードインならワイプサイズを大きくする
			if (fadeState_ == FadeState::Fade_In)
			{
				m_spriteRenderConstantBuffer.linearWipe.size += wipeScrollSpeed_ * g_gameTime->GetFrameDeltaTime();
			}

			//フェードステートがフェードアウトならワイプサイズを小さくする
			else if (fadeState_ == FadeState::Fade_Out)
			{
			    m_spriteRenderConstantBuffer.linearWipe.size -= wipeScrollSpeed_ * g_gameTime->GetFrameDeltaTime();
			}
		}

		/// <summary>
		/// 画像加工の更新処理
		/// </summary>
		void ScreenDrawingUpdate()
		{
			m_spriteRenderConstantBuffer.drawingRate += screenDrawingEasingSpeed_ * g_gameTime->GetFrameDeltaTime();
			//割合が1.0f以上になったら割合を固定する
			if (m_spriteRenderConstantBuffer.drawingRate > 1.0f)
			{
				m_spriteRenderConstantBuffer.drawingRate = 1.0f;
			}
		}
    private:
        Sprite m_sprite;
        SpriteRenderConstantBuffer m_spriteRenderConstantBuffer; // リニアワイプ
        Vector3 m_position = Vector3::Zero;                      // 座標。
        Quaternion m_rotation = Quaternion::Identity;            // 回転。
        Vector3 m_scale = Vector3::One;                          // 大きさ。
        Vector2 m_pivot = Sprite::DEFAULT_PIVOT;                 // ピボット。

		Vector2 pos_ = Vector2::Zero;

    private:
        int fadeState_ = FadeState::Fade_In;              // フェードステート
        float m_alpha = 1.0f;                     // 透明度。
        float wipeScrollSpeed_ = 1.0f;           // ワイプ速度
        float screenDrawingEasingSpeed_ = 0.01f; // 画像加工用のイージング速度
    };
} 

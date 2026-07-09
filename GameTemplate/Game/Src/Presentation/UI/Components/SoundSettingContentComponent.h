#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"

/**
 * @file   SoundSettingContentComponent.h
 * @brief  Setting パネル上の見出し・音量行・Controller を描画する。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class SoundSettingContentComponent
         * @brief base 上に Title / 音量行(ノブ+Toggle) / Controller を並べる。
         * @note  slider は 90x92 のノブ。音量で左右移動する（横に伸ばさない）。
         * @note  Master/BGM/SE のトグル溝は残す。消すのは Controller のフレームだけ。
         */
        class SoundSettingContentComponent : public IUIComponent
        {
        public:
            /**
             * @brief 各行の見た目をデータから更新する。
             * @param[in] masterVolume Master 音量 0〜1。
             * @param[in] bgmVolume BGM 音量 0〜1。
             * @param[in] seVolume SE 音量 0〜1。
             * @param[in] masterMuted Master ミュート。
             * @param[in] bgmMuted BGM ミュート。
             * @param[in] seMuted SE ミュート。
             * @param[in] controllerEnabled Controller ON/OFF。
             * @param[in] selectedIndex 0:Master 1:BGM 2:SE 3:Controller。
             */
            void ApplyValues(float masterVolume, float bgmVolume, float seVolume, bool masterMuted, bool bgmMuted,  bool seMuted, bool controllerEnabled, int selectedIndex);

            /**
             * @brief スプライト / フォントを初期化する。
             */
            void OnBuild() override;

            /**
             * @brief 毎フレーム更新する。
             * @param[in] deltaTime 経過秒数。
             */
            void OnUpdate(float deltaTime) override;

            /**
             * @brief 描画する。
             * @param[in] rc 描画コンテキスト。
             * @param[in] ownerWorld オーナー行列。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;


        private:
            /**
             * @brief ノブ位置・回転を音量に合わせて更新する。
             * @param[in,out] slider 対象ノブ。
             * @param[in,out] angleDeg 累積回転角（度）。
             * @param[in] volume 現在音量。
             * @param[in] prevVolume 前回音量（回転量の計算用）。
             * @param[in] muted ミュート。
             * @param[in] selected 選択中か。
             * @param[in] rowY 行の基準 Y。
             */
            void RefreshSlider(SpriteRender& slider, float& angleDeg, float volume, float prevVolume, bool muted, bool selected, float rowY);

            /**
             * @brief 音量行トグル（溝）の見た目を更新する。
             * @param[in,out] toggle Master/BGM/SE 用トグル。
             * @param[in] muted ミュート。
             * @param[in] selected 選択中か。
             * @param[in] pos 描画座標。
             */
            void RefreshToggle(SpriteRender& toggle, bool muted, bool selected, const Vector3& pos);


        private:
            SpriteRender titleSprite_;      //!< SoundSetting.DDS
            SpriteRender masterSlider_;     //!< ノブ（Master）
            SpriteRender bgmSlider_;        //!< ノブ（BGM）
            SpriteRender seSlider_;         //!< ノブ（SE）
            SpriteRender masterToggle_;     //!< 溝トグル（Master）※残す
            SpriteRender bgmToggle_;        //!< 溝トグル（BGM）※残す
            SpriteRender seToggle_;         //!< 溝トグル（SE）※残す
            SpriteRender controllerSprite_; //!< Controller アイコン

            FontRender masterLabel_; //!< "Master"
            FontRender bgmLabel_;    //!< "BGM"
            FontRender seLabel_;     //!< "SE"
            FontRender offLabel_;    //!< "OFF"
            FontRender onLabel_;     //!< "ON"

            bool isBuilt_ = false; //!< Build 済みか。

            float masterVolume_ = 1.0f;     //!< Master 音量。
            float bgmVolume_ = 1.0f;        //!< BGM 音量。
            float seVolume_ = 1.0f;         //!< SE 音量。
            bool masterMuted_ = false;      //!< Master ミュート。
            bool bgmMuted_ = false;         //!< BGM ミュート。
            bool seMuted_ = false;          //!< SE ミュート。
            bool controllerEnabled_ = true; //!< Controller ON/OFF。
            int selectedIndex_ = 0;         //!< 選択行。

            float masterAngleDeg_ = 0.0f;   //!< Master ノブ回転角。
            float bgmAngleDeg_ = 0.0f;      //!< BGM ノブ回転角。
            float seAngleDeg_ = 0.0f;       //!< SE ノブ回転角。
            float prevMasterVolume_ = 1.0f; //!< 回転計算用の前回音量。
            float prevBgmVolume_ = 1.0f;
            float prevSeVolume_ = 1.0f;

            /* ---- 共通 X（横を揃える） ---- */
            static constexpr float kLabelX = -340.0f;     //!< ラベル X。
            static constexpr float kTrackLeftX = -40.0f;  //!< ノブ移動の左端。
            static constexpr float kTrackRightX = 280.0f; //!< ノブ移動の右端。
            static constexpr float kToggleX = 120.0f;     //!< トグル中心 X。

            /* ---- 行の基準 Y ---- */
            static constexpr float kMasterRowY = 100.0f;
            static constexpr float kBgmRowY = 10.0f;
            static constexpr float kSeRowY = -80.0f;
            static constexpr float kControllerRowY = -190.0f;

            /* ノブと溝は同じ高さ（トグルの真ん中を走る）。ズレたらここを微調整。 */
            static constexpr float kSliderOffsetY = 0.0f;
            static constexpr float kToggleOffsetY = 0.0f;

            /* 音量 1.0 動かすとノブが 1 回転。 */
            static constexpr float kRotatePerVolume = 360.0f;

            Vector3 titlePos_{0.0f, 230.0f, 0.0f}; //!< 見出し位置。

            float titleW_ = 420.0f;
            float titleH_ = 120.0f;
            float sliderW_ = 90.0f; //!< ノブ実寸。横に伸ばさない。
            float sliderH_ = 92.0f;
            float toggleW_ = 320.0f; //!< Master/BGM/SE 溝の幅。
            float toggleH_ = 56.0f;
            float controllerW_ = 110.0f;
            float controllerH_ = 70.0f;

            const char* pathTitle_ = "Assets/UI/setting/SoundSetting.DDS";
            const char* pathSlider_ = "Assets/UI/setting/slider.DDS";
            const char* pathToggle_ = "Assets/UI/setting/Toggle.DDS";
            const char* pathController_ = "Assets/UI/setting/Controller.DDS";
        };
    } // namespace nsUI
} // namespace nsApp

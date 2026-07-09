#include "stdafx.h"

#include "SoundSettingContentComponent.h"

namespace nsApp
{
    namespace nsUI
    {
        void SoundSettingContentComponent::OnBuild()
        {
            /* 二重初期化防止。 */
            if (isBuilt_)
                return;

            /* 見出し。 */
            titleSprite_.Init(pathTitle_, titleW_, titleH_);
            titleSprite_.SetPivot({0.5f, 0.5f});

            /* ノブ（90x92）。中心基準で左右移動。横に伸ばさない。 */
            masterSlider_.Init(pathSlider_, sliderW_, sliderH_);
            bgmSlider_.Init(pathSlider_, sliderW_, sliderH_);
            seSlider_.Init(pathSlider_, sliderW_, sliderH_);
            masterSlider_.SetPivot({0.5f, 0.5f});
            bgmSlider_.SetPivot({0.5f, 0.5f});
            seSlider_.SetPivot({0.5f, 0.5f});

            /* Master / BGM / SE の溝トグルは残す（3本必要）。 */
            masterToggle_.Init(pathToggle_, toggleW_, toggleH_);
            bgmToggle_.Init(pathToggle_, toggleW_, toggleH_);
            seToggle_.Init(pathToggle_, toggleW_, toggleH_);
            masterToggle_.SetPivot({0.5f, 0.5f});
            bgmToggle_.SetPivot({0.5f, 0.5f});
            seToggle_.SetPivot({0.5f, 0.5f});

            /* Controller はアイコンのみ（4つ目のフレームは作らない）。 */
            controllerSprite_.Init(pathController_, controllerW_, controllerH_);
            controllerSprite_.SetPivot({0.5f, 0.5f});

            /* ラベル文字。 */
            masterLabel_.SetText(L"Master");
            bgmLabel_.SetText(L"BGM");
            seLabel_.SetText(L"SE");
            offLabel_.SetText(L"OFF");
            onLabel_.SetText(L"ON");

            masterLabel_.SetScale(1.1f);
            bgmLabel_.SetScale(1.1f);
            seLabel_.SetScale(1.1f);
            offLabel_.SetScale(0.9f);
            onLabel_.SetScale(0.9f);

            const Vector4 white = {1, 1, 1, 1};
            masterLabel_.SetColor(white);
            bgmLabel_.SetColor(white);
            seLabel_.SetColor(white);
            offLabel_.SetColor(white);
            onLabel_.SetColor(white);

            isBuilt_ = true;

            /* 初期値で見た目を揃える。 */
            ApplyValues(1.0f, 1.0f, 1.0f, false, false, false, true, 0);
        }

        void SoundSettingContentComponent::ApplyValues(float masterVolume, float bgmVolume, float seVolume,
                                                       bool masterMuted, bool bgmMuted, bool seMuted,
                                                       bool controllerEnabled, int selectedIndex)
        {
            /* Data / 選択状態を保持する。 */
            masterVolume_ = masterVolume;
            bgmVolume_ = bgmVolume;
            seVolume_ = seVolume;
            masterMuted_ = masterMuted;
            bgmMuted_ = bgmMuted;
            seMuted_ = seMuted;
            controllerEnabled_ = controllerEnabled;
            selectedIndex_ = selectedIndex;
        }

        void SoundSettingContentComponent::RefreshSlider(SpriteRender& slider, float& angleDeg, float volume,
                                                         float prevVolume, bool muted, bool selected, float rowY)
        {
            /* 音量 0〜1 をレール X へ変換する（拡縮しない）。 */
            float v = volume;
            if (v < 0.0f)
                v = 0.0f;
            if (v > 1.0f)
                v = 1.0f;
            if (muted)
                v = 0.0f;

            float prev = prevVolume;
            if (prev < 0.0f)
                prev = 0.0f;
            if (prev > 1.0f)
                prev = 1.0f;

            /* 動かした量だけ歯車を回す。 */
            const float delta = v - prev;
            angleDeg += delta * kRotatePerVolume;

            const float x = kTrackLeftX + (kTrackRightX - kTrackLeftX) * v;
            /* トグルと同じ Y = 溝の真ん中を走らせる。 */
            const Vector3 pos(x, rowY + kSliderOffsetY, 0.0f);

            /* Z 軸回転（度→ラジアン）。 */
            const float rad = angleDeg * (3.14159265f / 180.0f);
            Quaternion rot = Quaternion::Identity;
            rot.SetRotation(Vector3::AxisZ, rad);

            slider.SetPosition(pos);
            slider.SetRotation(rot);
            /* 選択中は少し大きくして、どのスライダーか分かりやすくする。 */
            slider.SetScale(selected ? Vector3(1.15f, 1.15f, 1.0f) : Vector3::One);
            slider.SetMulColor(muted ? Vector4{0.45f, 0.45f, 0.45f, 1.0f}
                                     : (selected ? Vector4{1.0f, 0.95f, 0.35f, 1.0f} : Vector4{1, 1, 1, 1}));
            slider.Update();
        }

        void SoundSettingContentComponent::RefreshToggle(SpriteRender& toggle, bool muted, bool selected,
                                                         const Vector3& pos)
        {
            /* Master/BGM/SE の溝。選択中は少し強調する。 */
            toggle.SetPosition(pos);
            if (muted)
                toggle.SetMulColor(Vector4{0.55f, 0.55f, 0.55f, 1.0f});
            else if (selected)
                toggle.SetMulColor(Vector4{1.0f, 0.92f, 0.45f, 1.0f});
            else
                toggle.SetMulColor(Vector4{1, 1, 1, 1});
            toggle.Update();
        }

        void SoundSettingContentComponent::OnUpdate(float /*deltaTime*/)
        {
            if (!isBuilt_)
                return;

            /* 見出し。 */
            titleSprite_.SetPosition(titlePos_);
            titleSprite_.Update();

            /* ラベル位置。 */
            masterLabel_.SetPosition(Vector3(kLabelX, kMasterRowY, 0.0f));
            bgmLabel_.SetPosition(Vector3(kLabelX, kBgmRowY, 0.0f));
            seLabel_.SetPosition(Vector3(kLabelX, kSeRowY, 0.0f));

            /* 選択行の文字色を変えて、どれを触っているか分かるようにする。 */
            const Vector4 active = {1.0f, 0.9f, 0.2f, 1.0f};
            const Vector4 normal = {1.0f, 1.0f, 1.0f, 1.0f};
            masterLabel_.SetColor(selectedIndex_ == 0 ? active : normal);
            bgmLabel_.SetColor(selectedIndex_ == 1 ? active : normal);
            seLabel_.SetColor(selectedIndex_ == 2 ? active : normal);

            /* Master/BGM/SE の溝位置（3本とも描画する）。 */
            const Vector3 masterTogglePos(kToggleX, kMasterRowY + kToggleOffsetY, 0.0f);
            const Vector3 bgmTogglePos(kToggleX, kBgmRowY + kToggleOffsetY, 0.0f);
            const Vector3 seTogglePos(kToggleX, kSeRowY + kToggleOffsetY, 0.0f);

            /* ノブ更新（溝の真ん中を走る + 回転）。 */
            RefreshSlider(masterSlider_, masterAngleDeg_, masterVolume_, prevMasterVolume_, masterMuted_,
                          selectedIndex_ == 0, kMasterRowY);
            RefreshSlider(bgmSlider_, bgmAngleDeg_, bgmVolume_, prevBgmVolume_, bgmMuted_, selectedIndex_ == 1,
                          kBgmRowY);
            RefreshSlider(seSlider_, seAngleDeg_, seVolume_, prevSeVolume_, seMuted_, selectedIndex_ == 2, kSeRowY);

            /* 溝トグル更新（3本残す）。 */
            RefreshToggle(masterToggle_, masterMuted_, selectedIndex_ == 0, masterTogglePos);
            RefreshToggle(bgmToggle_, bgmMuted_, selectedIndex_ == 1, bgmTogglePos);
            RefreshToggle(seToggle_, seMuted_, selectedIndex_ == 2, seTogglePos);

            /* 次回の回転計算用に、実質音量を保存する。 */
            prevMasterVolume_ = masterMuted_ ? 0.0f : masterVolume_;
            prevBgmVolume_ = bgmMuted_ ? 0.0f : bgmVolume_;
            prevSeVolume_ = seMuted_ ? 0.0f : seVolume_;

            /* Controller 行: アイコン + OFF/ON のみ（フレームなし）。 */
            controllerSprite_.SetPosition(Vector3(kLabelX + 40.0f, kControllerRowY, 0.0f));
            controllerSprite_.Update();

            offLabel_.SetPosition(Vector3(kToggleX - 80.0f, kControllerRowY, 0.0f));
            onLabel_.SetPosition(Vector3(kToggleX + 80.0f, kControllerRowY, 0.0f));

            const Vector4 dim = {1.0f, 1.0f, 1.0f, 0.55f};
            const bool controllerSelected = (selectedIndex_ == 3);
            offLabel_.SetColor(controllerEnabled_ ? dim : (controllerSelected ? active : normal));
            onLabel_.SetColor(controllerEnabled_ ? (controllerSelected ? active : normal) : dim);
        }

        void SoundSettingContentComponent::OnDraw(RenderContext& rc, const Matrix& /*ownerWorld*/)
        {
            if (!isBuilt_)
                return;

            /* 描画直前に最新見た目へ更新する。 */
            OnUpdate(0.0f);

            titleSprite_.Draw(rc);
            masterLabel_.Draw(rc);
            bgmLabel_.Draw(rc);
            seLabel_.Draw(rc);

            /* 溝（Master/BGM/SE）→ ノブ の順。溝は3本とも描く。 */
            masterToggle_.Draw(rc);
            bgmToggle_.Draw(rc);
            seToggle_.Draw(rc);
            masterSlider_.Draw(rc);
            bgmSlider_.Draw(rc);
            seSlider_.Draw(rc);

            /* Controller はフレーム無し。アイコンと OFF/ON のみ。 */
            controllerSprite_.Draw(rc);
            offLabel_.Draw(rc);
            onLabel_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp

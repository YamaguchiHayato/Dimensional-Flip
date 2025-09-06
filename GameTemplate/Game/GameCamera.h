#pragma once

class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera() {};
	~GameCamera() {};

	/// <summary>
  	/// 初期化処理。
	/// </summary>
	bool Start();                 

	/// <summary>
   	/// Lerpでの更新処理。
	/// </summary>
	void Update();                


private:
	/// <summary>
    /// カメラの視点切替。
    /// </summary>
	void CameraSwitch();
	
	/// <summary>
	/// カメラの2D視点用。
	/// </summary>
	void SwitchTo2DMode();

	/// <summary>
	/// カメラの3D視点用。
	/// </summary>
	void SwitchTo3DMode();

	void CameraMove();

	/// <summary>
	/// // 正射影・透視投影切り替え
	/// </summary>
	/// <param name="isOrtho"></param>
	/// <param name="width">幅。</param>
	/// <param name="height">高さ。</param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <param name="fovY"></param>
	/// <param name="aspect">アスペクト比の計算</param>
	void SetProjectionOrthographic(bool isOrtho, float width, float height, float nearZ, float farZ, float fovY, float aspect);
	
private:
	Player* m_player = nullptr; //プレイヤー     

private:
	// 追従設定
	float followDistance_ = 6.0f;   // 背後の距離
	float verticalOffset_ = 2.0f;   // 高さのオフセット
	float followLerp_ = 8.0f;       // 補間速度

private:
	Vector3 m_CameraTarget;
	Vector3 m_toCameraPos = Vector3::One;
	Vector3 m_playerPos;

private:
	enum CameraMode
	{
		mode_2D, // 2Dモード。
		mode_3D, // 3Dモード。
		None,
	};

	CameraMode cameraMode = mode_2D; // カメラモード
};

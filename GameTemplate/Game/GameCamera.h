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
   	/// 更新処理。
	/// </summary>
	void Update();                

	/// <summary>
	/// カメラの移動処理。
	/// </summary>
	void CameraMove();

	void CameraSwitch();
	
	void SwitchTo2DMode();

	void SwitchTo3DMode();

	// 正射影・透視投影切り替え
	void SetProjectionOrthographic(bool isOrtho, float width = 1280.0f, float height = 720.0f, float nearZ = 1.0f, float farZ = 10000.0f, float fovY = 60.0f, float aspect = 16.0f/9.0f);
private:
	Player* m_player = nullptr; //プレイヤー     


private:
	Vector3 axisX;
	Vector3 m_toCameraPos = Vector3::One;
	Quaternion qRot;

private:
	enum CameraMode
	{
		mode_2D, // 2Dモード
		mode_3D, // 3Dモード
		None,
	};

	CameraMode cameraMode = mode_2D; // カメラモード

};

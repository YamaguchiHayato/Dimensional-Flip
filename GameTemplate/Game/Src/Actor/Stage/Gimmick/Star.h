#pragma once
class Player;
class Star : public IGameObject
{
public:
	Star() {};
	virtual ~Star() {};
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
	void Rotation();

	inline void SetStarPosition(const Vector3& starPosition)
	{
		m_starPosition = starPosition;
	}
	const Vector3& GetStarPosition()
	{
		return m_starPosition;
	}


private:
	Player* m_player = nullptr;

private:
	ModelRender m_starRender;
	Vector3 m_starPosition;
	Quaternion m_starRotation;

};


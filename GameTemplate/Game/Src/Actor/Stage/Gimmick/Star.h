#pragma once
class Player;
class GameClear;
class Star : public IGameObject
{
public:
	Star() {};
	virtual ~Star() {};
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
	void Rotation();

	const void SetStarPosition(const Vector3& starPosition)
	{
		m_starPosition = starPosition;
	}
	const Vector3& GetStarPosition()
	{
		return m_starPosition;
	}

    inline int GetStarCount() const
    {
        int count = 0;
        for (bool collected : starCollected_) {
        if (!collected) ++count;
        }
        return count;
    }

private:
	Player* m_player = nullptr;

private:
	ModelRender m_starRender;
	Vector3 m_starPosition;
	Quaternion m_starRotation;
	GameClear* m_gameClear = nullptr;


private:
     std::vector<bool> starCollected_;
};


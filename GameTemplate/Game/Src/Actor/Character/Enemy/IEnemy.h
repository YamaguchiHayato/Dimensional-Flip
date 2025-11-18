#pragma once
#include "Src/Actor/Actor.h"
class TrackingEnemy;

class IEnemy : public Actor
{
public:
	IEnemy() {};
	virtual ~IEnemy() {};
	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;
    inline const virtual std::string InitModel(const std::string& enemyName)
    {
        std::string enemyPath = "Assets/stage/" + enemyName + ".tkm";
        return enemyPath;
    };

protected:
	CharacterController charaCon_;
	Vector3 pos_ = Vector3::Zero;
    Vector3 initPos_ = Vector3::Zero;
    PhysicsStaticObject phisics_;
	ModelRender render_ ;

};


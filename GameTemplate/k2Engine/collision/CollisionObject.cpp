#include "k2EnginePreCompile.h"
#include "CollisionObject.h"


namespace nsK2Engine {
	CollisionObject::CollisionObject()
	{

	}

	CollisionObject::~CollisionObject()
	{
		// マネージャーから解除。
		app::collision::CollisionManager::GetInstance().UnRegisterObject(this);

		if (g_collisionObjectManager) {
			g_collisionObjectManager->RemoveCollisionObject(this);
		}
	}

	bool CollisionObject::Start()
	{
		// 2. グローバルマネージャーに登録。
		if( g_collisionObjectManager == nullptr ) {
			g_collisionObjectManager->AddCollisionObject(this);
		}


		// マネージャーに登録。
		app::collision::CollisionManager::GetInstance().RegisterObject(this);

		// 3. 現在の次元に即座に合わせる
		OnDimensionChanged(app::collision::DimensionMode::dim2D); // 初期状態		

		return true;
	}

	void CollisionObject::Update()
	{
		if (m_isEnableAutoDelete == false)
		{
			return;
		}


		if (m_timer > m_timeLimit + 0000.1f)
		{
			DeleteGO(this);
		}
		m_timer += g_gameTime->GetFrameDeltaTime();
	}


	CollisionObjectManager::CollisionObjectManager()
	{

	}

	CollisionObjectManager::~CollisionObjectManager()
	{
		m_collisionObjectVector.clear();
	}
}
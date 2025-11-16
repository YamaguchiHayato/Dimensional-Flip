#pragma once
#include "physics/PhysicsGhostObject.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "LinearMath/btDefaultMotionState.h"

namespace nsK2Engine {
	/// <summary>
	/// 他のコリジョンと当たり判定を行うクラス。
	/// </summary>
	class CollisionObject : public IGameObject
	{
	public:
		CollisionObject();
		~CollisionObject();
		bool Start();
		void Update();
		/// <summary>
	    /// ボックス形状のゴーストオブジェクトを作成。
	    /// </summary>
	    /// <param name="pos">座標。</param>
	    /// <param name="rot">回転。</param>
	    /// <param name="size">サイズ。</param>
		void CreateBox(Vector3 pos, Quaternion rot, Vector3 size)
		{
			m_physicsGhostObject.CreateBox(pos, rot, size);
		}
		/// <summary>
		/// カプセル形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="radius">カプセルの半径。</param>
		/// <param name="height">カプセルの高さ。</param>
		void CreateCapsule(Vector3 pos, Quaternion rot, float radius, float height)
		{
			m_physicsGhostObject.CreateCapsule(pos, rot, radius, height);
		}
		/// <summary>
		/// 球形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="radius">球形の半径。</param>
		void CreateSphere(Vector3 pos, Quaternion rot, float radius)
		{
			m_physicsGhostObject.CreateSphere(pos, rot, radius);
		}
		/// <summary>
		/// メッシュ形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="model">モデル。</param>
		/// <param name="worldMatrix">ワールド行列。</param>
		void CreateMesh(Vector3 pos, Quaternion rot, const Model& model, const Matrix& worldMatrix)
		{
			m_physicsGhostObject.CreateMesh(pos, rot, model, worldMatrix);
		}
		/// <summary>
		/// 設定した時間(秒)が経過すると自動で削除されます。0.0fを設定したら1フレーム経過で削除されます。
		/// </summary>
		/// <param name="timeLimit">削除したい時間(秒)。</param>
		void SetTimeLimit(const float timeLimit)
		{
			m_timeLimit = timeLimit;
		}
		/// <summary>
		/// 名前を設定。
		/// </summary>
		/// <param name="name">名前。</param>
		void SetName(const char* name)
		{
			m_name = name;
		}
		/// <summary>
		/// 名前を取得。
		/// </summary>
		/// <returns></returns>
		const char* GetName() const
		{
			return m_name;
		}
		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="position"></param>
		void SetPosition(const Vector3& position)
		{
			m_physicsGhostObject.SetPosition(position);
		}

		/// <summary>
		/// ヘルパー関数。
		/// <summary>
        inline btVector3 ConvertToBtVector3(const Vector3& v)
	    {
		       return btVector3(v.x, v.y, v.z);
	    }
	    inline Vector3 ConvertToVector3(const btVector3& v)
 	    {
	 	       return Vector3(v.getX(), v.getY(), v.getZ());
	    }
	    inline btQuaternion ConvertToBtQuaternion(const Quaternion& q)
	    {
		       return btQuaternion(q.x, q.y, q.z, q.w);
	    }
	    inline Quaternion ConvertToQuaternion(const btQuaternion& q)
	    {
		       return Quaternion(q.getX(), q.getY(), q.getZ(), q.getW());
	    }

		/// <summary>
		/// 球形状の「動的な剛体(Rigidbody)」を作成します。
		/// </summary>
  //      void CreateDynamicSphere(Vector3 pos, float radius, float mass)
		//{
		//	// 1. 球の「形状(Shape)」を作成
		//	// (※ 形状は使い回せるため、本当は new しない方がよいが、一旦これで動かす)
		//	btCollisionShape* collisionShape = new btSphereShape(radius);

		//	// 2. 「モーション状態(MotionState)」を作成 (位置や回転の管理)
		//	btTransform startTransform;
		//	startTransform.setIdentity();
		//	// ↓ 【バグ修正】pos_ ではなく引数の pos を使う
		//	startTransform.setOrigin(ConvertToBtVector3(pos)); 
		//	btMotionState* motionState = new btDefaultMotionState(startTransform);

		//	// 3. 慣性(Inertia)の計算 (質量が0でなければ)
		//	btVector3 localInertia(0, 0, 0);
		//	if (mass != 0.0f) {
		//		collisionShape->calculateLocalInertia(mass, localInertia);
		//	}

		//	// 4. 剛体(Rigidbody)の「構築情報」を作成
		//	// ↓ 【バグ修正】localInertia を追加
		//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);

		//	// 5. 剛体(Rigidbody)を new で作成
		//	// ↓ 【バグ修正】const を削除
		//	btRigidBody* body = new btRigidBody(rbInfo);

		//	// 6. メンバー変数に保存
		//	this->btRigidBody_ = body;

		//	// 7. 物理ワールドに剛体を追加 (※この関数はあなたのエンジンに合わせてください)
		//	PhysicsWorld::GetInstance()->AddRigidBody(body); 

		//	// 8. 剛体に紐づける (当たり判定のコールバックで使うため)
		//	body->setUserPointer(this);
		//}
		const Quaternion GetRot() const
		{
			return rot_;
		}

		const Vector3 GetPos() const
		{
			return pos_;
		}

		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rotation"></param>
		void SetRotation(const Quaternion& rotation)
		{
			m_physicsGhostObject.SetRotation(rotation);
		}
		/// <summary>
		/// 行列を設定。
		/// </summary>
		/// <param name="rotation">行列。</param>
		void SetWorldMatrix(const Matrix& matrix)
		{
			Vector3 position;
			position.x = matrix.m[3][0];
			position.y = matrix.m[3][1];
			position.z = matrix.m[3][2];
			SetPosition(position);
			Quaternion rotation;
			rotation.SetRotation(matrix);
			SetRotation(rotation);
		}
		/// <summary>
		/// 自動で削除するかどうかを設定する。
		/// </summary>
		/// <param name="isFlag">trueなら自動で削除される。falseなら自動で削除されないので、DeleteGOをする必要がある。</param>
		void SetIsEnableAutoDelete(bool isFlag)
		{
			m_isEnableAutoDelete = isFlag;
		}
		/// <summary>
		/// コリジョンオブジェクト同士の当たり判定。
		/// </summary>
		/// <param name="collisionObject">衝突判定したいコリジョンオブジェクト。</param>
		/// <returns>衝突したらtrue。</returns>
		const bool IsHit(CollisionObject* collisionObject) const
		{
			bool isCollision = false;
			PhysicsWorld::GetInstance()->ContactTest(&collisionObject->GetbtCollisionObject(), [&](const btCollisionObject& contactObject) {
				if (m_physicsGhostObject.IsSelf(contactObject) == true) {
					isCollision = true;
				}
			});
			return isCollision;
		}
		/// <summary>
		/// コリジョンオブジェクトとキャラコンの当たり判定。
		/// </summary>
		/// <param name="collisionObject">衝突判定したいキャラコン。</param>
		/// <returns>衝突したらtrue。</returns>
		const bool IsHit(CharacterController& characterController) const
		{
			bool isCollision = false;
			PhysicsWorld::GetInstance()->ContactTest(characterController, [&](const btCollisionObject& contactObject) {
				if (m_physicsGhostObject.IsSelf(contactObject) == true) {
					isCollision = true;
				}
			});
			return isCollision;
		}
		//何故か反応しないので現在は使用不可。
		/*/// <summary>
		/// コリジョンオブジェクトとフィジックススタティックオブジェクトの当たり判定。
		/// </summary>
		/// <param name="physicsStaticObject">衝突判定したいフィジックススタティックオブジェクト。</param>
		/// <returns>衝突したらtrue。</returns>
		bool IsHit(PhysicsStaticObject& physicsStaticObject)
		{
			bool isCollision = false;
			PhysicsWorld::GetInstance()->ContactTest(physicsStaticObject.GetbtCollisionObject(), [&](const btCollisionObject& contactObject) {
				if (m_physicsGhostObject.IsSelf(contactObject) == true) {
					isCollision = true;
				}
				});
			return isCollision;
		}*/
		/// <summary>
		/// ゴーストオブジェクトを取得。
		/// </summary>
		/// <returns>ゴーストオブジェクト。</returns>
		PhysicsGhostObject& GetGhostObject()
		{
			return m_physicsGhostObject;
		}
		/// <summary>
		/// btコリジョンオブジェクトを取得。
		/// </summary>
		/// <returns>btコリジョンオブジェクト。</returns>
		btCollisionObject& GetbtCollisionObject()
		{
			return m_physicsGhostObject.GetbtCollisionObject();
		}
		/// <summary>
		/// 当たり判定が有効かどうかを設定する。
		/// </summary>
		/// <param name="isEnable">有効にしたいならtrue。</param>
		void SetIsEnable(bool isEnable)
		{
			m_isEnable = isEnable;
		}
		/// <summary>
		/// 当たり判定が有効かどうかを取得する。
		/// </summary>
		/// <returns>当たり判定が有効ならtrue。</returns>
		const bool IsEnable() const
		{
			return m_isEnable;
		}
	private:
		btRigidBody* btRigidBody_ = nullptr; // 動的な剛体(Rigidbody)。
		Quaternion rot_ = Quaternion::Identity;	//回転。
		Vector3 pos_ = Vector3::Zero;		//座標。
		PhysicsGhostObject			m_physicsGhostObject;				//ゴーストオブジェクト。
		const char* m_name = nullptr;					//名前。
		float						m_timer = 0.0f;						//タイマー。
		float						m_timeLimit = 0.0f;					//削除されるまでの時間。
		bool						m_isEnableAutoDelete = true;		//自動で削除されるならtrue。
		bool						m_isEnable = true;					//trueなら当たり判定有効。
	};

	class CollisionObjectManager
	{
	public:
		CollisionObjectManager();
		~CollisionObjectManager();
		void AddCollisionObject(CollisionObject* collisionObject)
		{
			m_collisionObjectVector.push_back(collisionObject);
		}
		/// <summary>
		/// 名前が完全一致するコリジョンオブジェクトを検索する。こちらは1つだけ。
		/// </summary>
		/// <param name="name">名前。</param>
		/// <returns>コリジョンオブジェクト。見つからなかった場合はnullptr。</returns>
		CollisionObject* FindCollisionObject(const char* name)
		{
			for (auto collisionObject : m_collisionObjectVector)
			{
				//名前一致！
				if (strcmp(collisionObject->GetName(), name) == 0)
				{
					//当たり判定が有効なら。
					if (collisionObject->IsEnable() == true)
					{
						return collisionObject;
					}
				}
			}
			return nullptr;
		}
		/// <summary>
		/// 名前が前方一致するコリジョンオブジェクトを検索する。こちらは1つだけ。
		/// </summary>
		/// <param name="name">名前。</param>
		/// <returns>コリジョンオブジェクト。見つからなかった場合はnullptr。</returns>
		const CollisionObject* FindMatchForwardNameCollisionObject(const char* name) const
		{
			for (auto collisionObject : m_collisionObjectVector)
			{
				auto len = strlen(name);
				auto namelen = strlen(collisionObject->GetName());
				if (len > namelen) {
					//名前が長い。不一致。
					continue;
				}
				if (strncmp(name, collisionObject->GetName(), len) == 0)
				{
					//当たり判定が有効なら。
					if (collisionObject->IsEnable() == true)
					{
						return collisionObject;
					}
				}
			}
			return nullptr;
		}
		/// <summary>
		/// 名前が完全一致するコリジョンオブジェクトを検索する。こちらは複数。
		/// </summary>
		/// <param name="name">名前。</param>
		/// <returns>コリジョンオブジェクトの配列。</returns>
		const std::vector<CollisionObject*>& FindCollisionObjects(const char* name)
		{
			m_findMatchForwardNameCollisionObjectVector.clear();
			for (auto collisionObject : m_collisionObjectVector)
			{
				//名前一致！
				if (strcmp(collisionObject->GetName(), name) == 0)
				{
					//当たり判定が有効なら。
					if (collisionObject->IsEnable() == true)
					{
						m_findMatchForwardNameCollisionObjectVector.push_back(collisionObject);
					}
				}
			}
			return m_findMatchForwardNameCollisionObjectVector;
		}
		/// <summary>
		/// 名前が前方一致するコリジョンオブジェクトを検索する。こちらは複数。
		/// </summary>
		/// <param name="name">名前。</param>
		/// <returns>コリジョンオブジェクトの配列。</returns>
		const std::vector<CollisionObject*>& FindMatchForwardNameCollisionObjects(const char* name)
		{
			m_findsCollisionObjectVector.clear();
			for (auto collisionObject : m_collisionObjectVector)
			{
				auto len = strlen(name);
				auto namelen = strlen(collisionObject->GetName());
				if (len > namelen) {
					//名前が長い。不一致。
					continue;
				}
				if (strncmp(name, collisionObject->GetName(), len) == 0)
				{
					//当たり判定が有効なら。
					if (collisionObject->IsEnable() == true)
					{
						m_findsCollisionObjectVector.push_back(collisionObject);
					}
				}
			}
			return m_findsCollisionObjectVector;
		}
		/// <summary>
		/// 配列からコリジョンオブジェクトを削除。
		/// </summary>
		/// <param name="deleteCollisionObject">削除したいコリジョンオブジェクト。</param>
		void RemoveCollisionObject(CollisionObject* deleteCollisionObject)
		{
			for (auto it = m_collisionObjectVector.begin(); it != m_collisionObjectVector.end();) {
				// 条件一致した要素を削除する
				if (*it == deleteCollisionObject) {
					// 削除された要素の次を指すイテレータが返される。
					it = m_collisionObjectVector.erase(it);
				}
				// 要素削除をしない場合に、イテレータを進める
				else {
					++it;
				}
			}
		}
	private:
		std::vector<CollisionObject*>		m_collisionObjectVector;
		std::vector<CollisionObject*>		m_findsCollisionObjectVector;
		std::vector<CollisionObject*>		m_findMatchForwardNameCollisionObjectVector;
	};
	
}
/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"

namespace nsK2EngineLow {
	/// <summary>
	/// 静的物理オブジェクト
	/// </summary>
	class PhysicsStaticObject : public Noncopyable {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		PhysicsStaticObject();
		/// <summary>
		/// デストラクタ
		/// </summary>
		~PhysicsStaticObject();
		/// <summary>
		/// モデルからの静的オブジェクトの作成。
		/// </summary>
		/// <param name="model">モデル</param>
		/// <param name="worldMatrix">ワールド行列</param>
		/// <param name="restitution">反発力</param>
		void CreateFromModel(Model& model, const Matrix& worldMatrix, const float restitution = 0.0f);
		/// <summary>
		/// 摩擦力を設定する。
		/// </summary>
		/// <param name="friction">摩擦力。10が最大値。</param>
		void SetFriction(float friction)
		{
			m_rigidBody.SetFriction(friction);
		}
		btCollisionObject* GetbtCollisionObject()
		{
			return m_rigidBody.GetBody();
		}
		/// <summary>
		/// 剛体を破棄。
		/// </summary>
		void Release()
		{
			m_rigidBody.Release();
		}

		// 箱型のコライダーを作成する処理。
		void CreateBox(const Vector3& pos, const Quaternion rotation, const Vector3& size)
		{
			// 【修正】念のため、既存の剛体があれば解放する（これでゾンビ化を防ぐ）
			Release();

			// 箱型のメッシュコライダーを作成。
			m_boxCollider.Create(size);

			// 剛体の作成。
			RigidBodyInitData rbInfo;
			rbInfo.collider = &m_boxCollider;
			rbInfo.mass = 0.0f;
			m_rigidBody.Init(rbInfo);

			// 座標と回転をセット。
			SetPosition(pos);
			m_rigidBody.SetPositionAndRotation(pos, rotation);
		}


		void SetPosition(const Vector3& pos)
		{
			btCollisionObject* body = m_rigidBody.GetBody();
			if (!body)
				return;

			btTransform trans = body->getWorldTransform();
			trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
			body->setWorldTransform(trans);
			body->activate(true);

			if (auto* world = PhysicsWorld::GetInstance()->GetDynamicWorld())
				world->updateSingleAabb(body);
		}

		// 動く設定を有効にする処理。
		void SetKinematic(bool isKinematic)
		{
			btCollisionObject* body = m_rigidBody.GetBody();

			if (!isKinematic)
				return;

			// フラグを追加して動く物として設定する。
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

            // 自動スリープを無効化する
			// → 動いている最中に勝手に判定が消えるのを防ぐ。
			body->setActivationState(DISABLE_DEACTIVATION);
		}	

		// 物理判定のオンオフ。
		void SetIsEnable(bool isEnable)
		{
			btCollisionObject* body = m_rigidBody.GetBody();
			if(!body)
				return;

			// ターゲットの設定。
			int target = body->getCollisionFlags();

			// 
			if (isEnable)
			{
				// 有効化。
				target &= ~btCollisionObject::CF_NO_CONTACT_RESPONSE;
				body->activate(true);
			}

			else
			{
				target |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
			}

			body->setCollisionFlags(target);
		}

	private:
		BoxCollider m_boxCollider;
		MeshCollider m_meshCollider;		//メッシュコライダー。
		RigidBody m_rigidBody;				//剛体。
	};
}
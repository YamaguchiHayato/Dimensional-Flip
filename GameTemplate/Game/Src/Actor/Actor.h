/*
 *@file Autor.h。
 *@brief ゲームオブジェクトの基底クラス。
 */

#pragma once

namespace nsK2EngineLow
{
	class RenderContext;
	/*
	 * @brief ゲームオブジェクト。
	 */
	class Actor : public IGameObject
	{
	protected:
		/*
		 * @brief デストラクタ。
		 */
		virtual ~Actor() {};


	protected:
		/*
		 * @brief 処理のはじめに呼ばれる関数。
		 * @details
		 * 
		 * この関数でtrueを返すことはGameObjectの準備を完了したと判断し、
		 * Update関数を呼び始めます。
		 * その処理以降はStart関数は呼ばれなくなります。
		 */
		virtual bool Start() { return true; }
		/*
		 * @brief 更新。
		 */
		virtual void Update() {};
		/*
		 * @brief 描画。
		 */
		virtual void Render(RenderContext& rc) {};
	};

}
#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

#include"Quaternion.h"

class SphereCollider;

class OBBCollider : public InstancingGameObject {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="tag">コライダーのタグ</param>
	/// <param name="parent">親のワールド</param>
	void Initialize(const std::string& tag, const WorldTransform& parent);

	void Update();

	void Draw();

	void Debug(const char* name);

	//円コライダーとの判定
	bool IsCollision(SphereCollider*collider);

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color) { normalColor=color; }

	/// <summary>
	/// アルファ地の設定
	/// </summary>
	/// <param name="alpha"></param>
	void SetAlpha(const float alpha) { normalColor.w = alpha; }
private:
	//コライダータグ
	std::string colliderTag_;

	//すべての描画フラグ
	static bool isDraw_;

	Quaternion rotation_ = {};

	//透明度
	float alpha_ = 0.5f;

	//通常時色
	Vector4 normalColor = { 1,1,1,alpha_ };
	//ヒット時色
	Vector4 hitColor = { 1,0,0,alpha_ };

	

	struct OBB {
		Vector3 center;
		Vector3 orientations[3];
		Vector3 size;				//各半径
	};

	OBB obb_;

	//過去情報
	WorldTransform preWorld_;
};
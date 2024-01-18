#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

class SphereCollider : InstancingGameObject {

public:
	//
	SphereCollider();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="tag">コライダータグ</param>
	/// <param name="target">座標追従するワールド</param>
	/// <param name="isparent">親子関係を結ぶか</param>
	void Initialize(const std::string& tag,WorldTransform& target,bool isparent = false);

	void Update();

	void Draw();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sphere">基の円</param>
	/// <param name="backVec">基の円の戻る量</param>
	/// <returns></returns>
	bool IsHit(const SphereCollider& sphere, Vector3& backVec);

	//デバッグImGui
	void Debug(const char* name);

	void SetAlpha(const float alpha);

	void SetScale(float wide) { wide_ = wide; }

	void SetTranslate(const Vector3& pos) {position = pos; }

	float GetScale() { return wide_; }

	Vector3 GetTranslate() { return world_.translate_; }

	std::string GetTag()const { return tag_; }
private:

	float wide_=1;

	//ターゲットのワールド
	const WorldTransform* targetWorld_=nullptr;

	std::string colliderTag_;

	bool isDraw_ = true;

	Vector3 position{};
};
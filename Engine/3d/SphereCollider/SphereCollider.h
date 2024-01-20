#pragma once
#include"InstancingGameObject/InstancingGameObject.h"


class PlaneCollider;

class SphereCollider : InstancingGameObject {

public:
	//
	SphereCollider();

	void Initialize(const std::string& tag,const WorldTransform& parent);

	void Update();

	void Draw();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sphere">基の円</param>
	/// <param name="backVec">基の円の戻る量</param>
	/// <returns></returns>
	bool IsHit(const SphereCollider& sphere, Vector3& backVec);

	/// <summary>
	/// 当たったか否か
	/// </summary>
	/// <param name="plane">平面</param>
	/// <param name="beckVec">押し出し量</param>
	/// <returns></returns>
	bool IsHit(const PlaneCollider& plane, Vector3& beckVec);

	//デバッグImGui
	void Debug(const char* name);

	void SetAlpha(const float alpha);

	void SetScale(float wide) { wide_ = wide; }

	void SetTranslate(const Vector3& pos) { world_.translate_ = pos; }

	float GetScale() { return wide_; }


	Vector3 GetTranslate() { return world_.translate_; }

	std::string GetTag()const { return tag_; }

	WorldTransform GetWorld()const { return world_; }
private:

	float wide_=1;

	std::string colliderTag_;

	bool isDraw_ = true;

	//過去の状態
	WorldTransform preWorld_;
};
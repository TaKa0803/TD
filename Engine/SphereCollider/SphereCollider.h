#pragma once
#include"Instancing/InstancingGameObject.h"

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

	//デバッグImGui
	void Debug(const char* name);

	void SetScale(float wide) { wide_ = wide; }

	void SetTranslate(const Vector3& pos) { world_.translate_ = pos; }

	float GetScale() { return wide_; }

	Vector3 GetTranslate() { return world_.translate_; }

	std::string GetTag()const { return tag_; }
private:

	float wide_=1;

	std::string colliderTag_;

	const float alpha_ = 0.5f;
	
	bool isDraw_ = true;
};
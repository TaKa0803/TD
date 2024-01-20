#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

#include"Quaternion.h"

class SphereCollider;

class OBBCollider : public InstancingGameObject {
public:

	void Initialize(const std::string& tag, const WorldTransform& parent);

	void Update();

	void Draw();

	void Debug(const char* name);


	bool IsCollision(SphereCollider*collider);

private:
	std::string colliderTag_;

	bool isDraw_ = true;

	Quaternion rotation_ = {};

	Vector4 normalColor = { 1,1,1,1 };

	Vector4 hitColor = { 0,0,0,1 };

	AABB aabb_;


	struct OBB {
		Vector3 center;
		Vector3 orientations[3];
		Vector3 size;				//各半径
	};

	OBB obb_;
};
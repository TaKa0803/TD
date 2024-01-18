#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"ColliderSphere/SphereCollider.h"

class OBBCollider : public InstancingGameObject {

public:

	void Initialize(const std::string& collidertag, const WorldTransform& world);

	void Update();

	void Draw();

	bool IsCollision(const SphereCollider&collider);

	void DebugWindow(const char*namer);
private:

	//直径
	Vector3 size = { 1,1,1 };

	std::string colliderTag_;

	static bool isDraw;
};
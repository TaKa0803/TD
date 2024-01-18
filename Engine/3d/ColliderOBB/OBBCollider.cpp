#include "OBBCollider.h"

void OBBCollider::Initialize(const std::string& tag, const WorldTransform& world)
{
	InstancingGameObject::Initialize("box");

	colliderTag_ = tag;
}

void OBBCollider::Update()
{
}

void OBBCollider::Draw()
{
}

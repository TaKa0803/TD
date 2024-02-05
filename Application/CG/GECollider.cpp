#include "GECollider.h"

GEColliderScene::GEColliderScene()
{
	camera_ = std::make_unique<Camera>();
	plane_ = std::make_unique<Plane>();
	wall = std::make_unique<OBBCollider>();
	sphere = std::make_unique<SphereCollider>();

}

GEColliderScene::~GEColliderScene()
{
}

void GEColliderScene::Initialize()
{
}

void GEColliderScene::Update()
{
}

void GEColliderScene::Draw()
{
}

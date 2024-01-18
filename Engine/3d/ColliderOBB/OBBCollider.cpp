#include "OBBCollider.h"

void OBBCollider::Initialize(const std::string& tag, const WorldTransform& parent)
{
	InstancingGameObject::Initialize("z+Plane");

	world_.SetParent(&parent);

	colliderTag_ = tag;

	IMM_->SetEnableTexture(tag_, false);
	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
}

void OBBCollider::Update()
{

	world_.UpdateMatrix();

}

void OBBCollider::Draw()
{
#ifdef _DEBUG
	if (isDraw_) {
		InstancingModelManager::GetInstance()->SetWorld(tag_, world_);
	}
#endif // _DEBUG
}

void OBBCollider::Debug(const char* name)
{
}

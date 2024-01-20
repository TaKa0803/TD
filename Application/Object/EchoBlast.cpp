#include "EchoBlast.h"

void EchoBlast::Initialize(const std::string& tag)
{
	InstancingGameObject::Initialize(tag);



}

void EchoBlast::Update()
{
	//行列更新
	world_.UpdateMatrix();
	//タグに対応したモデルにワールド追加
	IMM_->SetWorld(tag_, world_);
}

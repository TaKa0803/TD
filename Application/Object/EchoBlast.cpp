#include "EchoBlast.h"

#include "SphereCollider/SphereCollider.h"
#include "ImGuiManager/ImGuiManager.h"

void EchoBlast::Initialize(const std::string& tag, const Infomation& info)
{
	InstancingGameObject::Initialize(tag);

	world_.Initialize();
	colliderWorld_.Initialize();

	data_ = info;
	direct3_ = { data_.direction_.x,0.0f,data_.direction_.y };
	world_.translate_ = data_.popPosition_ + direct3_;
	colliderWorld_.parent_ = &world_;

	colliderWorld_.translate_.y = 2.0f;

	isActive_ = true;

	collider_.reset(new OBBCollider);
	collider_->Initialize(tag, colliderWorld_);
	sCollider_.reset(new SphereCollider);
	sCollider_->Initialize(tag, colliderWorld_);

	collider_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void EchoBlast::Update()
{
	if (aliveFrame_ <= 0)
	{
		isActive_ = false;
	}
	else
	{
		aliveFrame_--;

		world_.translate_ += direct3_ * (data_.power_ * 0.1f);

		//行列更新
		world_.UpdateMatrix();
		colliderWorld_.UpdateMatrix();
		collider_->Update();
		//sCollider_->Update();
		//タグに対応したモデルにワールド追加
		IMM_->SetData(tag_, colliderWorld_);
	}
}

void EchoBlast::Draw()
{
	if (isActive_)
	{
		collider_->Draw();
		//sCollider_->Draw();
	}
}

void EchoBlast::DebugWindow()
{
	collider_->Debug("echo");
	sCollider_->Debug("echo");

	ImGui::Begin(tag_.c_str());

	ImGui::Text("tra : %.2f,%.2f", world_.translate_.x, world_.translate_.z);
	ImGui::Text("dir : %.2f,%.2f", direct3_.x, direct3_.z);
	ImGui::Separator();

	ImGui::End();
}

void EchoBlast::OnCollision()
{
}

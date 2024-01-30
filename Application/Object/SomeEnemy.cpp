#include "SomeEnemy.h"

#include "ImGuiManager/ImGuiManager.h"

void SomeEnemy::Initialize()
{
	InstancingGameObject::Initialize("parrot");
	world_.Initialize();
	world_.scale_ = { 1.0f,1.0f,1.0f };
	world_.translate_.y = 2.0f;
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("enemy");

	isActive_ = true;
}

void SomeEnemy::Update()
{
	if (!isActive_)
	{
		return;
	}
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case SomeEnemy::IDOL:

			break;
		case SomeEnemy::BURST:
			
			break;
		default:
			break;
		}
	}

	switch (behavior_)
	{
	case SomeEnemy::IDOL:

		break;
	case SomeEnemy::BURST:
		toBurstFrame_--;
		if (toBurstFrame_ <= 0)
		{
			isActive_ = false;
		}
		world_.translate_ += direct3_ * 0.5f;
		break;
	default:
		break;
	}

	world_.UpdateMatrix();
	collider_->SetTranslate(world_.translate_);
	collider_->Update();
}

void SomeEnemy::DebagWindow(int num)
{
	collider_->Debug("enemy");

	ImGui::Begin("SomeEnemy");

	ImGui::DragFloat3(std::to_string(num).c_str(), &world_.translate_.x, 0.02f);

	ImGui::End();
}

void SomeEnemy::Draw()
{
	if (!isActive_)
	{
		return;
	}
	collider_->Draw();
	IMM_->SetData(tag_, world_);
}

void SomeEnemy::OnCollision(const Vector3& direction)
{
	//if (behavior_ != BURST)
	{
		direct3_ = direction;
		reqBehavior_ = BURST;
	}
}


#include "BossEnemy.h"

#include "ImGuiManager/ImGuiManager.h"
#include "SomeEnemy.h"

BossEnemy::BossEnemy()
{
	GameObject::Initialize("ALPlayer");
}

void BossEnemy::Initialize()
{
	world_.scale_ = { 1.0f,1.0f,1.0f };
	model_->SetUVScale({ 1.0f,1.0f,1.0f });
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("boss", world_);

	enemies_.clear();
}

void BossEnemy::Update()
{
	enemies_.remove_if([](const std::unique_ptr<SomeEnemy>& some) {
		if (some->GetIsActive())
		{
			return false;
		}
		return true;
		});

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case BossEnemy::IDOL:
			break;
		case BossEnemy::SUMMON:
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}

	switch (behavior_)
	{
	case BossEnemy::IDOL:

		break;
	case BossEnemy::SUMMON:
		SummmonEnemy();
		break;
	default:
		break;
	}
	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->Update();
	}

	world_.UpdateMatrix();
	collider_->Update();
}

void BossEnemy::DebagWindow()
{
	//model_->DebugParameter("boss");
	collider_->Debug("boss");
	
	ImGui::Begin("boss");

	ImGui::DragFloat3("position", &world_.translate_.x, 0.01f);

	switch (behavior_)
	{
	case BossEnemy::IDOL:
		ImGui::Text("IDOL");
		break;
	case BossEnemy::SUMMON:
		ImGui::Text("SUMMON");
		break;
	default:
		break;
	}
	if (ImGui::Button("Summon"))
	{
		reqBehavior_ = SUMMON;
	}

	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	int num = 0;
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->DebagWindow(num++);
	}

	ImGui::End();
}

void BossEnemy::Draw(const Matrix4x4& viewp)
{
	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->Draw();
	}
	GameObject::Draw(viewp);
	collider_->Draw();
}

void BossEnemy::OnCollision()
{
}

void BossEnemy::SummmonEnemy()
{
	SomeEnemy* data = new SomeEnemy;
	data->Initialize();
	enemies_.emplace_back(data);
	reqBehavior_ = IDOL;
}

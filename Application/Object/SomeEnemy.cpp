#include "SomeEnemy.h"

#include "ImGuiManager/ImGuiManager.h"
#include "RandomNum/RandomNum.h"

void SomeEnemy::Initialize(const WorldTransform& world)
{
	InstancingGameObject::Initialize("parrot");
	world_.Initialize();
	world_.scale_ = { 1.0f,1.0f,1.0f };
	world_.translate_.y = 2.0f;
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("enemy");

	isActive_ = true;
	isHit_ = false;
	isFactor_ = false;

	playerW_ = &world;

	//初期化
	reqBehavior_ = IDOL;
}

void SomeEnemy::Update()
{
	//����ł�����I���
	if (!isActive_){
		return;
	}

	//�e�^�C�v�ɂ��s������
	switch (eType_)
	{
	case SomeEnemy::Move:
		MoveEnemyUpdate();
		break;
	case SomeEnemy::Explo:
		ExpEnemyUpdate();
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
	if (behavior_ == BURST)
	{
		direct3_ = direction;
	}
	
	//�����^�C�v�̎�
	if (eType_==Move)
	{
		direct3_ = direction;
		reqBehavior_ = BURST;
	}//�����^�C�v�̎�
	else if (eType_==Explo)
	{
		reqBehavior_ = DESTROY;
	}
}

void SomeEnemy::MoveEnemyUpdate()
{
#pragma region �ړ�����G
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case SomeEnemy::IDOL:
			momentFrame_ = cALIVEFRAME_ + (int)RandomNumber::Get(0.0f,10.0f);
			break;
		case SomeEnemy::MOVE:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::BURST:
			momentFrame_ = cBURSTFRAME_;
			break;
		case SomeEnemy::DESTROY:
			momentFrame_ = 60;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}

	switch (behavior_)
	{
	case SomeEnemy::IDOL:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
		break;
	case SomeEnemy::MOVE:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
		break;
	case SomeEnemy::BURST:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = DESTROY;
		}
		world_.translate_ += direct3_ * 0.5f;
		break;
	case SomeEnemy::DESTROY:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
		collider_->SetRadius(1.5f + (1.0f - momentFrame_ / 60.0f));
		break;
	default:
		break;
	}


#pragma endregion
}

void SomeEnemy::ExpEnemyUpdate()
{
#pragma region ��������G
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case SomeEnemy::IDOL:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::MOVE:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::BURST:
			momentFrame_ = cBURSTFRAME_;
			break;
		case SomeEnemy::DESTROY:
			momentFrame_ = 60;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}

	if (behavior_ != BURST) {
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
	}

	switch (behavior_)
	{
	case SomeEnemy::IDOL:

		break;
	case SomeEnemy::MOVE:

		break;
	case SomeEnemy::BURST:
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = DESTROY;
		}
		world_.translate_ += direct3_ * 0.5f;
		break;
	case SomeEnemy::DESTROY:

		collider_->SetRadius(1.5f + (1.0f - momentFrame_ / 60.0f));
		break;
	default:
		break;
	}

#pragma endregion

}

void SomeEnemy::OnEnemy(const Vector3& direction)
{
	if (behavior_ == IDOL)
	{
		isFactor_ = true;
		reqBehavior_ = DESTROY;
	}
	else if(behavior_ == MOVE)
	{
		isFactor_ = true;
		direct3_ = Normalize(direction);
		reqBehavior_ = BURST;
	}
}


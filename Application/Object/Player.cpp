#include "Player.h"

Player::Player()
{
	GameObject::Initialize("player");
	input_ = Input::GetInstance();
}

void Player::Initialize()
{
	world_.scale_ = { 1.0f,1.0f,1.0f };
	model_->SetUVScale({ 1.0f,1.0f,1.0f });
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("player", world_);

	blasts_.clear();
}

void Player::Update()
{
	blasts_.remove_if([](const std::unique_ptr<EchoBlast>& echo) {
		if (echo->GetIsActive())
		{
			return false;
		}
		return true;
		});


	float speed = 0.3f;
	if (input_->PushKey(DIK_W))
	{
		world_.translate_.z += speed;
		direction_.y = 1.0f;
		direction_ = Normalize(direction_);
	}if (input_->PushKey(DIK_S))
	{
		world_.translate_.z -= speed;
		direction_.y = -1.0f;
		direction_ = Normalize(direction_);
	}if (input_->PushKey(DIK_A))
	{
		world_.translate_.x -= speed;
		direction_.x = -1.0f;
		direction_ = Normalize(direction_);
	}if (input_->PushKey(DIK_D))
	{
		world_.translate_.x += speed;
		direction_.x = 1.0f;
		direction_ = Normalize(direction_);
	}

	if (input_->TriggerKey(DIK_SPACE))
	{
		reqBehavior_ = ATTACK;
		EchoBlast::Infomation info;
		info.direction_ = direction_;
		info.popPosition_ = world_.translate_;
		info.power_ = 1.0f;
		CreateEcho(info);
	}

	world_.UpdateMatrix();
	collider_->Update();

	std::list<std::unique_ptr<EchoBlast>>::iterator itr = blasts_.begin();
	for (; itr != blasts_.end(); ++itr)
	{
		itr->get()->Update();
	}
}

void Player::DebagWindow()
{
	model_->DebugParameter("player");
	collider_->Debug("player");

	std::list<std::unique_ptr<EchoBlast>>::iterator itr = blasts_.begin();
	for (; itr != blasts_.end(); ++itr)
	{
		itr->get()->DebugWindow();
	}
}

void Player::Draw(const Matrix4x4& viewp)
{
	GameObject::Draw(viewp);
	collider_->Draw();
	std::list<std::unique_ptr<EchoBlast>>::iterator itr = blasts_.begin();
	for (; itr != blasts_.end(); ++itr)
	{
		itr->get()->Draw();
	}
}

void Player::OnCollision()
{
}

void Player::CreateEcho(const EchoBlast::Infomation& info)
{
	EchoBlast* echo = new EchoBlast;
	echo->Initialize("weapon", info);
	blasts_.emplace_back(echo);
}

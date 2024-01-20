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
}

void Player::Update()
{
	float speed = 0.3f;
	if (input_->PushKey(DIK_W))
	{
		world_.translate_.z += speed;
	}if (input_->PushKey(DIK_S))
	{
		world_.translate_.z -= speed;
	}if (input_->PushKey(DIK_A))
	{
		world_.translate_.x -= speed;
	}if (input_->PushKey(DIK_D))
	{
		world_.translate_.x += speed;
	}
	world_.UpdateMatrix();
}

void Player::DebagWindow()
{
	model_->DebugParameter("player");
}

void Player::Draw(const Matrix4x4& viewp)
{
	GameObject::Draw(viewp);
}

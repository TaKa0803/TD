#include "Player.h"

Player::Player()
{
	GameObject::Initialize("player");
}

void Player::Initialize()
{
	world_.scale_ = { 1.0f,1.0f,1.0f };
	model_->SetUVScale({ 1.0f,1.0f,1.0f });
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

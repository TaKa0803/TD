#pragma once

#include "SingleGameObjects/GameObject.h"
#include "Input/Input.h"

class Player :public GameObject
{
private:
	// ワールド
	WorldTransform transform_;

	// 中心からの距離
	float moveLength_ = 0.0f;

	// 今向いている方向
	Vector2 direction_ = { 0.0f, 0.0f };

	Input* input_ = nullptr;

public:
	Player();

	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow();

	void Draw(const Matrix4x4& viewp);


private:
};

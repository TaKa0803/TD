#pragma once

#include "SingleGameObjects/GameObject.h"
#include "Input/Input.h"

#include <optional>

class Player :public GameObject
{
private:

	enum BEHAVIOR
	{
		IDOL,	// なにもしてない
		MOVE,	// 移動
		ATTACK,	// 攻撃してる？
		MOMENT,	// 後隙的な動けない時間

		_COUNT,	// カウント用
	};

private:

	// 中心からの距離
	float moveLength_ = 0.0f;

	// 今向いている方向
	Vector2 direction_ = { 0.0f, 0.0f };

	Input* input_ = nullptr;
	BEHAVIOR behavior = IDOL;
	std::optional<BEHAVIOR> reqBehavior_ = std::nullopt;

public:
	Player();

	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow();

	void Draw(const Matrix4x4& viewp);


private:
};

#pragma once

#include "InstancingGameObject/InstancingGameObject.h"
#include "SphereCollider/SphereCollider.h"

#include <optional>

class SomeEnemy :public InstancingGameObject
{
private:

	enum BEHAVIOR
	{
		IDOL,	// なにもしてない

		_COUNT,	// カウント用
	};

private:

	// 中心からの距離
	float moveLength_ = 0.0f;

	// 今向いている方向
	Vector2 direction_ = { 0.0f, 0.0f };

	BEHAVIOR behavior_ = IDOL;
	std::optional<BEHAVIOR> reqBehavior_ = std::nullopt;

	std::unique_ptr<SphereCollider> collider_;

public:
	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow();

	void Draw();

private:
};

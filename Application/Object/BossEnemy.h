#pragma once

#include "SingleGameObjects/GameObject.h"
#include "SphereCollider/SphereCollider.h"

#include <optional>
#include <list>

class SomeEnemy;

class BossEnemy :public GameObject
{
private:

	enum BEHAVIOR
	{
		IDOL,	// なにもしてない
		SUMMON,	// 雑魚敵召喚

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

	std::list<std::unique_ptr<SomeEnemy>> enemies_;

public:
	BossEnemy();

	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow();

	void Draw(const Matrix4x4& viewp);


private:

	void SummmonEnemy();

};

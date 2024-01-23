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
		BURST,	// 弾かれ中

		_COUNT,	// カウント用
	};

private:

	// 中心からの距離
	float moveLength_ = 0.0f;

	// 今向いている方向
	Vector3 direct3_ = { 0.0f,0.0f, 0.0f };

	BEHAVIOR behavior_ = IDOL;
	std::optional<BEHAVIOR> reqBehavior_ = std::nullopt;

	std::unique_ptr<SphereCollider> collider_;

	// 爆発までのフレーム
	uint32_t toBurstFrame_ = 600;

	bool isActive_ = false;

public:
	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow(int num);

	void Draw();

	SphereCollider* GetCollider() { return collider_.get(); }

	void OnCollision(const Vector3& direction);

	bool GetIsActive() const { return isActive_; }
private:
};

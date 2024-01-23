#pragma once

#include "InstancingGameObject/InstancingGameObject.h"
#include "ColliderOBB/OBBCollider.h"

class EchoBlast : public InstancingGameObject
{
public:

	struct Infomation
	{
		Vector3 popPosition_;
		Vector2	direction_;
		float power_;
	};

private:
	Infomation data_;

	// 攻撃の中心地
	// 保存用
	WorldTransform colliderWorld_;
	// 当たり判定をたくさん取る
	//std::vector<std::unique_ptr<SphereCollider>> spheres_;
	std::unique_ptr<OBBCollider> collider_;
	std::unique_ptr<SphereCollider> sCollider_;

	Vector3 direct3_;

	// 生きているフレーム
	uint32_t aliveFrame_ = 80;

	bool isActive_ = false;

public:

	void Initialize(const std::string& tag, const Infomation& info);

	void Update();

	void Draw();

	void DebugWindow();

	bool GetIsActive()const { return isActive_; }

	OBBCollider* GetCollider() { return collider_.get(); }

	void OnCollision();

	const Vector3& GetDirection() const { return direct3_; }
};
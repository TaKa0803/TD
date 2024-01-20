#pragma once

#include "InstancingGameObject/InstancingGameObject.h"
#include "SphereCollider/SphereCollider.h"

class EchoBlast : public InstancingGameObject
{
private:

	// 攻撃の中心地
	WorldTransform center_;
	// 当たり判定をたくさん取る
	std::vector<std::unique_ptr<SphereCollider>> spheres_;

public:

	void Initialize(const std::string& tag) override;

	void Update() override;

};
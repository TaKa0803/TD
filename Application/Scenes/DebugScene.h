#pragma once

#include "IScene/IScene.h"
#include"Input/Input.h"
#include"Camera/Camera.h"
#include<iostream>
#include"SphereCollider/SphereCollider.h"
#include"ColliderPlane/PlaneCollider.h"
#include"ColliderOBB/OBBCollider.h"

class DebugScene : public IScene {

public:

	DebugScene();

	~DebugScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	void Debug();

private:
	//キー入力
	Input* input_ = nullptr;

	WorldTransform cWorld_;
	std::unique_ptr<Camera> camera_;

	WorldTransform sWorld_;
	std::unique_ptr<SphereCollider> sphere_;

	WorldTransform pWorld_;
	std::unique_ptr<PlaneCollider>plane_;

	WorldTransform oWorld_;
	std::unique_ptr<OBBCollider>obb_;

};
#pragma once
#include"IScene/IScene.h"
#include"Camera/Camera.h"
#include"SingleGameObjects/GameObject.h"
#include"Object/Plane.h"
#include"SphereCollider/SphereCollider.h"
#include"ColliderOBB/OBBCollider.h"
#include<iostream>

class GEColliderScene : public IScene {

public:

	GEColliderScene();

	~GEColliderScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<Camera>camera_;

	std::unique_ptr<Plane>plane_;

	std::unique_ptr<OBBCollider> wall;
	std::unique_ptr<SphereCollider> sphere;

};
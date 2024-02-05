#pragma once
#include"IScene/IScene.h"
#include"Camera/Camera.h"
#include"SingleGameObjects/GameObject.h"
#include"Input/Input.h"
#include<iostream>

class GEEffectScene : public IScene {

public:

	GEEffectScene();

	~GEEffectScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<Camera>camera_;

	Input* input_;

	int tubuNum_ = 10;

	WorldTransform effectSpawn_;

	Vector3 velomin = { -1,-1,-1 };
	Vector3 velomax = { 1,1,1 };

	Vector3 acce = { 0,-0.1f,0 };
	int deadCount = 60;
	
};
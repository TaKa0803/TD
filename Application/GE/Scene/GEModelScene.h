#pragma once
#include"IScene/IScene.h"
#include"Camera/Camera.h"
#include"SingleGameObjects/GameObject.h"
#include<iostream>

class GEModelScene : public IScene {

public:

	GEModelScene();

	~GEModelScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<Camera>camera_;

	std::unique_ptr<GameObject>object_;
};
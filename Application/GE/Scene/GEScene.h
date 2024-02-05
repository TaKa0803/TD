#pragma once
#include"IScene/IScene.h"
#include"Camera/Camera.h"
#include"Sprite/Sprite.h"
#include<iostream>

class GEScene : public IScene {

public:

	GEScene();

	~GEScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<Camera>camera_;

	std::unique_ptr<Sprite> sprite_;
};
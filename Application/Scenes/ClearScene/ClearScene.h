#pragma once
#include "IScene/IScene.h"

#include"Input/Input.h"

#include"Sprite/Sprite.h"
#include<string>

class ClearScene :public IScene
{
public:

	ClearScene();

	~ClearScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	//デバッグ
	void Debug();

	//シーンチェンジ
	void SceneCahnge();
private:
	//キー入力
	Input* input_ = nullptr;

	std::string space = "resources/AppResource/space.png";
	std::unique_ptr<Sprite>space_;

};


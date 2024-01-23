#pragma once
#include "IScene/IScene.h"
#include"Input/Input.h"

class FailScene : public IScene
{
public:

	FailScene();

	~FailScene();

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



};


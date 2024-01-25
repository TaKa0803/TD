#pragma once
#include "IScene/IScene.h"

#include<iostream>
#include<list>

#include"Input/Input.h"
#include"Camera/Camera.h"
#include"Object/Objects.h"
#include"Sprite/Sprite.h"

#include"Effect/EffectExplosion.h"

class GameScene : public IScene {

public:

	GameScene();

	~GameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	void DebugWindows();

	void CameraUpdate();

	void CheckCollision();

private:
	//キー入力
	Input* input_ = nullptr;
	//カメラクラス
	std::unique_ptr<Camera> camera_;

	float xrotateNum = 0.05f;
	float yrotatenum = 0.02f;

	///以下ゲーム実装
	std::unique_ptr<Plane>plane_;

	std::unique_ptr<Player>player_;

	std::unique_ptr<Stage>stage_;

	std::unique_ptr<BossEnemy> boss_;

	std::unique_ptr<Skydome>skydome_;

	//エフェクト
	EffectExplosion* EffectExp_;
	//エフェクト
	std::string eTag_ = "box";
};
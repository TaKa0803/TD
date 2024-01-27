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


#pragma region ゲームルール関連

	//画像の名前
	enum SpriteName {
		Frame,
		Back,
		Good,
		Bad,
		kNumSprite
	};


	//ゲージのスプライト
	std::unique_ptr<Sprite>gageSprite_[SpriteName::kNumSprite];
	
	//スプライトのパス
	std::string gageSpritePath[SpriteName::kNumSprite] = {
		"resources/AppResource/UI/Gage_Frame.png",
		"resources/AppResource/UI/Gage_BACK.png",
		"resources/AppResource/UI/Gage_BAD.png",
		"resources/AppResource/UI/Gage_GOOD.png"
	};

	//ゲージのUIの全体の位置
	WorldTransform UIWorld_ ;

	//UI全体の座標
	Vector3 UIPosition_ = { 7.8f,71.0f,0 };
	//UIのサイズ
	Vector3 UIScale_ = { 0.7f,0.76f,1.0f };
#pragma endregion


};
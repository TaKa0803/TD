#include "IScene.h"

//初期シーンの設定
int IScene::sceneNo = MT4;

IScene::~IScene() {}

int IScene::GetSceneNo() { return sceneNo; }
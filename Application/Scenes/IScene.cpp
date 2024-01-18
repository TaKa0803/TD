#include"IScene/IScene.h"
#include"Scenes/Scenes.h"

int IScene::sceneNo = GAME;

IScene::~IScene() {}

int IScene::GetSceneNo() { return sceneNo; }


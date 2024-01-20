#include"IScene/IScene.h"
#include"Scenes/Scenes.h"

int IScene::sceneNo = TITLE;

IScene::~IScene() {}

int IScene::GetSceneNo() { return sceneNo; }


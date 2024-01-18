#include"IScene/IScene.h"
#include"Scenes/Scenes.h"

int IScene::sceneNo = DEBUG;

IScene::~IScene() {}

int IScene::GetSceneNo() { return sceneNo; }


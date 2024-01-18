#include"SceneManager/SceneManager.h"

#include"./Scenes/Scenes.h"

#include<imgui.h>


#pragma region シーンのh
#include"./Scenes/TitleScene.h"
#include"./Scenes/GameScene.h"
#pragma endregion



void SceneManager::Initialize()
{
	currentSceneNo_ = TITLE;
	//シーンの数取得
	sceneArr_.resize((size_t)SCENE::SceneCount);

	//各シーンの情報設定
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[GAME] = std::make_unique<GameScene>();

	sceneName_.clear();
	sceneName_.push_back("TITLE");
	sceneName_.push_back("GAME");

	/*
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<PlayScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	*/
}

void SceneManager::Update()
{

	//シーンチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

	//デバッグ表示
	DebugWindow();

	//シーン変更チェック
	if (prevSceneNo_ != currentSceneNo_)
	{
		//変更していたら		
		//初期化処理
		sceneArr_[currentSceneNo_]->Initialize();
	}

	//シーン更新処理
	sceneArr_[currentSceneNo_]->Update();
}

void SceneManager::Draw()
{
	//描画処理
	sceneArr_[currentSceneNo_]->Draw();

}

void SceneManager::EndFrame()
{

}

void SceneManager::Finalize()
{

}

void SceneManager::DebugWindow()
{
#ifdef _DEBUG
	ImGui::Begin("SceneManager");
	ImGui::Text("SceneNo.%d", currentSceneNo_);
	ImGui::Text("%s", sceneName_[currentSceneNo_].c_str());
	ImGui::End();
#endif // _DEBUG

}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager ins;
	return &ins;
}
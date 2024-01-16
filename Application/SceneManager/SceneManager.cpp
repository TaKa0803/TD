#include"SceneManager/SceneManager.h"

#include"Scenes.h"

#include<imgui.h>


#pragma region シーンのh
#include"TitleScene.h"
#include"GameScene.h"
#include"MT4Scene.h"
#pragma endregion



void SceneManager::Initialize() {

	currentSceneNo_ = TITLE;
	
	//シーンの数取得
	sceneArr_.resize((size_t)SCENE::SceneCount);
	
	//各シーンの情報設定
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<GameScene>();
	sceneArr_[MT4] = std::make_unique<MT4Scene>();
	/*
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<PlayScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	*/
}

void SceneManager::Update() {

	//デバッグ表示
#pragma region メニューバー表示

	if (!ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		assert(false);
	}
	if (!ImGui::BeginMenuBar()) { assert(false); }

	if (!deleteWindow) {	
		ImGui::Text("SceneNo.%d", currentSceneNo_);
		ImGui::Checkbox("deleteWindow", &deleteWindow);
	}

#pragma endregion

	

	//シーンチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

	//シーン変更チェック
	if (prevSceneNo_ != currentSceneNo_) {
		//変更していたら		
		//初期化処理
		sceneArr_[currentSceneNo_]->Initialize();
	}

	//シーン更新処理
	sceneArr_[currentSceneNo_]->Update();

#pragma region メニューバー関係
	ImGui::EndMenuBar();
	ImGui::End();
#pragma endregion

}

void SceneManager::Draw() {
	//描画処理
	sceneArr_[currentSceneNo_]->Draw();

}

void SceneManager::EndFrame() {

}

void SceneManager::Finalize() {

}



SceneManager* SceneManager::GetInstance() {
	static SceneManager ins;
	return &ins;
}
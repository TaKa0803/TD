#include"SceneManager/SceneManager.h"

#include"./Scenes/Scenes.h"

#include<imgui.h>


#pragma region シーンのh
#include"GE/Scene/GEScene.h"
#include"GE/Scene/GEModelScene.h"
#include"GE/Scene/GEParticle.h"
#include"GE/Scene/GECollider.h"
#pragma endregion



void SceneManager::Initialize()
{
	currentSceneNo_ = -1;
	//シーンの数取得
	sceneArr_.resize((size_t)SCENE::SceneCount);

	//各シーンの情報設定
	sceneArr_[SPRITE] = std::make_unique<GEScene>();
	sceneArr_[MODEL] = std::make_unique<GEModelScene>();
	sceneArr_[GEParticle] = std::make_unique<GEEffectScene>();
	sceneArr_[Collider] = std::make_unique<GEColliderScene>();
	sceneName_.clear();
	sceneName_.push_back("SPRITE");
	sceneName_.push_back("MODEL AND CAMERA");
	sceneName_.push_back("effect");
	sceneName_.push_back("collider");



	/*
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<PlayScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	*/


	//初期シーン設定
	IScene::SetSceneNo(SPRITE);

}

void SceneManager::Update()
{
	
	//シーンチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = IScene::GetSceneNo();
	
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

	int num = IScene::GetSceneNo();

	ImGui::Begin("SceneManager");
	ImGui::Text("SceneNo.%d", currentSceneNo_);
	ImGui::Text("%s", sceneName_[currentSceneNo_].c_str());
	ImGui::SliderInt("sceneNo", &num,0, SceneCount-1);
	ImGui::End();

	IScene::SetSceneNo(num);
#endif // _DEBUG

}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager ins;
	return &ins;
}
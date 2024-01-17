#include "MainSystem.h"
#include"SceneManager/SceneManager.h"
#include"DebugScene.h"
#include"MT4Scene.h"
#include"GlobalVariables/GlobalVariables.h"
#include"RandomNum/RandomNum.h"
#include"AudioManager/AudioManager.h"

#include"SingleGameObjects/ObjectPSO.h"

MainSystem* MainSystem::GetInstance() {
	static MainSystem instance;
	return &instance;
}

void MainSystem::Run() {
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	//エンジン初期化処理
	Initializes();

	//更新処理
	MainRoop();

	//終了処理
	Finalize();

	//終わり
	CoUninitialize();
}

void MainSystem::Initializes() {
	//windowsアプリケーション
	winApp = WindowApp::GetInstance();
	winApp->Initialize(L"LE2A_07_キクチ",1280,720);

	//DirectX
	DXF = DirectXFunc::GetInstance();
	DXF->Initialize(winApp);

	
	//SRV
	SRVM_ = SRVManager::GetInstance();
	SRVM_->Initialize(DXF);
	
	//画像関係
	textureManager= TextureManager::GetInstance();
	textureManager->Initialize(DXF);

	
	//imgui
	imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(winApp, DXF);

	//入力
	input = Input::GetInstance();
	input->Initialize(winApp);


	//モデルデータ関係
	ModelManager::GetInstance()->LoadAllModels();

	//
	instancingMM_ = InstancingModelManager::GetInstance();
	instancingMM_->LoadAllModel();

	//乱数クラス
	randomNumClass_ = RandomNumber::GetInstance();
	randomNumClass_->RandomNumberProcessInitialize();
	
	//音声マネージャ
	AudioManager *audioManager = AudioManager::GetInstance();
	audioManager->Initialize();
	audioManager->LoadAllSoundData();
	
}

void MainSystem::MainRoop() {
	//保存データ取得
	GlobalVariables::GetInstance()->LoadFiles();

	//シーンマネージャ初期化
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Initialize();

	//DebugScene* dScene = new DebugScene();
	//dScene->Initialize();

	//MT4Scene* mt4 = new MT4Scene();
	//mt4->Initialize();

	//読み込んだ画像をGPUに送信
	SRVM_->PostInitialize();

	auto processTime = 0.0f;

	while (winApp->ProcessMessage()) {
		//現時間取得
		auto start = std::chrono::high_resolution_clock::now();

#pragma region 状態更新
		///更新前処理
		//ImGui
		imguiManager->PreUpdate();

		//インスタンシングの更新前処理
		instancingMM_->PreUpdate();

		//キー入力
		input->Update();
		///=以下更新=//

#ifdef _DEBUG
		//GlobalVariableデータの更新処理
		GlobalVariables::GetInstance()->Update();
		ImGui::Begin("Engine");
		ImGui::End();
#endif // _DEBUG

		sceneManager->Update();
		//dScene->Update();
		//mt4->Update();


		//==更新終わり==//
		//更新終わり描画前処理
		imguiManager->PostUpdate();
#pragma endregion
#pragma region 描画		
		///描画前処理
		//DirectX
		DXF->PreDraw();
		//ImGui
		imguiManager->PreDraw();
		
		//==以下描画==//


		sceneManager->Draw();
		//dScene->Draw();
		//mt4->Draw();

		
		//==描画終わり==//

		///描画あと処理
		//imGui
		imguiManager->PostDraw();
		//DirectX
		DXF->PostDraw();
#pragma endregion
		///フレーム終了時処理
		sceneManager->EndFrame();

		auto end = std::chrono::high_resolution_clock::now();
	}


	sceneManager->Finalize();
	//dScene->Finalize();
	//mt4->Finalize();

}


void MainSystem::Finalize() {
	///開放処理
	
	SRVM_->Finalize();
	textureManager->Finalize();
	imguiManager->Finalize();
	DXF->Finalize();
	winApp->Finalize();

}

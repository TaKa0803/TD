#include "DebugScene.h"
#include"Scenes.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"RandomNum/RandomNum.h"

DebugScene::DebugScene()
{
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();

	/*pHead_ = std::make_unique<InstancingGameObject>();
	pLA_ = std::make_unique<InstancingGameObject>();
	pRA_ = std::make_unique<InstancingGameObject>();
	pLF_ = std::make_unique<InstancingGameObject>();
	pRF_ = std::make_unique<InstancingGameObject>();
	pHI_ = std::make_unique<InstancingGameObject>();

	pHead_->SetParent(&cWorld_);
	pLA_->SetParent(&pHead_->GetWorld());
	pRA_->SetParent(&pHead_->GetWorld());
	pLF_->SetParent(&pHead_->GetWorld());
	pRF_->SetParent(&pHead_->GetWorld());
	pHI_->SetParent(&pHead_->GetWorld());

	pLA_->SetTranslate({ -0.8f,0,0 });
	pRA_->SetTranslate({ 0.8f,0,0 });
	pLF_->SetTranslate({ -0.6f,-1.0f,0 });
	pRF_->SetTranslate({ 0.6f,-0.98f,0 });
	pHI_->SetTranslate({0,0.93f,0});*/

#pragma region 観客
	h_ = std::make_unique<InstancingGameObject>();
	B_ = std::make_unique<InstancingGameObject>();
	LA_ = std::make_unique<InstancingGameObject>();
	RA_ = std::make_unique<InstancingGameObject>();

	B_->SetParent(&cWorld_);
	h_->SetParent(&B_->GetWorld());
	LA_->SetParent(&B_->GetWorld());
	RA_->SetParent(&B_->GetWorld());

	B_->SetTranslate({});
	h_->SetTranslate({ 0,0.57f,0 });
	LA_->SetTranslate({ -1.0f,0,0, });
	RA_->SetTranslate({ 1.0f,0,0, });

	animeData[NORMAL].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[NORMAL].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[NORMAL].parts[BODY].pos.st;
	animeData[NORMAL].parts[BODY].pos.ed;
	animeData[NORMAL].parts[LARM].pos.st = { -1.0f,0.1f,0, };
	animeData[NORMAL].parts[LARM].pos.ed = { -1.0f,-0.1f,0, };
	animeData[NORMAL].parts[RARM].pos.st = { 1.0f,0.1f,0, };
	animeData[NORMAL].parts[RARM].pos.ed = { 1.0f,-0.1f,0, };

	animeData[GOOD].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[GOOD].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[GOOD].parts[BODY].pos.st = { 0,0.1f,0 };
	animeData[GOOD].parts[BODY].pos.ed = { 0,0.1f,0 };
	animeData[GOOD].parts[BODY].rotate.st = { 0,0,0.2f };
	animeData[GOOD].parts[BODY].rotate.ed = { 0,0,-0.2f };
	animeData[GOOD].parts[LARM].pos.st = { -1.0f,1.2f,0, };
	animeData[GOOD].parts[LARM].pos.ed = { -1.0f,1.2f,0, };
	animeData[GOOD].parts[RARM].pos.st = { 1.0f,1.2f,0, };
	animeData[GOOD].parts[RARM].pos.ed = { 1.0f,1.2f,0, };

	animeData[BAD].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[BAD].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[BAD].parts[HEAD].rotate.st = { 0.5f,0,0 };
	animeData[BAD].parts[HEAD].rotate.ed = { 0.4f,0,0 };
	animeData[BAD].parts[BODY].pos.st;
	animeData[BAD].parts[BODY].pos.ed;
	animeData[BAD].parts[BODY].rotate.st;
	animeData[BAD].parts[BODY].rotate.ed;
	animeData[BAD].parts[LARM].pos.st = { -0.6f,-0.22f,0, };
	animeData[BAD].parts[LARM].pos.ed = { -0.7f,-0.07f,0, };
	animeData[BAD].parts[RARM].pos.st = { 0.6f,-0.22f,0, };
	animeData[BAD].parts[RARM].pos.ed = { 0.7f,-0.07f,0, };

#pragma endregion



	//エフェクト
	EffectExp_ = EffectExplosion::GetInstance();
}

DebugScene::~DebugScene() {}


void DebugScene::Initialize()
{

	cWorld_.Initialize();
	camera_->Initialize();
	camera_->SetTarget(&cWorld_);

	/*pHead_->Initialize("pH");
	pLA_->Initialize("pLA");
	pRA_->Initialize("pRA");
	pLF_->Initialize("pLF");
	pRF_->Initialize("pRF");
	pHI_->Initialize("pHI");*/

	h_->Initialize("aH");
	B_->Initialize("aB");
	LA_->Initialize("aLA");
	RA_->Initialize("aRA");


	EffectExp_->Initialize();

	nowAnime = animeData[BAD];
}

void DebugScene::Update()
{
	Debug();

	camera_->Update();


	Vector3 move = input_->GetAllArrowKey();



	Vector3 backV;


	if (input_->TriggerKey(DIK_SPACE)) {

		EffectData newData;

		newData.tag = eTag_;

		for (int i = 0; i < 10; ++i) {

			moveData movedata;

			movedata.world = { 0,2,0 };
			movedata.world.scale_ = { 0.2f,0.2f ,0.2f };
			movedata.velo = {
				RandomNumber::Get(-1,1),
				RandomNumber::Get(-1,1),
				RandomNumber::Get(-1,1)
			};

			movedata.velo.SetNormalize();
			movedata.velo *= 1.0f;

			movedata.acce = { 0,-0.1f,0 };

			movedata.maxDeadCount = 60;

			newData.mData.push_back(movedata);

		}
		EffectExp_->AddEffectData(newData);
	}

	EffectExp_->Update();

	/*pHead_->Update();
	pLA_->Update();
	pRA_->Update();
	pLF_->Update();
	pRF_->Update();
	pHI_->Update();*/

	Animation();

	h_->SetTranslate(nowAnime.pos[HEAD]);
	h_->SetRotate(nowAnime.rotate[HEAD]);

	B_->SetTranslate(nowAnime.pos[BODY]);
	B_->SetRotate(nowAnime.rotate[BODY]);

	LA_->SetTranslate(nowAnime.pos[LARM]);
	LA_->SetRotate(nowAnime.rotate[LARM]);

	RA_->SetTranslate(nowAnime.pos[RARM]);
	RA_->SetRotate(nowAnime.rotate[RARM]);


	h_->Update();
	B_->Update();
	LA_->Update();
	RA_->Update();

}

void DebugScene::Draw()
{
	/*pHead_->Draw();
	pLA_->Draw();
	pRA_->Draw();
	pLF_->Draw();
	pRF_->Draw();
	pHI_->Draw();*/
	h_->Draw();
	B_->Draw();
	LA_->Draw();
	RA_->Draw();


	EffectExp_->Draw();

	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}

void DebugScene::Debug()
{
	camera_->DrawDebugWindow("camera");

	/*pHead_->Debug("pH");
	pLA_->Debug("pLA");
	pRA_->Debug("pRA");
	pLF_->Debug("pLF");
	pRF_->Debug("pRF");
	pHI_->Debug("pHI");*/

	h_->Debug("h");
	B_->Debug("B");
	LA_->Debug("LA");
	RA_->Debug("RA");

}

void DebugScene::Animation()
{
	float t = float(nowAnime.count) / float(nowAnime.maxCount);
	if (t > 1.0f) {
		t = 1.0f;
	}

	//すべてのパーツのデータを代入した
	int count = 0;
	for (auto& p : nowAnime.pos) {

		p = Esing(nowAnime.parts[count].pos.st, nowAnime.parts[count].pos.ed, t);
		nowAnime.rotate[count] = Esing(nowAnime.parts[count].rotate.st, nowAnime.parts[count].rotate.ed, t);
		count++;
	}


	if (nowAnime.count++ >= nowAnime.maxCount) {
		nowAnime.count = 0;
		for (int i = 0; i < _count; i++) {
			std::swap(nowAnime.parts[i].pos.st, nowAnime.parts[i].pos.ed);
			std::swap(nowAnime.parts[i].rotate.st, nowAnime.parts[i].rotate.ed);
		}
	}

}

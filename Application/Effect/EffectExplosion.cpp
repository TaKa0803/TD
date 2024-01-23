#include "EffectExplosion.h"

EffectExplosion* EffectExplosion::GetInstance()
{
	static EffectExplosion instance;
	return &instance;
}

void EffectExplosion::Initialize()
{
	IMM_ = InstancingModelManager::GetInstance();
}

void EffectExplosion::AddEffectData(const EffectData& eData)
{
}

void EffectExplosion::Update()
{

	//各データの動きのデータの処理
	for (auto& data : datas_) {

		//各パーツ更新
		for (auto& pdata : data->mData) {
			//移動量更新
			pdata->world.translate_ += pdata->velo;
			//加速度処理
			pdata->velo += pdata->acce;

			//行列更新
			pdata->world.UpdateMatrix();

			//死亡チェック
			if (pdata->deadCount++>=pdata->maxDeadCount) {
				//データ削除
				delete pdata;
				pdata = nullptr;
			}
		}



		//ワールドのデータがなかったら元を削除
		if (data->mData.size() == 0) {
			delete data;
			data = nullptr;
		}


	}




}

void EffectExplosion::Draw()
{

	for (auto& data : datas_) {

		//各ワールドデータ
		for (auto& worldData : data->mData) {
			//ワールド取り出し
			WorldTransform world = worldData->world;

			//モデルにワールド送信
			IMM_->SetWorld(data->tag, world);
		}

	}

}

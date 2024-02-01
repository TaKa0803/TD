#include "ESphereExplo.h"

EfSphereExplosion* EfSphereExplosion::GetInstance()
{
	static EfSphereExplosion instance;
	return &instance;
}

void EfSphereExplosion::Initialize(const std::string& tag)
{
	IMM_ = InstancingModelManager::GetInstance();
	tag_ = tag;
	datas_.clear();
}

void EfSphereExplosion::AddEffectData(const ExploData& eData)
{
	datas_.emplace_back(eData);
}

void EfSphereExplosion::Update()
{

	for (auto& data : datas_) {

		if (data.count++ >= data.maxDeadCount) {
			continue;
		}

		//サイズ関係
		float t = float(data.count / data.maxScaleCount);
		if (t >= 1.0f) {
			t = 1.0f;
		}		
		float scale = data.minScale * (1.0f - t) + data.maxScale * t;
		data.world.scale_ = { scale,scale ,scale };

		//色関係
		/*if (data.count >= data.minAlphaCount) {
			float colort = float((data.count-data.minAlphaCount) / (data.maxDeadCount - data.minAlphaCount));
			data.color = Esing(data.mincolor, data.maxColor, colort);
		}*/

		data.world.UpdateMatrix();
	}

	//死亡チェック
	datas_.remove_if([](ExploData& data) {
		if (data.count >= data.maxDeadCount) {
			return true;
		}
		else {
			return false;
		}
	});

}

void EfSphereExplosion::Draw()
{
	for (auto& data : datas_) {
		IMM_->SetData(tag_, data.world, data.color);
	}
}


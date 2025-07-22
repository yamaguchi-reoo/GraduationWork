#include "SceneBase.h"
#include "../common.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Initialize()
{
}

eSceneType SceneBase::Update()
{
	return GetNowSceneType();
}

void SceneBase::Draw()
{
}

void SceneBase::Finalize()
{
	for (auto obj : objects)
	{
		if (obj)
		{
			obj->Finalize();
			delete obj;
		}
	}
	objects.clear();
}

void SceneBase::DeleteObject(GameObject* obj)
{
	if (obj == nullptr) return;

	auto it = std::find(objects.begin(), objects.end(), obj);

	if (it != objects.end()) {
		//見つかった場合、オブジェクトを削除
		(*it)->Finalize(); //終了処理
		delete* it;         //メモリ解放
		objects.erase(it);  //リストから削除
	}
}

template<class T>
inline T* SceneBase::CreateObject(const Vector2D& _location, const Vector2D& _box_size)
{
	//新しいインスタンスを生成
	T* new_instance = new T();

	GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

	if (new_object == nullptr)
	{
		delete new_instance;
		throw("ゲームオブジェクトが生成できませんでした\n");
	}
	new_object->Initialize(_location, _box_size);

	new_object->SetLocation(_location);

	objects.push_back(new_object);

	//生成したインスタンスを返す
	return new_instance;
}
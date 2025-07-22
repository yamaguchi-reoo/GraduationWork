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
		//���������ꍇ�A�I�u�W�F�N�g���폜
		(*it)->Finalize(); //�I������
		delete* it;         //���������
		objects.erase(it);  //���X�g����폜
	}
}

template<class T>
inline T* SceneBase::CreateObject(const Vector2D& _location, const Vector2D& _box_size)
{
	//�V�����C���X�^���X�𐶐�
	T* new_instance = new T();

	GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

	if (new_object == nullptr)
	{
		delete new_instance;
		throw("�Q�[���I�u�W�F�N�g�������ł��܂���ł���\n");
	}
	new_object->Initialize(_location, _box_size);

	new_object->SetLocation(_location);

	objects.push_back(new_object);

	//���������C���X�^���X��Ԃ�
	return new_instance;
}
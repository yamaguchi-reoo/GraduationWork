#pragma once
#include <vector>
#include "../Utility/Vector2D.h"
#include "GameObject.h"

class ObjectManager
{
private:
	std::vector<GameObject*> objects;

public:
    ObjectManager();
    ~ObjectManager();

    void Initialize();
    void Update();
    void Draw(Vector2D offset, double rate);
    void Finalize();

    void DeleteObject(GameObject* obj);


	//�Q�[���I�u�W�F�N�g�𐶐�����e���v���[�g�֐�
	template <class T>
	T* CreateObject(const Vector2D& _location, const Vector2D& _box_size)
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

		objects.push_back(new_object);

		//���������C���X�^���X��Ԃ�
		return new_instance;
	}


	GameObject* FindObjectType(eObjectType type);
};


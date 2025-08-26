#pragma once
#include <vector>
#include "../Utility/Vector2D.h"
#include "GameObject.h"

class ObjectManager
{
private:
	std::vector<GameObject*> objects;

	// �I�u�W�F�N�g�̍폜�p
	std::vector<GameObject*> delete_objects;

public:
    ObjectManager();
    ~ObjectManager();

    void Initialize();
    void Update(Vector2D offset);
    void Draw(Vector2D offset, double rate);
    void Finalize();

	//�\�񂵂Ă���I�u�W�F�N�g���폜
	void RequestDeleteObject(GameObject* obj);

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

		new_object->SetObjectManager(this);
		objects.push_back(new_object);

		//���������C���X�^���X��Ԃ�
		return new_instance;
	}


	GameObject* FindObjectType(eObjectType type);

	std::vector<GameObject*>GetObjects(eObjectType type);

	bool IsOnScreen(GameObject* obj, Vector2D offset);


};


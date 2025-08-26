#pragma once
#include <vector>
#include "../Utility/Vector2D.h"
#include "GameObject.h"

class ObjectManager
{
private:
	std::vector<GameObject*> objects;

	// オブジェクトの削除用
	std::vector<GameObject*> delete_objects;

public:
    ObjectManager();
    ~ObjectManager();

    void Initialize();
    void Update(Vector2D offset);
    void Draw(Vector2D offset, double rate);
    void Finalize();

	//予約しているオブジェクトを削除
	void RequestDeleteObject(GameObject* obj);

    void DeleteObject(GameObject* obj);


	//ゲームオブジェクトを生成するテンプレート関数
	template <class T>
	T* CreateObject(const Vector2D& _location, const Vector2D& _box_size)
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

		new_object->SetObjectManager(this);
		objects.push_back(new_object);

		//生成したインスタンスを返す
		return new_instance;
	}


	GameObject* FindObjectType(eObjectType type);

	std::vector<GameObject*>GetObjects(eObjectType type);

	bool IsOnScreen(GameObject* obj, Vector2D offset);


};


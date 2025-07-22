#include "ObjectManager.h"
#include <algorithm>

ObjectManager::ObjectManager() {}

ObjectManager::~ObjectManager()
{
    Finalize(); 
}

void ObjectManager::Initialize()
{
    
}

void ObjectManager::Update()
{
    for (auto obj : objects)
    {
        if (obj) obj->Update();
    }

	GameObject* player = FindObjectType(eObjectType::PLAYER);

    if (player)
    {
        for (auto obj : objects)
        {
            if (obj && obj != player)
            {
                if (player->CheckBoxCollision(obj))
                {
                    player->OnHitCollision(obj);
                    obj->OnHitCollision(player);
                }
            }
        }
    }
}


void ObjectManager::Draw(Vector2D offset, double rate)
{
    for (auto obj : objects)
    {
        if (obj) obj->Draw(offset, rate);
    }
}

void ObjectManager::Finalize()
{
    //オブジェクトが空の場合は処理をスキップ
    if (objects.empty())
    {
        return;
    }
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

void ObjectManager::DeleteObject(GameObject* obj)
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

GameObject* ObjectManager::FindObjectType(eObjectType type)
{
    for (auto obj : objects)
    {
        if (obj && obj->GetObjectType() == type)
        {
            return obj;
        }
    }
    return nullptr;
}



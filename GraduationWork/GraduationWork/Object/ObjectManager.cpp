#include "ObjectManager.h"
#include <algorithm>
#include <DxLib.h>

#include "ObjectInfo.h"
#include "ObjectList.h"

#include <Windows.h>


ObjectManager::ObjectManager() {}

ObjectManager::~ObjectManager()
{
    Finalize(); 
}

void ObjectManager::Initialize()
{
	uniform_grid.Initialize(0.0f, 0.0f, static_cast<float>(BLOCK_SIZE * 87), static_cast<float>(BLOCK_SIZE * 15), static_cast<float>(BLOCK_SIZE));
}

void ObjectManager::Update(Vector2D offset)
{
	//Gridの更新
	uniform_grid.Clear();
    for(auto obj : objects)
    {
        uniform_grid.InsertObject(obj);
	}

	//画面内にあるObjectのみを対象にUpdate
    for (auto obj : objects)
    {
        if (IsOnScreen(obj, offset))
        {
            obj->Update();
        }
    }

     // 全オブジェクト同士の衝突判定（重複チェック回避のため片側ループ）
    //for (size_t i = 0; i < objects.size(); ++i)
    //{
    //    for (size_t j = i + 1; j < objects.size(); ++j)
    //    {
    //        auto obj_a = objects[i];
    //        auto obj_b = objects[j];
    //        if (obj_a && obj_b)
    //        {
    //            // Player と Light の組み合わせはスキップ
    //            if ((obj_a->GetObjectType() == PLAYER && obj_b->GetObjectType() == LIGHT) ||
    //                (obj_a->GetObjectType() == LIGHT && obj_b->GetObjectType() == PLAYER))
    //            {
    //                continue;
    //            }

    //            if (obj_a->CheckBoxCollision(obj_b))
    //            {
    //                obj_a->OnHitCollision(obj_b);
    //                obj_b->OnHitCollision(obj_a);
    //            }
    //        }
    //    }
    //}

    //GameObject* enemy_f = FindObjectType(eObjectType::ENEMY);

    //if (enemy_f)
    //{
    //    for (auto obj : objects)
    //    {
    //        if (obj && obj != enemy_f)
    //        {
    //            if (enemy_f->CheckBoxCollision(obj))
    //            {
    //                enemy_f->OnHitCollision(obj);
    //                obj->OnHitCollision(enemy_f);
    //            }
    //        }
    //    }
    //}

    for (auto obj : objects)
    {
        Vector2D loc = obj->GetLocation();
        Vector2D size = obj->GetBoxSize();

        std::vector<GameObject*> nearby;
        uniform_grid.QueryArea(loc.x, loc.y, loc.x + size.x, loc.y + size.y, nearby);

        for (auto other : nearby)
        {
            if (obj == other) continue;

            // Player-Light 組み合わせスキップ
            if ((obj->GetObjectType() == PLAYER && other->GetObjectType() == LIGHT) ||
                (obj->GetObjectType() == LIGHT && other->GetObjectType() == PLAYER))
                continue;

            if (obj->CheckBoxCollision(other))
            {
                obj->OnHitCollision(other);
                other->OnHitCollision(obj);
            }
        }
    }


    //Update後に削除を実行
    for (auto obj : delete_objects)
    {
        DeleteObject(obj);
    }
    delete_objects.clear();
}

void ObjectManager::Draw(Vector2D offset, double rate)
{
    std::vector<GameObject*> draw_list;

    // 画面内のみを描画対象に
    for (auto obj : objects)
    {
        if (IsOnScreen(obj, offset))
        {
            draw_list.push_back(obj);
        }
    }

    // 描画優先度でソート
    std::sort(draw_list.begin(), draw_list.end(),
        [](GameObject* a, GameObject* b)
        {
            return a->GetDrawPriority() < b->GetDrawPriority();
        });

    // 描画
    for (auto obj : draw_list)
    {
        obj->Draw(offset, rate);
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

void ObjectManager::RequestDeleteObject(GameObject* obj)
{
    if (obj && std::find(delete_objects.begin(), delete_objects.end(), obj) == delete_objects.end()) {
        delete_objects.push_back(obj);
    }
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

std::vector<GameObject*> ObjectManager::GetObjects(eObjectType type)
{
    std::vector<GameObject*> result;

    for (auto* obj : objects)
    {
        if (obj->GetObjectType() == type)
        {
            result.push_back(obj);
        }
    }

    return result;
}

std::vector<GameObject*> ObjectManager::GetNearbyObjects(Vector2D _location, Vector2D _box_size, int _type)
{
    std::vector<GameObject*> nearby;
    std::vector<GameObject*> result;

    // uniform_grid を使って範囲内のオブジェクトを取得
    uniform_grid.QueryArea(_location.x, _location.y, _location.x + _box_size.x, _location.y + _box_size.y, nearby);

    for (auto obj : nearby)
    {
        if (!obj) continue;

        int type = obj->GetObjectType();
        if ((_type & type) == 0) continue; 

        result.push_back(obj);
    }

    return result;
}


bool ObjectManager::IsOnScreen(GameObject* obj, Vector2D offset)
{
    if (!obj) return false;

    Vector2D loc = obj->GetLocation();
    Vector2D size = obj->GetBoxSize();

    float screen_left = offset.x - 300;
    float screen_right = offset.x + SCREEN_WIDTH + 300;
    float screen_top = offset.y;
    float screen_bottom = offset.y + SCREEN_HEIGHT;

    return loc.x + size.x > screen_left &&
        loc.x < screen_right &&
        loc.y + size.y > screen_top &&
        loc.y < screen_bottom;
}

void ObjectManager::DrawNoBlendObjects(Vector2D offset, double rate)
{
    std::vector<GameObject*> draw_list;

    for (auto obj : objects)
    {
        if (!IsOnScreen(obj, offset)) continue;

        eObjectType type = obj->GetObjectType();
        // Blend演出を「受けない」オブジェクトを再描画対象に
        if (type == PLAYER || type == LIGHT || type == ENEMY || type == INVISIBLEFLOOR)
        {
            draw_list.push_back(obj);
        }
    }

    std::sort(draw_list.begin(), draw_list.end(), [](GameObject* a, GameObject* b)
        {
            return a->GetDrawPriority() < b->GetDrawPriority();
        });

    for (auto obj : draw_list)
    {
        obj->Draw(offset, rate);
    }
}



#pragma once
#include <DxLib.h>

enum eObjectType
{
	NONE = 0,
	BLOCK,
	PLAYER,
	WALL,
	LIGHT,
	INVISIBLEFLOOR,
	PUSHBLOCK,
	OBJECTTYPE_COUNT // タイルの種類数（最後に追加）

	ENEMY,
	REALENEMY,
};

struct TypeInfo
{
    eObjectType type;
    const char* name;
    int color;
};

// タイル情報テーブル（グローバルでもOK、StageEditorクラス内に static 配列でもOK）
static TypeInfo TILE_INFO[OBJECTTYPE_COUNT] = {
    { NONE,          "None",      GetColor(55, 55, 55) },
    { BLOCK,         "Block",     GetColor(100, 200, 100) },
    { PLAYER,        "Player",    GetColor(200, 100, 100) },
    { WALL,          "Wall",      GetColor(150, 150, 150) },
    { LIGHT,         "Light",     GetColor(255, 255, 0) },
    { INVISIBLEFLOOR,"Invisible", GetColor(100, 100, 200) },
    { PUSHBLOCK,     "Push",      GetColor(200, 100, 200) }
};

// ヘルパー関数
inline const TypeInfo& GetTypeInfo(eObjectType type)
{
    int idx = static_cast<int>(type);
    if (idx < 0 || idx >= OBJECTTYPE_COUNT)
        return TILE_INFO[0]; // NONEを返す
    return TILE_INFO[idx];
}

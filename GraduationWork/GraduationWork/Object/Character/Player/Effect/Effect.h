#pragma once
#include <DxLib.h>
#include "../../../../Utility/Vector2D.h"
#include <vector>

class Effect 
{
private:
	std::vector<int> shadow_image;
	std::vector<int> real_image;
	std::vector<int>* current_image;

	float frame = 0.0f;
	float speed = 0.0f;
	Vector2D position;

	bool is_active = false;

public:
	void Initialize();
	void Update(Vector2D player_center);
	void Draw(Vector2D offset);
	void Start(Vector2D pos, bool is_shadow);

	bool IsActive() const { return is_active; }
};

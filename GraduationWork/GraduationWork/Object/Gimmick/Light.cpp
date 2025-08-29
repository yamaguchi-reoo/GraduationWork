#include "Light.h"
#include "DxLib.h"
#include "../../Object/ObjectManager.h"
#include "../Character/Player/Player.h"

#include <cmath>

void Light::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = LIGHT;
	__super::Initialize(_location, _box_size);

	width = box_size.x * 5.0f;  // ���̕��i�e���͈́j
	height = box_size.y * 5.0f; // ���̍����i�e���͈́j

	light_data.origin = location;
	light_data.angle = 90.0f;			// �����p�x�i������j
	light_data.min_angle = 60.0f;		// �ŏ��p�x
	light_data.max_angle = 120.0f;		// �ő�p�x
	light_data.rotate_speed = 30.0f;	// ��]���x�i�x/�b�j
	light_data.direction = 1;			// ��������]
	light_data.moving = false;			// ���쒆
}

void Light::Update()
{
	__super::Update();

	if (light_data.moving)
	{
		light_data.angle += light_data.rotate_speed * (1.0f / 60.0f) * light_data.direction; 

		if(light_data.angle >= light_data.max_angle)
		{
			light_data.angle = light_data.max_angle;
			light_data.direction = -1; // �t�����ɉ�]
		}
		else if (light_data.angle <= light_data.min_angle)
		{
			light_data.angle = light_data.min_angle;
			light_data.direction = 1; // �������ɉ�]
		}
	}

	// ObjectManager �o�R�� Player ���擾
	if (object_manager)
	{
		for (auto obj : object_manager->GetObjects(PLAYER))
		{
			if (CheckLightCollision(obj))
			{
				this->OnHitCollision(obj); // ���ɑ΂��锽���i�폜�Ȃǁj
			}
		}
	}

	// ���Ƃ̓����蔻��ō����𐧌�
	max_height = height; // ��U���̍����Ƀ��Z�b�g
	if (object_manager)
	{
		for (auto obj : object_manager->GetObjects(BLOCK)) // ���I�u�W�F�N�g
		{
			Vector2D obj_pos = obj->GetLocation();
			Vector2D obj_size = obj->GetBoxSize();

			// tip �̍��W�v�Z
			float rad = light_data.angle * DX_PI_F / 180.0f;
			float base_half = width;
			Vector2D tip = location;

			float tip_x = tip.x + cos(rad) * height; // ���̐�[X
			float tip_y = tip.y + sin(rad) * height; // ���̐�[Y

			// ���̐�[�����ɓ͂��ꍇ
			if (tip_x >= obj_pos.x && tip_x <= obj_pos.x + obj_size.x &&
				tip_y >= obj_pos.y)
			{
				float distance_to_floor = obj_pos.y - tip.y;
				if (distance_to_floor < max_height)
					max_height = distance_to_floor; // ���܂ł̋����ɐ���
			}
		}
	}
}

void Light::Draw(Vector2D offset, double rate)
{
	Vector2D tip = location - offset;
	float rad = light_data.angle * DX_PI_F / 180.0f;
	float half_width = width;

	const int SEGMENTS = 20;
	std::vector<Vector2D> base_points;

	// ���̐�[
	float effective_height = height; // �ő卂��
	Vector2D top = {
		tip.x + cos(rad) * effective_height,
		tip.y + sin(rad) * effective_height
	};

	// ���E�̊��_�itip ��菭�����ɐݒ肵�Ăɂ��݂�h���j
	Vector2D left_base = { top.x - half_width, top.y + 0.5f };
	Vector2D right_base = { top.x + half_width, top.y + 0.5f };

	// SEGMENTS �ŕ���
	for (int i = 0; i <= SEGMENTS; i++)
	{
		float t = (float)i / SEGMENTS;
		Vector2D pt;
		pt.x = left_base.x + (right_base.x - left_base.x) * t;
		pt.y = left_base.y;

		// ���ɉ��킹��
		if (object_manager)
		{
			for (auto block : object_manager->GetObjects(BLOCK))
			{
				Vector2D bpos = block->GetLocation();
				Vector2D bsize = block->GetBoxSize();

				if (pt.x >= bpos.x && pt.x <= bpos.x + bsize.x)
				{
					if (pt.y < bpos.y && bpos.y > tip.y)
						pt.y = bpos.y;
				}
			}
		}

		base_points.push_back(pt);
	}

	SetDrawBlendMode(DX_BLENDMODE_ADD, 128);

	// tip �ƕ����_�ŎO�p�`�`��iint �ɃL���X�g�j
	for (int i = 0; i < SEGMENTS; i++)
	{
		DrawTriangle(
			static_cast<int>(tip.x), static_cast<int>(tip.y),
			static_cast<int>(base_points[i].x), static_cast<int>(base_points[i].y),
			static_cast<int>(base_points[i + 1].x), static_cast<int>(base_points[i + 1].y),
			GetColor(255, 255, 0),
			TRUE
		);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Light::Finalize()
{
	__super::Finalize();
}

void Light::OnHitCollision(GameObject* hit_object)
{
	 // hit_object �� Player ���`�F�b�N
	Player* player = dynamic_cast<Player*>(hit_object);
	if (player && player->IsPlayerShadow())
	{
		// �e��Ԃ̃v���C���[�ɓ��������� Light ���폜
		if (object_manager)
		{
			object_manager->RequestDeleteObject(player);
		}
	}

}

bool Light::PointInTriangle(const Vector2D& p, const Vector2D& a, const Vector2D& b, const Vector2D& c)
{
	float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
	float alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
	float beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
	float gamma = 1.0f - alpha - beta;

	return alpha >= 0 && beta >= 0 && gamma >= 0;
}


bool Light::CheckLightCollision(GameObject* player)
{
	Vector2D player_pos = player->GetLocation();
	Vector2D player_size = player->GetBoxSize();

	// Player ��4��
	Vector2D corners[4] = {
		player_pos,
		{player_pos.x + player_size.x, player_pos.y},
		{player_pos.x, player_pos.y + player_size.y},
		{player_pos.x + player_size.x, player_pos.y + player_size.y}
	};

	// ���̓񓙕ӎO�p�`���_
	float rad = light_data.angle * DX_PI_F / 180.0f;
	float h = height;          // ����
	float b = width;           // ��Ӕ���

	Vector2D tip = location;
	Vector2D base_left = {
		tip.x + cos(rad) * h - sin(rad) * b,
		tip.y + sin(rad) * h + cos(rad) * b
	};
	Vector2D base_right = {
		tip.x + cos(rad) * h + sin(rad) * b,
		tip.y + sin(rad) * h - cos(rad) * b
	};

	// Player �̎l���̂ǂꂩ���O�p�`���ɂ��邩�`�F�b�N
	for (int i = 0; i < 4; ++i)
	{
		if (PointInTriangle(corners[i], tip, base_left, base_right))
		{
			return true;
		}
	}

	// �O�p�`�̒��_�� Player ��`���ɂ���ꍇ���Փ�
	Vector2D tri_vertices[3] = { tip, base_left, base_right };
	for (int i = 0; i < 3; ++i)
	{
		if (player_pos.x <= tri_vertices[i].x && tri_vertices[i].x <= player_pos.x + player_size.x &&
			player_pos.y <= tri_vertices[i].y && tri_vertices[i].y <= player_pos.y + player_size.y)
		{
			return true;
		}
	}

	return false;
}

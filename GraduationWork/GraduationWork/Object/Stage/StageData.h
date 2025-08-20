#pragma once
#include <vector>
#include <string>

class StageData
{
private:
	int width;       // �X�e�[�W�̕�
	int height;      // �X�e�[�W�̍���
	std::vector<std::vector<int>> data; // �X�e�[�W�f�[�^���i�[����2�����x�N�^�[

public:
	StageData(int _width, int _height);

	void Resize(int _width, int _height);  // �X�e�[�W�T�C�Y�ύX
	void Clear(int value = 0);  // ������

	int GetTile(int x, int y) const;
	void SetTile(int x, int y, int id);

	// �t�@�C�����o��
	bool LoadCSV(const std::string& filename);
	bool SaveCSV(const std::string& filename);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

};


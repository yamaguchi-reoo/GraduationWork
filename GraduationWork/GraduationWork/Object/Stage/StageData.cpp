#include "StageData.h"
#include <fstream>
#include <sstream>
#include <iostream>

StageData::StageData(int _width, int _height)
	: width(_width), height(_height)
{
	data.resize(height, std::vector<int>(width, 0));
}

void StageData::Resize(int _width, int _height)
{
	width = _width;
	height = _height;
	data.clear();
	data.resize(height, std::vector<int>(width, 0));
}

void StageData::Clear(int value)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			data[y][x] = value;
		}
	}
}

int StageData::GetTile(int x, int y) const
{
	if (x < 0 || x >= width || y < 0 || y >= height) return -1; // 範囲外
	return data[y][x];
}

void StageData::SetTile(int x, int y, int id)
{
	if (x < 0 || x >= width || y < 0 || y >= height) return; // 範囲外
	data[y][x] = id;
}

bool StageData::LoadCSV(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file) return false;

	std::string line;
	if (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string w, h;
		std::getline(ss, w, ',');
		std::getline(ss, h, ',');
		width = std::stoi(w);
		height = std::stoi(h);
		Resize(width, height);
	}

	for (int y = 0; y < height; ++y)
	{
		if (!std::getline(file, line)) break;
		std::stringstream ss(line);
		for (int x = 0; x < width; ++x)
		{
			std::string value;
			if (std::getline(ss, value, ','))
			{
				if (value.empty()) value = "0";  // 空文字を0に置換
				data[y][x] = std::stoi(value);
			}
			else
			{
				data[y][x] = 0; // 値が足りない場合は0
			}
		}
	}

	file.close();
	return true;
}

bool StageData::SaveCSV(const std::string& filename)
{
	std::ofstream file(filename);
	if (!file) return false;

	// 1行目に幅と高さ
	file << width << "," << height << "\n";

	// 2行目以降にタイルデータ
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			file << data[y][x];
			if (x < width - 1) file << ",";
		}
		file << "\n";
	}

	file.close();
	return true;
}

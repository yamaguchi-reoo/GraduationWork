#include "StageData.h"
#include <fstream>
#include <sstream>
#include <iostream>

StageData::StageData(int _width, int _height)
	: width(_width), height(_height)
{
	tile_layer.resize(height, std::vector<int>(width, 0));
	object_layer.resize(height, std::vector<int>(width, 0));
}

void StageData::Resize(int _width, int _height)
{
	width = _width;
	height = _height;
	tile_layer.resize(height);
	object_layer.resize(height);
	for (auto& row : tile_layer) row.resize(width, -1);
	for (auto& row : object_layer) row.resize(width, -1);
}

void StageData::Clear(int value)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tile_layer[y][x] = value;
			object_layer[y][x] = value;
		}
	}
}

int StageData::GetTile(int x, int y) const
{
	if (x < 0 || x >= width || y < 0 || y >= height) return -1; // �͈͊O
	return tile_layer[y][x];
}

void StageData::SetTile(int x, int y, int id)
{
	if (x < 0 || x >= width || y < 0 || y >= height) return; // �͈͊O
	tile_layer[y][x] = id;
}

int StageData::GetObj(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height) return -1;
	return object_layer[y][x];
}

void StageData::SetObj(int x, int y, int id)
{
	if (x < 0 || x >= width || y < 0 || y >= height) return;
	object_layer[y][x] = id;
}

bool StageData::LoadCSV(const std::string& filename)
{
	return LoadCSVInternal(filename, object_layer);
}

bool StageData::SaveCSV(const std::string& filename)
{
	return SaveCSVInternal(filename, object_layer);
}

bool StageData::LoadTileCSV(const std::string& filename)
{
	return LoadCSVInternal(filename, tile_layer);
}

bool StageData::SaveTileCSV(const std::string& filename)
{
	return SaveCSVInternal(filename, tile_layer);
}

bool StageData::LoadCSVInternal(const std::string& filename, std::vector<std::vector<int>>& layer)
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
				if (value.empty()) value = "-1";
				layer[y][x] = std::stoi(value);
			}
			else
			{
				layer[y][x] = -1;
			}
		}
	}

	file.close();
	return true;
}

bool StageData::SaveCSVInternal(const std::string& filename, const std::vector<std::vector<int>>& layer)
{
	std::ofstream file(filename);
	if (!file) return false;

	file << width << "," << height << "\n";

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			file << layer[y][x];
			if (x < width - 1) file << ",";
		}
		file << "\n";
	}

	file.close();
	return true;
}

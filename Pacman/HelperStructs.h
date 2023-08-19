#pragma once
#include <vector>

struct LevelData
{
	//int levelHeight;
	//int levelWidth;
	int tileSize;
	int nrRows;
	int nrCols;
	std::vector<int> levelLayout;
};
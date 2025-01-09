#include "DrawMap.h"

/// <summary>
/// ファイルを読み込む
/// </summary>
/// <param name="fName">ファイルのパス</param>
void DrawMap::LoadFile(const char* fName)
{
	// null を探す
	if (fName == nullptr)
	{
		return;
	}

	FILE* fp;

	fopen_s(&fp, fName, "r");

	if (fp == 0)
	{
		return;
	}
	else
	{
		// ファイルを開けたら、数値を入力する
		for (int row = 0; row < kMapRow; row++)
		{
			for (int column = 0; column < kMapColumn; column++)
			{
				fscanf_s(fp, "%d", &map_[row][column]);
			}
		}
	}

	fclose(fp);
}

/// <summary>
/// 描画する
/// </summary>
void DrawMap::Draw()
{

}
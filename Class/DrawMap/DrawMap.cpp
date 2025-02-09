#include "DrawMap.h"


int DrawMap::map_[kMapRow][kMapColumn];


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
	// マップの画像
	int ghMap[4];
	ghMap[0] = Novice::LoadTexture("./Resources/Images/Map/block1.png");
	ghMap[1] = Novice::LoadTexture("./Resources/Images/Map/block2.png");
	ghMap[2] = Novice::LoadTexture("./Resources/Images/Map/slope1.png");
	ghMap[3] = Novice::LoadTexture("./Resources/Images/Map/slope2.png");

	for (int row = 0; row < kMapRow; row++)
	{
		for (int column = 0; column < kMapColumn; column++)
		{
			if (map_[row][column] >= 0)
			{
				// タイルの切り替え
				switch (map_[row][column])
				{
				case TILE_DESIGN_CENTER_TOP:
					// 真上

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghMap[0], 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_CENTER_CENTER:
					// 真ん中

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghMap[1], 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_SLOPE_RIGHT_BOTTOM:
					// 右上がりの下

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghMap[2], 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_SLOPE_RIGHT_TOP:
					// 右上がりの上

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghMap[3], 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_SLOPE_LEFT_BOTTOM:
					// 左上がりの下

					Novice::DrawQuad
					(
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghMap[2], 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_SLOPE_LEFT_TOP:
					// 左上がりの上

					Novice::DrawQuad
					(
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghMap[3], 0xFFFFFFFF
					);

					break;
				}
			}
		}
	}
}
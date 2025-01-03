#include "Map.h"

/// <summary>
/// ファイルを読み込む
/// </summary>
/// <param name="fName"></param>
void Map::LoadFile(const char* fName)
{
	FILE* fp;

	fopen_s(&fp, fName, "r");

	// ファイルが開けなかったら処理を修了する
	if (fp == nullptr)
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
/// <param name="gh">画像</param>
void Map::Draw(int gh)
{
	for (int row = 0; row < kMapRow; row++)
	{
		for (int column = 0; column < kMapColumn; column++)
		{
			// タイルの切り替え
			switch (map_[row][column])
			{
			case TILE_GROUND:

				Novice::DrawQuad
				(
					column * kTileSize, row * kTileSize,
					column * kTileSize + kTileSize, row * kTileSize,
					column * kTileSize, row * kTileSize + kTileSize,
					column * kTileSize + kTileSize, row * kTileSize + kTileSize,
					0, 0, 1, 1, gh, 0xFFFFFFFF
				);

				break;

			case TILE_ROTTED:

				Novice::DrawQuad
				(
					column * kTileSize, row * kTileSize,
					column * kTileSize + kTileSize, row * kTileSize,
					column * kTileSize, row * kTileSize + kTileSize,
					column * kTileSize + kTileSize, row * kTileSize + kTileSize,
					0, 0, 1, 1, gh, 0x004400FF
				);

				break;
			}
		}
	}
}
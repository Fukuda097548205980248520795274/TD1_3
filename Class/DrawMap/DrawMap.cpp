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
	int ghGround = Novice::LoadTexture("./Resources/Images/Map/ground.png");

	for (int row = 0; row < kMapRow; row++)
	{
		for (int column = 0; column < kMapColumn; column++)
		{
			if (map_[row][column] >= 0)
			{
				// タイルの切り替え
				switch (map_[row][column])
				{
				case TILE_DESIGN_LEFT_TOP:
					// 左上

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_CENTER_TOP:
					// 真上

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						32, 0, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_RIGHT_TOP:
					// 右上

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						64, 0, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_LEFT_CENTER:
					// 左

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 32, 32, 32, ghGround, 0xFFFFFFFF
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
						32, 32, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_RIGHT_CENTER:
					// 右

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						64, 32, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_LEFT_BOTTOM:
					// 左下

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 64, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_CENTER_BOTTOM:
					// 真下

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						32, 64, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_RIGHT_BOTTOM:
					// 左下

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						64, 64, 32, 32, ghGround, 0xFFFFFFFF
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
						96, 0, 32, 32, ghGround, 0xFFFFFFFF
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
						128, 0, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_SLOPE_LEFT_BOTTOM:
					// 左上がりの下

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						96, 32, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;

				case TILE_DESIGN_SLOPE_LEFT_TOP:
					// 左上がりの上

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						128, 32, 32, 32, ghGround, 0xFFFFFFFF
					);

					break;
				}
			}
			else
			{
				// 腐りが進行しているブロック
				Novice::DrawQuad
				(
					column * kTileSize, row * kTileSize,
					column * kTileSize + kTileSize, row * kTileSize,
					column * kTileSize, row * kTileSize + kTileSize,
					column * kTileSize + kTileSize, row * kTileSize + kTileSize,
					0, 0, 1, 1, ghGround, 0x00AA00FF
				);
			}
		}
	}
}
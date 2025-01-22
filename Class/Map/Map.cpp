#include "Map.h"


// マップ
int Map::map_[kMapRow][kMapColumn];
int Map::treasureNum = 0;


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
/// 腐らせる
/// </summary>
void Map::Rotten()
{
	// ひびが入る効果音
	int shHole = Novice::LoadAudio("./Resources/Sounds/Se/iceHole.mp3");

	// 割れる効果音
	int shBreak = Novice::LoadAudio("./Resources/Sounds/Se/iceBreak.mp3");

	for (int row = 0; row < kMapRow; row++)
	{
		for (int column = 0; column < kMapColumn; column++)
		{
			// 腐っているブロックが、時間切れで消滅する
			if (map_[row][column] < 0)
			{
				map_[row][column]++;

				if (map_[row][column] >= 0)
				{
					Novice::PlayAudio(shBreak, 0, 0.5f);

					map_[row][column] = TILE_NOTHING;
				}

				// 効果音を流す
				if (map_[row][column] == static_cast<int>(static_cast<float>(kStartRotten) * (2.0f / 3.0f)))
				{
					Novice::PlayAudio(shHole, 0, 0.5f);
				}

				// 効果音を流す
				if (map_[row][column] == static_cast<int>(static_cast<float>(kStartRotten) * (1.0f / 3.0f)))
				{
					Novice::PlayAudio(shHole, 0, 0.5f);
				}
			}
		}
	}
}

/// <summary>
/// 描画する
/// </summary>
void Map::Draw()
{
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");

	int ghLadder = Novice::LoadTexture("./Resources/Images/Map/ladder.png");

	for (int row = 0; row < kMapRow; row++)
	{
		for (int column = 0; column < kMapColumn; column++)
		{
			if (map_[row][column] >= 0)
			{
				// タイルの切り替え
				switch (map_[row][column])
				{
				case TILE_GROUND:
					// 地上

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 1, 1, ghWhite, 0xFFFFFFFF
					);

					break;

				case TILE_SLOPE_RIGHT_BOTTOM:
					// 右上がりの下

					Novice::DrawLine
					(column * kTileSize, row * kTileSize + kTileSize, column * kTileSize + kTileSize, row * kTileSize + kTileSize / 2, 0xFFFFFFFF);

					break;

				case TILE_SLOPE_RIGHT_TOP:
					// 右上がりの上

					Novice::DrawLine
					(column * kTileSize, row * kTileSize + kTileSize / 2, column * kTileSize + kTileSize, row * kTileSize, 0xFFFFFFFF);

					break;

				case TILE_SLOPE_LEFT_BOTTOM:
					// 左下がりの下

					Novice::DrawLine
					(column * kTileSize, row * kTileSize + kTileSize / 2, column * kTileSize + kTileSize, row * kTileSize + kTileSize, 0xFFFFFFFF);

					break;

				case TILE_SLOPE_LEFT_TOP:
					// 左上がりの上

					Novice::DrawLine
					(column * kTileSize, row * kTileSize, column * kTileSize + kTileSize, row * kTileSize + kTileSize / 2, 0xFFFFFFFF);

					break;

				case TILE_BLOCK:
					// ブロック

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 1, 1, ghWhite, 0xAAFFAAFF
					);

					break;

				case TILE_ROTTED:
					// 腐ったブロック

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 1, 1, ghWhite, 0x004400FF
					);

					break;

				case TILE_GOAL:
					// ゴール

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 1, 1, ghWhite, 0xFF0000FF
					);

					break;

				case TILE_LADDER:
					// 梯子

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghLadder, 0xFFFFFFFF
					);

					break;

				case TILE_FIRE:
					// 炎

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0xFF5500AA
					);

					break;
				case TILE_DOOR1:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR2:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR3:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR4:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR5:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR6:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR7:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR8:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR9:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				case TILE_DOOR10:
					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x84331FFF
					);
					break;
				}
			} else
			{
				// 腐りが進行しているブロック
				Novice::DrawQuad
				(
					column * kTileSize, row * kTileSize,
					column * kTileSize + kTileSize, row * kTileSize,
					column * kTileSize, row * kTileSize + kTileSize,
					column * kTileSize + kTileSize, row * kTileSize + kTileSize,
					0, 0, 1, 1, ghWhite, 0x00AA00FF
				);
			}
		}
	}
}
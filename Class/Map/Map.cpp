#include "Map.h"


// マップ
int Map::map_[kMapRow][kMapColumn];
int Map::treasureNum = 0;
int Map::frame = 0;

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
/// 更新処理
/// </summary>
void Map::Update()
{
	Map::frame++;

	if (Map::frame >= 60)
	{
		Map::frame = 0;
	}
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
			// 腐っているブロックが、時間切れで消滅するa
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
	/*int ghMap = Novice::LoadTexture("./Resources/Images/Map/mapbgground.png");*/
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");

	int ghLadder = Novice::LoadTexture("./Resources/Images/Map/ladder.png");

	int ghGool[8];
	ghGool[0] = Novice::LoadTexture("./Resources/Images/Map/Gool/gool1.png");
	ghGool[1] = Novice::LoadTexture("./Resources/Images/Map/Gool/gool2.png");
	ghGool[2] = Novice::LoadTexture("./Resources/Images/Map/Gool/gool3.png");
	ghGool[3] = Novice::LoadTexture("./Resources/Images/Map/Gool/gool4.png");
	ghGool[4] = Novice::LoadTexture("./Resources/Images/Map/Gool/gool5.png");
	ghGool[5] = Novice::LoadTexture("./Resources/Images/Map/Gool/gool6.png");
	ghGool[6] = Novice::LoadTexture("./Resources/Images/Map/Gool/gool7.png");
	ghGool[7] = Novice::LoadTexture("./Resources/Images/Map/Gool/gool8.png");

	int ghIceBlock[5];
	ghIceBlock[0] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock.png");
	ghIceBlock[1] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock1.png");
	ghIceBlock[2] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock2.png");
	ghIceBlock[3] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock3.png");
	ghIceBlock[4] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock4.png");

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
						32, 0, 48, 48, ghWhite, 0xFFFFFFFF
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
						0, 0, 48, 48, ghIceBlock[1], 0xFFFFFFFF
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
						0, 0, 48, 48, ghIceBlock[3], 0xFFFFFFFF
					);

					break;

				case TILE_CONCRETE:
					// コンクリート

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghWhite, 0x666666FF
					);

					break;

				case TILE_GOAL:
					// ゴール

					if (Map::frame <= 7)
					{
						Novice::DrawQuad
						(
							column * kTileSize, row * kTileSize,
							column * kTileSize + kTileSize, row * kTileSize,
							column * kTileSize, row * kTileSize + kTileSize,
							column * kTileSize + kTileSize, row * kTileSize + kTileSize,
							0, 0, 48, 48, ghGool[0], 0xFFFFFFFF
						);
					}
					else if (Map::frame <= 15)
					{
						Novice::DrawQuad
						(
							column * kTileSize, row * kTileSize,
							column * kTileSize + kTileSize, row * kTileSize,
							column * kTileSize, row * kTileSize + kTileSize,
							column * kTileSize + kTileSize, row * kTileSize + kTileSize,
							0, 0, 48, 48, ghGool[1], 0xFFFFFFFF
						);
					}
					else if (Map::frame <= 22)
					{
						Novice::DrawQuad
						(
							column * kTileSize, row * kTileSize,
							column * kTileSize + kTileSize, row * kTileSize,
							column * kTileSize, row * kTileSize + kTileSize,
							column * kTileSize + kTileSize, row * kTileSize + kTileSize,
							0, 0, 48, 48, ghGool[2], 0xFFFFFFFF
						);
					}
					else if (Map::frame <= 30)
					{
						Novice::DrawQuad
						(
							column * kTileSize, row * kTileSize,
							column * kTileSize + kTileSize, row * kTileSize,
							column * kTileSize, row * kTileSize + kTileSize,
							column * kTileSize + kTileSize, row * kTileSize + kTileSize,
							0, 0, 48, 48, ghGool[3], 0xFFFFFFFF
						);
					}
					else if (Map::frame <= 37)
					{
						Novice::DrawQuad
						(
							column * kTileSize, row * kTileSize,
							column * kTileSize + kTileSize, row * kTileSize,
							column * kTileSize, row * kTileSize + kTileSize,
							column * kTileSize + kTileSize, row * kTileSize + kTileSize,
							0, 0, 48, 48, ghGool[4], 0xFFFFFFFF
						);
					}
					else if (Map::frame <= 45)
					{
						Novice::DrawQuad
						(
							column * kTileSize, row * kTileSize,
							column * kTileSize + kTileSize, row * kTileSize,
							column * kTileSize, row * kTileSize + kTileSize,
							column * kTileSize + kTileSize, row * kTileSize + kTileSize,
							0, 0, 48, 48, ghGool[5], 0xFFFFFFFF
						);
					}
					else if (Map::frame <= 52)
					{
						Novice::DrawQuad
						(
							column * kTileSize, row * kTileSize,
							column * kTileSize + kTileSize, row * kTileSize,
							column * kTileSize, row * kTileSize + kTileSize,
							column * kTileSize + kTileSize, row * kTileSize + kTileSize,
							0, 0, 48, 48, ghGool[6], 0xFFFFFFFF
						);
					}
					else if (Map::frame <= 60)
					{
						Novice::DrawQuad
						(
							column * kTileSize, row * kTileSize,
							column * kTileSize + kTileSize, row * kTileSize,
							column * kTileSize, row * kTileSize + kTileSize,
							column * kTileSize + kTileSize, row * kTileSize + kTileSize,
							0, 0, 48, 48, ghGool[7], 0xFFFFFFFF
						);
					}
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

				case TILE_WATER:

					Novice::DrawQuad
					(
						column * kTileSize, row * kTileSize,
						column * kTileSize + kTileSize, row * kTileSize,
						column * kTileSize, row * kTileSize + kTileSize,
						column * kTileSize + kTileSize, row * kTileSize + kTileSize,
						0, 0, 48, 48, ghIceBlock[0], 0xFFFFFFFF
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
						0, 0, 48, 48, ghWhite, 0xFFFFFFFF
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
					0, 0, 48, 48, ghIceBlock[1], 0xFFFFFFFF
				);

				// 効果音を流す
				if (map_[row][column] > static_cast<int>(static_cast<float>(kStartRotten) * (3.0f / 3.0f)))
				{
					Novice::DrawQuad
					(
						column* kTileSize, row* kTileSize,
						column* kTileSize + kTileSize, row* kTileSize,
						column* kTileSize, row* kTileSize + kTileSize,
						column* kTileSize + kTileSize, row* kTileSize + kTileSize,
						0, 0, 48, 48, ghIceBlock[2], 0xFFFFFFFF
					);
				}

				// 効果音を流す
				if (map_[row][column] > static_cast<int>(static_cast<float>(kStartRotten) * (2.0f / 3.0f)))
				{
					Novice::DrawQuad
					(
						column* kTileSize, row* kTileSize,
						column* kTileSize + kTileSize, row* kTileSize,
						column* kTileSize, row* kTileSize + kTileSize,
						column* kTileSize + kTileSize, row* kTileSize + kTileSize,
						0, 0, 48, 48, ghIceBlock[3], 0xFFFFFFFF
					);
				}

				// 効果音を流す
				if (map_[row][column] > static_cast<int>(static_cast<float>(kStartRotten) * (1.0f / 3.0f)))
				{
					Novice::DrawQuad
					(
						column* kTileSize, row* kTileSize,
						column* kTileSize + kTileSize, row* kTileSize,
						column* kTileSize, row* kTileSize + kTileSize,
						column* kTileSize + kTileSize, row* kTileSize + kTileSize,
						0, 0, 48, 48, ghIceBlock[4], 0xFFFFFFFF
					);
				}
			}
		}
	}
}
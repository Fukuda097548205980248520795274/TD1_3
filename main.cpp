#include <Novice.h>
#include "Constant.h"
#include "Enumeration.h"
#include "./Class/Map/Map.h"
#include "./Class/Object/Player/Player.h"
#include "./Class/Object/CarryBlock/Plastic/Plastic.h"
#include "./Class/Object/CarryBlock/Treasure/Treasure.h"

const char kWindowTitle[] = "LC1C_20_フクダソウワ_タイトル";

// マップ
TILE Map::map_[kMapRow][kMapColumn];

// プラスチック
int Plastic::countID;

// 宝
int Treasure::countID;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kScreenWidth, kScreenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	/*---------------
	    変数を作る
	---------------*/

	// マップ
	Map::LoadFile("./TextFiles/Stage/stage1.csv");

	// プレイヤー
	Player* player = new Player();

	// プラスチック
	Plastic* plastic[kBlockNum];
	for (int i = 0; i < kBlockNum; i++)
	{
		plastic[i] = new Plastic();
	}

	// 宝
	Treasure* treasure[kBlockNum];
	for (int i = 0; i < kBlockNum; i++)
	{
		treasure[i] = new Treasure();
	}

	plastic[0]->Putting(3, 3);
	treasure[0]->Putting(3, 6);

	// 画像
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// 操作する
		player->Operation(keys, preKeys);

		for (int i = 0; i < kBlockNum; i++)
		{
			player->BlockLanding(plastic[i]);
			player->BlockLanding(treasure[i]);
		}

		for (int i = 0; i < kBlockNum; i++)
		{
			plastic[i]->Move();
			treasure[i]->Move();
		}

		for (int i = 0; i < kBlockNum; i++)
		{
			for (int j = 0; j < kBlockNum; j++)
			{
				// 同じ種類のブロック
				if (i != j)
				{
					plastic[i]->BlockLanding(plastic[j]);
					treasure[i]->BlockLanding(treasure[j]);
					player->Carry(plastic[i] , plastic[j]);
					player->Carry(treasure[i], treasure[j]);
				}

				// 別種類のブロック
				plastic[i]->BlockLanding(treasure[j]);
				treasure[i]->BlockLanding(plastic[j]);
				player->Carry(plastic[i], treasure[j]);
				player->Carry(treasure[i], plastic[j]);

			}
		}



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// マップ
		Map::Draw(ghWhite);

		// プレイヤー
		player->Draw(ghWhite);

		// ブロック
		for (int i = 0; i < kBlockNum; i++)
		{
			plastic[i]->Draw(ghWhite);
			treasure[i]->Draw(ghWhite);
		}


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}


	/*--------------------------
	    インスタンスを削除する
	--------------------------*/

	// プレイヤー
	delete player;

	// ブロック
	for (int i = 0; i < kBlockNum; i++)
	{
		delete plastic[i];
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

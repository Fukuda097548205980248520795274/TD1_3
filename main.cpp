#include <Novice.h>
#include "Constant.h"
#include "Enumeration.h"
#include "./Class/Map/Map.h"
#include "./Class/Object/Player/Player.h"
#include "./Class/Object/CarryBlock/CarryBlock.h"

const char kWindowTitle[] = "LC1C_20_フクダソウワ_タイトル";

// マップ
TILE Map::map_[kMapRow][kMapColumn];

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

	// ブロック
	CarryBlock* block1 = new CarryBlock(300.0f , 100.0f);
	CarryBlock* block2 = new CarryBlock(320.0f, 300.0f);

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
		player->BlockLanding(block1);
		player->BlockLanding(block2);

		block1->Move();
		block2->Move();
		block1->BlockLanding(block2);
		block2->BlockLanding(block1);

		player->Carry(block1 , block2);
		player->Carry(block2, block1);

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
		block1->Draw(ghWhite);
		block2->Draw(ghWhite);


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
	delete block1;
	delete block2;


	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

#pragma once
#include <Novice.h>
#include <stdio.h>
#include "Constant.h"
#include "Enumeration.h"

class Map
{
public:

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	/// <param name="fName">ファイルのパス</param>
	static void LoadFile(const char* fName);

	/// <summary>
	/// 更新処理
	/// </summary>
	static void Update();

	/// <summary>
	/// 腐らせる
	/// </summary>
	static void Rotten();

	/// <summary>
	/// 描画する
	/// </summary>
	static void Draw();

	// マップ
	static int map_[kMapRow][kMapColumn];

	// 宝の数
	static int treasureNum;

	//フレーム
	static int frame;
};
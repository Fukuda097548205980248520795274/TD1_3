#pragma once

// タイルのサイズ
const int kTileSize = 48;

// マップ
const int kMapColumn = 28;
const int kMapRow = 16;

// 画面の幅
const int kScreenWidth = kMapColumn * kTileSize;
const int kScreenHeight = kMapRow * kTileSize;

// 重力
const float kGravity = -9.8f;

// 空気抵抗の係数
const float kAir = 0.4f;


/*   数   */

// 水の数
const int kWaterNum = 32;

// ブロックの数
const int kBlockNum = 32;

// 敵の数
const int kEnemyNum = 8;
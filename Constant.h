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

// 腐り始める時間
const int kStartRotten = -360;


/*   数   */

// ブロックの数
const int kBlockNum = 32;

// 敵の数
const int kEnemyNum = 8;

// 雪の数
const int kSnowNum = 300;

// 水の数
const int kParticleWater = 300;

// 蒸気の数
const int kParticleVapor = 128;

// 運ぶときの雪の数
const int kParticleSnowCarry = 64;

// 着地したときの粒子の数
const int kParticleLanding = 24;

// 輝く雪の数
const int kParticleSnowShining = 128;

// 切り替え用の雪の数
const int kParticleSnowSwitching = 128;

// 火の粉の数
const int kParticleHinoko = 32;

// 冷気の数
const int kParticleCold = 192;

// 欠片の数
const int kParticleDebris = 64;

// 画像の数
const int kTexutreNum = 10;
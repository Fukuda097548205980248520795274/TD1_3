#include "Object.h"

/// <summary>
/// マップを更新する
/// </summary>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
void Object::MapUpdate(float velX , float velY)
{
	map_.leftTop = { static_cast<int>(pos_.screen.leftTop.x + velX) / kTileSize ,static_cast<int>(pos_.screen.leftTop.y - velY) / kTileSize };
	map_.rightTop = { static_cast<int>(pos_.screen.rightTop.x + velX - 1.0f) / kTileSize ,static_cast<int>(pos_.screen.rightTop.y - velY) / kTileSize };
	map_.leftBottom = { static_cast<int>(pos_.screen.leftBottom.x + velX) / kTileSize ,static_cast<int>(pos_.screen.leftBottom.y - velY - 1.0f) / kTileSize };
	map_.rightBottom = { static_cast<int>(pos_.screen.rightBottom.x + velX - 1.0f) / kTileSize ,static_cast<int>(pos_.screen.rightBottom.y - velY - 1.0f) / kTileSize };
}

/// <summary>
/// ローカル座標をスクリーン座標に変換する
/// </summary>
void Object::LocalToScreen()
{
	// 全てを結合した行列
	struct Matrix3x3 wvpVpMatrix;

	wvpVpMatrix = Multiply(MakeWorldMatrix(shape_.scale, shape_.theta, shape_.translate),
		MakeInverseMatrix(MakeWorldMatrix({ 1.0f , 1.0f }, 0.0f, { 0.0f , 0.0f })));

	wvpVpMatrix = Multiply(wvpVpMatrix, MakeOrthoMatrix(0.0f, static_cast<float>(kScreenHeight), static_cast<float>(kScreenWidth), 0.0f));

	wvpVpMatrix = Multiply(wvpVpMatrix, MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kScreenWidth), static_cast<float>(kScreenHeight)));

	pos_.screen.leftTop = Transform(pos_.local.leftTop, wvpVpMatrix);
	pos_.screen.rightTop = Transform(pos_.local.rightTop, wvpVpMatrix);
	pos_.screen.leftBottom = Transform(pos_.local.leftBottom, wvpVpMatrix);
	pos_.screen.rightBottom = Transform(pos_.local.rightBottom, wvpVpMatrix);

	// 四捨五入を行う
	pos_.screen.leftTop.x = roundf(pos_.screen.leftTop.x);
	pos_.screen.leftTop.y = roundf(pos_.screen.leftTop.y);
	pos_.screen.rightTop.x = roundf(pos_.screen.rightTop.x);
	pos_.screen.rightTop.y = roundf(pos_.screen.rightTop.y);
	pos_.screen.leftBottom.x = roundf(pos_.screen.leftBottom.x);
	pos_.screen.leftBottom.y = roundf(pos_.screen.leftBottom.y);
	pos_.screen.rightBottom.x = roundf(pos_.screen.rightBottom.x);
	pos_.screen.rightBottom.y = roundf(pos_.screen.rightBottom.y);

	// マップを更新する
	MapUpdate(0.0f, 0.0f);
}

const int kParticleCount = 200; // パーティクル数を増加
Particles particles_[kParticleCount];


/// <summary>
/// パーティクルの初期化
/// </summary>
void InitializeParticles()
{
	for (int i = 0; i < kParticleCount; i++)
	{
		particles_[i].active = false;
	}
}

/// <summary>
/// パーティクルのアップデート
/// </summary>
void UpdateParticles()
{
	for (int i = 0; i < kParticleCount; i++)
	{
		if (particles_[i].active)
		{
			particles_[i].pos.x += particles_[i].velocity.x;
			particles_[i].pos.y += particles_[i].velocity.y;
			particles_[i].life += 0.1f;
			if (particles_[i].life >= particles_[i].maxLife)
			{
				particles_[i].active = false;
			}
		}
	}
}

/// <summary>
/// パーティクルの描画
/// </summary>
void DrawParticles()
{
	for (int i = 0; i < kParticleCount; i++)
	{
		if (particles_[i].active)
		{
			unsigned int alpha = static_cast<unsigned int>(255 * (1.0f - particles_[i].life / particles_[i].maxLife));
			unsigned int color = (particles_[i].color & 0xFFFFFF) | (alpha << 24);
			Novice::DrawBox(
				static_cast<int>(particles_[i].pos.x),
				static_cast<int>(particles_[i].pos.y),
				6, 6, 0.0f, color, kFillModeSolid // 大きさを変更して派手に
			);
		}
	}
}

/// <summary>
/// はじけ飛ぶパーティクル
/// </summary>
/// <param name="position">描画する座標</param>
void SpawnRandomParticles(Vec2 position)
{
	const int randomParticleCount = 20; // パーティクル数を増加
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < randomParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos = position;
			particles_[i].velocity.x = static_cast<float>(rand() % 200 - 100) / 40.0f; // スピードを強化
			particles_[i].velocity.y = static_cast<float>(rand() % 200 - 100) / 40.0f;
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 3.0f; // 持続時間を延長
			particles_[i].color = 0xFFFF00FF;
			spawned++;
		}
	}
}


/// <summary>
/// 真ん中から円状になるパーティクル
/// </summary>
/// <param name="position">描画する座標</param>
void SpawnSpiralParticles(Vec2 position)
{
	const int spiralParticleCount = 20; // パーティクル数を増加
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < spiralParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos = position;
			float angle = static_cast<float>(spawned) * (360.0f / spiralParticleCount) * 3.14159f / 180.0f;
			float speed = 3.0f; // スピードを強化
			particles_[i].velocity.x = cosf(angle) * speed;
			particles_[i].velocity.y = sinf(angle) * speed;
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 3.0f; // 持続時間を延長
			particles_[i].color = 0xFF00FFFF;
			spawned++;
		}
	}
}

/// <summary>
/// カラフルに描画するパーティクル
/// </summary>
/// <param name="position">描画の中心座標</param>
void SpawnColorfulParticles(Vec2 position)
{
	const int colorfulParticleCount = 20; // パーティクル数を増加
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < colorfulParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos = position;
			particles_[i].velocity.x = static_cast<float>(rand() % 200 - 100) / 40.0f; // スピードを強化
			particles_[i].velocity.y = static_cast<float>(rand() % 200 - 100) / 40.0f;
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 4.0f; // 持続時間を延長
			unsigned int randomColor = rand() % 0xFFFFFF;
			particles_[i].color = randomColor | 0xFF000000;  // Alpha set to fully opaque
			spawned++;
		}
	}
}

/// <summary>
/// 画面上にランダムで点を出す
/// </summary>
void SpawnFlashEffect()
{
	const int flashParticleCount = 40; // パーティクル数を増加
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < flashParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos.x = float(rand() % 1280);
			particles_[i].pos.y = float(rand() % 720);
			particles_[i].velocity = { 0.0f, 0.0f };
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 2.0f; // 持続時間を延長
			particles_[i].color = 0xFFFFFFFF;
			spawned++;
		}
	}
}

/// <summary>
/// 中心から大きな円を描画するパーティクル
/// </summary>
/// <param name="position">描画する座標</param>
void SpawnExplosionEffect(Vec2 position)
{
	const int explosionParticleCount = 50; // パーティクル数を増加
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < explosionParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos = position;
			float angle = static_cast<float>(rand() % 360);
			float speed = 6.0f; // スピードを強化
			particles_[i].velocity.x = cosf(angle) * speed;
			particles_[i].velocity.y = sinf(angle) * speed;
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 3.0f; // 持続時間を延長
			particles_[i].color = 0xFF0000FF;
			spawned++;
		}
	}
}


/// <summary>
/// 色を少しずつ変え花火みたいにするパーティクル
/// </summary>
/// <param name="position"></param>
void SpawnRainbowEffect(Vec2 position)
{
	const int rainbowParticleCount = 25;
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < rainbowParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos = position;
			float angle = static_cast<float>(rand() % 360);  // Random angle
			float speed = 2.5f;  // Moderate speed
			particles_[i].velocity.x = cosf(angle) * speed;
			particles_[i].velocity.y = sinf(angle) * speed;
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 4.0f;  // Longer duration
			unsigned int randomColor = (rand() % 6) * 0x333333 + 0xFF000000;  // Random rainbow color
			particles_[i].color = randomColor;
			spawned++;
		}
	}
}

/// <summary>
/// 風のように描画するパーティクル
/// </summary>
/// <param name="position"></param>
void SpawnWindParticles(Vec2 position)
{
	const int windParticleCount = 50;
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < windParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos = position;
			particles_[i].velocity.x = static_cast<float>(rand() % 30 - 15) / 5.0f; // 軽い風の動き
			particles_[i].velocity.y = static_cast<float>(rand() % 20 - 10) / 5.0f; // 少しランダム
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 4.0f; // 少し長め
			particles_[i].color = 0xC0C0C0FF; // 風のような色
			spawned++;
		}
	}
}

/// <summary>
/// 煙のように描画するパーティクル
/// </summary>
/// <param name="position">描画する座標</param>
void SpawnSmokeParticles(Vec2 position)
{
	const int smokeParticleCount = 40;
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < smokeParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos = position;
			particles_[i].velocity.x = static_cast<float>(rand() % 30 - 15) / 20.0f; // ゆっくりとランダムに
			particles_[i].velocity.y = -1.0f; // 上昇
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 5.0f;
			particles_[i].color = 0xA9A9A9FF; // 灰色の煙
			spawned++;
		}
	}
}

/// <summary>
/// 葉っぱをふらせるようなパーティクル
/// </summary>
/// <param name="position"></param>
void SpawnLeafParticles(Vec2 position)
{
	const int leafParticleCount = 30;
	int spawned = 0;

	for (int i = 0; i < kParticleCount && spawned < leafParticleCount; i++)
	{
		if (!particles_[i].active)
		{
			particles_[i].active = true;
			particles_[i].pos = position;
			particles_[i].velocity.x = static_cast<float>(rand() % 50 - 25) / 10.0f; // 少しゆっくり
			particles_[i].velocity.y = 1.0f; // ゆっくり落ちる
			particles_[i].life = 0.0f;
			particles_[i].maxLife = 4.0f;
			particles_[i].color = 0x228B22FF; // 緑色
			spawned++;
		}
	}
}

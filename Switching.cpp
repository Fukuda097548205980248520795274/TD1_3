#include"Switching.h"

Switching::Switching()
{
	isTrigger_ = false;
	isSizeUp_ = false;
	TrianglePos_ = { 200,384 };
	timer_ = 0.0f;
}

/// <summary>
/// 画面切り替えの作動
/// </summary>
void Switching::Trigger()
{
	if (isTrigger_)
	{
		return;
	}
	if (isSizeUp_)
	{
		return;
	}

	if (!isSizeUp_)
	{
		isSizeUp_ = true;
	}

	TrianglePos_ = { 100,100 };
}

/// <summary>
/// easeInOutQuartの関数
/// </summary>
/// <param name="a">スタート地点</param>
/// <param name="b">ゴール地点</param>
/// <param name="t">イージング関数</param>
/// <returns></returns>
Vector2 Switching::Easing(const Vector2 a, const Vector2 b, float t)
{
	// easeInOutCirc 補間
	t = 1.0f * t;  // `t` を倍にして補間を区切る

	if (t < 1.0f)
	{
		t = -0.2f * (sqrtf(1.0f - t * t) - 1.0f);  // 前半
	}
	else
	{
		t = 0.4f * (sqrtf(1.0f - (t -= 2.0f) * t) + 1.0f);  // 後半
	}

	// 補間後の座標を計算
	Vector2 result = {
		(1.0f - t) * a.x + t * b.x,
		(1.0f - t) * a.y + t * b.y
	};

	// 現在の位置をデバッグ描画
	Novice::DrawEllipse(
		static_cast<int>(result.x),
		static_cast<int>(result.y),
		30,
		30,
		0.0f,
		0xFFFFFFFF,
		kFillModeWireFrame
	);

	return result;
}

/// <summary>
/// 
/// </summary>
/// <param name="center">三角形の中心座標</param>
/// <param name="baseSize">三角形の初期サイズ</param>
/// <param name="t">時間の進行速度</param>
void Switching::DrawEasingTriangle(Vector2 center, float baseSize, float t)
{


	// easeInOutQuart を適用
	if (t < 0.5f)
	{
		t = 8.0f * t * t * t * t;
	}
	else
	{
		t = 1.0f - powf(-2.0f * t + 2.0f, 4.0f) / 2.0f;
	}

	// 三角形のサイズを拡大
	float size = baseSize * (1.0f + 10.0f * t); // `t` に応じて拡大（最大3倍）

	// 三角形の3点を計算（正三角形を前提）
	Vector2 aPos = { center.x, center.y - size };
	Vector2 bPos = { center.x - size * 0.866f, center.y + size * 0.5f }; // 左下
	Vector2 cPos = { center.x + size * 0.866f, center.y + size * 0.5f }; // 右下

	//// 三角形を描画
	//Novice::DrawTriangle(
	//	static_cast<int>(aPos.x), static_cast<int>(aPos.y),
	//	static_cast<int>(bPos.x), static_cast<int>(bPos.y),
	//	static_cast<int>(cPos.x), static_cast<int>(cPos.y),
	//	0xFFFFFFFF, kFillModeWireFrame
	//);

	Novice::DrawEllipse(
		static_cast<int>(center.x),
		static_cast<int>(center.y),
		static_cast<int>(size),
		static_cast<int>(size),
		0.0f,
		0xFFFFFFFF,
		kFillModeWireFrame
	);
}



///// <summary>
///// 
///// </summary>
///// <param name="aPos"></param>
///// <param name="size">底辺(x)と高さ(y)</param>
///// <param name="isFacingLeft">左向きかどうか</param>
//void Switching::TriangleDraw(Vector2 aPos, Vector2 size, bool isFacingLeft)
//{
//
//	if (isFacingLeft)
//	{
//		Novice::DrawTriangle(
//			static_cast<int>(aPos.x + (size.y * 0.5f)),
//			static_cast<int>(aPos.y - (size.x * 0.5f)),
//			static_cast<int>(aPos.x + (size.y * 0.5f)),
//			static_cast<int>(aPos.y + (size.x * 0.5f)),
//			static_cast<int>(aPos.x - (size.y * 0.5f)),
//			static_cast<int>(aPos.y),
//			0xFFFFFFFF,
//			kFillModeWireFrame
//		);
//	}
//
//	if (!isFacingLeft)
//	{
//		Novice::DrawTriangle(
//			static_cast<int>(aPos.x - (size.y * 0.5f)),
//			static_cast<int>(aPos.y - (size.x * 0.5f)),
//			static_cast<int>(aPos.x - (size.y * 0.5f)),
//			static_cast<int>(aPos.y + (size.x * 0.5f)),
//			static_cast<int>(aPos.x + (size.y * 0.5f)),
//			static_cast<int>(aPos.y),
//			0xFFFFFFFF,
//			kFillModeWireFrame
//		);
//	}
//
//}
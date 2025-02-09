#pragma once

// タイル
enum TILE
{
	TILE_NOTHING,//0・・・空気
	TILE_GROUND,//1・・・地面
	TILE_SLOPE_RIGHT_BOTTOM,//2・・・右坂①
	TILE_SLOPE_RIGHT_TOP,//3・・・右坂②
	TILE_SLOPE_LEFT_BOTTOM,//4・・・左坂①
	TILE_SLOPE_LEFT_TOP,//5・・・左坂②
	TILE_BLOCK,//6・・・敵が触れると溶け始めるブロック
	TILE_ROTTED,//7・・・既に溶けているブロック
	TILE_LADDER,//8・・・梯子
	TILE_FIRE,//9・・・炎
	TILE_GOAL,//10・・・ゴール
	TILE_PLAYER,//11・・・プレイヤー出現するポイント
	TILE_PLASTIC,//12・・・運べるブロック
	TILE_CUSHION,//13・・・クッションブロック
	TILE_TREASURE,//14・・・宝箱
	TILE_ICE_GHOST,//15・・・使用不可
	TILE_GHOST,//16・・・敵

	// 特殊
	TILE_SLOPE_SAFE,//17・・・坂の上にブロック
	TILE_BOMB,//18　爆弾
	TILE_CONCRETE,//19　コンクリート
	TILE_WATER,//20 水
};

// エリア
enum AREA
{
	AREA_1,
	AREA_2,
	AREA_3,
	AREA_4,
};

// ステージ
enum STAGE
{
	STAGE_1,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_6,
	STAGE_7,
	STAGE_8,
	STAGE_9,
	STAGE_10,
	STAGE_11,
	STAGE_12,
};

// タイルのデザイン
enum TILE_DESIGN
{
	//地面と坂のみ,それ以外は書かない
	TILE_DESIGN_CENTER_TOP = 1,//1・・・地面(雪あり)
	TILE_DESIGN_CENTER_CENTER,//2・・・地面(雪なし)
	TILE_DESIGN_SLOPE_RIGHT_BOTTOM,//3・・・右坂(登り始め)
	TILE_DESIGN_SLOPE_RIGHT_TOP,//4・・・右坂(登り終わり)
	TILE_DESIGN_SLOPE_LEFT_BOTTOM,//5・・・左坂(下り終わり)
	TILE_DESIGN_SLOPE_LEFT_TOP,//6・・・左坂(下り始め)
};

// ブロック
enum BLOCK
{
	BLOCK_PLASTIC,
	BLOCK_CUSHION,
	BLOCK_TREASURE,
	BLOCK_ICE_GHOST,
	BLOCK_BOMB,
};

// 方向
enum DIRECTION
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
};

// シーン
enum SCENE
{
	SCENE_START,
	SCENE_STAGE,
	SCENE_GAME,
};

// クリア
enum CLEAR
{
	CLEAR_NEXT_GAME,
	CLEAR_END_GAME,
};

// ゲームオーバー
enum GAMEOVER
{
	GAMEOVER_RESET_GAME,
	GAMEOVER_END_GAME,
};

// ポーズ
enum POSE
{
	POSE_GAME_RETURN,
	POSE_GAME_GIVEUP,
};
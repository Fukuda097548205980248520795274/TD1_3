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
	//ステージ切り替え
	TILE_STAGESERECT1,//17
	TILE_STAGESELECT2,//18
	TILE_STAGESELECT3,//19
	TILE_STAGESELECT4,//20
	TILE_STAGESELECT5,//21

	//ステージドア
	// ステージ1 北海道
	TILE_DOOR_1,//22
	TILE_DOOR_2,//23
	TILE_DOOR_3,//24
	TILE_DOOR_4,//25
	TILE_DOOR_5,//26
	TILE_DOOR_6,//27
	TILE_DOOR_7,//28
	TILE_DOOR_8,//29

	// 特殊
	TILE_BOMB,//30
	TILE_CONCRETE,//31
	TILE_WATER,//32
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
	TILE_DESIGN_LEFT_TOP = 1,//1
	TILE_DESIGN_CENTER_TOP,//2
	TILE_DESIGN_RIGHT_TOP,//3
	TILE_DESIGN_LEFT_CENTER,//4
	TILE_DESIGN_CENTER_CENTER,//5
	TILE_DESIGN_RIGHT_CENTER,//6
	TILE_DESIGN_LEFT_BOTTOM,//7
	TILE_DESIGN_CENTER_BOTTOM,//8
	TILE_DESIGN_RIGHT_BOTTOM,//9
	TILE_DESIGN_SLOPE_RIGHT_BOTTOM,//10
	TILE_DESIGN_SLOPE_RIGHT_TOP,//11
	TILE_DESIGN_SLOPE_LEFT_BOTTOM,//12
	TILE_DESIGN_SLOPE_LEFT_TOP,//13
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
#pragma once

// タイル
enum TILE
{
	TILE_NOTHING,//0
	TILE_GROUND,//1
	TILE_SLOPE_RIGHT_BOTTOM,//2
	TILE_SLOPE_RIGHT_TOP,//3
	TILE_SLOPE_LEFT_BOTTOM,//4
	TILE_SLOPE_LEFT_TOP,//5
	TILE_BLOCK,//6
	TILE_ROTTED,//7
	TILE_LADDER,//8
	TILE_FIRE,//9
	TILE_GOAL,//10
	TILE_PLAYER,//11
	TILE_PLASTIC,//12
	TILE_CUSHION,//13
	TILE_TREASURE,//14
	TILE_ICE_GHOST,//15
	TILE_GHOST,//16
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

enum SceneType
{
	TITLE,
	STAGE_SELECT,
	GAME,
	GAME_OVER,
	CLEAR,
};
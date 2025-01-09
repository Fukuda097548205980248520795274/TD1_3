#pragma once

// タイル
enum TILE
{
	TILE_NOTHING,
	TILE_GROUND,
	TILE_SLOPE_RIGHT_BOTTOM,
	TILE_SLOPE_RIGHT_TOP,
	TILE_SLOPE_LEFT_BOTTOM,
	TILE_SLOPE_LEFT_TOP,
	TILE_BLOCK,
	TILE_ROTTED,
	TILE_GOAL,
	TILE_PLAYER,
	TILE_PLASTIC,
	TILE_TREASURE,
	TILE_LADDER,
	TILE_ICE_GHOST,
	TILE_GHOST,
};

enum SceneType
{
	TITLE,
	STAGE_SELECT,
	GAME,
	GAME_OVER,
	CLEAR,
};
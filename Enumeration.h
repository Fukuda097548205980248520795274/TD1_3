#pragma once

// タイル
enum TILE
{
	TILE_NOTHING,
	TILE_GROUND,
	TILE_BLOCK,
	TILE_ROTTED,
	TILE_PLASTIC,
	TILE_TREASURE,
	TILE_LADDER,
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
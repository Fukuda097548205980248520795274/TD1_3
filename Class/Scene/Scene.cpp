#include "Scene.h"


SCENE Scene::sceneNo_ = SCENE_START;
int Scene::areaNo_ = AREA_1;
int Scene::stageNo_ = STAGE_1;

CLEAR Scene::clearNo_ = CLEAR_NEXT_GAME;
GAMEOVER Scene::gameoverNo_ = GAMEOVER_RESET_GAME;

int Scene::isClear_ = false;
int Scene::isGameOver_ = false;

int Scene::isPutPreparation_ = false;
#include "Scene.h"


SCENE Scene::sceneNo_ = SCENE_START;
AREA Scene::areaNo_ = AREA_STAR_LAND;

int Scene::isClear_ = false;
int Scene::isGameOver_ = false;

int Scene::selectStage_ = 0;
int Scene::isPutPreparation_ = false;
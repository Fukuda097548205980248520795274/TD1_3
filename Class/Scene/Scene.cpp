#include "Scene.h"


SCENE Scene::sceneNo_ = SCENE_START;
AREA Scene::areaNo_ = AREA_1;
STAGE Scene::stageNo_ = STAGE_1;

int Scene::isClear_ = false;
int Scene::isGameOver_ = false;

int Scene::isPutPreparation_ = false;
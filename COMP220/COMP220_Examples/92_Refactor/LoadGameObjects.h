#pragma once

#include "gameObject.h"
#include "textureLoader.h"
#include "MeshLoader.h"

std::vector<GameObject*>& loadGameObjects(TextureLoader* textureLoader, MeshLoader* meshLoader);

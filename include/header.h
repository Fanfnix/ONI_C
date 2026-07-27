#ifndef HEADER
#define HEADER

#define BITSET_SIZE (TAG_MAX_COUNT / 32)

#define TRANSPARENT_COLOR {255, 0, 255, 255}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <omp.h>

#include "utils/mass.h"
#include "utils/temperature.h"
#include "utils/tag.h"

#include "elements/element.h"
#include "elements/elements_generated.h"

#include "item.h"

#include "map/tile.h"
#include "map/mapGeneration.h"
#include "map/map.h"

#include "gameWindow/gameWindow.h"
#include "gameWindow/events.h"

#include "elements/elementColorMapping.h"
#include "elements/elementTextureMapping.h"

#endif
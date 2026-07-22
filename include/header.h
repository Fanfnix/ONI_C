#ifndef HEADER
#define HEADER

#define BITSET_SIZE (TAG_MAX_COUNT / 32)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <omp.h>

#include "cJSON.h"

#include "mass.h"
#include "temperature.h"
#include "tag.h"

#include "element.h"
#include "item.h"
#include "tile.h"
#include "map.h"

#include "gameWindow.h"
#include "events.h"

#include "elementColorMapping.h"
#include "elementTextureMapping.h"

#endif
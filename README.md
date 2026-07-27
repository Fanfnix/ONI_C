# Oxygen Not Included — Clone (C / SDL2)

Un petit moteur de simulation 2D inspiré d'_Oxygen Not Included_, écrit en C avec SDL2. Le projet gère une carte de tuiles peuplées d'éléments physiques (solides, liquides, gaz) possédant leurs propres propriétés thermiques, de masse et de tags, ainsi qu'une fenêtre de jeu avec caméra, zoom et gestion du plein écran.

## Fonctionnalités

- **Système d'éléments** généré automatiquement (`elements_generated.h/.c`) à partir de fichiers `.json` décrivant plus de 140 éléments (métaux, roches, liquides, gaz, etc.), chacun avec :
  - propriétés physiques communes (capacité thermique, conductivité, masse, température) ;
  - propriétés spécifiques à l'état (solide / liquide / gaz) via une `union` ;
  - transitions de phase (basse/haute température) et sublimation ;
  - un système de **tags** (`ElementTag`) en bitset pour classer les éléments (Ore, Metal, Combustible, etc.).
- **Carte (`Map`)** de taille `256 x 385` tuiles, chaque tuile pouvant contenir un `Item` (élément + masse + température).
- **Rendu** : damier de fond, textures colorées par élément (`SDL_SetTextureColorMod`), gestion du zoom et de la caméra.
- **Fenêtre de jeu** : redimensionnement, bascule plein écran (F11), restauration de position/taille.
- **Génération procédurale** basique de tuiles aléatoires (`mapGeneration.c`) à partir d'une graine (splitmix64).

## Architecture du projet

```
include/
  elements/        # Element, ElementId (généré), mapping couleurs/textures
  gameWindow/       # GameWindow, gestion des événements
  map/              # Map, Tile, génération de carte
  utils/            # Mass, Temperature, Tag (unités & conversions)
  item.h            # Item (élément + masse + température)
  header.h          # Header unique regroupant tous les includes

src/
  elements/         # Implémentation element.c, mapping couleurs/textures, données générées
  gameWindow/       # Fenêtre SDL, boucle d'événements, déplacement caméra
  map/              # Map, tuiles, génération
  utils/            # Conversions de masse/température, tags
  item.c
  main.c            # Point d'entrée / boucle principale
```

Tous les fichiers sources incluent un unique header central `header.h`, qui regroupe SDL2, OpenMP et l'ensemble des headers du projet (utils, elements, item, map, gameWindow).

## Prérequis

- Un compilateur C (gcc ou clang)
- [SDL2](https://www.libsdl.org/) (`libsdl2-dev`)
- OpenMP (généralement inclus avec gcc/clang)
- Les fichiers de textures (`.bmp`) et l'icône attendus dans `./data/tex/` et `./data/img/logo.bmp`

Sur Debian/Ubuntu :

```bash
sudo apt install libsdl2-dev gcc
```

## Compilation

Exemple de compilation manuelle (à adapter selon l'organisation réelle du Makefile/CMake du dépôt) :

```bash
gcc -fopenmp -Iinclude $(find src -name "*.c") -o oni $(sdl2-config --cflags --libs)
```

## Exécution

```bash
./oni
```

## Contrôles

| Touche / action       | Effet                     |
| --------------------- | ------------------------- |
| `Z` / `Q` / `S` / `D` | Déplacer la caméra (WASD) |
| Molette souris        | Zoomer / dézoomer         |
| `F11`                 | Basculer en plein écran   |
| `Échap`               | Quitter le jeu            |

## Génération des éléments

Les fichiers `elements_generated.h` et `elements_generated.c` sont **générés automatiquement** par un script `generate_elements.py` à partir de définitions `.json`. Ils ne doivent **pas être édités à la main** : toute modification doit passer par les fichiers JSON source puis une régénération.
Pour des raisons de license du jeu **Oxygen Not Included** et conformémént aux conditions d'utilisations établies par la société **Klei Entertainement**, les fichiers `.json` dérivés des `.yaml` du jeu **Oxygen Not Included** ainsi que le script `generate_elements.py` ne sont pas disponibles. Vous avez cependant le fichier `elements_generated.c`. Modification à vos risques et périls...

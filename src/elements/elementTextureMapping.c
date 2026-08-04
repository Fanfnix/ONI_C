#include "header.h"

SDL_Texture *ELEMENT_TEXTURE_TABLE[ELEMENT_ID_COUNT];

const char *ELEMENT_TEXTURE_FILES[ELEMENT_ID_COUNT] = {
    [ELEMENT_AEROGEL]            = NULL,
    [ELEMENT_ALGAE]              = "algae.bmp",
    [ELEMENT_ALUMINUM]           = "aluminum.bmp",
    [ELEMENT_ALUMINUMORE]        = "aluminum_ore.bmp",
    [ELEMENT_BITUMEN]            = NULL,
    [ELEMENT_BLEACHSTONE]        = "bleach_stone.bmp",
    [ELEMENT_BRICK]              = NULL,
    [ELEMENT_BRINEICE]           = "frozen_brine.bmp",
    [ELEMENT_MURKYBRINEICE]      = NULL,
    [ELEMENT_CARBON]             = "carbon.bmp",
    [ELEMENT_PEAT]               = "peat.bmp",
    [ELEMENT_CARBONFIBRE]        = NULL,
    [ELEMENT_CEMENT]             = NULL,
    [ELEMENT_CEMENTMIX]          = NULL,
    [ELEMENT_CERAMIC]            = "ceramic.bmp",
    [ELEMENT_CLAY]               = "clay.bmp",
    [ELEMENT_COPPER]             = "copper.bmp",
    [ELEMENT_CREATURE]           = NULL,
    [ELEMENT_CRUSHEDICE]         = "crushed_ice.bmp",
    [ELEMENT_CRUSHEDROCK]        = "crushed_rock.bmp",
    [ELEMENT_CUPRITE]            = "cuprite.bmp",
    [ELEMENT_DIAMOND]            = "diamond.bmp",
    [ELEMENT_DIRT]               = "dirt.bmp",
    [ELEMENT_DIRTYICE]           = "dirty_ice.bmp",
    [ELEMENT_FERTILIZER]         = "fertilizer.bmp",
    [ELEMENT_FOOLSGOLD]          = "fools_gold.bmp",
    [ELEMENT_FOSSIL]             = "fossil.bmp",
    [ELEMENT_FULLERENE]          = "fullerene.bmp",
    [ELEMENT_GLASS]              = NULL,
    [ELEMENT_GOLD]               = "gold.bmp",
    [ELEMENT_GOLDAMALGAM]        = "gold_amalgam.bmp",
    [ELEMENT_GRANITE]            = "granite.bmp",
    [ELEMENT_ICE]                = "ice.bmp",
    [ELEMENT_IGNEOUSROCK]        = "igneous_rock.bmp",
    [ELEMENT_IRON]               = "iron.bmp",
    [ELEMENT_IRONORE]            = "hematite.bmp",
    [ELEMENT_LEAD]               = "lead.bmp",
    [ELEMENT_LIME]               = "limestone.bmp",
    [ELEMENT_MAFICROCK]          = "mafic_rock.bmp",
    [ELEMENT_NIOBIUM]            = "niobium.bmp",
    [ELEMENT_CORIUM]             = NULL,
    [ELEMENT_OBSIDIAN]           = "obsidian.bmp",
    [ELEMENT_OXYROCK]            = "oxyrocks.bmp",
    [ELEMENT_PHOSPHATENODULES]   = "phosphate_nodules.bmp",
    [ELEMENT_PHOSPHORITE]        = "phosphorite.bmp",
    [ELEMENT_PHOSPHORUS]         = "phosphorus.bmp",
    [ELEMENT_REFINEDCARBON]      = NULL,
    [ELEMENT_REGOLITH]           = "regolith.bmp",
    [ELEMENT_RUST]               = NULL,
    [ELEMENT_SALT]               = "salt.bmp",
    [ELEMENT_SAND]               = "sand.bmp",
    [ELEMENT_SANDCEMENT]         = NULL,
    [ELEMENT_SANDSTONE]          = "sand_stone.bmp",
    [ELEMENT_SEDIMENTARYROCK]    = "sedimentary_rock.bmp",
    [ELEMENT_SHALE]              = "shale.bmp",
    [ELEMENT_SLABS]              = NULL,
    [ELEMENT_SLIMEMOLD]          = "slime_mold.bmp",
    [ELEMENT_SNOW]               = NULL,
    [ELEMENT_STABLESNOW]         = NULL,
    [ELEMENT_SOLIDCARBONDIOXIDE] = "solid_carbon_dioxide.bmp",
    [ELEMENT_SOLIDCHLORINE]      = "solid_chlorine.bmp",
    [ELEMENT_SOLIDCRUDEOIL]      = "frozen_crude_oil.bmp",
    [ELEMENT_SOLIDHYDROGEN]      = "solid_hydrogen.bmp",
    [ELEMENT_SOLIDMERCURY]       = "solid_mercury.bmp",
    [ELEMENT_SOLIDMETHANE]       = "solid_methane.bmp",
    [ELEMENT_SOLIDNAPHTHA]       = NULL,
    [ELEMENT_SOLIDOXYGEN]        = "solid_oxygen.bmp",
    [ELEMENT_SOLIDPETROLEUM]     = NULL,
    [ELEMENT_SOLIDPROPANE]       = "solid_propane.bmp",
    [ELEMENT_SOLIDRESIN]         = "resin_solid.bmp",
    [ELEMENT_SOLIDSUPERCOOLANT]  = "super_coolant.bmp",
    [ELEMENT_SOLIDVISCOGEL]      = "viscogel.bmp",
    [ELEMENT_STEEL]              = "steel.bmp",
    [ELEMENT_SULFUR]             = "sulfur.bmp",
    [ELEMENT_SUPERINSULATOR]     = "super_insulator.bmp",
    [ELEMENT_TEMPCONDUCTORSOLID] = "temp_conductor.bmp",
    [ELEMENT_TOXICSAND]          = "polluted_dirt.bmp",
    [ELEMENT_TUNGSTEN]           = "tungsten.bmp",
    [ELEMENT_UNOBTANIUM]         = "neutronium.bmp",
    [ELEMENT_WOLFRAMITE]         = "wolframite.bmp",
    [ELEMENT_BRINE]              = NULL,
    [ELEMENT_CHLORINE]           = NULL,
    [ELEMENT_CRUDEOIL]           = NULL,
    [ELEMENT_DIRTYWATER]         = NULL,
    [ELEMENT_LIQUIDCARBONDIOXIDE]= NULL,
    [ELEMENT_LIQUIDHELIUM]       = NULL,
    [ELEMENT_LIQUIDHYDROGEN]     = NULL,
    [ELEMENT_LIQUIDMETHANE]      = NULL,
    [ELEMENT_LIQUIDOXYGEN]       = NULL,
    [ELEMENT_LIQUIDPHOSPHORUS]   = NULL,
    [ELEMENT_LIQUIDPROPANE]      = NULL,
    [ELEMENT_LIQUIDSULFUR]       = NULL,
    [ELEMENT_MAGMA]              = NULL,
    [ELEMENT_MERCURY]            = NULL,
    [ELEMENT_MOLTENALUMINUM]     = NULL,
    [ELEMENT_MOLTENCARBON]       = NULL,
    [ELEMENT_MOLTENCOPPER]       = NULL,
    [ELEMENT_MOLTENGLASS]        = NULL,
    [ELEMENT_MOLTENGOLD]         = NULL,
    [ELEMENT_MOLTENIRON]         = NULL,
    [ELEMENT_MOLTENCOBALT]       = NULL,
    [ELEMENT_MOLTENLEAD]         = NULL,
    [ELEMENT_MOLTENNIOBIUM]      = NULL,
    [ELEMENT_MOLTENSALT]         = NULL,
    [ELEMENT_MOLTENSTEEL]        = NULL,
    [ELEMENT_MOLTENTUNGSTEN]     = NULL,
    [ELEMENT_NAPHTHA]            = NULL,
    [ELEMENT_PETROLEUM]          = NULL,
    [ELEMENT_RESIN]              = NULL,
    [ELEMENT_SALTWATER]          = NULL,
    [ELEMENT_SUPERCOOLANT]       = NULL,
    [ELEMENT_VISCOGEL]           = NULL,
    [ELEMENT_WATER]              = NULL,
    [ELEMENT_ETHANOL]            = NULL,
    [ELEMENT_PHYTOOIL]           = NULL,
    [ELEMENT_ALUMINUMGAS]        = NULL,
    [ELEMENT_CARBONDIOXIDE]      = NULL,
    [ELEMENT_CARBONGAS]          = NULL,
    [ELEMENT_CHLORINEGAS]        = NULL,
    [ELEMENT_CONTAMINATEDOXYGEN] = NULL,
    [ELEMENT_COPPERGAS]          = NULL,
    [ELEMENT_GOLDGAS]            = NULL,
    [ELEMENT_HYDROGEN]           = NULL,
    [ELEMENT_IRONGAS]            = NULL,
    [ELEMENT_COBALTGAS]          = NULL,
    [ELEMENT_LEADGAS]            = NULL,
    [ELEMENT_MERCURYGAS]         = NULL,
    [ELEMENT_METHANE]            = NULL,
    [ELEMENT_NIOBIUMGAS]         = NULL,
    [ELEMENT_OXYGEN]             = NULL,
    [ELEMENT_PHOSPHORUSGAS]      = NULL,
    [ELEMENT_PROPANE]            = NULL,
    [ELEMENT_ROCKGAS]            = NULL,
    [ELEMENT_SALTGAS]            = NULL,
    [ELEMENT_SOURGAS]            = NULL,
    [ELEMENT_STEAM]              = NULL,
    [ELEMENT_STEELGAS]           = NULL,
    [ELEMENT_SULFURGAS]          = NULL,
    [ELEMENT_SUPERCOOLANTGAS]    = NULL,
    [ELEMENT_TUNGSTENGAS]        = NULL,
    [ELEMENT_ETHANOLGAS]         = NULL,
    [ELEMENT_VACUUM]             = NULL,
    [ELEMENT_VOID]               = NULL,
    [ELEMENT_COMPOSITION]        = NULL,
};


SDL_Texture *element_get_texture(ElementId element_index) {
    if (element_index < 0 || element_index >= ELEMENT_ID_COUNT) {
        printf("ERROR (element_get_texture) : Index out of range : %d\n", element_index);
        return ELEMENT_TEXTURE_TABLE[ELEMENT_ID_COUNT];
    }
    return ELEMENT_TEXTURE_TABLE[element_index];
}


void element_texture_init(GameWindow *game) {
    char tex_path[100];
    SDL_Surface *surf;

    for (int i = 0; i < ELEMENT_ID_COUNT; i++) {
        if (ELEMENT_TEXTURE_FILES[i] != NULL) {
            snprintf(tex_path, sizeof(tex_path), "%s%s", ELEMENT_TEX_BASE_PATH, ELEMENT_TEXTURE_FILES[i]);
        } else {
            snprintf(tex_path, sizeof(tex_path), "%sbase.bmp", ELEMENT_TEX_BASE_PATH);
        }
        surf = SDL_LoadBMP(tex_path);
        if (surf != NULL) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(game->renderer, surf);
            SDL_FreeSurface(surf);
            ELEMENT_TEXTURE_TABLE[i] = tex;
        }
    }
}


void element_texture_destroy() {
    for (int i = 0; i < ELEMENT_ID_COUNT; i++) {
        if (ELEMENT_TEXTURE_TABLE[i] != NULL) SDL_DestroyTexture(ELEMENT_TEXTURE_TABLE[i]);
    }
}
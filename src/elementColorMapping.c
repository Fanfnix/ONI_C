#include "header.h"


static const ElementColorMapping ELEMENT_COLOR_TABLE[] = {
    {"Aerogel",             {230, 240, 245, 255}}, /* bleu très clair translucide */
    {"Algae",               {60,  140, 60,  255}}, /* vert algue */
    {"Aluminum",            {211, 214, 218, 255}}, /* argenté */
    {"AluminumOre",         {160, 160, 165, 255}}, /* gris minerai */
    {"Bitumen",             {40,  30,  25,  255}}, /* brun-noir bitume */
    {"BleachStone",         {230, 225, 190, 255}}, /* blanc-jaune pâle */
    {"Brick",               {178, 90,  60,  255}}, /* rouge brique */
    {"BrineIce",            {200, 220, 225, 255}}, /* bleu glace saumâtre */
    {"MurkyBrineIce",       {150, 165, 160, 255}}, /* bleu-gris trouble */
    {"Carbon",              {30,  30,  30,  255}}, /* noir carbone */
    {"Peat",                {70,  50,  35,  255}}, /* brun tourbe */
    {"CarbonFibre",         {50,  50,  55,  255}}, /* gris fibre de carbone */
    {"Cement",              {180, 178, 170, 255}}, /* gris ciment */
    {"CementMix",           {150, 148, 140, 255}}, /* gris ciment humide */
    {"Ceramic",             {200, 165, 130, 255}}, /* terre cuite claire */
    {"Clay",                {170, 120, 90,  255}}, /* ocre argile */
    {"Copper",              {184, 115, 51,  255}}, /* cuivre */
    {"Creature",            {200, 150, 140, 255}}, /* chair générique */
    {"CrushedIce",          {210, 230, 235, 255}}, /* blanc-bleu glace pilée */
    {"CrushedRock",         {130, 125, 120, 255}}, /* gris gravier */
    {"Cuprite",             {150, 70,  50,  255}}, /* rouge cuprite */
    {"Diamond",             {240, 248, 255, 255}}, /* blanc diamant */
    {"Dirt",                {110, 80,  55,  255}}, /* brun terre */
    {"DirtyIce",            {180, 190, 190, 255}}, /* gris-bleu glace sale */
    {"Fertilizer",          {90,  80,  40,  255}}, /* brun-vert engrais */
    {"FoolsGold",           {200, 170, 60,  255}}, /* jaune laiton pyrite */
    {"Fossil",              {140, 110, 85,  255}}, /* brun os */
    {"Fullerene",           {60,  60,  70,  255}}, /* gris foncé fullerène */
    {"Glass",               {200, 230, 230, 255}}, /* verre translucide */
    {"Gold",                {255, 215, 0,   255}}, /* or */
    {"GoldAmalgam",         {190, 175, 120, 255}}, /* or terne amalgame */
    {"Granite",             {193, 154, 150, 255}}, /* rose-gris */
    {"Ice",                 {190, 220, 230, 255}}, /* bleu glace */
    {"IgneousRock",         {105, 100, 95,  255}}, /* gris volcanique */
    {"Iron",                {160, 160, 160, 255}}, /* gris fer */
    {"IronOre",             {140, 90,  70,  255}}, /* brun minerai de fer */
    {"Lead",                {90,  90,  95,  255}}, /* gris plomb */
    {"Lime",                {235, 235, 225, 255}}, /* blanc chaux */
    {"MaficRock",           {70,  65,  75,  255}}, /* gris-violet sombre */
    {"Niobium",             {170, 180, 190, 255}}, /* gris-bleu niobium */
    {"Corium",              {80,  60,  50,  255}}, /* brun-gris corium */
    {"Obsidian",            {35,  30,  40,  255}}, /* noir-violet */
    {"OxyRock",             {180, 210, 190, 255}}, /* vert-bleu pâle */
    {"PhosphateNodules",    {190, 170, 130, 255}}, /* beige nodules */
    {"Phosphorite",         {170, 150, 100, 255}}, /* brun-jaune phosphorite */
    {"Phosphorus",          {220, 210, 150, 255}}, /* jaune pâle */
    {"RefinedCarbon",       {45,  45,  45,  255}}, /* gris foncé */
    {"Regolith",            {155, 150, 140, 255}}, /* gris régolithe */
    {"Rust",                {150, 80,  50,  255}}, /* brun-orange rouille */
    {"Salt",                {245, 245, 240, 255}}, /* blanc sel */
    {"Sand",                {225, 205, 155, 255}}, /* beige sable */
    {"SandCement",          {195, 180, 155, 255}}, /* beige-gris */
    {"SandStone",           {224, 196, 145, 255}}, /* sable/beige */
    {"SedimentaryRock",     {165, 145, 120, 255}}, /* brun-gris */
    {"Shale",               {90,  90,  100, 255}}, /* gris-bleu schiste */
    {"Slabs",               {170, 165, 160, 255}}, /* gris dalle */
    {"SlimeMold",           {110, 140, 60,  255}}, /* vert moisi */
    {"Snow",                {250, 250, 255, 255}}, /* blanc neige */
    {"StableSnow",          {235, 240, 245, 255}}, /* blanc-bleu */
    {"SolidCarbonDioxide",  {225, 240, 245, 255}}, /* glace carbonique */
    {"SolidChlorine",       {200, 225, 180, 255}}, /* vert pâle */
    {"SolidCrudeOil",       {35,  25,  20,  255}}, /* noir-brun */
    {"SolidHydrogen",       {225, 235, 245, 255}}, /* bleu très pâle */
    {"SolidMercury",        {200, 200, 205, 255}}, /* argenté */
    {"SolidMethane",        {215, 230, 235, 255}}, /* blanc-bleu */
    {"SolidNaphtha",        {180, 140, 70,  255}}, /* ambre */
    {"SolidOxygen",         {200, 220, 240, 255}}, /* bleu pâle */
    {"SolidPetroleum",      {60,  45,  30,  255}}, /* brun foncé */
    {"SolidPropane",        {225, 220, 180, 255}}, /* jaune pâle */
    {"SolidResin",          {170, 110, 40,  255}}, /* ambre résine */
    {"SolidSuperCoolant",   {130, 220, 220, 255}}, /* cyan */
    {"SolidViscoGel",       {190, 140, 190, 255}}, /* rose-violet */
    {"Steel",               {150, 155, 160, 255}}, /* gris-bleu acier */
    {"Sulfur",              {220, 200, 50,  255}}, /* jaune soufre */
    {"SuperInsulator",      {90,  80,  100, 255}}, /* gris-violet */
    {"TempConductorSolid",  {200, 140, 90,  255}}, /* orange conducteur */
    {"ToxicSand",           {170, 180, 90,  255}}, /* vert-jaune */
    {"Tungsten",            {110, 110, 115, 255}}, /* gris tungstène */
    {"Unobtanium",          {150, 80,  200, 255}}, /* violet exotique */
    {"Wolframite",          {70,  55,  45,  255}}, /* brun foncé */

    {"Brine",               {130, 160, 150, 255}}, /* gris-vert saumure */
    {"Chlorine",            {200, 220, 120, 255}}, /* jaune-vert */
    {"CrudeOil",            {20,  15,  10,  255}}, /* noir pétrole */
    {"DirtyWater",          {110, 120, 90,  255}}, /* brun-vert */
    {"LiquidCarbonDioxide", {190, 220, 230, 255}}, /* bleu pâle */
    {"LiquidHelium",        {210, 230, 240, 255}}, /* bleu très pâle */
    {"LiquidHydrogen",      {200, 220, 240, 255}}, /* bleu pâle */
    {"LiquidMethane",       {180, 220, 220, 255}}, /* cyan pâle */
    {"LiquidOxygen",        {150, 190, 230, 255}}, /* bleu clair */
    {"LiquidPhosphorus",    {210, 200, 140, 255}}, /* jaune pâle */
    {"LiquidPropane",       {220, 210, 160, 255}}, /* ambre pâle */
    {"LiquidSulfur",        {180, 160, 40,  255}}, /* jaune foncé */
    {"Magma",               {230, 90,  30,  255}}, /* orange-rouge */
    {"Mercury",             {195, 195, 200, 255}}, /* argenté */
    {"MoltenAluminum",      {220, 200, 190, 255}}, /* argenté-orange */
    {"MoltenCarbon",        {90,  40,  30,  255}}, /* rouge-noir */
    {"MoltenCopper",        {200, 100, 50,  255}}, /* orange-rouge */
    {"MoltenGlass",         {220, 150, 100, 255}}, /* orange translucide */
    {"MoltenGold",          {255, 190, 60,  255}}, /* orange-jaune */
    {"MoltenIron",          {210, 110, 40,  255}}, /* orange */
    {"MoltenCobalt",        {180, 120, 90,  255}}, /* bleu-orange */
    {"MoltenLead",          {140, 110, 100, 255}}, /* gris-orange */
    {"MoltenNiobium",       {190, 140, 110, 255}}, /* orange-gris */
    {"MoltenSalt",          {230, 190, 150, 255}}, /* blanc-orange */
    {"MoltenSteel",         {220, 130, 50,  255}}, /* orange vif */
    {"MoltenTungsten",      {200, 100, 40,  255}}, /* orange foncé */
    {"Naphtha",             {190, 150, 80,  255}}, /* ambre */
    {"Petroleum",           {70,  50,  30,  255}}, /* brun */
    {"Resin",               {180, 120, 50,  255}}, /* ambre liquide */
    {"SaltWater",           {120, 160, 170, 255}}, /* bleu-vert */
    {"SuperCoolant",        {80,  210, 220, 255}}, /* cyan vif */
    {"ViscoGel",            {200, 130, 190, 255}}, /* rose-violet */
    {"Water",               {90,  160, 220, 255}}, /* bleu eau */
    {"Ethanol",             {210, 230, 235, 255}}, /* bleu très pâle */
    {"PhytoOil",            {170, 190, 80,  255}}, /* vert-jaune */

    {"AluminumGas",         {210, 210, 215, 255}}, /* gris pâle */
    {"CarbonDioxide",       {190, 190, 195, 255}}, /* gris pâle */
    {"CarbonGas",           {80,  80,  80,  255}}, /* gris */
    {"ChlorineGas",         {200, 225, 140, 255}}, /* jaune-vert */
    {"ContaminatedOxygen",  {170, 190, 150, 255}}, /* vert-gris */
    {"CopperGas",           {200, 140, 110, 255}}, /* orange */
    {"GoldGas",             {230, 200, 120, 255}}, /* jaune-or */
    {"Hydrogen",            {220, 230, 240, 255}}, /* bleu très pâle */
    {"IronGas",             {180, 150, 140, 255}}, /* brun-gris */
    {"CobaltGas",           {150, 170, 200, 255}}, /* bleu */
    {"LeadGas",             {160, 155, 160, 255}}, /* gris */
    {"MercuryGas",          {195, 195, 190, 255}}, /* gris argenté */
    {"Methane",             {200, 210, 180, 255}}, /* vert pâle */
    {"NiobiumGas",          {180, 190, 200, 255}}, /* gris-bleu */
    {"Oxygen",              {150, 200, 240, 255}}, /* bleu clair */
    {"PhosphorusGas",       {220, 210, 160, 255}}, /* jaune pâle */
    {"Propane",             {215, 205, 170, 255}}, /* beige */
    {"RockGas",             {170, 160, 150, 255}}, /* gris-brun */
    {"SaltGas",             {225, 220, 210, 255}}, /* blanc-gris */
    {"SourGas",             {180, 180, 140, 255}}, /* jaune-gris */
    {"Steam",               {230, 235, 240, 255}}, /* blanc-gris */
    {"SteelGas",            {190, 180, 170, 255}}, /* gris acier */
    {"SulfurGas",           {210, 200, 90,  255}}, /* jaune soufre */
    {"SuperCoolantGas",     {150, 225, 225, 255}}, /* cyan pâle */
    {"TungstenGas",         {170, 170, 175, 255}}, /* gris tungstène */
    {"EthanolGas",          {215, 225, 230, 255}}, /* bleu très pâle */

    {"Vacuum",              {0,   0,   0,   0}},   /* transparent total */
    {"Void",                {0,   0,   0,   0}},   /* transparent total */
    {"COMPOSITION",         {255, 255, 255, 255}}, /* marqueur neutre */

    {NULL, {255, 0, 255, 255}}
};


SDL_Color element_get_color(const char *element_id) {
    size_t table_size = sizeof(ELEMENT_COLOR_TABLE) / sizeof(ELEMENT_COLOR_TABLE[0]);
    SDL_Color default_color = ELEMENT_COLOR_TABLE[table_size - 1].color;

    if (element_id == NULL) return default_color;

    for (size_t i = 0; i < table_size - 1; i++) {
        if (strcmp(element_id, ELEMENT_COLOR_TABLE[i].id) == 0) {
            return ELEMENT_COLOR_TABLE[i].color;
        }
    }

    printf("Error : No color mapped for element id : %s\n", element_id);
    return default_color;
}
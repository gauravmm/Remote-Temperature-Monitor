#include "colormap.h"

// Sin-cubic wave to maintain perceptual linearity of brightness
#define LED_BRIGHTNESS_LEN 32
#define LED_BRIGHTNESS_DIV 4
uint8_t LED_BRIGHTNESS_TABLE[] = {255,252,247,241,233,224,213,201,189,175,161,147,132,118,104,90,77,65,54,43,34,26,20,14,9,6,3,1,0,0,0,0};

uint8_t LED_COLOR_ALARM[3] = {255, 0, 0};

#define LED_COLORMAP_LEN 127
// Based on Inferno, from Matplotlib
// http://bids.github.io/colormap/
uint8_t LED_COLORMAP[][3] = {
	{0,     0,      4},
	{1,     1,      6},
	{2,     1,      10},
	{2,     2,      14},
	{4,     3,      18},
	{5,     4,      23},
	{7,     5,      27},
	{9,     6,      31},
	{11,    7,      36},
	{13,    8,      41},
	{16,    9,      45},
	{18,    10,     50},
	{21,    11,     55},
	{24,    12,     60},
	{27,    12,     65},
	{30,    12,     69},
	{33,    12,     74},
	{36,    12,     79},
	{40,    11,     83},
	{43,    11,     87},
	{47,    10,     91},
	{50,    10,     94},
	{54,    9,      97},
	{57,    9,      99},
	{61,    9,      101},
	{64,    10,     103},
	{68,    10,     104},
	{71,    11,     106},
	{74,    12,     107},
	{77,    13,     108},
	{81,    14,     108},
	{84,    15,     109},
	{87,    16,     110},
	{90,    17,     110},
	{93,    18,     110},
	{97,    19,     110},
	{100,   21,     110},
	{103,   22,     110},
	{106,   23,     110},
	{109,   24,     110},
	{113,   25,     110},
	{116,   26,     110},
	{119,   28,     109},
	{122,   29,     109},
	{125,   30,     109},
	{128,   31,     108},
	{132,   32,     107},
	{135,   33,     107},
	{138,   34,     106},
	{141,   35,     105},
	{144,   37,     104},
	{147,   38,     103},
	{151,   39,     102},
	{154,   40,     101},
	{157,   41,     100},
	{160,   42,     99},
	{163,   44,     97},
	{166,   45,     96},
	{169,   46,     94},
	{173,   48,     93},
	{176,   49,     91},
	{179,   50,     90},
	{182,   52,     88},
	{185,   53,     86},
	{188,   55,     84},
	{191,   57,     82},
	{193,   58,     80},
	{196,   60,     78},
	{199,   62,     76},
	{202,   64,     74},
	{204,   66,     72},
	{207,   68,     70},
	{210,   70,     68},
	{212,   72,     66},
	{215,   75,     63},
	{217,   77,     61},
	{219,   80,     59},
	{222,   82,     56},
	{224,   85,     54},
	{226,   87,     52},
	{228,   90,     49},
	{230,   93,     47},
	{232,   96,     45},
	{234,   99,     42},
	{235,   102,    40},
	{237,   105,    37},
	{239,   108,    35},
	{240,   111,    32},
	{241,   115,    29},
	{243,   118,    27},
	{244,   121,    24},
	{245,   125,    21},
	{246,   128,    19},
	{247,   132,    16},
	{248,   135,    14},
	{249,   139,    11},
	{249,   142,    9},
	{250,   146,    7},
	{251,   150,    6},
	{251,   153,    6},
	{251,   157,    7},
	{252,   161,    8},
	{252,   165,    10},
	{252,   168,    13},
	{252,   172,    17},
	{252,   176,    20},
	{252,   180,    24},
	{251,   184,    29},
	{251,   188,    33},
	{250,   192,    38},
	{250,   196,    42},
	{249,   199,    47},
	{249,   203,    53},
	{248,   207,    58},
	{247,   211,    64},
	{246,   215,    70},
	{245,   219,    76},
	{244,   223,    83},
	{243,   227,    90},
	{242,   230,    97},
	{242,   234,    105},
	{241,   237,    113},
	{241,   241,    121},
	{242,   244,    130},
	{243,   246,    138},
	{245,   249,    146},
	{248,   251,    154},
	{250,   253,    161}};

uint16_t LED_BRIGHTNESS(uint8_t phase) {
	// Restrict to first quadrant:
	phase = phase > 127?(255-phase):phase;
	phase = phase/LED_BRIGHTNESS_DIV;
	if (phase >= LED_BRIGHTNESS_LEN)
		phase = LED_BRIGHTNESS_LEN - 1;

	return (uint16_t) LED_BRIGHTNESS_TABLE[phase];
}

uint8_t *LED_COLOR(uint8_t val) {
	return *(LED_COLORMAP + (val >> 1));
}

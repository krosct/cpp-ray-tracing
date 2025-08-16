#pragma once
#include "Operations.h"

// Cores Primárias e Secundárias
#define RED Vector(red(255), blue(0), green(0))
#define GREEN Vector(red(0), blue(255), green(0))
#define BLUE Vector(red(0), blue(0), green(255))
#define YELLOW Vector(red(255), blue(255), green(0))
#define CYAN Vector(red(0), blue(255), green(255))
#define MAGENTA Vector(red(255), blue(0), green(255))

// Escala de Cinza
#define WHITE Vector(red(255), blue(255), green(255))
#define BLACK Vector(red(0), blue(0), green(0))
#define GRAY Vector(red(128), blue(128), green(128))
#define LIGHT_GRAY Vector(red(192), blue(192), green(192))
#define DARK_GRAY Vector(red(64), blue(64), green(64))

// Cores Adicionais
#define ORANGE Vector(red(255), blue(165), green(0))
#define PURPLE Vector(red(128), blue(0), green(128))
#define BROWN Vector(red(165), blue(42), green(42))
#define PINK Vector(red(255), blue(192), green(203))
#define TEAL Vector(red(0), blue(128), green(128))
#define OLIVE Vector(red(128), blue(128), green(0))
#define NAVY Vector(red(0), blue(0), green(128))
#define MAROON Vector(red(128), blue(0), green(0))
#define GOLD Vector(red(255), blue(215), green(0))
#define SKY_BLUE Vector(red(135), blue(206), green(235))
#define VIOLET Vector(red(238), blue(130), green(238))
#define INDIGO Vector(red(75), blue(0), green(130))
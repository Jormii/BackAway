#ifndef PATH_H
#define PATH_H

#define TITLE "compilation"

#ifdef PPSSPP
#define ROOT "umd0:/"
#else
#define ROOT "ms0:/PSP/GAME/" TITLE "/"
#endif

#define SPRITE(PATH) ROOT "resources/sprites/" PATH ".sprite"
#define MP3(PATH) ROOT "resources/sounds/" PATH ".mp3"
#define LEVEL(PATH) ROOT "resources/levels/" PATH ".level"

#endif

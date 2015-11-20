#ifndef Texture_h
#define Texture_h

#include "Common.h"

GLuint loadTextureFromFile(const string& filename);

GLuint loadTextureFromFont(const string& fontFilename, int pointSize, const string& text);

GLuint convertSDLSurfaceToTexture(SDL_Surface * surface);

#endif

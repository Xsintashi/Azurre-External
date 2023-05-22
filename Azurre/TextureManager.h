#pragma once

typedef struct IDirect3DTexture9* LPDIRECT3DTEXTURE9, * PDIRECT3DTEXTURE9;

namespace TextureManager {
	bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* txt, int* width, int* height);
	bool LoadTextureFromMemory(const char* memory, PDIRECT3DTEXTURE9* txt, int* width, int* height);
}
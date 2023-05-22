#include "TextureManager.h"

#include "GUI.h"
#include "resources.h"
#include "Texture.h"

#include <d3dx9tex.h>
#include <d3dx9.h>

#pragma comment(lib, "d3dx9")

bool TextureManager::LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* txt, int* width, int* height) {

	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(GUI::device, filename, &texture);
	if (hr != S_OK)
		return false;
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*txt = texture;
	*width = (int)my_image_desc.Width;
	*height = (int)my_image_desc.Height;
	return true;
}

bool TextureManager::LoadTextureFromMemory(const char* memory, PDIRECT3DTEXTURE9* txt, int* width, int* height) {
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileInMemory(GUI::device, memory, sizeof(memory), &texture);
	if (hr != S_OK)
		return false;

	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*txt = texture;
	*width = (int)my_image_desc.Width;
	*height = (int)my_image_desc.Height;
	return true;
}


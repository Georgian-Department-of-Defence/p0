#include "Textures.h"
#include "rlgl.h"

Textures g_textures;

void LoadTextures()
{
	{
        Texture2D& tex = g_textures.white;
        tex.width = 1;
        tex.height = 1;
        tex.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        tex.mipmaps = 1;
        unsigned char pixels[4] = { 255, 255, 255, 255 };
        unsigned int id = rlLoadTexture(pixels, tex.width, tex.height, tex.format, tex.mipmaps);
        tex.id = id;
	}

    //g_textures.mech_red = LoadTexture("./assets/textures/mech_tex_red.png");
    //g_textures.mech_blue = LoadTexture("./assets/textures/mech_tex_blue.png");
}

void UnloadTextures()
{
    rlUnloadTexture(g_textures.white.id);
    //UnloadTexture(g_textures.mech_blue);
    //UnloadTexture(g_textures.mech_red);
}

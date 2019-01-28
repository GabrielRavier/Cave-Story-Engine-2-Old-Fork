#include <stddef.h>
#include "Types.h"
#include "CommonDefines.h"
#include <stdio.h>
#include <stdint.h>

#include <SDL_render.h>
#include <SDL_rwops.h>
#include <SDL_timer.h>

#include "WindowsWrapper.h"

#include "Draw.h"
#include "Font.h"
#include "Tags.h"
#include "Resource.h"

RECT grcGame = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
RECT grcFull = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SURFACE surf[SURFACE_ID_MAX];

FontObject *gFont;

#define FRAMERATE 20

bool Flip_SystemTask()
{
	while (true)
	{
		if (!SystemTask())
			return false;

		//Framerate limiter
		static uint32_t timePrev;
		const uint32_t timeNow = SDL_GetTicks();

		if (timeNow >= timePrev + FRAMERATE)
		{
			if (timeNow >= timePrev + 100)
				timePrev = timeNow;	// If the timer is freakishly out of sync, panic and reset it, instead of spamming frames for who-knows how long
			else
				timePrev += FRAMERATE;

			break;
		}

		SDL_Delay(1);
	}

	SDL_RenderPresent(gRenderer);
	return true;
}

bool StartDirectDraw()
{
	//Create renderer
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	return true;
}

void EndDirectDraw()
{
	//Release all surfaces
	for (int i = 0; i < SURFACE_ID_MAX; i++)
	{
		if (surf[i].texture)
		{
			SDL_DestroyTexture(surf[i].texture);
			surf[i].texture = NULL;
		}
	}
}

void ReleaseSurface(int s)
{
	//Release the surface we want to release
	if (surf[s].texture)
	{
		SDL_DestroyTexture(surf[s].texture);
		surf[s].texture = NULL;
	}
}

bool MakeSurface(SDL_RWops *fp, int surf_no)
{
	//Check if surf_no can be used
	if (surf_no > SURFACE_ID_MAX)
	{
		printf("Tried to create surface with invalid id %d\n", surf_no);
		return false;
	}
	if (surf[surf_no].texture)
	{
		printf("Tried to create surface at id %d, but there's already a texture there\n", surf_no);
		return false;
	}
	
	//Load surface from file
	SDL_Surface *surface = SDL_LoadBMP_RW(fp, 1);
	if (!surface)
	{
		printf("Couldn't load bitmap for surface id %d\nSDL Error: %s\n", surf_no, SDL_GetError());
		return false;
	}
	
	//Make sure surface has color key on
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
	
	//Get texture from surface
	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (!texture)
	{
		printf("Failed to convert SDL_Surface to SDL_Texture for surface id %d\nSDL Error: %s\n", surf_no, SDL_GetError());
		return false;
	}
	
	//Create real texture, and copy loaded texture here (has texture target access)
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	SDL_Texture *textureAccessible = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w * gWindowScale, h * gWindowScale);
	
	if (!textureAccessible)
	{
		printf("Failed to create real texture for surface id %d\nSDL Error: %s\n", surf_no, SDL_GetError());
		return false;
	}
	
	SDL_SetTextureBlendMode(textureAccessible, SDL_BLENDMODE_BLEND);
	
	SDL_SetRenderTarget(gRenderer, textureAccessible);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, texture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	SDL_SetRenderTarget(gRenderer, NULL);
	
	//Set surface's metadata
	surf[surf_no].texture = textureAccessible;
	
	//Free surface and texture
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	
	printf(" ^ Successfully loaded\n");
	return true;
}

bool MakeSurface_File(const char *name, int surf_no)
{
	char path[PATH_LENGTH];
	SDL_RWops *fp;
	
	//Attempt to load PBM
	sprintf(path, "%s/%s.pbm", gDataPath, name);
	fp = SDL_RWFromFile(path, "rb");
	if (fp)
	{
		printf("Loading surface (as .pbm) from %s for surface id %d\n", path, surf_no);
		if (MakeSurface(fp, surf_no))
			return true;
	}
	
	//Attempt to load BMP
	sprintf(path, "%s/%s.bmp", gDataPath, name);
	fp = SDL_RWFromFile(path, "rb");
	if (fp)
	{
		printf("Loading surface (as .bmp) from %s for surface id %d\n", path, surf_no);
		if (MakeSurface(fp, surf_no))
			return true;
	}
	
	printf("Failed to open file %s\n", name);
	return false;
}

bool MakeSurface_Resource(const char *res, int surf_no)
{
	SDL_RWops *fp = FindResource(res);
	
	if (fp)
	{
		printf("Loading surface from resource %s for surface id %d\n", res, surf_no);
		if (MakeSurface(fp, surf_no))
			return true;
	}
	
	printf("Failed to open resource %s\n", res);
	return false;
}

bool ReloadBitmap_File(const char *name, int surf_no)
{
	ReleaseSurface(surf_no);
	return MakeSurface_File(name, surf_no);
}

bool ReloadBitmap_Resource(const char *res, int surf_no)
{
	ReleaseSurface(surf_no);
	return MakeSurface_Resource(res, surf_no);
}

bool MakeSurface_Generic(int bxsize, int bysize, int surf_no)
{
	//Delete old surface
	ReleaseSurface(surf_no);
	
	//Create surface
	surf[surf_no].texture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, bxsize * gWindowScale, bysize * gWindowScale);
	
	if (!surf[surf_no].texture)
	{
		printf("Failed to create drawable surface %d\nSDL Error: %s\n", surf_no, SDL_GetError());
		return false;
	}
	
	SDL_SetTextureBlendMode(surf[surf_no].texture, SDL_BLENDMODE_BLEND);
	return true;
}

SDL_Rect RectToSDLRect(RECT *rect)
{
	SDL_Rect SDLRect = { rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top};
	return SDLRect;
}

void BackupSurface(int surf_no, RECT *rect)
{
	//Get renderer size
	int w, h;
	SDL_GetRendererOutputSize(gRenderer, &w, &h);

	//Get texture of what's currently rendered on screen
	SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 0, SDL_PIXELFORMAT_RGBA32);
	SDL_RenderReadPixels(gRenderer, nullptr, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);

	SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(gRenderer, surface);

	//Free surface
	SDL_FreeSurface(surface);
	
	//Get rects
	SDL_Rect frameRect = RectToSDLRect(rect);
	frameRect = {frameRect.x * gWindowScale, frameRect.y * gWindowScale, frameRect.w * gWindowScale, frameRect.h * gWindowScale};
	
	SDL_Texture *textureAccessible = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, frameRect.w, frameRect.h);
	
	if (!textureAccessible)
	{
		printf("Failed to create real texture for surface id %d\nSDL Error: %s\n", surf_no, SDL_GetError());
		return;
	}
	
	SDL_SetTextureBlendMode(textureAccessible, SDL_BLENDMODE_BLEND);
	
	SDL_SetRenderTarget(gRenderer, textureAccessible);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, screenTexture, &frameRect, NULL);
	SDL_RenderPresent(gRenderer);
	SDL_SetRenderTarget(gRenderer, NULL);
	
	//Set surface's metadata
	surf[surf_no].texture = textureAccessible;
	
	//Free stuff
	SDL_DestroyTexture(screenTexture);
}

void PutBitmap3(RECT *rcView, int x, int y, RECT *rect, int surf_no) //Transparency
{
	//Get SDL_Rects
	SDL_Rect clipRect = RectToSDLRect(rcView);
	
	SDL_Rect frameRect = RectToSDLRect(rect);
	frameRect = {frameRect.x * gWindowScale, frameRect.y * gWindowScale, frameRect.w * gWindowScale, frameRect.h * gWindowScale};
	
	//Get dest rect
	SDL_Rect destRect = {x * gWindowScale, y * gWindowScale, frameRect.w, frameRect.h};
	
	//Set cliprect
	clipRect = {clipRect.x * gWindowScale, clipRect.y * gWindowScale, clipRect.w * gWindowScale, clipRect.h * gWindowScale};
	SDL_RenderSetClipRect(gRenderer, &clipRect);
	
	//Draw to screen
	if (SDL_RenderCopy(gRenderer, surf[surf_no].texture, &frameRect, &destRect) < 0)
		printf("Failed to draw texture %d\nSDL Error: %s\n", surf_no, SDL_GetError());
	
	//Undo cliprect
	SDL_RenderSetClipRect(gRenderer, NULL);
}

void PutBitmap4(RECT *rcView, int x, int y, RECT *rect, int surf_no) //No Transparency
{
	//Get SDL_Rects
	SDL_Rect clipRect = RectToSDLRect(rcView);
	
	SDL_Rect frameRect = RectToSDLRect(rect);
	frameRect = {frameRect.x * gWindowScale, frameRect.y * gWindowScale, frameRect.w * gWindowScale, frameRect.h * gWindowScale};
	
	//Get dest rect
	SDL_Rect destRect = {x * gWindowScale, y * gWindowScale, frameRect.w, frameRect.h};
	
	//Set cliprect
	clipRect = {clipRect.x * gWindowScale, clipRect.y * gWindowScale, clipRect.w * gWindowScale, clipRect.h * gWindowScale};
	SDL_RenderSetClipRect(gRenderer, &clipRect);
	
	//Get original drawing colour
	uint8_t origR, origG, origB, origA;
	SDL_GetRenderDrawColor(gRenderer, &origR, &origG, &origB, &origA);
	
	//Draw black behind texture
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(gRenderer, &destRect);
	
	//Draw texture
	if (SDL_RenderCopy(gRenderer, surf[surf_no].texture, &frameRect, &destRect) < 0)
		printf("Failed to draw texture %d\nSDL Error: %s\n", surf_no, SDL_GetError());
	
	//Restore original colour, and undo cliprect
	SDL_RenderSetClipRect(gRenderer, NULL);
	SDL_SetRenderDrawColor(gRenderer, origR, origG, origB, origA);
}

void Surface2Surface(int x, int y, RECT *rect, int to, int from)
{
	//Get rects
	SDL_Rect rcSet = {x * gWindowScale, y * gWindowScale, (rect->right - rect->left) * gWindowScale, (rect->bottom - rect->top) * gWindowScale};
	SDL_Rect frameRect = RectToSDLRect(rect);
	frameRect = {frameRect.x * gWindowScale, frameRect.y * gWindowScale, frameRect.w * gWindowScale, frameRect.h * gWindowScale};
	
	//Target surface
	if (!surf[to].texture)
	{
		printf("Tried to draw to surface %d, which doesn't exist\n", to);
		return;
	}
	
	SDL_SetRenderTarget(gRenderer, surf[to].texture);
	
	//Draw texture
	if (SDL_RenderCopy(gRenderer, surf[from].texture, &frameRect, &rcSet) < 0)
		printf("Failed to draw texture %d to %d\nSDL Error: %s\n", from, to, SDL_GetError());
	
	//Stop targetting surface
	SDL_SetRenderTarget(gRenderer, NULL);
}

void CortBox(RECT *rect, uint32_t col)
{
	//Get rect
	SDL_Rect destRect = RectToSDLRect(rect);
	destRect = {destRect.x * gWindowScale, destRect.y * gWindowScale, destRect.w * gWindowScale, destRect.h * gWindowScale};
	
	//Set colour and draw
	SDL_SetRenderDrawColor(gRenderer, (col & 0xFF0000) >> 16, (col & 0x00FF00) >> 8, col & 0x0000FF, 0xFF);
	SDL_RenderFillRect(gRenderer, &destRect);
}

void CortBox2(RECT *rect, uint32_t col, int surf_no)
{
	//Get rect
	SDL_Rect destRect = RectToSDLRect(rect);
	destRect = {destRect.x * gWindowScale, destRect.y * gWindowScale, destRect.w * gWindowScale, destRect.h * gWindowScale};
	
	//Target surface
	if (!surf[surf_no].texture)
	{
		printf("Tried to draw a rectangle to surface %d, which doesn't exist\n", surf_no);
		return;
	}
	
	SDL_SetRenderTarget(gRenderer, surf[surf_no].texture);
	
	const unsigned char col_red = (col & 0xFF0000) >> 16;
	const unsigned char col_green = (col & 0x00FF00) >> 8;
	const unsigned char col_blue = col & 0x0000FF;
	const unsigned char col_alpha = (col_red || col_green || col_blue) ? 0xFF : 0;

	//Set colour and draw
	SDL_SetRenderDrawColor(gRenderer, col_red, col_green, col_blue, col_alpha);
	SDL_RenderFillRect(gRenderer, &destRect);
	
	//Stop targetting surface
	SDL_SetRenderTarget(gRenderer, NULL);
}

void InitTextObject()
{
	//Get font size
	unsigned int fontWidth, fontHeight;
	if (gWindowScale == 1)
	{
		fontWidth = 6;
		fontHeight = 12;
	}
	else
	{
		fontWidth = 5 * gWindowScale;
		fontHeight = 10 * gWindowScale;
	}
	
	//Open Font.ttf
	char path[PATH_LENGTH];
#ifdef JAPANESE
	sprintf(path, "%s/msgothic.ttc", gDataPath);
#else
	sprintf(path, "%s/cour.ttf", gDataPath);
#endif

	gFont = LoadFont(fontWidth, fontHeight, path);
}

void PutText(int x, int y, const char *text, uint32_t color)
{
	DrawText(gFont, gRenderer, NULL, x * gWindowScale, y * gWindowScale, color, text, strlen(text));
}

void PutText2(int x, int y, const char *text, uint32_t color, int surf_no)
{
	DrawText(gFont, gRenderer, surf[surf_no].texture, x * gWindowScale, y * gWindowScale, color, text, strlen(text));
}

void EndTextObject()
{
	//Destroy font
	UnloadFont(gFont);
	gFont = nullptr;
}

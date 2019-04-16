#include "MapName.h"

#include <stdint.h>
#include <string.h>

#include "CommonDefines.h"
#include "Draw.h"
#include "WindowsWrapper.h"

MAP_NAME gMapName;
RECT rc = { 0, 0, 160, 12 };

void ReadyMapName(const char *str)
{
	int a;

	//Handle "Studio Pixel presents" text in the intro
	unsigned char presentText[24] = {
	#ifdef JAPANESE
		// "�J����Pixel presents"
		0x8A - 1,	// �J
		0x4A - 1,
		0x94 - 1,	// ��
		0xAD - 1,
		0x8E - 1,	// ��
		0xBA - 1,
		'P' - 1,
		'i' - 1,
		'x' - 1,
		'e' - 1,
		'l' - 1,
		' ' - 1,
		'p' - 1,
		'r' - 1,
		'e' - 1,
		's' - 1,
		'e' - 1,
		'n' - 1,
		't' - 1,
		's' - 1,
	#else
		// "  Studio Pixel presents"
		' ' - 1,
		' ' - 1,
		'S' - 1,
		't' - 1,
		'u' - 1,
		'd' - 1,
		'i' - 1,
		'o' - 1,
		' ' - 1,
		'P' - 1,
		'i' - 1,
		'x' - 1,
		'e' - 1,
		'l' - 1,
		' ' - 1,
		'p' - 1,
		'r' - 1,
		'e' - 1,
		's' - 1,
		'e' - 1,
		'n' - 1,
		't' - 1,
		's' - 1,
	#endif
		0xFF
	};

	//Reset map name flags
	gMapName.flag = 0;
	gMapName.wait = 0;

	if (!strcmp(str, "u"))
	{
		for (a = 0; a < (int)sizeof(presentText); ++a)
			presentText[a] = presentText[a] + 1;

		str = (char*)presentText;
	}
	
	//Copy map's name to the MapName
	strcpy(gMapName.name, str);
	
	//Draw the text to the surface
	a = (int)strlen(gMapName.name);

	CortBox2(&rc, 0, SURFACE_ID_ROOM_NAME);
	PutText2((160 - 6 * a) / 2 + 6, 1, gMapName.name, RGB(0x11, 0x00, 0x22), SURFACE_ID_ROOM_NAME);
	PutText2((160 - 6 * a) / 2 + 6, 0, gMapName.name, RGB(0xFF, 0xFF, 0xFE), SURFACE_ID_ROOM_NAME);
}

void PutMapName(BOOL bMini)
{
	// 'unused_rect' isn't the original name. The Linux port optimised this out, so there's no name for it.
	RECT unused_rect = {0, 0, 160, 16};

	if (bMini)
	{
		//Map system
		RECT rcBack;
		rcBack.left = 0;
		rcBack.right = WINDOW_WIDTH;
		rcBack.top = 7;
		rcBack.bottom = 24;

		CortBox(&rcBack, 0x000000);
		PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH - 172) / 2), PixelToScreenCoord(10), &rc, SURFACE_ID_ROOM_NAME);
	}
	else if (gMapName.flag)
	{
		//MNA
		PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH - 172) / 2), PixelToScreenCoord((WINDOW_HEIGHT - 80) / 2), &rc, SURFACE_ID_ROOM_NAME);
		if (++gMapName.wait > 160)
			gMapName.flag = 0;
	}
}

void StartMapName()
{
	gMapName.flag = 1;
	gMapName.wait = 0;
}

void RestoreMapName()
{
	int len = (int)strlen(gMapName.name);
	
	CortBox2(&rc, 0, SURFACE_ID_ROOM_NAME);
	PutText2((160 - 6 * len) / 2 + 6, 1, gMapName.name, RGB(0x11, 0x00, 0x22), SURFACE_ID_ROOM_NAME);
	PutText2((160 - 6 * len) / 2 + 6, 0, gMapName.name, RGB(0xFF, 0xFF, 0xFE), SURFACE_ID_ROOM_NAME);
}

#include "Frame.h"

#include <string.h>

#include "Boss.h"
#include "CommonDefines.h"
#include "Game.h"
#include "Map.h"
#include "MyChar.h"
#include "NpChar.h"

FRAME gFrame;

void MoveFrame3()
{
	int16_t map_w, map_l;
	GetMapData(0, &map_w, &map_l);

#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240	// TODO - Really need to make this a compiler flag
	if (g_GameFlags & 8)
	{
		// Use the original camera boundaries during the credits
		gFrame.x += (*gFrame.tgt_x - (320 * 0x100) - gFrame.x) / gFrame.wait;
		gFrame.y += (*gFrame.tgt_y - (240 * 0x100) - gFrame.y) / gFrame.wait;

		if (gFrame.x < 0)
			gFrame.x = 0;
		if (gFrame.y < 0)
			gFrame.y = 0;

		if (gFrame.x > ((map_w - 1) * 0x10 - 320) * 0x200)
			gFrame.x = ((map_w - 1) * 0x10 - 320) * 0x200;
		if (gFrame.y > ((map_l - 1) * 0x10 - 240) * 0x200)
			gFrame.y = ((map_l - 1) * 0x10 - 240) * 0x200;

		gFrame.x -= ((WINDOW_WIDTH - 320) / 2) * 0x200;
		gFrame.y -= ((WINDOW_HEIGHT - 240) / 2) * 0x200;
	}
	else
	{
		// Widescreen/tallscreen-safe behaviour
		if (map_w * 0x10 < WINDOW_WIDTH)
		{
			gFrame.x = -((WINDOW_WIDTH - map_w * 0x10) * 0x200 / 2);
		}
		else
		{
			gFrame.x += (*gFrame.tgt_x - (WINDOW_WIDTH * 0x100) - gFrame.x) / gFrame.wait;

			if (gFrame.x < 0)
				gFrame.x = 0;

			if (gFrame.x > ((map_w - 1) * 0x10 - WINDOW_WIDTH) * 0x200)
				gFrame.x = ((map_w - 1) * 0x10 - WINDOW_WIDTH) * 0x200;
		}

		if (map_l * 0x10 < WINDOW_HEIGHT)
		{
			gFrame.y = -((WINDOW_HEIGHT - map_l * 0x10) * 0x200 / 2);
		}
		else
		{
			gFrame.y += (*gFrame.tgt_y - (WINDOW_HEIGHT * 0x100) - gFrame.y) / gFrame.wait;

			if (gFrame.y < 0)
				gFrame.y = 0;

			if (gFrame.y > ((map_l - 1) * 0x10 - WINDOW_HEIGHT) * 0x200)
				gFrame.y = ((map_l - 1) * 0x10 - WINDOW_HEIGHT) * 0x200;
		}
	}
#else
	// Vanilla behaviour
	gFrame.x += (*gFrame.tgt_x - (WINDOW_WIDTH * 0x100) - gFrame.x) / gFrame.wait;
	gFrame.y += (*gFrame.tgt_y - (WINDOW_HEIGHT * 0x100) - gFrame.y) / gFrame.wait;

	if (gFrame.x < 0)
		gFrame.x = 0;
	if (gFrame.y < 0)
		gFrame.y = 0;

	if (gFrame.x > ((map_w - 1) * 0x10 - WINDOW_WIDTH) * 0x200)
		gFrame.x = ((map_w - 1) * 0x10 - WINDOW_WIDTH) * 0x200;
	if (gFrame.y > ((map_l - 1) * 0x10 - WINDOW_HEIGHT) * 0x200)
		gFrame.y = ((map_l - 1) * 0x10 - WINDOW_HEIGHT) * 0x200;
#endif

	//Quake
	if (gFrame.quake2)
	{
		gFrame.x += (Random(-5, 5) * 0x200);
		gFrame.y += (Random(-3, 3) * 0x200);
		--gFrame.quake2;
	}
	else if (gFrame.quake)
	{
		gFrame.x += (Random(-1, 1) * 0x200);
		gFrame.y += (Random(-1, 1) * 0x200);
		--gFrame.quake;
	}

	// This code exists in the Linux port, but not the Windows version
/*	if (gFrame.x < 0)
		gFrame.x = 0;
	if (gFrame.y < 0)
		gFrame.y = 0;*/
}

void GetFramePosition(int *fx, int *fy)
{
	*fx = gFrame.x;
	*fy = gFrame.y;
}

void SetFramePosition(int fx, int fy)
{
	//End quake
	gFrame.quake = 0;
	gFrame.quake2 = 0;

	//Move frame position
	int16_t map_w, map_l;
	GetMapData(0, &map_w, &map_l);
	
	gFrame.x = fx;
	gFrame.y = fy;
	
	//Keep in bounds
	if (gFrame.x < 0)
		gFrame.x = 0;
	if (gFrame.y < 0)
		gFrame.y = 0;

	if (gFrame.x > ((((map_w - 1) * 0x10) - WINDOW_WIDTH)) * 0x200)
		gFrame.x = (((map_w - 1) * 0x10) - WINDOW_WIDTH) * 0x200;
	if (gFrame.y > ((((map_l - 1) * 0x10) - WINDOW_HEIGHT)) * 0x200)
		gFrame.y = (((map_l - 1) * 0x10) - WINDOW_HEIGHT) * 0x200;
}

void SetFrameMyChar()
{
	//Move frame position
	int mc_x, mc_y;
	GetMyCharPosition(&mc_x, &mc_y);
	
	int16_t map_w, map_l;
	GetMapData(0, &map_w, &map_l);
	
	gFrame.x = mc_x - (WINDOW_WIDTH << 8);
	gFrame.y = mc_y - (WINDOW_HEIGHT << 8);
	
	//Keep in bounds
	if (gFrame.x < 0)
		gFrame.x = 0;
	if (gFrame.y < 0)
		gFrame.y = 0;
	
	if (gFrame.x > ((((map_w - 1) * 0x10) - WINDOW_WIDTH)) * 0x200)
		gFrame.x = (((map_w - 1) * 0x10) - WINDOW_WIDTH) * 0x200;
	if (gFrame.y > ((((map_l - 1) * 0x10) - WINDOW_HEIGHT)) * 0x200)
		gFrame.y = (((map_l - 1) * 0x10) - WINDOW_HEIGHT) * 0x200;
}

void SetFrameTargetMyChar(int wait)
{
	gFrame.tgt_x = &gMC.tgt_x;
	gFrame.tgt_y = &gMC.tgt_y;
	gFrame.wait = wait;
}

void SetFrameTargetNpChar(int event, int wait)
{
	int i;
	for (i = 0; i < NPC_MAX; i++)
		if (gNPC[i].code_event == event)
			break;

	if (i == NPC_MAX)
		return;

	gFrame.tgt_x = &gNPC[i].x;
	gFrame.tgt_y = &gNPC[i].y;
	gFrame.wait = wait;
}

void SetFrameTargetBoss(int no, int wait)
{
	gFrame.tgt_x = &gBoss[no].x;
	gFrame.tgt_y = &gBoss[no].y;
	gFrame.wait = wait;
}

void SetQuake(int time)
{
	gFrame.quake = time;
}

void SetQuake2(int time)
{
	gFrame.quake2 = time;
}

void ResetQuake()
{
	gFrame.quake = 0;
	gFrame.quake2 = 0;
}

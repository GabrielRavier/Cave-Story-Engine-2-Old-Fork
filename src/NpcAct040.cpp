#include "WindowsWrapper.h"

#include "NpcAct.h"

#include "MyChar.h"
#include "NpChar.h"
#include "Game.h"
#include "Caret.h"
#include "Sound.h"
#include "Back.h"
#include "Triangle.h"
#include "CommonDefines.h"

//Santa
void ActNpc040(NPCHAR *npc)
{
	RECT rcLeft[7];
	RECT rcRight[7];
	
	rcLeft[0] = {0, 32, 16, 48};
	rcLeft[1] = {16, 32, 32, 48};
	rcLeft[2] = {32, 32, 48, 48};
	rcLeft[3] = {0, 32, 16, 48};
	rcLeft[4] = {48, 32, 64, 48};
	rcLeft[5] = {0, 32, 16, 48};
	rcLeft[6] = {64, 32, 80, 48};
	
	rcRight[0] = {0, 48, 16, 64};
	rcRight[1] = {16, 48, 32, 64};
	rcRight[2] = {32, 48, 48, 64};
	rcRight[3] = {0, 48, 16, 64};
	rcRight[4] = {48, 48, 64, 64};
	rcRight[5] = {0, 48, 16, 64};
	rcRight[6] = {64, 48, 80, 64};
	
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}
			if (npc->x - 0x4000 < gMC.x && npc->x + 0x4000 > gMC.x && npc->y - 0x4000 < gMC.y && npc->y + 0x2000 > gMC.y)
			{
				if (npc->x <= gMC.x)
					npc->direct = 2;
				else
					npc->direct = 0;
			}
			break;
		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}
			break;
		case 3:
			npc->act_no = 4;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				npc->ani_no++;
			}
			if (npc->ani_no > 5)
				npc->ani_no = 2;
			if (npc->direct)
				npc->x += 0x200;
			else
				npc->x -= 0x200;
			break;
		case 5:
			npc->ani_no = 6;
			break;
		default:
			break;
	}
	
	if (npc->direct)
		npc->rect = rcRight[npc->ani_no];
	else
		npc->rect = rcLeft[npc->ani_no];
}

//Busted Door
void ActNpc041(NPCHAR *npc)
{
	RECT rect[1];

	rect[0] = {0, 80, 48, 112};

	if (npc->act_no == 0)
	{
		++npc->act_no;
		npc->y -= 0x2000;	// Move a tile up
	}

	npc->rect = rect[0];
}

//Sue
void ActNpc042(NPCHAR *npc)
{
	RECT rcLeft[13];
	RECT rcRight[13];

	rcLeft[0] = {0, 0, 16, 16};
	rcLeft[1] = {16, 0, 32, 16};
	rcLeft[2] = {32, 0, 48, 16};
	rcLeft[3] = {0, 0, 16, 16};
	rcLeft[4] = {48, 0, 64, 16};
	rcLeft[5] = {0, 0, 16, 16};
	rcLeft[6] = {64, 0, 80, 16};
	rcLeft[7] = {80, 32, 96, 48};
	rcLeft[8] = {96, 32, 112, 48};
	rcLeft[9] = {128, 32, 144, 48};
	rcLeft[10] = {0, 0, 16, 16};
	rcLeft[11] = {112, 32, 128, 48};
	rcLeft[12] = {160, 32, 176, 48};

	rcRight[0] = {0, 16, 16, 32};
	rcRight[1] = {16, 16, 32, 32};
	rcRight[2] = {32, 16, 48, 32};
	rcRight[3] = {0, 16, 16, 32};
	rcRight[4] = {48, 16, 64, 32};
	rcRight[5] = {0, 16, 16, 32};
	rcRight[6] = {64, 16, 80, 32};
	rcRight[7] = {80, 48, 96, 64};
	rcRight[8] = {96, 48, 112, 64};
	rcRight[9] = {128, 48, 144, 64};
	rcRight[10] = {0, 16, 16, 32};
	rcRight[11] = {112, 48, 128, 64};
	rcRight[12] = {160, 48, 176, 64};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			break;

		case 5:
			npc->ani_no = 6;
			npc->xm = 0;
			break;

		case 6:
			PlaySoundObject(50, 1);
			npc->act_wait = 0;
			npc->act_no = 7;
			npc->ani_no = 7;
			// Fallthrough
		case 7:
			if (++npc->act_wait > 10)
				npc->act_no = 0;

			break;

		case 8:
			PlaySoundObject(50, 1);
			npc->act_wait = 0;
			npc->act_no = 9;
			npc->ani_no = 7;
			npc->ym = -0x200;

			if (npc->direct == 0)
				npc->xm = 0x400;
			else
				npc->xm = -0x400;

			// Fallthrough
		case 9:
			if (++npc->act_wait > 3 && npc->flag & 8)
			{
				npc->act_no = 10;

				if (npc->direct == 0)
					npc->direct = 2;
				else
					npc->direct = 0;
			}

			break;

		case 10:
			npc->xm = 0;
			npc->ani_no = 8;
			break;

		case 11:
			npc->act_no = 12;
			npc->act_wait = 0;
			npc->ani_no = 9;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 12:
			if (++npc->ani_wait > 8)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 10)
				npc->ani_no = 9;

			break;

		case 13:
			npc->ani_no = 11;
			npc->xm = 0;
			npc->ym = 0;
			npc->act_no = 14;

			int i;
			for (i = 0; i < 0x200 && gNPC[i].code_event != 501; ++i)
				;

			if (i == 0x200)
			{
				npc->act_no = 0;
				break;
			}
			else
			{
				npc->pNpc = &gNPC[i];
			}
			// Fallthrough
		case 14:
			if (npc->pNpc->direct == 0)
				npc->direct = 2;
			else
				npc->direct = 0;

			if (npc->pNpc->direct == 0)
				npc->x = npc->pNpc->x - 0xC00;
			else
				npc->x = npc->pNpc->x + 0xC00;

			npc->y = npc->pNpc->y + 0x800;

			if (npc->pNpc->ani_no == 2 || npc->pNpc->ani_no == 4)
				npc->y -= 0x200;

			break;

		case 15:
			npc->act_no = 16;
			SetNpChar(257, npc->x + 0x10000, npc->y, 0, 0, 0, 0, 0);
			SetNpChar(257, npc->x + 0x10000, npc->y, 0, 0, 2, 0, 0x80);
			npc->xm = 0;
			npc->ani_no = 0;
			// Fallthrough
		case 16:
			gSuperXpos = npc->x - 0x3000;
			gSuperYpos = npc->y - 0x1000;
			break;

		case 17:
			npc->xm = 0;
			npc->ani_no = 12;
			gSuperXpos = npc->x;
			gSuperYpos = npc->y - 0x1000;
			break;

		case 20:
			npc->act_no = 21;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 21:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			if (npc->x < gMC.x - 0x1000)
			{
				npc->direct = 2;
				npc->act_no = 0;
			}

			break;

		case 30:
			npc->act_no = 31;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 31:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			break;

		case 40:
			npc->act_no = 41;
			npc->ani_no = 9;
			npc->ym = -0x400;
			break;
	}

	if (npc->act_no != 14)
	{
		npc->ym += 0x40;

		if (npc->xm > 0x400)
			npc->xm = 0x400;
		if (npc->xm < -0x400)
			npc->xm = -0x400;

		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;

		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Chalkboard
void ActNpc043(NPCHAR *npc)
{
	RECT rcLeft[1];
	RECT rcRight[1];

	rcLeft[0] = {128, 80, 168, 112};
	rcRight[0] = {168, 80, 208, 112};

	if (npc->act_no == 0)
	{
		npc->act_no = 1;
		npc->y -= 0x2000;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[0];
	else
		npc->rect = rcRight[0];
}

//Polish
void ActNpc044(NPCHAR *npc)
{
	RECT rcRight[3];
	RECT rcLeft[3];

	rcRight[0] = {0, 0, 32, 32};
	rcRight[1] = {32, 0, 64, 32};
	rcRight[2] = {64, 0, 96, 32};

	rcLeft[0] = {0, 0, 32, 32};
	rcLeft[1] = {96, 0, 128, 32};
	rcLeft[2] = {128, 0, 160, 32};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			// Fallthrough
		case 1:
			npc->ani_no = 0;

			if (npc->direct == 0)
				npc->act_no = 8;
			else
				npc->act_no = 2;
			// Fallthrough
		case 2:
			npc->ym += 0x20;

			if (npc->ym > 0 && npc->flag & 8)
			{
				npc->ym = -0x100;
				npc->xm += 0x100;
			}

			if (npc->flag & 4)
				npc->act_no = 3;

			break;

		case 3:
			npc->xm += 0x20;

			if (npc->xm > 0 && npc->flag & 4)
			{
				npc->xm = -0x100;
				npc->ym -= 0x100;
			}

			if (npc->flag & 2)
				npc->act_no = 4;

			break;

		case 4:
			npc->ym -= 0x20;

			if (npc->ym < 0 && npc->flag & 2)
			{
				npc->ym = 0x100;
				npc->xm -= 0x100;
			}

			if (npc->flag & 1)
				npc->act_no = 5;

			break;

		case 5:
			npc->xm -= 0x20;

			if (npc->xm < 0 && npc->flag & 1)
			{
				npc->xm = 0x100;
				npc->ym += 0x100;
			}

			if (npc->flag & 8)
				npc->act_no = 2;

			break;

		case 6:
			npc->ym += 0x20;

			if (npc->ym > 0 && npc->flag & 8)
			{
				npc->ym = -0x100;
				npc->xm -= 0x100;
			}

			if (npc->flag & 1)
				npc->act_no = 7;

			break;

		case 7:
			npc->xm -= 0x20;

			if (npc->xm < 0 && npc->flag & 1)
			{
				npc->xm = 0x100;
				npc->ym -= 0x100;
			}

			if (npc->flag & 2)
				npc->act_no = 8;

			break;

		case 8:
			npc->ym -= 0x20;

			if (npc->ym < 0 && npc->flag & 2)
			{
				npc->ym = 0x100;
				npc->xm += 0x100;
			}

			if (npc->flag & 4)
				npc->act_no = 9;

			break;

		case 9:
			npc->xm += 0x20;

			if (npc->xm > 0 && npc->flag & 4)
			{
				npc->xm = -0x100;
				npc->ym += 0x100;
			}

			if (npc->flag & 8)
				npc->act_no = 6;

			break;
	}

	if (npc->life <= 100)
	{
		for (int i = 0; i < 10; ++i)
			SetNpChar(45, npc->x, npc->y, 0, 0, 0, 0, 0x100);

		SetDestroyNpChar(npc->x, npc->y, npc->view.back, 8);
		PlaySoundObject(25, 1);
		npc->cond = 0;
	}

	if (npc->xm > 0x200)
		npc->xm = 0x200;
	if (npc->xm < -0x200)
		npc->xm = -0x200;

	if (npc->ym > 0x200)
		npc->ym = 0x200;
	if (npc->ym < -0x200)
		npc->ym = -0x200;

	if (npc->shock)
	{
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	}
	else
	{
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if (npc->act_no >= 2 && npc->act_no <= 9 && ++npc->ani_no > 2)
		npc->ani_no = 1;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Baby
void ActNpc045(NPCHAR *npc)
{
	RECT rect[3];

	rect[0] = {0, 32, 16, 48};
	rect[1] = {16, 32, 32, 48};
	rect[2] = {32, 32, 48, 48};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 2;

			if (Random(0, 1))
				npc->xm = Random(-0x200, -0x100);
			else
				npc->xm = Random(0x100, 0x200);

			if (Random(0, 1))
				npc->ym = Random(-0x200, -0x100);
			else
				npc->ym = Random(0x100, 0x200);

			npc->xm2 = npc->xm;
			npc->ym2 = npc->ym;
			// Fallthrough
		case 1:
		case 2:
			if (++npc->ani_no > 2)
				npc->ani_no = 1;

			break;
	}

	if (npc->xm2 < 0 && npc->flag & 1)
		npc->xm2 = -npc->xm2;
	if (npc->xm2 > 0 && npc->flag & 4)
		npc->xm2 = -npc->xm2;

	if (npc->ym2 < 0 && npc->flag & 2)
		npc->ym2 = -npc->ym2;
	if (npc->ym2 > 0 && npc->flag & 8)
		npc->ym2 = -npc->ym2;

	if (npc->xm2 > 0x200)
		npc->xm2 = 0x200;
	if (npc->xm2 < -0x200)
		npc->xm2 = -0x200;

	if (npc->ym2 > 0x200)
		npc->ym2 = 0x200;
	if (npc->ym2 < -0x200)
		npc->ym2 = -0x200;

	if (npc->shock)
	{
		npc->x += npc->xm2 / 2;
		npc->y += npc->ym2 / 2;
	}
	else
	{
		npc->x += npc->xm2;
		npc->y += npc->ym2;
	}

	npc->rect = rect[npc->ani_no];
}

//H/V Trigger
void ActNpc046(NPCHAR *npc)
{
	RECT rect[1];
	rect[0] = {0, 0, 16, 16};

	npc->bits |= 0x100;

	if (npc->direct == 0)
	{
		if (npc->x >= gMC.x)
			npc->x -= 0x5FF;
		else
			npc->x += 0x5FF;
	}
	else
	{
		if (npc->y >= gMC.y)
			npc->y -= 0x5FF;
		else
			npc->y += 0x5FF;
	}

	npc->rect = rect[0];
}

//Sandcroc
void ActNpc047(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->ani_no = 0;
			npc->act_no = 1;
			npc->act_wait = 0;
			npc->tgt_y = npc->y;
			npc->bits &= ~0x20;
			npc->bits &= ~4;
			npc->bits &= ~1;
			npc->bits &= ~8;
			// Fallthrough
		case 1:
			if (gMC.x > npc->x - 0x1000 && gMC.x < npc->x + 0x1000 && gMC.y > npc->y && gMC.y < npc->y + 0x1000)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				PlaySoundObject(102, 1);
			}

			if (gMC.x > npc->x)
				npc->x += 0x400;

			if (gMC.x < npc->x)
				npc->x -= 0x400;

			break;

		case 2:
			if (++npc->ani_wait > 3)
			{
				++npc->ani_no;
				npc->ani_wait = 0;
			}

			if (npc->ani_no == 3)
				npc->damage = 10;

			if (npc->ani_no == 4)
			{
				npc->bits |= 0x20;
				npc->act_no = 3;
				npc->act_wait = 0;
			}

			break;

		case 3:
			npc->bits |= 1;
			npc->damage = 0;
			++npc->act_wait;

			if (npc->shock)
			{
				npc->act_no = 4;
				npc->act_wait = 0;
			}

			break;

		case 4:
			npc->bits |= 8;
			npc->y += 0x200;

			if (++npc->act_wait == 32)
			{
				npc->bits &= ~1;
				npc->bits &= ~0x20;
				npc->act_no = 5;
				npc->act_wait = 0;
			}

			break;

		case 5:
			if (npc->act_wait < 100)
			{
				++npc->act_wait;
			}
			else
			{
				npc->y = npc->tgt_y;
				npc->ani_no = 0;
				npc->act_no = 0;
			}

			break;
	}

	RECT rect[5];

	rect[0] = {0, 48, 48, 80};
	rect[1] = {48, 48, 96, 80};
	rect[2] = {96, 48, 144, 80};
	rect[3] = {144, 48, 192, 80};
	rect[4] = {192, 48, 240, 80};

	npc->rect = rect[npc->ani_no];
}

//Omega projectiles
void ActNpc048(NPCHAR *npc)
{
	if (npc->flag & 1 && npc->xm < 0)
	{
		npc->xm = -npc->xm;
	}
	else if (npc->flag & 4 && npc->xm > 0)
	{
		npc->xm = -npc->xm;
	}
	else if (npc->flag & 8)
	{
		if (++npc->count1 <= 2 && npc->direct != 2)
		{
			npc->ym = -0x100;
		}
		else
		{
			VanishNpChar(npc);
			SetCaret(npc->x, npc->y, 2, 0);
		}
	}
	
	if (npc->direct == 2)
	{
		npc->bits &= ~npc_shootable;
		npc->bits |= npc_invulnerable;
	}
	
	npc->ym += 5;
	npc->y += npc->ym;
	npc->x += npc->xm;
	
	RECT rcLeft[2];
	RECT rcRight[2];
	rcLeft[0] = {288, 88, 304, 104};
	rcLeft[1] = {304, 88, 320, 104};
	rcRight[0] = {288, 104, 304, 120};
	rcRight[1] = {304, 104, 320, 120};
	
	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		if (++npc->ani_no > 1)
			npc->ani_no = 0;
	}
	
	if (++npc->act_wait > 750)
	{
		SetCaret(npc->x, npc->y, 2, 0);
		npc->cond = 0;
	}
	
	if (npc->direct)
		npc->rect = rcRight[npc->ani_no];
	else
		npc->rect = rcLeft[npc->ani_no];
}

//Skullhead
void ActNpc049(NPCHAR *npc)
{
	if (npc->act_no >= 10 && npc->pNpc->code_char == 3)
	{
		npc->act_no = 3;
		npc->xm = 0;
		npc->ym = 0;
		npc->count2 = 1;
	}

	if (npc->flag & 1)
	{
		npc->direct = 2;
		npc->xm = 0x100;
	}

	if (npc->flag & 4)
	{
		npc->direct = 0;
		npc->xm = -0x100;
	}

	switch (npc->act_no)
	{
		case 0:
			if (npc->pNpc)
				npc->act_no = 10;
			else
				npc->act_no = 1;
			// Fallthrough
		case 1:
			if (++npc->act_wait > 3)
			{
				npc->ym = -0x400;
				npc->act_no = 3;
				npc->ani_no = 2;

				if (npc->count2 == 0)
				{
					if (npc->direct == 0)
						npc->xm = -0x100;
					else
						npc->xm = 0x100;
				}
				else
				{
					if (npc->direct == 0)
						npc->xm = -0x200;
					else
						npc->xm = 0x200;
				}
			}

			npc->ani_no = 1;
			break;

		case 3:
			if (npc->flag & 8)
			{
				npc->act_no = 1;
				npc->act_wait = 0;
				npc->xm = 0;
			}

			if (npc->flag & 8 || npc->ym > 0)
				npc->ani_no = 1;
			else
				npc->ani_no = 2;

			break;

		case 10:
			if (npc->count1 < 50)
			{
				++npc->count1;
			}
			else
			{
				if (gMC.x > npc->x - 0x10000 && gMC.x < npc->x + 0x10000 && gMC.y > npc->y - 0xC000 && gMC.y < npc->y + 0xC000)
				{
					npc->act_no = 11;
					npc->act_wait = 0;
					npc->ani_no = 2;
				}
			}

			break;

		case 11:
			if (++npc->act_wait == 30 || npc->act_wait == 35)
			{
				const unsigned char deg = GetArktan(npc->x - gMC.x, npc->y + 0x800 - gMC.y);
				const int ym = 2 * GetSin(deg);
				const int xm = 2 * GetCos(deg);
				SetNpChar(50, npc->x, npc->y, xm, ym, 0, 0, 0x100);
				PlaySoundObject(39, 1);
			}

			if (npc->act_wait > 50)
			{
				npc->count1 = 0;
				npc->act_no = 10;
				npc->ani_no = 1;
			}

			break;
	}

	if (npc->act_no >= 10)
	{
		npc->x = npc->pNpc->x;
		npc->y = npc->pNpc->y + 0x2000;
		npc->direct = npc->pNpc->direct;
		--npc->pNpc->count1;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	RECT rcLeft[3];
	RECT rcRight[3];

	rcLeft[0] = {0, 80, 32, 104};
	rcLeft[1] = {32, 80, 64, 104};
	rcLeft[2] = {64, 80, 96, 104};

	rcRight[0] = {0, 104, 32, 128};
	rcRight[1] = {32, 104, 64, 128};
	rcRight[2] = {64, 104, 96, 128};

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Skeleton projectile
void ActNpc050(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			if (npc->direct == 2)
			{
				npc->act_no = 2;
			}
			// Fallthrough
		case 1:
			npc->x += npc->xm;
			npc->y += npc->ym;

			if (npc->flag & 1)
			{
				npc->act_no = 2;
				npc->xm = 0x200;
				++npc->count1;
			}

			if (npc->flag & 4)
			{
				npc->act_no = 2;
				npc->xm = -0x200;
				++npc->count1;
			}

			if (npc->flag & 2)
			{
				npc->act_no = 2;
				npc->ym = 0x200;
				++npc->count1;
			}

			if (npc->flag & 8)
			{
				npc->act_no = 2;
				npc->ym = -0x200;
				++npc->count1;
			}

			break;

		case 2:
			npc->ym += 0x40;

			npc->x += npc->xm;
			npc->y += npc->ym;

			if (npc->flag & 8)
			{
				if (++npc->count1 > 1)
				{
					SetCaret(npc->x, npc->y, 2, 0);
					npc->cond = 0;
				}
			}

			break;
	}

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;
	if (npc->ym < -0x5FF)
		npc->ym = -0x5FF;

	RECT rect[4];

	rect[0] = {48, 32, 64, 48};
	rect[1] = {64, 32, 80, 48};
	rect[2] = {80, 32, 96, 48};
	rect[3] = {96, 32, 112, 48};

	if (npc->direct == 0)
	{
		if (++npc->ani_wait > 1)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 3)
			npc->ani_no = 0;
	}
	else
	{
		if (++npc->ani_wait > 1)
		{
			npc->ani_wait = 0;
			--npc->ani_no;
		}

		if (npc->ani_no < 0)
			npc->ani_no = 3;
	}

	npc->rect = rect[npc->ani_no];
}

//Crow & Skullhead
void ActNpc051(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			if (gMC.x > npc->x - (WINDOW_WIDTH * 0x200) && gMC.x < npc->x + (WINDOW_WIDTH * 0x200) && gMC.y > npc->y - (WINDOW_WIDTH * 0x200) && gMC.y < npc->y + (WINDOW_WIDTH * 0x200))
			{
				npc->tgt_x = npc->x;
				npc->tgt_y = npc->y;
				npc->ym = 0x400;
				npc->act_no = 1;
				SetNpChar(49, 0, 0, 0, 0, 0, npc, 0);
			}
			else
				break;

			// Fallthrough
		case 1:
			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->tgt_y < npc->y)
				npc->ym -= 10;
			if (npc->tgt_y > npc->y)
				npc->ym += 10;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			if (npc->count1 < 10)
				++npc->count1;
			else
				npc->act_no = 2;

			break;

		case 2:
			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->y > gMC.y + 0x4000)
			{
				if (gMC.x < npc->x)
					npc->xm += 0x10;
				if (gMC.x > npc->x)
					npc->xm -= 0x10;
			}
			else
			{
				if (gMC.x < npc->x)
					npc->xm -= 0x10;
				if (gMC.x > npc->x)
					npc->xm += 0x10;
			}

			if (gMC.y < npc->y)
				npc->ym -= 0x10;
			if (gMC.y > npc->y)
				npc->ym += 0x10;

			if (npc->shock)
			{
				npc->ym += 0x20;
				npc->xm = 0;
			}

			break;
	}

	if (npc->xm < 0 && npc->flag & 1)
		npc->xm = 0x100;
	if (npc->xm > 0 && npc->flag & 4)
		npc->xm = -0x100;

	if (npc->ym < 0 && npc->flag & 2)
		npc->ym = 0x100;
	if (npc->ym > 0 && npc->flag & 8)
		npc->ym = -0x100;

	if (npc->xm > 0x400)
		npc->xm = 0x400;
	if (npc->xm < -0x400)
		npc->xm = -0x400;

	if (npc->ym > 0x200)
		npc->ym = 0x200;
	if (npc->ym < -0x200)
		npc->ym = -0x200;

	npc->x += npc->xm;
	npc->y += npc->ym;

	RECT rect_left[5];
	RECT rect_right[5];

	rect_left[0] = {96, 80, 128, 112};
	rect_left[1] = {128, 80, 160, 112};
	rect_left[2] = {160, 80, 192, 112};
	rect_left[3] = {192, 80, 224, 112};
	rect_left[4] = {224, 80, 256, 112};

	rect_right[0] = {96, 112, 128, 144};
	rect_right[1] = {128, 112, 160, 144};
	rect_right[2] = {160, 112, 192, 144};
	rect_right[3] = {192, 112, 224, 144};
	rect_right[4] = {224, 112, 256, 144};

	if (npc->shock)
	{
		npc->ani_no = 4;
	}
	else if (npc->act_no == 2 && npc->y < gMC.y - 0x4000)
	{
		npc->ani_no = 0;
	}
	else
	{
		if (npc->act_no)
		{
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;
		}
	}

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

//Bliue robot (sitting)
void ActNpc052(NPCHAR *npc)
{
	RECT rect[1];

	rect[0] = {240, 96, 256, 112};

	npc->rect = rect[0];
}

//Skullstep leg
void ActNpc053(NPCHAR *npc)
{
	RECT rcLeft[2];
	RECT rcRight[2];

	rcLeft[0] = {0, 128, 24, 144};
	rcLeft[1] = {24, 128, 48, 144};

	rcRight[0] = {48, 128, 72, 144};
	rcRight[1] = {72, 128, 96, 144};

	if (npc->pNpc->code_char == 3)
	{
		VanishNpChar(npc);
		SetDestroyNpChar(npc->x, npc->y, npc->view.back, 4);
		return;
	}

	unsigned char deg;
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->count1 = 10;
			// Fallthrough
		case 1:
			if (npc->direct == 0 && npc->flag & 0x20)
			{
				npc->pNpc->y -= 0x400;
				npc->pNpc->ym -= 0x100;
			}

			if (npc->direct == 2 && npc->flag & 0x10)
			{
				npc->pNpc->y -= 0x400;
				npc->pNpc->ym -= 0x100;
			}

			if (npc->flag & 8)
			{
				npc->pNpc->y -= 0x400;
				npc->pNpc->ym -= 0x100;

				if (npc->pNpc->direct == 0)
					npc->pNpc->xm -= 0x80;
				else
					npc->pNpc->xm += 0x80;
			}

			deg = npc->xm + npc->pNpc->count2;
			npc->x = npc->pNpc->x + npc->count1 * GetCos(deg);
			npc->y = npc->pNpc->y + npc->count1 * GetSin(deg);
			npc->direct = npc->pNpc->direct;
			break;
	}

	npc->direct = npc->pNpc->direct;
	npc->ani_no = deg < 20 || deg > 108;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Skullstep
void ActNpc054(NPCHAR *npc)
{
	RECT rcLeft[3];
	RECT rcRight[3];

	rcLeft[0] = {0, 80, 32, 104};
	rcLeft[1] = {32, 80, 64, 104};
	rcLeft[2] = {64, 80, 96, 104};

	rcRight[0] = {0, 104, 32, 128};
	rcRight[1] = {32, 104, 64, 128};
	rcRight[2] = {64, 104, 96, 128};

	unsigned char deg;
	switch (npc->act_no)
	{
		case 0:
			SetNpChar(53, 0, 0, 0, 0, npc->direct, npc, 0x100);
			SetNpChar(53, 0, 0, 128, 0, npc->direct, npc, 0);
			npc->act_no = 1;
			npc->ani_no = 1;
			// Fallthrough
		case 1:
			deg = npc->count2;

			if (npc->direct == 0)
				deg -= 6;
			else
				deg += 6;

			npc->count2 = deg;

			if (npc->flag & 8)
			{
				npc->xm = 3 * npc->xm / 4;

				if (++npc->act_wait > 60)
				{
					npc->act_no = 2;
					npc->act_wait = 0;
				}
			}
			else
			{
				npc->act_wait = 0;
			}

			if (npc->direct == 0 && npc->flag & 1)
			{
				if (++npc->count1 > 8)
				{
					npc->direct = 2;
					npc->xm = -npc->xm;
				}
			}
			else if (npc->direct == 2 && npc->flag & 4)
			{
				if (++npc->count1 > 8)
				{
					npc->direct = 0;
					npc->xm = -npc->xm;
				}
			}
			else
			{
				npc->count1 = 0;
			}

			break;

		case 2:
			++npc->act_wait;
			npc->shock += npc->act_wait;

			if (npc->act_wait > 50)
			{
				VanishNpChar(npc);
				SetDestroyNpChar(npc->x, npc->y, npc->view.back, 8);
				PlaySoundObject(25, 1);
			}

			break;
	}

	npc->ym += 0x80;

	if (npc->xm > 0x2FF)
		npc->xm = 0x2FF;
	if (npc->xm < -0x2FF)
		npc->xm = -0x2FF;

	if (npc->ym > 0x2FF)
		npc->ym = 0x2FF;
	if (npc->ym < -0x2FF)
		npc->ym = -0x2FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Cairn
void ActNpc055(NPCHAR *npc)
{
	RECT rcLeft[6];
	RECT rcRight[6];

	rcLeft[0] = {192, 192, 208, 216};
	rcLeft[1] = {208, 192, 224, 216};
	rcLeft[2] = {192, 192, 208, 216};
	rcLeft[3] = {224, 192, 240, 216};
	rcLeft[4] = {192, 192, 208, 216};
	rcLeft[5] = {240, 192, 256, 216};

	rcRight[0] = {192, 216, 208, 240};
	rcRight[1] = {208, 216, 224, 240};
	rcRight[2] = {192, 216, 208, 240};
	rcRight[3] = {224, 216, 240, 240};
	rcRight[4] = {192, 216, 208, 240};
	rcRight[5] = {240, 216, 256, 240};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			if (npc->direct == 0)
				npc->x -= 0x200;
			else
				npc->x += 0x200;

			break;

		case 5:
			npc->ani_no = 5;
			break;
	}

	npc->ym += 0x20;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Beetle (Sand Zone)
void ActNpc056(NPCHAR *npc)
{
	RECT rcLeft[3];
	RECT rcRight[3];

	rcLeft[0] = {0, 144, 16, 160};
	rcLeft[1] = {16, 144, 32, 160};
	rcLeft[2] = {32, 144, 48, 160};

	rcRight[0] = {0, 160, 16, 176};
	rcRight[1] = {16, 160, 32, 176};
	rcRight[2] = {32, 160, 48, 176};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;

			if (npc->direct == 0)
				npc->act_no = 1;
			else
				npc->act_no = 3;

			break;

		case 1:
			npc->xm -= 0x10;

			if (npc->xm < -0x400)
				npc->xm = -0x400u;

			if (npc->shock)
				npc->x += npc->xm / 2;
			else
				npc->x += npc->xm;

			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 2)
				npc->ani_no = 1;

			if (npc->flag & 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 0;
				npc->xm = 0;
				npc->direct = 2;
			}

			break;

		case 2:
			if (npc->x < gMC.x && npc->x > gMC.x - 0x20000 && npc->y < gMC.y + 0x1000 && npc->y > gMC.y - 0x1000)
			{
				npc->act_no = 3;
				npc->ani_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 3:
			npc->xm += 0x10;

			if (npc->xm > 0x400)
				npc->xm = 0x400;

			if (npc->shock)
				npc->x += npc->xm / 2;
			else
				npc->x += npc->xm;

			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 2)
				npc->ani_no = 1;

			if (npc->flag & 4)
			{
				npc->act_no = 4;
				npc->act_wait = 0;
				npc->ani_no = 0;
				npc->xm = 0;
				npc->direct = 0;
			}

			break;

		case 4:
			if (npc->x < gMC.x + 0x20000 && npc->x > gMC.x && npc->y < gMC.y + 0x1000 && npc->y > gMC.y - 0x1000)
			{
				npc->act_no = 1;
				npc->ani_wait = 0;
				npc->ani_no = 1;
			}

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Crow
void ActNpc057(NPCHAR *npc)
{
	unsigned char deg;
	switch (npc->act_no)
	{
		case 0:
			deg = Random(0, 0xFF);
			npc->xm = GetCos(deg);
			npc->tgt_x = npc->x + 8 * GetCos(deg + 0x40);

			deg = Random(0, 0xFF);
			npc->ym = GetSin(deg);
			npc->tgt_y = npc->y + 8 * GetSin(deg + 0x40);

			npc->act_no = 1;
			npc->count1 = 120;
			npc->ani_no = Random(0, 1);
			npc->ani_wait = Random(0, 4);
			// Fallthrough
		case 1:
			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->tgt_x < npc->x)
				npc->xm -= 0x10;
			if (npc->tgt_x > npc->x)
				npc->xm += 0x10;

			if (npc->tgt_y < npc->y)
				npc->ym -= 0x10;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x10;

			if (npc->xm > 0x200)
				npc->xm = 0x200;
			if (npc->xm < -0x200)
				npc->xm = -0x200;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			if (npc->shock)
			{
				npc->act_no = 2;
				npc->act_wait = 0;

				if (npc->direct == 2)
					npc->xm = -0x200;
				else
					npc->xm = 0x200;

				npc->ym = 0;
			}

			break;

		case 2:
			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->y > gMC.y + 0x6000)
			{
				if (gMC.x < npc->x)
					npc->xm += 0x10;
				if (gMC.x > npc->x)
					npc->xm -= 0x10;
			}
			else
			{
				if (gMC.x < npc->x)
					npc->xm -= 0x10;
				if (gMC.x > npc->x)
					npc->xm += 0x10;
			}

			if (gMC.y < npc->y)
				npc->ym -= 0x10;
			if (gMC.y > npc->y)
				npc->ym += 0x10;

			if (npc->shock)
			{
				npc->ym += 0x20;
				npc->xm = 0;
			}

			if (npc->xm < 0 && npc->flag & 1)
				npc->xm = 0x200;
			if (npc->xm > 0 && npc->flag & 4)
				npc->xm = -0x200;

			if (npc->ym < 0 && npc->flag & 2)
				npc->ym = 0x200;
			if (npc->ym > 0 && npc->flag & 8)
				npc->ym = -0x200;

			if (npc->xm > 0x5FF)
				npc->xm = 0x5FF;
			if (npc->xm < -0x5FF)
				npc->xm = -0x5FF;

			if (npc->ym > 0x5FF)
				npc->ym = 0x5FF;
			if (npc->ym < -0x5FF)
				npc->ym = -0x5FF;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	RECT rect_left[5];
	RECT rect_right[5];

	rect_left[0] = {96, 80, 128, 112};
	rect_left[1] = {128, 80, 160, 112};
	rect_left[2] = {160, 80, 192, 112};
	rect_left[3] = {192, 80, 224, 112};
	rect_left[4] = {224, 80, 256, 112};

	rect_right[0] = {96, 112, 128, 144};
	rect_right[1] = {128, 112, 160, 144};
	rect_right[2] = {160, 112, 192, 144};
	rect_right[3] = {192, 112, 224, 144};
	rect_right[4] = {224, 112, 256, 144};

	if (npc->shock)
	{
		npc->ani_no = 4;
	}
	else
	{
		if (++npc->ani_wait > 1)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 1)
			npc->ani_no = 0;
	}

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

//Basu (Egg Corridor)
void ActNpc058(NPCHAR *npc)
{
	RECT rcLeft[3];
	RECT rcRight[3];

	rcLeft[0] = {192, 0, 216, 24};
	rcLeft[1] = {216, 0, 240, 24};
	rcLeft[2] = {240, 0, 264, 24};

	rcRight[0] = {192, 24, 216, 48};
	rcRight[1] = {216, 24, 240, 48};
	rcRight[2] = {240, 24, 264, 48};

	switch (npc->act_no)
	{
		case 0:
			if (gMC.x >= npc->x + 0x2000 || gMC.x <= npc->x - 0x2000)
			{
				npc->rect.right = 0;
				npc->damage = 0;
				npc->xm = 0;
				npc->ym = 0;
				npc->bits &= ~0x20;
			}
			else
			{
				npc->bits |= 0x20;
				npc->ym = -0x100;
				npc->tgt_x = npc->x;
				npc->tgt_y = npc->y;
				npc->act_no = 1;
				npc->act_wait = 0;
				npc->count1 = npc->direct;
				npc->count2 = 0;
				npc->damage = 6;

				if (npc->direct == 0)
				{
					npc->x = gMC.x + 0x20000;
					npc->xm = -0x2FFu;
				}
				else
				{
					npc->x = gMC.x - 0x20000;
					npc->xm = 0x2FF;
				}
			}

			break;

		case 1:
			if (npc->x <= gMC.x)
			{
				npc->direct = 2;
				npc->xm += 0x10;
			}
			else
			{
				npc->direct = 0;
				npc->xm -= 0x10;
			}

			if (npc->flag & 1)
				npc->xm = 0x200;

			if (npc->flag & 4)
				npc->xm = -0x200;

			if (npc->y >= npc->tgt_y)
				npc->ym -= 8;
			else
				npc->ym += 8;

			if (npc->xm > 0x2FF)
				npc->xm = 0x2FF;
			if (npc->xm < -0x2FF)
				npc->xm = -0x2FF;

			if (npc->ym > 0x100)
				npc->ym = 0x100;
			if (npc->ym < -0x100)
				npc->ym = -0x100;

			if (npc->shock)
			{
				npc->x += npc->xm / 2;
				npc->y += npc->ym / 2;
			}
			else
			{
				npc->x += npc->xm;
				npc->y += npc->ym;
			}

			if (gMC.x > npc->x + 0x32000 || gMC.x < npc->x - 0x32000)	// TODO: Maybe do something about this for widescreen
			{
				npc->act_no = 0;
				npc->xm = 0;
				npc->direct = npc->count1;
				npc->x = npc->tgt_x;
				npc->rect.right = 0;
				npc->damage = 0;
				break;
			}

			// Fallthrough
		case 2:
			if (npc->act_no)
			{
				if (npc->act_wait < 150)
					++npc->act_wait;

				if (npc->act_wait == 150)
				{
					if ((++npc->count2 % 8) == 0 && npc->x < gMC.x + 0x14000 && npc->x > gMC.x - 0x14000)	// TODO: Maybe do something about this for tallscreen
					{
						unsigned char deg = GetArktan(npc->x - gMC.x, npc->y - gMC.y);
						unsigned char rand_deg = Random(-6, 6) + deg;
						int ym = 2 * GetSin(rand_deg);
						int xm = 2 * GetCos(rand_deg);
						SetNpChar(84, npc->x, npc->y, xm, ym, 0, 0, 0x100);
						PlaySoundObject(39, 1);
					}

					if (npc->count2 > 8)
					{
						npc->act_wait = 0;
						npc->count2 = 0;
					}
				}
			}

			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (npc->act_wait > 120 && npc->act_wait / 2 % 2 == 1 && npc->ani_no == 1)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->rect = rcLeft[npc->ani_no];
			else
				npc->rect = rcRight[npc->ani_no];

			break;
	}
}

//Eye door
void ActNpc059(NPCHAR *npc)
{
	RECT rcLeft[4];

	rcLeft[0] = {224, 16, 240, 40};
	rcLeft[1] = {208, 80, 224, 104};
	rcLeft[2] = {224, 80, 240, 104};
	rcLeft[3] = {240, 80, 256, 104};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			// Fallthrough

		case 1:
			if (npc->x - 0x8000 < gMC.x && npc->x + 0x8000 > gMC.x && npc->y - 0x8000 < gMC.y && npc->y + 0x8000 > gMC.y)
			{
				npc->act_no = 2;
				npc->ani_wait = 0;
			}

			break;

		case 2:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no == 2)
				npc->act_no = 3;

			break;

		case 3:
			if (npc->x - 0x8000 >= gMC.x || npc->x + 0x8000 <= gMC.x || npc->y - 0x8000 >= gMC.y || npc->y + 0x8000 <= gMC.y)
			{
				npc->act_no = 4;
				npc->ani_wait = 0;
			}

			break;

		case 4:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				--npc->ani_no;
			}

			if (npc->ani_no == 0)
				npc->act_no = 1;

			break;
	}

	if (npc->shock)
		npc->rect = rcLeft[3];
	else
		npc->rect = rcLeft[npc->ani_no];
}

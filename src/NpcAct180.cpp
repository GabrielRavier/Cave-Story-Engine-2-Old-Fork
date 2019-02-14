#include "WindowsWrapper.h"

#include "NpcAct.h"

#include "MyChar.h"
#include "NpChar.h"
#include "Game.h"
#include "Sound.h"
#include "Back.h"
#include "Triangle.h"
#include "Caret.h"
#include "Frame.h"
#include "Bullet.h"
#include "Flags.h"
#include "NpcHit.h"

//Curly AI
void ActNpc180(NPCHAR *npc)
{
	RECT rcLeft[11];
	RECT rcRight[11];

	rcLeft[0] = {0, 96, 16, 112};
	rcLeft[1] = {16, 96, 32, 112};
	rcLeft[2] = {0, 96, 16, 112};
	rcLeft[3] = {32, 96, 48, 112};
	rcLeft[4] = {0, 96, 16, 112};
	rcLeft[5] = {48, 96, 64, 112};
	rcLeft[6] = {64, 96, 80, 112};
	rcLeft[7] = {48, 96, 64, 112};
	rcLeft[8] = {80, 96, 96, 112};
	rcLeft[9] = {48, 96, 64, 112};
	rcLeft[10] = {144, 96, 160, 112};

	rcRight[0] = {0, 112, 16, 128};
	rcRight[1] = {16, 112, 32, 128};
	rcRight[2] = {0, 112, 16, 128};
	rcRight[3] = {32, 112, 48, 128};
	rcRight[4] = {0, 112, 16, 128};
	rcRight[5] = {48, 112, 64, 128};
	rcRight[6] = {64, 112, 80, 128};
	rcRight[7] = {48, 112, 64, 128};
	rcRight[8] = {80, 112, 96, 128};
	rcRight[9] = {48, 112, 64, 128};
	rcRight[10] = {144, 112, 160, 128};

	if (npc->y < gMC.y - 0x14000)
	{
		if (npc->y >= 0x20000)
		{
			npc->tgt_x = 0;
			npc->tgt_y = npc->y;
		}
		else
		{
			npc->tgt_x = 0x280000;
			npc->tgt_y = npc->y;
		}
	}
	else
	{
		if (gCurlyShoot_wait)
		{
			npc->tgt_x = gCurlyShoot_x;
			npc->tgt_y = gCurlyShoot_y;
		}
		else
		{
			npc->tgt_x = gMC.x;
			npc->tgt_y = gMC.y;
		}
	}

	if (npc->xm < 0 && npc->flag & 1)
		npc->xm = 0;
	if (npc->xm > 0 && npc->flag & 4)
		npc->xm = 0;

	switch (npc->act_no)
	{
		case 20:
			npc->x = gMC.x;
			npc->y = gMC.y;
			npc->act_no = 100;
			npc->ani_no = 0;
			SetNpChar(183, 0, 0, 0, 0, 0, npc, 0x100);

			if (GetNPCFlag(563))
				SetNpChar(182, 0, 0, 0, 0, 0, npc, 0x100);
			else
				SetNpChar(181, 0, 0, 0, 0, 0, npc, 0x100);

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 0;
			npc->ani_no = 10;
			// Fallthrough
		case 41:
			if (++npc->act_wait == 750)
			{
				npc->bits &= ~0x2000;
				npc->ani_no = 0;
			}

			if (npc->act_wait > 1000)
			{
				npc->act_no = 100;
				npc->ani_no = 0;
				SetNpChar(183, 0, 0, 0, 0, 0, npc, 0x100);

				if (GetNPCFlag(563))
					SetNpChar(182, 0, 0, 0, 0, 0, npc, 0x100);
				else
					SetNpChar(181, 0, 0, 0, 0, 0, npc, 0x100);
			}

			break;

		case 100:
			npc->ani_no = 0;
			npc->xm = 7 * npc->xm / 8;
			npc->count1 = 0;

			if (npc->x > npc->tgt_x + 0x2000)
			{
				npc->act_no = 200;
				npc->ani_no = 1;
				npc->direct = 0;
				npc->act_wait = Random(20, 60);
			}
			else if (npc->x < npc->tgt_x - 0x2000)
			{
				npc->act_no = 300;
				npc->ani_no = 1;
				npc->direct = 2;
				npc->act_wait = Random(20, 60);
			}

			break;

		case 200:
			npc->xm -= 0x20;
			npc->direct = 0;

			if (npc->flag & 1)
				++npc->count1;
			else
				npc->count1 = 0;

			break;

		case 210:
			npc->xm -= 0x20;
			npc->direct = 0;

			if (npc->flag & 8)
				npc->act_no = 100;

			break;

		case 300:
			npc->xm += 0x20;
			npc->direct = 2;

			if (npc->flag & 4)
				++npc->count1;
			else
				npc->count1 = 0;

			break;

		case 310:
			npc->xm += 0x20;
			npc->direct = 2;

			if (npc->flag & 8)
				npc->act_no = 100;

			break;
	}

	if (gCurlyShoot_wait)
		--gCurlyShoot_wait;

	if (gCurlyShoot_wait == 70)
		npc->count2 = 10;

	if (gCurlyShoot_wait == 60 && npc->flag & 8 && Random(0, 2))
	{
		npc->count1 = 0;
		npc->ym = -0x600;
		npc->ani_no = 1;
		PlaySoundObject(15, 1);

		if (npc->tgt_x < npc->x)
			npc->act_no = 210;
		else
			npc->act_no = 310;
	}

	int xx = npc->x - npc->tgt_x;
	int yy = npc->y - npc->tgt_y;

	if (xx < 0)
		xx = -xx;

	if (npc->act_no == 100)
	{
		if (xx + 0x400 < yy)
			npc->ani_no = 5;
		else
			npc->ani_no = 0;
	}

	if (npc->act_no == 210 || npc->act_no == 310)
	{
		if (xx + 0x400 < yy)
			npc->ani_no = 6;
		else
			npc->ani_no = 1;
	}

	if (npc->act_no == 200 || npc->act_no == 300)
	{
		++npc->ani_wait;

		if (xx + 0x400 < yy)
			npc->ani_no = 6 + (npc->ani_wait / 4 % 4);
		else
			npc->ani_no = 1 + (npc->ani_wait / 4 % 4);

		if (npc->act_wait)
		{
			--npc->act_wait;

			if (npc->flag && npc->count1 > 10)
			{
				npc->count1 = 0;
				npc->ym = -0x600;
				npc->act_no += 10;
				npc->ani_no = 1;
				PlaySoundObject(15, 1);
			}
		}
		else
		{
			npc->act_no = 100;
			npc->ani_no = 0;
		}
	}

	if (npc->act_no >= 100 && npc->act_no < 500)
	{
		if (npc->x >= gMC.x - 0xA000 && npc->x <= gMC.x + 0xA000)
		{
			npc->ym += 0x33;
		}
		else
		{
			if (npc->flag)
				npc->ym += 0x10;
			else
				npc->ym += 0x33;
		}
	}

	if (npc->xm > 0x300)
		npc->xm = 0x300;
	if (npc->xm < -0x300)
		npc->xm = -0x300u;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->act_no >= 100 && (npc->flag & 8) == 0 && npc->ani_no != 1000)
	{
		if (xx + 0x400 < yy)
			npc->ani_no = 6;
		else
			npc->ani_no = 1;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Curly AI Machine Gun
void ActNpc181(NPCHAR *npc)
{
	RECT rcLeft[2];
	RECT rcRight[2];

	rcLeft[0] = {216, 152, 232, 168};
	rcLeft[1] = {232, 152, 248, 168};

	rcRight[0] = {216, 168, 232, 184};
	rcRight[1] = {232, 168, 248, 184};

	if (npc->pNpc)
	{
		if (npc->pNpc->ani_no < 5)
		{
			if (npc->pNpc->direct == 0)
			{
				npc->direct = 0;
				npc->x = npc->pNpc->x - 0x1000;
			}
			else
			{
				npc->direct = 2;
				npc->x = npc->pNpc->x + 0x1000;
			}

			npc->y = npc->pNpc->y;
			npc->ani_no = 0;
		}
		else
		{
			if (npc->pNpc->direct == 0)
			{
				npc->direct = 0;
				npc->x = npc->pNpc->x;
			}
			else
			{
				npc->direct = 2;
				npc->x = npc->pNpc->x;
			}

			npc->y = npc->pNpc->y - 0x1400;
			npc->ani_no = 1;
		}

		if (npc->pNpc->ani_no == 1 || npc->pNpc->ani_no == 3 || npc->pNpc->ani_no == 6 || npc->pNpc->ani_no == 8)
			npc->y -= 0x200;

		switch (npc->act_no)
		{
			case 0:
				if (npc->pNpc->count2 == 10)
				{
					npc->pNpc->count2 = 0;
					npc->act_no = 10;
					npc->act_wait = 0;
				}

				break;

			case 10:
				if (++npc->act_wait % 6 == 1)
				{
					if (npc->ani_no)
					{
						if (npc->direct == 0)
						{
							SetBullet(12, npc->x - 0x400, npc->y - 0x800, 1);
							SetCaret(npc->x - 0x400, npc->y - 0x800, 3, 0);
						}
						else
						{
							SetBullet(12, npc->x + 0x400, npc->y - 0x800, 1);
							SetCaret(npc->x + 0x400, npc->y - 0x800, 3, 0);
						}
					}
					else
					{
						if (npc->direct == 0)
						{
							SetBullet(12, npc->x - 0x800, npc->y + 0x600, 0);
							SetCaret(npc->x - 0x800, npc->y + 0x600, 3, 0);
						}
						else
						{
							SetBullet(12, npc->x + 0x800, npc->y + 0x600, 2);
							SetCaret(npc->x + 0x800, npc->y + 0x600, 3, 0);
						}
					}
				}

				if (npc->act_wait == 60)
					npc->act_no = 0;

				break;
		}

		if (npc->direct == 0)
			npc->rect = rcLeft[npc->ani_no];
		else
			npc->rect = rcRight[npc->ani_no];
	}
}

//Curly AI Polar Star
void ActNpc182(NPCHAR *npc)
{
	RECT rcLeft[2];
	RECT rcRight[2];

	rcLeft[0] = {184, 152, 200, 168};
	rcLeft[1] = {200, 152, 216, 168};

	rcRight[0] = {184, 168, 200, 184};
	rcRight[1] = {200, 168, 216, 184};

	if (npc->pNpc)
	{
		if (npc->pNpc->ani_no < 5)
		{
			if (npc->pNpc->direct == 0)
			{
				npc->direct = 0;
				npc->x = npc->pNpc->x - 0x1000;
			}
			else
			{
				npc->direct = 2;
				npc->x = npc->pNpc->x + 0x1000;
			}

			npc->y = npc->pNpc->y;
			npc->ani_no = 0;
		}
		else
		{
			if (npc->pNpc->direct == 0)
			{
				npc->direct = 0;
				npc->x = npc->pNpc->x;
			}
			else
			{
				npc->direct = 2;
				npc->x = npc->pNpc->x;
			}

			npc->y = npc->pNpc->y - 0x1400;
			npc->ani_no = 1;
		}

		if (npc->pNpc->ani_no == 1 || npc->pNpc->ani_no == 3 || npc->pNpc->ani_no == 6 || npc->pNpc->ani_no == 8)
			npc->y -= 0x200;

		switch (npc->act_no)
		{
			case 0:
				if (npc->pNpc->count2 == 10)
				{
					npc->pNpc->count2 = 0;
					npc->act_no = 10;
					npc->act_wait = 0;
				}

				break;

			case 10:
				if (++npc->act_wait % 12 == 1)
				{
					if (npc->ani_no)
					{
						if (npc->direct == 0)
						{
							SetBullet(6, npc->x - 0x400, npc->y - 0x800, 1);
							SetCaret(npc->x - 0x400, npc->y - 0x800, 3, 0);
						}
						else
						{
							SetBullet(6, npc->x + 0x400, npc->y - 0x800, 1);
							SetCaret(npc->x + 0x400, npc->y - 0x800, 3, 0);
						}
					}
					else
					{
						if (npc->direct == 0)
						{
							SetBullet(6, npc->x - 0x800, npc->y + 0x600, 0);
							SetCaret(npc->x - 0x800, npc->y + 0x600, 3, 0);
						}
						else
						{
							SetBullet(6, npc->x + 0x800, npc->y + 0x600, 2);
							SetCaret(npc->x + 0x800, npc->y + 0x600, 3, 0);
						}
					}
				}

				if (npc->act_wait == 60)
					npc->act_no = 0;

				break;
		}

		if (npc->direct == 0)
			npc->rect = rcLeft[npc->ani_no];
		else
			npc->rect = rcRight[npc->ani_no];
	}
}

//Curly Air Tank Bubble
void ActNpc183(NPCHAR *npc)
{
	RECT rect[2];

	rect[0] = {56, 96, 80, 120};
	rect[1] = {80, 96, 104, 120};

	if (npc->pNpc)
	{
		if (npc->act_no == 0)
		{
			npc->x = npc->pNpc->x;
			npc->y = npc->pNpc->y;
			npc->act_no = 1;
		}

		npc->x += (npc->pNpc->x - npc->x) / 2;
		npc->y += (npc->pNpc->y - npc->y) / 2;

		if (++npc->ani_wait > 1)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 1)
			npc->ani_no = 0;

		if (npc->pNpc->flag & 0x100)
		{
			npc->rect = rect[npc->ani_no];
		}
		else
		{
			npc->rect.right = 0;
		}
	}
}

//Shutter Big
void ActNpc184(NPCHAR *npc)
{
	RECT rc[4];
	rc[0] = {0, 64, 32, 96};
	rc[1] = {32, 64, 64, 96};
	rc[2] = {64, 64, 96, 96};
	rc[3] = {32, 64, 64, 96};
	
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->x += 0x1000;
			npc->y += 0x1000;
			break;
		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->act_wait = 0;
			npc->bits |= npc_ignoreSolid;
			// Fallthrough
		case 11:
			switch (npc->direct)
			{
				case 0:
					npc->x -= 0x80;
					break;
				case 1:
					npc->y -= 0x80;
					break;
				case 2:
					npc->x += 0x80;
					break;
				case 3:
					npc->y += 0x80;
					break;
			}
			if (!(++npc->act_wait & 7))
				PlaySoundObject(26, 1);
			SetQuake(20);
			break;
		case 20:
			for (int i = 0; i < 4; i++)
				SetNpChar(4, npc->x + (Random(-12, 12) << 9), npc->y + 0x2000, Random(-0x155, 0x155), Random(-0x600, 0), 0, 0, 0x100);
			npc->act_no = 1;
			break;
		default:
		break;
	}
	
	if (++npc->ani_wait > 10)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}
	
	if (npc->ani_no > 3)
		npc->ani_no = 0;
	
	npc->rect = rc[npc->ani_no];
}

//Shutter Small
void ActNpc185(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y += 0x1000;
			break;
		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->act_wait = 0;
			npc->bits |= npc_ignoreSolid;
			//Fallthrough
		case 11:
			switch (npc->direct)
			{
				case 0:
					npc->x -= 0x80;
					break;
				case 1:
					npc->y -= 0x80;
					break;
				case 2:
					npc->x += 0x80;
					break;
				case 3:
					npc->y += 0x80;
					break;
			}
			++npc->act_wait;
			break;
		case 0x14:
			npc->y -= 0x3000;
			npc->act_no = 1;
			break;
		default:
			break;
	}
	npc->rect.left = 96;
	npc->rect.top = 64;
	npc->rect.right = 112;
	npc->rect.bottom = 96;
}

//Lift block
void ActNpc186(NPCHAR *npc)
{
	RECT rc[4];
	rc[0] = {48, 48, 64, 64};
	rc[1] = {64, 48, 80, 64};
	rc[2] = {80, 48, 96, 64};
	rc[3] = {64, 48, 80, 64};
	
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			//Fallthrough
		case 1:
			break;
		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->act_wait = 0;
			npc->bits |= 8;
			//Fallthrough
		case 11:
			switch (npc->direct)
			{
				case 0:
					npc->x -= 0x80;
					break;
				case 1:
					npc->y -= 0x80;
					break;
				case 2:
					npc->x += 0x80;
					break;
				case 3:
					npc->y += 0x80;
					break;
			}
			++npc->act_wait;
			break;
	}
	
	if (++npc->ani_wait > 10)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}
	
	if (npc->ani_no > 3)
		npc->ani_no = 0;
	
	npc->rect = rc[npc->ani_no];
}

//Fuzz Core
void ActNpc187(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->count1 = 120;
			npc->act_wait = Random(0, 50);

			for (int i = 0; i < 5; ++i)
				SetNpChar(188, 0, 0, 0, 0, 51 * i, npc, 0x100);
			// Fallthrough
		case 1:
			if (++npc->act_wait >= 50)
			{
				npc->act_wait = 0;
				npc->act_no = 2;
				npc->ym = 0x300;
			}

			break;

		case 2:
			npc->count1 += 4;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->tgt_y < npc->y)
				npc->ym -= 0x10;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x10;

			if (npc->ym > 0x355)
				npc->ym = 0x355;
			if (npc->ym < -0x355)
				npc->ym = -0x355;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	RECT rect_left[2];
	RECT rect_right[2];

	rect_left[0] = {224, 104, 256, 136};
	rect_left[1] = {256, 104, 288, 136};

	rect_right[0] = {224, 136, 256, 168};
	rect_right[1] = {256, 136, 288, 168};

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

//Fuzz
void ActNpc188(NPCHAR *npc)
{
	RECT rect_left[2];
	RECT rect_right[2];

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->count1 = npc->direct;
			// Fallthrough
		case 1:
			if (npc->pNpc->code_char == 187 && npc->pNpc->cond & 0x80)
			{
				const unsigned char deg = npc->count1 + npc->pNpc->count1;
				npc->x = npc->pNpc->x + 20 * GetSin(deg);
				npc->y = npc->pNpc->y + 0x20 * GetCos(deg);
			}
			else
			{
				npc->xm = Random(-0x200, 0x200);
				npc->ym = Random(-0x200, 0x200);
				npc->act_no = 10;
			}

			break;

		case 10:
			if (gMC.x < npc->x)
				npc->xm -= 0x20;
			else
				npc->xm += 0x20;

			if (gMC.y < npc->y)
				npc->ym -= 0x20;
			else
				npc->ym += 0x20;

			if (npc->xm > 0x800)
				npc->xm = 0x800;
			if (npc->xm < -0x800)
				npc->xm = -0x800;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			npc->x += npc->xm;
			npc->y += npc->ym;

			break;
	}

	if (gMC.x < npc->x)
		npc->direct = 0;
	else
		npc->direct = 2;

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	rect_left[0] = {288, 104, 304, 120};
	rect_left[1] = {304, 104, 320, 120};

	rect_right[0] = {288, 120, 304, 136};
	rect_right[1] = {304, 120, 320, 136};

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

//Unused homing flame object (possibly related to the Core?)
void ActNpc189(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->xm = -0x40;
			// Fallthrough
		case 1:
			npc->y += npc->ym;

			if (++npc->act_wait > 0x100)
				npc->act_no = 10;

			break;

		case 10:
			if (gMC.x < npc->x)
				npc->xm -= 8;
			else
				npc->xm += 8;

			if (gMC.y < npc->y)
				npc->ym -= 8;
			else
				npc->ym += 8;

			if (npc->xm > 0x400)
				npc->xm = 0x400;
			if (npc->xm < -0x400)
				npc->xm = -0x400;

			if (npc->ym > 0x400)
				npc->ym = 0x400;
			if (npc->ym < -0x400)
				npc->ym = -0x400;

			npc->x += npc->xm;
			npc->y += npc->ym;
			break;
	}

	if (gMC.x < npc->x)
		npc->direct = 0;
	else
		npc->direct = 2;

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	RECT rect[3];

	rect[0] = {224, 184, 232, 200};
	rect[1] = {232, 184, 240, 200};
	rect[2] = {240, 184, 248, 200};

	npc->rect = rect[npc->ani_no];
}

//Broken robot
void ActNpc190(NPCHAR *npc)
{
	RECT rect[2];
	rect[0] = {192, 32, 208, 48};
	rect[1] = {208, 32, 224, 48};
	
	switch (npc->act_no)
	{
		case 0:
			npc->ani_no = 0;
			break;
		case 10:
			PlaySoundObject(72, 1);
			for (int i = 0; i < 8; i++)
				SetNpChar(4, npc->x, npc->y + (Random(-8, 8) << 9), Random(-8, -2) << 9, Random(-3, 3) << 9, 0, 0, 0x100);
			npc->cond = 0;
			break;
		case 20:
			if (++npc->ani_wait > 10)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}
			if (npc->ani_no > 1)
				npc->ani_no = 0;
			break;
	}
	
	npc->rect = rect[npc->ani_no];
}

//Water level
void ActNpc191(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 10;
			npc->tgt_y = npc->y;
			npc->ym = 512;
			//Fallthrough
		case 10:
			if (npc->y >= npc->tgt_y)
				npc->ym -= 4;
			else
				npc->ym += 4;
			if (npc->ym < -0x100)
				npc->ym = -0x100;
			if (npc->ym > 0x100)
				npc->ym = 0x100;
			npc->y += npc->ym;
			break;
		case 20:
			npc->act_no = 21;
			npc->act_wait = 0;
			//Fallthrough
		case 21:
			if (npc->y >= npc->tgt_y)
				npc->ym -= 4;
			else
				npc->ym += 4;
			if (npc->ym < -0x200)
				npc->ym = -0x200;
			if (npc->ym > 0x200)
				npc->ym = 0x200;
			npc->y += npc->ym;
			if (++npc->act_wait > 1000)
				npc->act_no = 22;
			break;
		case 22:
			if (npc->y >= 0)
				npc->ym -= 4;
			else
				npc->ym += 4;
			if (npc->ym < -0x200)
				npc->ym = -0x200;
			if (npc->ym > 0x200)
				npc->ym = 0x200;
			npc->y += npc->ym;
			if (npc->y < 0x8000 || gSuperYpos)
			{
				npc->act_no = 21;
				npc->act_wait = 0;
			}
			break;
		case 30:
			if (npc->y >= 0)
				npc->ym -= 4;
			else
				npc->ym += 4;
			if (npc->ym < -0x200)
				npc->ym = -0x200;
			if (npc->ym > 0x100)
				npc->ym = 0x100;
			npc->y += npc->ym;
			break;
		default:
			break;
	}
	
	gWaterY = npc->y;
	npc->rect.right = 0;
	npc->rect.bottom = 0;
}

//Scooter
void ActNpc192(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->view.back = 0x2000;
			npc->view.front = 0x2000;
			npc->view.top = 0x1000;
			npc->view.bottom = 0x1000;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->view.top = 0x2000;
			npc->view.bottom = 0x2000;
			npc->y -= 0xA00;
			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			// Fallthrough
		case 21:
			npc->x = npc->tgt_x + (Random(-1, 1) * 0x200);
			npc->y = npc->tgt_y + (Random(-1, 1) * 0x200);

			if (++npc->act_wait > 30)
				npc->act_no = 30;

			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 1;
			npc->xm = -0x800;
			npc->x = npc->tgt_x;
			npc->y = npc->tgt_y;
			PlaySoundObject(44, 1);
			// Fallthrough
		case 31:
			npc->xm += 0x20;
			npc->x += npc->xm;
			++npc->act_wait;
			npc->y = npc->tgt_y + (Random(-1, 1) * 0x200);

			if (npc->act_wait > 10)
				npc->direct = 2;

			if (npc->act_wait > 200)
				npc->act_no = 40;

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 2;
			npc->direct = 0;
			npc->y -= 0x6000;
			npc->xm = -0x1000;
			// Fallthrough
		case 41:
			npc->x += npc->xm;
			npc->y += npc->ym;

			npc->act_wait += 2;

			if (npc->act_wait > 1200)
				npc->cond = 0;

			break;
	}

	if (npc->act_wait % 4 == 0 && npc->act_no >= 20)
	{
		PlaySoundObject(34, 1);

		if (npc->direct == 0)
			SetCaret(npc->x + 5120, npc->y + 5120, 7, 2);
		else
			SetCaret(npc->x - 5120, npc->y + 5120, 7, 0);
	}

	RECT rcLeft[2];
	RECT rcRight[2];

	rcLeft[0] = {224, 64, 256, 80};
	rcLeft[1] = {256, 64, 288, 96};

	rcRight[0] = {224, 80, 256, 96};
	rcRight[1] = {288, 64, 320, 96};

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Scooter (broken)
void ActNpc193(NPCHAR *npc)
{
	RECT rc[1];

	rc[0] = {256, 96, 320, 112};

	if (npc->act_no == 0)
	{
		npc->act_no = 1;
		npc->y = npc->y;
		npc->x += 0x3000;
	}

	npc->rect = rc[0];
}

//Blue robot (broken)
void ActNpc194(NPCHAR *npc)
{
	RECT rc[1];

	rc[0] = {192, 120, 224, 128};

	if (npc->act_no == 0)
	{
		npc->act_no = 1;
		npc->y += 0x800;
	}

	npc->rect = rc[0];
}

//Grate
void ActNpc195(NPCHAR *npc)
{
	RECT rc = {112, 64, 128, 80};
	npc->rect = rc;
}

//Ironhead motion wall
void ActNpc196(NPCHAR *npc)
{
	RECT rcLeft = {112, 64, 144, 80};
	RECT rcRight = {112, 80, 144, 96};

	npc->x -= 0xC00;

	if (npc->x <= 0x26000)
		npc->x += 0x2C000;

	if (npc->direct == 0)
		npc->rect = rcLeft;
	else
		npc->rect = rcRight;
}

//Porcupine Fish
void ActNpc197(NPCHAR *npc)
{
	RECT rc[4];

	rc[0] = {0, 0, 16, 16};
	rc[1] = {16, 0, 32, 16};
	rc[2] = {32, 0, 48, 16};
	rc[3] = {48, 0, 64, 16};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 10;
			npc->ani_wait = 0;
			npc->ym = Random(-0x200, 0x200);
			npc->xm = 0x800;
			// Fallthrough
		case 10:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (npc->xm < 0)
			{
				npc->damage = 3;
				npc->act_no = 20;
			}

			break;

		case 20:
			npc->damage = 3;

			if (++npc->ani_wait > 0)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			if (npc->x < 0x6000)
			{
				npc->destroy_voice = 0;
				LoseNpChar(npc, 1);
			}

			break;
	}

	if (npc->flag & 2)
		npc->ym = 0x200;
	if (npc->flag & 8)
		npc->ym = -0x200;

	npc->xm -= 12;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

//Ironhead projectile
void ActNpc198(NPCHAR *npc)
{
	RECT rcRight[3];

	rcRight[0] = {208, 48, 224, 72};
	rcRight[1] = {224, 48, 240, 72};
	rcRight[2] = {240, 48, 256, 72};

	switch (npc->act_no)
	{
		case 0:
			if (++npc->act_wait > 20)
			{
				npc->act_no = 1;
				npc->xm = 0;
				npc->ym = 0;
				npc->count1 = 0;
			}

			break;

		case 1:
			npc->xm += 0x20;
			break;
	}

	if (++npc->ani_wait > 0)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rcRight[npc->ani_no];

	if (++npc->count1 > 100)
		npc->cond = 0;

	if (npc->count1 % 4 == 1)
		PlaySoundObject(46, 1);
}

//Water/wind particles
void ActNpc199(NPCHAR *npc)
{
	RECT rect[5];

	rect[0] = {72, 16, 74, 18};
	rect[1] = {74, 16, 76, 18};
	rect[2] = {76, 16, 78, 18};
	rect[3] = {78, 16, 80, 18};
	rect[4] = {80, 16, 82, 18};

	if (npc->act_no == 0)
	{
		npc->act_no = 1;
		npc->ani_no = Random(0, 2);

		switch (npc->direct)
		{
			case 0:
				npc->xm = -1;
				break;
			case 1:
				npc->ym = -1;
				break;
			case 2:
				npc->xm = 1;
				break;
			case 3:
				npc->ym = 1;
				break;
		}

		npc->xm *= (Random(4, 8) * 0x200) / 2;
		npc->ym *= (Random(4, 8) * 0x200) / 2;
	}

	if (++npc->ani_wait > 6)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 4)
		npc->cond = 0;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
}

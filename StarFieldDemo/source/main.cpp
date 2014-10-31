#include <nds.h>
#include <stdlib.h>

#define NUM_STARS 40

typedef struct
{
	int x;
	int y;
	int speed;
	unsigned short color;
} Star;

Star stars[NUM_STARS];

void RenewStar(Star * star)
{
	star->color = RGB15(31, 31, 31);
	star->x = 0;
	star->y = rand() % 192;
	star->speed = rand() % 4 + 1;
}

void MoveStar(Star * star)
{
	star->x += star->speed;

	if(star->x >= SCREEN_WIDTH)
	{
		RenewStar(star);
	}
}

void ClearScreen(void)
{
	for(int i = 0; i < 256 * 192; ++i)
		VRAM_A[i] = RGB15(0,0,0);
}

void InitStars(void)
{
	for(int i = 0; i < NUM_STARS; ++i)
	{
		RenewStar(&stars[i]);
	}
}

void DrawStar(Star * star)
{
	VRAM_A[star->x + star->y * SCREEN_WIDTH] = star->color;
}

void EraseStar(Star * star)
{
	VRAM_A[star->x + star->y * SCREEN_WIDTH] = RGB15(0,0,0);
}

int main(void)
{
	irqInit();
	irqEnable(IRQ_VBLANK);

	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);

	ClearScreen();
	InitStars();

	while(1)
	{
		swiWaitForVBlank();

		for(int i = 0; i < NUM_STARS; i++)
		{
			EraseStar(&stars[i]);
			MoveStar(&stars[i]);
			DrawStar(&stars[i]);
		}
	}

	return 0;
}
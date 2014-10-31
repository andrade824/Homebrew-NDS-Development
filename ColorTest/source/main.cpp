#include <nds.h>
#include <stdio.h>

// Given x and y coordinates, will return the index in the VRAM array of that pixel
#define PIXEL(x,y) (x + (y * 256))

int main(void)
{
	int i = 0;
	int j = 0;

	// Initialize the DS Dos-like functionality
	consoleDemoInit();

	// Set frame buffer mode to 0
	videoSetMode(MODE_FB0);

	// Enable VRAM A for writing by the cpu and use as a framebuffer by the video hardware
	vramSetBankA(VRAM_A_LCD);

	while(1)
	{
		u16 color = RGB15(31, 0, 0);	// red

		scanKeys();
		int held = keysHeld();
		
		if(held & KEY_X)
			color = RGB15(0, 0, 31);	// blue
		else if(held & KEY_A)
			color = RGB15(0, 31, 0);	// green

		swiWaitForVBlank();

		// Fill video memory with the chosen color
		for(i = 0; i < 192; i += 2)
		{
			for(j = 0; j < 256; j++)
			{
				VRAM_A[PIXEL(j,i)] = color;
			}
		}
	}

	return 0;
}
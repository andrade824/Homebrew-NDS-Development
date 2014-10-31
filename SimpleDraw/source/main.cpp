#include <nds.h>
#include <stdio.h>

// Given x and y coordinates, will return the index in the VRAM array of that pixel
#define PIXEL(x,y) ((x) + ((y) * 256))

int main(void)
{
	// Where the user is touching
	touchPosition touch;

	// The keys being held down
	int down = 0;

	// Color to draw with
	u16 color = 0;

	// Initialize the DS Dos-like functionality
	consoleDemoInit();

	// Set frame buffer mode to 0
	videoSetMode(MODE_FB0);

	// Enable VRAM A for writing by the cpu and use as a framebuffer by the video hardware
	vramSetBankA(VRAM_A_LCD);

	// Set repeat level for keys
	keysSetRepeat(20, 5);

	while(1)
	{
		// Get values for keys/touchscreen
		scanKeys();
		down = keysDownRepeat();

		// Read the coordinates of where the user is touching
		touchRead(&touch);

		// Change the color based on what keys are being pressed
		if(down & KEY_Y)
			color += RGB15(1,0,0);
		else if(down & KEY_X)
			color += RGB15(0,1,0);
		else if(down & KEY_A)
			color += RGB15(0,0,1);

		// Clear the color back to 0 if they press b
		if(down & KEY_B)
			color = 0;

		// Print the coordinates on the bottom screen
		printf("X Coordinate: %d\nY Coordinate: %d\n", touch.px, touch.py);

		// Print the current color
		printf("Red: %d\nGreen: %d\nBlue: %d", (color & 31), ((color >> 5) & 31), ((color >> 10) & 31) );

		// Draw a pixel on the top screen corresponding to where they're touching
		if(touch.px > 0 || touch.py > 0)
		{
			VRAM_A[PIXEL(touch.px, touch.py)] = color;
			VRAM_A[PIXEL(touch.px + 1, touch.py)] = color;
			VRAM_A[PIXEL(touch.px, touch.py + 1)] = color;
			VRAM_A[PIXEL(touch.px + 1, touch.py + 1)] = color;
		}

		// Clear the screen if they hit the left shoulder button or close the lid
		if(down & KEY_L)
		{
			for(int i = 0; i < 192 * 256; ++i)
				VRAM_A[i] = color;
		}

		// Clear the screen if they close the lid
		if(down & KEY_LID)
		{
			for(int i = 0; i < 192 * 256; ++i)
				VRAM_A[i] = 0;
		}

		swiWaitForVBlank();

		consoleClear();
	}

	return 0;
}
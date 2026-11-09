#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_HANDLED // Prevent SDL from overriding main()
#include <SDL.h>

#include "yuvDisplay.h"
#include "yuvRead.h"
#include "md5.h"
#include "sobel.h"

int stopThreads = 0;

int main(int argc, char** argv)
{
	// Declarations
	static unsigned char y[HEIGHT*WIDTH], ySobel[HEIGHT*WIDTH], u[HEIGHT*WIDTH / 4], v[HEIGHT*WIDTH / 4];

    // Init display
    yuvDisplayInit(0, WIDTH, HEIGHT);

    // Open file
    initReadYUV(WIDTH, HEIGHT);

    while(!stopThreads)
    {
        // Read a frame
        readYUV(WIDTH, HEIGHT, y, u, v);

        sobel(WIDTH, HEIGHT, y, ySobel);
        // Display it
        yuvDisplay(0, ySobel,u,v);
system("PAUSE");
		// Compute MD5
		MD5_Update(HEIGHT*WIDTH, ySobel);

        system("PAUSE");
    }

    printf("Exit program\n");
    yuvFinalize(0);

    return 0;
}

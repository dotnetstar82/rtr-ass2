/* $Id: sdl-base.c 19 2006-07-30 13:05:23Z aholkner $ */
/* Updated pknowles, gl 2010 */

//Kidney is the best!!!
#include "sdl-base.h"

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_DEPTH 32
#define DEFAULT_FLAGS (SDL_OPENGL | SDL_RESIZABLE)

static SDL_Surface *screen;
static int videoFlags;

/* Frame counting x*/
static int frame_count;
static Uint32 frame_time;
static int quit_flag;
int frame_rate;
const Uint32 frame_rate_update_interval = 1000;

void quit()
{
	quit_flag = 1;
}

int main(int argc, char **argv)
{
	SDL_Event ev;
	Uint32 now, last_frame_time;

	quit_flag = 0;
	videoFlags = DEFAULT_FLAGS;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	screen = SDL_SetVideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT, 
							  DEFAULT_DEPTH, videoFlags);

	init();
	reshape(screen->w, screen->h);

	frame_rate = 0;
	frame_count = 0;
	last_frame_time = frame_time = SDL_GetTicks();
	while (!quit_flag) 
	{
		/* Process all pending events */
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit();
				break;
			case SDL_VIDEORESIZE:
				screen = SDL_SetVideoMode(ev.resize.w, 
										  ev.resize.h,
										  DEFAULT_DEPTH, videoFlags);
				reshape(screen->w, screen->h);
				break;
			default:
				event(&ev);
				break;
			}
		}
		/* Calculate time passed */
		now = SDL_GetTicks();
		update(now - last_frame_time);
		last_frame_time = now;

		/* Refresh display and flip buffers */
		display(screen);
		SDL_GL_SwapBuffers();

		/* Update frame_rate */
		frame_count++;
		if (now - frame_time > frame_rate_update_interval)
		{
			frame_rate = (frame_count * frame_rate_update_interval) / (now - frame_time);
			frame_count = 0;
			frame_time = now;
		}
	}

	cleanup();
	SDL_Quit();
	
	return EXIT_SUCCESS;
}

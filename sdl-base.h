#include <SDL/SDL.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

/* Implement these yourself */
void init();
void reshape(int w, int h);
void update(int milliseconds);
void display(SDL_Surface *screen);
void event(SDL_Event *event);
void cleanup();

/* This is updated every second by the main loop -- no need to calculate it
 * yourself.*/
extern int frame_rate;

/* Call this to quit. */
void quit();


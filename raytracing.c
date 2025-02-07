#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#define WIDTH 1920
#define HEIGHT 1080
#define  Ray_Number 100
#define M_PI 3.14

struct Circle {
	double x;
	double y;
	double radius;
};


struct Rays
{
	double x_start, y_start;
	double angle;
	double x_end, y_end;
};

void FillCircle(SDL_Renderer* render, struct Circle circle,Uint8 r,Uint8 g,Uint8 b,Uint8 a ) {
	double radius_sqr = pow(circle.radius, 2);
	SDL_SetRenderDrawColor(render, r, g, b, a);
	for (double x = circle.x - circle.radius; x <= circle.x + circle.radius;x++) {
		for (double y = circle.y - circle.radius; y <= circle.y + circle.radius;y++) {
			double distance_sqrd =pow(x - circle.x,2) + pow(y - circle.y,2);
			if (distance_sqrd <= radius_sqr) {
				SDL_FRect pixel = { x,y,1,1 };
				SDL_RenderFillRect(render, &pixel);  
			}
		}
	}
}


void Ray_Generation(struct Circle circle,struct Rays ray[Ray_Number])
{
	for (int i=0;i<Ray_Number;i++)
	{
		double angle = ((double)i / Ray_Number) * 2*M_PI;
		struct Rays rays = { circle.x,circle.y,angle };
		ray[i] = rays;
		//printf("Angles %f\n", angle);
	}
}


void fillrays(SDL_Renderer* render, struct Rays rays[Ray_Number], Uint8 r, Uint8 g, Uint8 b, Uint8 a,struct Circle object) {
	for (int i = 0; i < Ray_Number; i++) {
		struct Rays ray = rays[i];
		int end_of_screen = 0;
		int object_hit = 0;
		double step = 1;
		double x_draw = ray.x_start;
		double y_draw = ray.y_start;
		double radius_sqr = pow(object.radius, 2);
		while (!end_of_screen && !object_hit) {
			x_draw +=step*cos(ray.angle);
			y_draw +=step*sin(ray.angle);
			SDL_FRect pixel = { x_draw,y_draw,1,1 };
			SDL_RenderFillRect(render, &pixel);
			if (x_draw < 0 || x_draw > WIDTH)
				end_of_screen = 1;
			if (y_draw<0 || y_draw>HEIGHT)
				end_of_screen = 1;
			double distance_sqrd = pow(x_draw - object.x, 2) + pow(y_draw - object.y, 2);
			if (distance_sqrd < radius_sqr) {
				break;
			}
		}
		
	}
}




int main() {
	SDL_Window* window = NULL;
	SDL_Renderer* render = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("RayTrace",WIDTH,HEIGHT,0);
	render = SDL_CreateRenderer(window, NULL);
	
	SDL_FRect rect = { 200,200,200,200 };
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
	//SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	//SDL_RenderFillRect(render, &rect);
	struct Circle circle = { 250,150,80 };
	struct Circle Shadowcircle = { 650,300,140 };
	struct Rays rays[Ray_Number];
	//Ray_Generation(circle, rays);
	int simulation_running = 1;
	SDL_Event event;
	while (simulation_running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type==SDL_EVENT_QUIT)
			{
				simulation_running = 0;
			}
			if (event.type=SDL_EVENT_MOUSE_MOTION &&event.motion.state!=0)
			{
				circle.x = event.motion.x;
				circle.y = event.motion.y;
				Ray_Generation(circle, rays);
			}
		}
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);
		FillCircle(render, circle, 255, 255, 255, 255);
		FillCircle(render,Shadowcircle,255,255,255,255);
		fillrays(render, rays,255,255,255,255,Shadowcircle);
		SDL_RenderPresent(render);
		SDL_Delay(0);
	}
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
#include "SDL2/SDL.h"
#include <iostream>
#include <chrono>
#include "GLEW\glew.h"
#include "Renderer.h"
#include <thread>         // std::this_thread::sleep_for
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include <thread>
#pragma comment(lib, "winmm.lib")

using namespace std;

//Screen attributes
SDL_Window* window;

//OpenGL context 
SDL_GLContext gContext;
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 640;

//Event handler
SDL_Event event;

Renderer* renderer = nullptr;

void playSoundAsync(const std::string& filename)
{
	std::string command = "open \"" + filename + "\" type mpegvideo alias mp3";
	mciSendString(command.c_str(), NULL, 0, NULL);
	mciSendString("play mp3 repeat", NULL, 0, NULL);
}

void clean_up()
{
	delete renderer;

	SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

// initialize SDL and OpenGL
bool init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	// use Double Buffering
	if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
		cout << "Error: No double buffering" << endl;

	// set OpenGL Version (3.3)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Create Window
	window = SDL_CreateWindow("OpenGL Lab 3",
		1, 1,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (window == NULL)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return false;
	}

	//Create OpenGL context
	gContext = SDL_GL_CreateContext(window);
	if (gContext == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	// Disable Vsync
	if (SDL_GL_SetSwapInterval(0) == -1)
		printf("Warning: Unable to disable VSync! SDL Error: %s\n", SDL_GetError());

	// Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Error loading GLEW\n");
		return false;
	}
	// some versions of glew may cause an opengl error in initialization
	glGetError();

	renderer = new Renderer();
	bool engine_initialized = renderer->Init(SCREEN_WIDTH, SCREEN_HEIGHT);

	return engine_initialized;
}

int main(int argc, char* argv[])
{
	//Initialize SDL, glew, engine
	if (init() == false)
	{
		system("pause");
		return EXIT_FAILURE;
	}

	//Quit flag
	bool quit = false;
	bool mouse_button_pressed = false;
	glm::vec2 prev_mouse_position(0);

	auto simulation_start = chrono::steady_clock::now();

	std::string filename = "Assets/Dungeon/ost.wav";
	std::thread t(playSoundAsync, filename);

	// Wait for user exit
	while (quit == false)
	{
		// While there are events to handle
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				// Key down events
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
				//else if (event.key.keysym.sym == SDLK_r) renderer->ReloadShaders();
				else if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP)
				{
					renderer->CameraMoveForward(true);
				}
				else if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
				{
					renderer->CameraMoveBackWard(true);
				}
				else if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT)
				{
					renderer->CameraMoveLeft(true);
				}
				else if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
				{
					renderer->CameraMoveRight(true);
				}
				else if (event.key.keysym.sym == SDLK_r) {
					renderer->HeroDoorCheck();
				}
				else if (event.key.keysym.sym == SDLK_RETURN && (renderer->GetHeroState() == false|| renderer->GetScore() == 2)) {
					renderer->~Renderer();
					renderer = new Renderer();
					renderer->Init(SCREEN_WIDTH, SCREEN_HEIGHT);
					
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				// Key up events
				if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP)
				{
					renderer->CameraMoveForward(false);
				}
				else if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
				{
					renderer->CameraMoveBackWard(false);
				}
				else if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT)
				{
					renderer->CameraMoveLeft(false);
				}
				else if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
				{
					renderer->CameraMoveRight(false);
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				int x = event.motion.x;
				int y = event.motion.y;

				if (mouse_button_pressed)
				{
					renderer->CameraLook(prev_mouse_position - glm::vec2(x, y));
					prev_mouse_position = glm::vec2(x, y);
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x = event.button.x;
					int y = event.button.y;
					mouse_button_pressed = (event.type == SDL_MOUSEBUTTONDOWN);
					prev_mouse_position = glm::vec2(x, y);
				}
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0) {
					renderer->CameraZoom(-0.5f);
				}
				else if (event.wheel.y < 0) {
					renderer->CameraZoom(0.5f);
				}
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					renderer->ResizeBuffers(event.window.data1, event.window.data2);
				}
			}
		}

		// Compute the ellapsed time
		auto simulation_end = chrono::steady_clock::now();

		float dt = chrono::duration <float>(
			simulation_end - simulation_start).count(); // in seconds

		simulation_start = chrono::steady_clock::now();
		//std::cout << renderer->GetHeroState() << std::endl;
		// Update
		if (renderer->GetHeroState() == true&&renderer->GetScore()!=2) {
			renderer->Update(dt);
			//std::cout << renderer->GetHeroState() << std::endl;
		}
		
		if (renderer->GetHeroState() == false) {

		}
		if (renderer->GetScore() == 2) {
		}


		// Draw
		renderer->Render();

		//Update screen (swap buffer for double buffering)
		SDL_GL_SwapWindow(window);
	}
	t.join();
	//Clean up
	clean_up();

	return 0;
}

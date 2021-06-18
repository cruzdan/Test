#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;
SDL_Window* window;
SDL_Renderer* renderer;
bool gameOver = false;
int MS = 10;
SDL_Texture* textMenuPong;
SDL_Rect menuRectPong;
Mix_Music* music;
SDL_Texture* texture;
SDL_Rect imageRect;

void updateMusic() {
	if (!Mix_PlayingMusic()) {
		Mix_PlayMusic(music, -1);
	}
	else if (Mix_PausedMusic()) {
		Mix_ResumeMusic();
	}
	else {
		Mix_PauseMusic();
	}
}

void closeTextures() {
	SDL_DestroyTexture(textMenuPong);
	SDL_DestroyTexture(texture);
}

void close() {
	closeTextures();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_FreeMusic(music);
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void show() {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 51, 168, 255, 0);
	SDL_Rect rect = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, 100,100 };
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0,0);
	SDL_RenderCopy(renderer, textMenuPong, NULL, &menuRectPong);
	SDL_RenderCopy(renderer, texture, NULL, &imageRect);
	SDL_RenderPresent(renderer);
}

void update() {

}

void detectKey() {
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		int typeEvent = event.type;
		if (typeEvent == SDL_QUIT) {
			gameOver = true;
		}
	}
}

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
		std::cout << "SDL did not initialize " << std::endl;
		SDL_Quit();
		return false;
	}
	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE,
		&window, &renderer) < 0) {
		std::cout << "window or renderer are not initialized " << std::endl;
		return false;
	}
	SDL_SetWindowTitle(window, "Game");

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Image",
			"The image PNG is not working", NULL);
		return false;
	}

	if (TTF_Init() < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Ttf",
			"The ttf is not working", NULL);
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Audio",
			"The audio is not working", NULL);
		return false;
	}

	music = Mix_LoadMUS("music/04_Boss Battle (Aircraft Carrier).mp3");
	if (!music) {
		std::cout << "The music is not loaded" << std::endl;
	}

	return true;
}

void initImage() {
	SDL_Surface* image;
	image = IMG_Load("image/ball1.png");
	if (!image) {
		printf("No se pudo cargar la imagen");
	}
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
}

void initTtfVariables() {
	TTF_Font* font = TTF_OpenFont("ttf/Oswald-BoldItalic.ttf", 16);
	SDL_Surface* textSurface;
	SDL_Color color = { 255,255,255 };
	textSurface = TTF_RenderText_Solid(font, "imprimiendo algo", color);
	textMenuPong = SDL_CreateTextureFromSurface(renderer, textSurface);
}

void initVariables() {
	menuRectPong = { 0,0,100,30 };
	initTtfVariables();
	initImage();
	imageRect = { 200,0,100,100 };
}

int main(int argc, char* args[]) {
	if (!init())
		return -1;
	initVariables();
	updateMusic();
	while (!gameOver) {
		detectKey();
		update();
		show();
	}
	close();
	return 0;
}
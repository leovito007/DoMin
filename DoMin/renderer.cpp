#include "renderer.h"

using namespace std;

Renderer::Renderer(const size_t screen_width,
	const size_t screen_height)
	: screen_width(screen_width),
	screen_height(screen_height)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "SDL could not initialize.\n";
		cerr << "SDL_Error: " << SDL_GetError() << "\n";
	}

	// Create Window
	sdl_window = SDL_CreateWindow("Minesweeper Game", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, screen_width,
		screen_height, SDL_WINDOW_SHOWN);

	if (nullptr == sdl_window)
	{
		cerr << "Window could not be created.\n";
		cerr << " SDL_Error: " << SDL_GetError() << "\n";
	}

	// Create renderer
	renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);

	if (nullptr == renderer)
	{
		cerr << "Renderer could not be created.\n";
		cerr << "SDL_Error: " << SDL_GetError() << "\n";
	}

	// Create TTF
	if (TTF_Init() == -1)
	{
		cerr << "Init TTF fail.\n";
		return;
	}
}

Renderer::~Renderer() {
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}

void Renderer::renderText(const char* text, SDL_Color color, int x, int y)
{
	TTF_Font* font = TTF_OpenFont("font.ttf", 20);
	if (!font) {
		SDL_Log("Load font fail !\n");
		return;
	}

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* textureMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	int width = surfaceMessage->w;
	int height = surfaceMessage->h;

	SDL_Rect dstRect = { x, y, width, height };
	SDL_RenderCopy(renderer, textureMessage, NULL, &dstRect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(textureMessage);
}

void Renderer::renderText(const char* text, SDL_Color color, SDL_Rect rect)
{
	TTF_Font* font = TTF_OpenFont("font.ttf", 16);
	if (!font) {
		cerr <<"Load font fail !\n";
		return;
	}

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* textureMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	int width = surfaceMessage->w;
	int height = surfaceMessage->h;

	int x = rect.x + (rect.w - width) / 2;
	int y = rect.y + (rect.h - height) / 2;

	SDL_Rect dstRect = { x, y, width, height };
	SDL_RenderCopy(renderer, textureMessage, NULL, &dstRect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(textureMessage);
}

void Renderer::renderImage(SDL_Rect rect, const char* imageName)
{
	SDL_Surface* surfaceButton = loadImage(imageName);
	SDL_Texture* textureButton = SDL_CreateTextureFromSurface(renderer, surfaceButton);
	SDL_FreeSurface(surfaceButton);

	// Render button on screen
	SDL_RenderCopy(renderer, textureButton, NULL, &rect);

	SDL_DestroyTexture(textureButton);
}

SDL_Surface* Renderer::loadImage(const char* fileName)
{
	SDL_Surface* surface = IMG_Load(fileName);
	if (!surface) {
		// Xử lý lỗi khi không tải được ảnh
		cerr << "Can not load img [" << fileName << "]\n";
	}
	return surface;
}

void Renderer::onGUI(Site* site, int mine, const char* time)
{
	//SDL_Renderer* renderer = renderer;

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 0xFF);
	SDL_RenderClear(renderer);

	int y = 0;
	int width = 2* DEFINE::windowWidth/DEFINE::numCol;
	int height = DEFINE::windowHeight/(DEFINE::numRow + 4);

	// Vẽ các ô
	renderSite(*site);

	// Vẽ text
	string text = "Mine:" + to_string(mine);
	SDL_Rect rect = { 0, y, width, height };
	renderText(text.c_str(), DEFINE::getColor(255, 0, 0), rect);

	SDL_Rect rect2 = { DEFINE::windowWidth - width, y, width, height };
	renderText(time, DEFINE::getColor(255, 0, 0), rect2);

	// Hiển thị trạng thái
	SDL_Rect rect3 = { (DEFINE::windowWidth - width) / 2, y, width, height };

	string state = isPlaying ? "Playing" : "Game Over";
	renderText(state.c_str(), DEFINE::getColor(0, 255, 0), rect3);

	// Update Screen
	SDL_RenderPresent(renderer);
}

void Renderer::renderSite(Site& site)
{
	SDL_Rect block{0,0, DEFINE::windowWidth, DEFINE::windowHeight};

	//string imageName = "bg.png";
	//renderImage(block, imageName.c_str());

	for (int i = 0; i < site.count(); i++)
	{
		Cell* cell = site.getAtIndex(i);
		int x = cell->getRow();
		int y = cell->getCol();

		// Lấy thông tin rect của ô
		block = cell->getRect();

		// Hiển thị ảnh tùy theo trạng thái của ô
		string imageName = "normal.png";

		// DEBUG
		//if (cell->isMine())
		//{
		//	imageName = "boom.png";
		//}

		if (cell->getFlagged())
		{
			imageName = "flagged.png";
		}
		if (cell->getClicked())
		{
			imageName = cell->isMine() ? "boom.png" : "checked.png";
		}

		// render ô
		renderImage(block, imageName.c_str());

		if (cell->getClicked() && !cell->isMine())
		{
			// Hiển thị cảnh báo các mìn cạnh bên
			int numMine = cell->getNumMineAround();

			if (numMine > 0)
			{
				// Vẽ text
				string text = to_string(numMine);
				renderText(text.c_str(), DEFINE::getColor(255, 0, 0), cell->getRect());
			}
		}
	}
}
#include "game.h"

using namespace std;

void Game::start(Renderer& _renderer) 
{
	start_time = steady_clock::now();
	renderer = &_renderer;
	site = new Site();
	site->init();

	bool quit = false;

	while (!quit) {

		click = false;

		// Xử lý sự kiện SDL
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				click = event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT;
				if (click)
				{
					mouseX = (int)event.button.x;
					mouseY = (int)event.button.y;
					isMouseLeft = event.button.button == SDL_BUTTON_LEFT;
				}
				break;
				// Xử lý các sự kiện khác nếu cần thiết
			}
		}

		// Cập nhật trạng thái game
		update();

		// Vẽ các đối tượng trong game lên màn hình
		onGUI();

		if (!isPlaying)
		{
			site->reset();
			initMine();
			start_time = steady_clock::now();
		}

		// Thực hiện delay để giới hạn số lần lặp lại trong 1 giây
		SDL_Delay(1);
	}

	// Thực hiện các công việc sau khi kết thúc game loop

	return;
}

void Game::onGUI()
{
	string time = getTime();

	// Hiển thị
	renderer->onGUI(site, numMine, time.c_str());
}

void Game::update()
{
	if (!isPlaying)
		return;

	if (click)
	{
		int index = site->checkClick(mouseX, mouseY);

		if (index != -1)
		{
			//std::cerr << "Clicked.\n";

			Cell* cell = site->getAtIndex(index);

			if (isMouseLeft)
			{
				bool state = cell->onClickLeft();

				// nếu hành động được chấp nhận thì thực hiện
				if (state)
				{
					if (!cell->isMine())
						onClickSafeCell(index);
					else
						onClickMine(cell);
				}
			}
			else
			{
				bool state = cell->onClickRight();

				// nếu hành động được chấp nhận thì thực hiện
				if (state)
				{
					flagged(index);
				}
			}
		}
	}
}

void Game::onClickMine(Cell* cell)
{
	cell->setClicked(true);

	isPlaying = false;
	renderer->isPlaying = false;
}

void Game::initMine()
{
	int count = 0;

	random_device rd; // Tạo một seed ngẫu nhiên từ thiết bị phần cứng
	mt19937 gen(rd()); // Tạo một generator
	uniform_int_distribution<> dis(0, DEFINE::totalCell); // Tạo một phân phối ngẫu nhiên trong khoảng [a, b]

	for (int i = 0; i < numMine; i++)
	{
		int randomIndex = 0;
		do
		{
			randomIndex = dis(gen);
		} while (DEFINE::isInArray(mineIndex, numMine, randomIndex));

		mineIndex[count++] = randomIndex;
		std::cerr << randomIndex  << "_";

		Cell* cell = site->getAtIndex(randomIndex);
		cell->setIsMine(true);

		int* neighbor = site->getAround(randomIndex);
		for (int i = 0; i < 8; i++)
		{
			if (neighbor[i] == -1)
				continue;

			Cell* cell = site->getAtIndex(neighbor[i]);
			int mine = cell->getNumMineAround();
			cell->setNumMineAround( mine+1);
		}

		delete[] neighbor;
	}

	isPlaying = true;
	renderer->isPlaying = true;
}

void Game::flagged(int index)
{
	if (!DEFINE::isInArray(flaggedIndex, numMine, index))
	{
		if (numflag >= numMine)
			return;

		flaggedIndex[numflag++] = index;
	}
	else
	{
		if (numflag <= 0)
			return;

		flaggedIndex = DEFINE::removeFromArray(flaggedIndex, numMine, index);
	}

}

void Game::onClickSafeCell(int index)
{
	Cell* cell =  site->getAtIndex(index);
	int mineCount = cell->getNumMineAround();

	if (cell->getClicked())
		return;

	cell->setClicked(true);

	if (DEFINE::isInArray(flaggedIndex, numMine, index))
	{
		flagged(index);
	}

	if (mineCount != 0)
		return;

	int* neighbor = site->getAround(index);
	for (int i = 0; i < 8; i++)
	{
		if (neighbor[i] == -1)
			continue;

		onClickSafeCell(neighbor[i]);
	}
}

string Game::getTime()
{
	Uint32 timer = 0;

	end_time = steady_clock::now(); // lưu thời gian kết thúc
	duration<double> elapsed_time = duration_cast<duration<double>>(end_time - start_time);
	timer = elapsed_time.count();

	// tính phút, giây
	int minutes = timer / 60;
	int seconds = timer % 60;

	string result = to_string(minutes);
	if (result.size() == 1) {
		result = '0' + result;
	}

	result += ':';

	string secondsString = to_string(seconds);
	if (secondsString.size() == 1) {
		result += '0';
	}

	result += secondsString;
	return result;
}
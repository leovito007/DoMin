#include "cell.h"
#include "DEFINE.h"
#include "renderer.h"

#ifndef Game_h_
#define Game_h_

using namespace std::chrono;

class Game 
{
public:
	// Khởi động game
	void start(Renderer& renderer);
	// Xử lý hiển thị các đối tượng game (text, rect)
	void onGUI();
	// Xử lý logic
	void update();
	// Xử lý khi người chơi click vào mìn
	void onClickMine(Cell* cell);

private:

	Site* site;
	Renderer* renderer;

	bool isPlaying = false;

	int numMine = DEFINE::numMine;
	int numflag = 0;

	int* mineIndex = new int[DEFINE::numMine]; // mảng vị trí các ô mìn
	int* clickedIndex = new int[DEFINE::numRow * DEFINE::numCol]; // mảng lưu trữ các ô đã chọn và các ô đã xử lý
	int* flaggedIndex = new int[DEFINE::numMine]; // mảng lưu trữ các ô đã đặt cờ

	steady_clock::time_point start_time, end_time;

	bool click = false;
	bool isMouseLeft = true;
	int mouseX, mouseY;

	// Khởi tạo mìn
	void initMine();
	void flagged(int index);

	void onClickSafeCell(int index);

	// Lấy thời gian hiện tại
	string getTime();
};

#endif
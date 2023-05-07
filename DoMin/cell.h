#ifndef Cell_h_
#define Cell_h_

#include "DEFINE.h"

class Cell {
private:
	bool _isMine = false;  // có chứa mìn hay không
	bool _isClicked = false;  // đã được người dùng chọn hoặc được xử lý hay chưa
	bool _isFlagged = false;  // đã được đánh dấu là ô mìn hay chưa
	int _row;  // hàng của ô trên bảng
	int _col;  // cột của ô trên bảng

	int _numMineAround;  // số ô xung quanh có mìn

	void setValue(bool& var, bool value) { var = value; }

	bool pointInRect(int x, int y) const
	{
		SDL_Rect rect = getRect();
		return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
	}
public:
	// Constructor
	Cell(int row, int col) {
		_row = row;
		_col = col;
		_isMine = false;
		_isClicked = false;
		_isFlagged = false;
		_numMineAround = 0;
	}

	// Getter methods
	bool isMine() const { return _isMine; }
	bool getClicked() const { return _isClicked; }
	bool getFlagged() const { return _isFlagged; }

	int getRow() const { return _row; }
	int getCol() const { return _col; }
	int getIndex() const { return _row * DEFINE::numRow + _col; }
	int getNumMineAround() const { return _numMineAround; }

	SDL_Rect getRect() const {
		int x = _col * (DEFINE::cellSize + DEFINE::spaceX) + DEFINE::paddingLeft;
		int y = _row * (DEFINE::cellSize + DEFINE::spaceY) + DEFINE::paddingTop;
		SDL_Rect rect = { x, y, DEFINE::cellSize, DEFINE::cellSize };
		return rect;
	}


	void setIsMine(bool value) { setValue(_isMine, value); }
	void setFlagged(bool value) { setValue(_isFlagged, value); }
	void setClicked(bool value) { setValue(_isClicked, value); }
	void setNumMineAround(int numNeighborMines) { _numMineAround = numNeighborMines; }

	bool checkClick(int mouseX, int mouseY)
	{
		// Kiểm tra xem con trỏ chuột có nằm trong phạm vi của button hay không
		if (pointInRect(mouseX, mouseY))
		{
			return true;
		}
		return false;
	};

	bool onClickLeft()
	{
		if (getFlagged() || getClicked())
			return false;

		std::cerr << "Clicked\n";

		if (isMine())
		{
			std::cerr << "BOOM !\n";
		}
		return true;
	}

	bool onClickRight()
	{
		if (getClicked())
			return false;

		if (!getFlagged())
		{
			setValue(_isFlagged, true);
			std::cerr << "Flag successed!\n";
		}
		else
		{
			setValue(_isFlagged, false);
			std::cerr << "Flag removed!\n";
		}
		return true;
	}

	void reset()
	{
		_isMine = false;
		_isClicked = false;
		_isFlagged = false;
		_numMineAround = 0;
	}
};
#endif

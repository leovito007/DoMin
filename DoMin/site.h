#ifndef Site_h_
#define Site_h_

#include "cell.h"

class Site {
private:
	static const int numRow = DEFINE::numRow;
	static const int numCol = DEFINE::numCol;

	Cell* cells[DEFINE::totalCell];

public:

	// Getter methods
	int count() const { return DEFINE::totalCell; }

	Cell* getAtIndex(int index) { return cells[index]; }

	int getRow(int index) const { return index / numCol; }
	int getCol(int index) const { return index % numCol; }

	int* getUDLR(int index)
	{
		int* result = new int[4] {-1};

		int row = getRow(index);
		int col = getCol(index);

		int count = 0;

		// left
		if (col - 1 >= 0)
			result[count++] = index - 1;

		// right
		if (col + 1 < numCol)
			result[count++] = index + 1;

		// up
		if (row - 1 >= 0)
			result[count++] = index - numCol;

		// down
		if (row + 1 < this->count() / numCol)
			result[count] = index + numCol;

		return result;
	}
	int* getAround(int index)
	{
		int* result = new int[8] {-1};

		int row = getRow(index);
		int col = getCol(index);

		bool up, down, left, right;

		int count = 0;

		// left
		left = col - 1 >= 0;
		result[count++] = left ? index - 1 : -1;
	
		// right
		right = col + 1 < numCol;
		result[count++] = right ? index + 1 : -1;

		// up
		up = row - 1 >= 0;
		result[count++] = up ? index - numCol : -1;
		
		// down
		down = row + 1 < numRow;
		result[count++] = down ? index + numCol : -1;

		// up-left
		result[count++] = up && left ? index - numCol - 1: -1;

		// up-right
		result[count++] = up && right ? index - numCol + 1 :-1;

		// up-left
		result[count++] = down && left ?  index + numCol - 1: -1;

		// up-rigt
		result[count++] = down && right? index + numCol + 1 : -1;


		std::cerr << "neigh = [" << result[0] << "," << result[1] << "," << result[2] << "," << result[3] << "," << result[4] << "," << result[5] << "," << result[6] << "," << result[7] << "]\n";
		return result;
	}

	void init() {

		for (int i = 0; i < count(); i++)
		{
			int row = getRow(i);
			int col = getCol(i);

			cells[i] = new Cell(row, col);
		}
	};

	void reset() {
		for (int i = 0; i < count(); i++)
		{
			Cell* cell = getAtIndex(i);
			cell->reset();
		}
	}

	int checkClick(int mouseX, int mouseY)
	{
		for (int i = 0; i < count(); i++)
		{
			Cell* cell = getAtIndex(i);
			bool isClickd = cell->checkClick(mouseX, mouseY);

			if (isClickd)
				return i;
		}

		return -1;
	}
};

#endif
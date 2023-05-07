#ifndef DEFINE_h_
#define DEFINE_h_

#include <chrono>
#include <random>
#include <iostream>
#include <thread>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

using namespace std;

class DEFINE 
{

public:
	static const int numCol = 8;  // cột của ô trên bảng
	static const int numRow = 10;  // dòng của ô trên bảng

	static const int totalCell = numCol * numRow;
	static const int numMine = 10;  // số lượng mìn

	static const int paddingTop = 100;
	static const int paddingLeft = 10;

	static const int spaceX = 1;
	static const int spaceY = 1;

	static const int cellSize = 30;

	static const int windowHeight = paddingTop + numRow * (cellSize + spaceY) + 20;
	static const int windowWidth = 2 * paddingLeft + numCol * (cellSize + spaceX) - spaceX;

	static const SDL_Color getColor(int r, int g, int b) {
		SDL_Color color = { r,g,b, 255 };
		return color; 
	}

	static bool isInArray(int arr[], int size, int x) {
		for (int i = 0; i < size; i++) {
			if (arr[i] == x) {
				return true;
			}
		}
		return false;
	}

	static int* removeFromArray(int arr[], int size, int val) {
		if (!isInArray(arr, size, val)) {
			// val không nằm trong mảng, không làm gì cả
			return arr;
		}

		int index = 0;
		for (int i = 0; i < size; i++) {
			if (arr[i] == val) {
				index = i;
				break;
			}
		}

		// dịch chuyển các phần tử trong mảng phía sau vị trí index lên một vị trí
		for (int i = index; i < size - 1; i++) {
			arr[i] = arr[i + 1];
		}

		// giảm số lượng phần tử của mảng đi một đơn vị
		size--;

		return arr;
	}
};

#endif
#include <iostream>
#include <thread>  
#include <chrono>

using namespace std;

class Cells {
private:
	bool nextstate;
public:
	bool state;
	void setnextstate(bool ns) { nextstate = ns; }
	bool getnextstate() { return nextstate; }
};

class Grid {
private:
	int x, y;
public:
	Cells** cell;
	Grid(int nr, int nc, int numalivecells, int **posalivecells) : x(nr), y(nc){
		cell = new Cells *[nr];
		for (int i = 0; i < nr; i++) {
			cell[i] = new Cells [nc];
			for (int j = 0; j < nc; j++) {
				cell[i][j].state = false;
			}
		}
		for (int i = 0; i < numalivecells; i++) {
			int xa = posalivecells[i][0];  
			int ya = posalivecells[i][1]; 
			if (xa >= 0 && xa < nr && ya >= 0 && ya < nc) {
				cell[ya][xa].state = true; 
			}
			else {
				cout << "Position (" << xa << ", " << ya << ") invalid." << endl;
			}
		}
	}

	void print() const {
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				cout << (cell[i][j].state ? "0" : ".");
			}
			cout << endl;
		}
		cout << endl;
	}

	~Grid() {
		for (int i = 0; i < x; i++) {
			delete[] cell[i];
		}
		delete[] cell;
	}

	int getrows() const { return x; }
	int getcols() const { return y; }

};

void examine(Grid& grid) {
	int rows = grid.getrows();
	int cols = grid.getcols();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int alive = 0;
			int neighbors[8][2] = {
				{-1, -1}, {0, -1}, {1, -1},
				{-1, 0},          {1, 0},
				{-1, 1}, {0, 1}, {1, 1}
			};
			for (int n = 0; n < 8; n++) {
				int ni = i + neighbors[n][0];
				int nj = j + neighbors[n][1];
				if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && grid.cell[ni][nj].state) {
					alive++;
				}
			}
			if (grid.cell[i][j].state && (alive < 2 || alive > 3)) {
				grid.cell[i][j].setnextstate(false);
			}
			else if (!grid.cell[i][j].state && alive == 3) {
				grid.cell[i][j].setnextstate(true);
			}
			else {
				grid.cell[i][j].setnextstate(grid.cell[i][j].state);
			}
		}
	}
}

void update(Grid& grid) {
	int rows = grid.getrows();
	int cols = grid.getcols();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid.cell[i][j].state = grid.cell[i][j].getnextstate();
		}
	}
}

int main() {
	int width, height; 
	int numalivecells;
	int **posalivecells;
	int numiteration;
	cout << "Enter width:" << endl;
	cin >> width;
	cout << "Enter height:" << endl;
	cin >> height;
	cout << "Enter number of alive cells:" << endl;
	cin >> numalivecells;
	posalivecells = new int* [numalivecells];
	for (int i = 0; i < numalivecells; i++) {
		posalivecells[i] = new int[2];
		for (int n = 0; n < 2; n++) {
			cout << "Enter position " << n + 1 << " of alive cell " << i + 1 << ":" << endl;
			cin >> posalivecells[i][n];
		}
	}
	cout << "Enter number of iterations:";
	cin >> numiteration;

	Grid grid(width, height, numalivecells, posalivecells);

	cout << "Initial:" << endl;
	grid.print();


	for (int i = 0; i < numiteration; i++) {
		examine(grid);
		update(grid);
		cout << "Iteration " << i + 1 << ":" << endl;
		grid.print();
		this_thread::sleep_for(chrono::milliseconds(500));
	}

	for (int i = 0; i < numalivecells; i++) {
		delete[] posalivecells[i];
	}
	delete[] posalivecells;

	return 0;
}
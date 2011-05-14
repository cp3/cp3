#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SPACE	0
#define RED		1
#define BLUE	2
#define GREEN	3

// Macros
#define getPiece(r,c) testBoard[skipPadding + r + c * rows]

// Function Prototypes

void readboard(void);
void freeboard(void);

// Global variables
int *board;
int columns;
int rows;
int last_move;
int total_time;
int player_1_time;
int last_move_time;
int skipPadding;
char pieces[4] = { 's', 'r', 'b', 'g' };
int points[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		768, 0, 0, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1024, 0, 0, 768, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 1280, 0, 0, 0,
		0, 0 };

/**
 * Reads in the board and other variables from standard in.
 */
void readboard(void) {
	int i, j;
	char tmpChar = 0;

	scanf("(%d,%d,%d,%d,%d,%d", &columns, &rows, &last_move, &total_time,
			&player_1_time, &last_move_time);
	rows += 6; // Make board bigger for padding
	columns += 6;
	skipPadding = 3 * rows + 3;
	board = (int *) calloc(sizeof(int), rows * columns); // Need to pad the board with spaces
	for (i = 0; i < columns; i++)
		for (j = 0; j < rows; j++) {
			if (i < 3 || i > columns - 4 || j < 3 || j > rows - 4) {
				board[i * rows + j] = SPACE;
			} else {
				scanf(",%c", &tmpChar);

				switch (tmpChar) {
				case 's':
					board[i * rows + j] = SPACE;
					break;
				case 'r':
					board[i * rows + j] = RED;
					break;
				case 'b':
					board[i * rows + j] = BLUE;
					break;
				case 'g':
					board[i * rows + j] = GREEN;
				}
			}
		}
	last_move--; // First column is zero, not one
}

/**
 * Frees the board
 */
void freeboard(void) {
	free(board);
}

/**
 * Prints out the board to standard out.
 * For testing purposes.
 */
void printBoard(int* testBoard) {
	int i, j;
	printf("From input:\n");

	for (i = 0; i < columns - 6; i++) {
		for (j = 0; j < rows - 6; j++) {
			printf("%c", pieces[getPiece(j,i)]);
		}
	}
	printf("\n\n");
	for (i = rows - 1; i >= 0; i--) {
		for (j = 0; j < columns; j++) {
			printf("%c ", pieces[testBoard[i + j * rows]]);
		}
		printf("\n");
	}
	printf("\n\n");
	for (i = rows - 7; i >= 0; i--) {
		for (j = 0; j < columns - 6; j++) {
			printf("%c ", pieces[getPiece(i,j)]);
		}
		printf("\n");
	}
	printf("\n");
}

/**
 * Takes in an array of integers and the last column played
 * Returns an int value of points scored from the board.
 * Negative values are points for the other team.
 */
int isWin(int* testBoard, int lastColumn) {
	int i, a, b, c, d;
	int lastRow;
	int left, right, top, bot;
	unsigned int redPoints = 0, bluePoints = 0, best = 0;
	//possible wins, values from -5 to 5
	unsigned int possible[13], winPoints;
	// Find height of last piece played, may hit padding
	for (i = 0;; i++) {
		//printf("%d ",testBoard[i+lastColumn*rows]);
		if (testBoard[skipPadding + i + lastColumn * rows] == SPACE) {
			lastRow = i - 1;
			break;
		}
	}
	//printf("last piece played at (%d,%d)\n", lastColumn, lastRow);
	//printf("Total of %d columns, %d rows\n", columns, rows);
	// Find boundaries to search, row 0 is bottom row, column 0 is left column
	top = lastRow + 3;
	bot = lastRow - 3;
	left = lastColumn - 3;
	right = lastColumn + 3;

	//printf("Top: %d, Bottom: %d, Right: %d, Left: %d\n", top, bot, right, left);

	// Search vertical
	a = getPiece(lastRow,lastColumn);
	b = getPiece(lastRow-1,lastColumn) << 2;
	c = getPiece(lastRow-2,lastColumn) << 4;
	d = getPiece(lastRow-3,lastColumn) << 6;
	possible[0] = points[a + b + c + d];

	// Search horizontal
	a = getPiece(lastRow,(left));
	b = getPiece(lastRow,(left + 1)) << 2;
	c = getPiece(lastRow,(left + 2)) << 4;
	d = getPiece(lastRow,(left + 3)) << 6;
	possible[1] = points[a + b + c + d];

	a = getPiece(lastRow,(left + 1));
	b = getPiece(lastRow,(left + 2)) << 2;
	c = getPiece(lastRow,(left + 3)) << 4;
	d = getPiece(lastRow,(left + 4)) << 6;
	possible[2] = points[a + b + c + d];

	a = getPiece(lastRow,(left + 2));
	b = getPiece(lastRow,(left + 3)) << 2;
	c = getPiece(lastRow,(left + 4)) << 4;
	d = getPiece(lastRow,(left + 5)) << 6;
	possible[3] = points[a + b + c + d];

	a = getPiece(lastRow,(left + 3));
	b = getPiece(lastRow,(left + 4)) << 2;
	c = getPiece(lastRow,(left + 5)) << 4;
	d = getPiece(lastRow,(left + 6)) << 6;
	possible[4] = points[a + b + c + d];

	// Search diagonal from top left to bottom right
	a = getPiece((lastRow + 0),(lastColumn - 0));
	b = getPiece((lastRow + 1),(lastColumn - 1)) << 2;
	c = getPiece((lastRow + 2),(lastColumn - 2)) << 4;
	d = getPiece((lastRow + 3),(lastColumn - 3)) << 6;
	possible[5] = points[a + b + c + d];

	a = getPiece((lastRow - 1),(lastColumn + 1));
	b = getPiece((lastRow + 0),(lastColumn - 0)) << 2;
	c = getPiece((lastRow + 1),(lastColumn - 1)) << 4;
	d = getPiece((lastRow + 2),(lastColumn - 2)) << 6;
	possible[6] = points[a + b + c + d];

	a = getPiece((lastRow - 2),(lastColumn + 2));
	b = getPiece((lastRow - 1),(lastColumn + 1)) << 2;
	c = getPiece((lastRow + 0),(lastColumn - 0)) << 4;
	d = getPiece((lastRow + 1),(lastColumn - 1)) << 6;
	possible[7] = points[a + b + c + d];

	a = getPiece((lastRow - 3),(lastColumn + 3));
	b = getPiece((lastRow - 2),(lastColumn + 2)) << 2;
	c = getPiece((lastRow - 1),(lastColumn + 1)) << 4;
	d = getPiece((lastRow + 0),(lastColumn - 0)) << 6;
	possible[8] = points[a + b + c + d];

	// Search diagonal from top right to bottom left
	a = getPiece((lastRow + 0),(lastColumn + 0));
	b = getPiece((lastRow + 1),(lastColumn + 1)) << 2;
	c = getPiece((lastRow + 2),(lastColumn + 2)) << 4;
	d = getPiece((lastRow + 3),(lastColumn + 3)) << 6;
	possible[9] = points[a + b + c + d];

	a = getPiece((lastRow - 1),(lastColumn - 1));
	b = getPiece((lastRow + 0),(lastColumn + 0)) << 2;
	c = getPiece((lastRow + 1),(lastColumn + 1)) << 4;
	d = getPiece((lastRow + 2),(lastColumn + 2)) << 6;
	possible[10] = points[a + b + c + d];

	a = getPiece((lastRow - 2),(lastColumn - 2));
	b = getPiece((lastRow - 1),(lastColumn - 1)) << 2;
	c = getPiece((lastRow + 0),(lastColumn + 0)) << 4;
	d = getPiece((lastRow + 1),(lastColumn + 1)) << 6;
	possible[11] = points[a + b + c + d];

	a = getPiece((lastRow - 3),(lastColumn - 3));
	b = getPiece((lastRow - 2),(lastColumn - 2)) << 2;
	c = getPiece((lastRow - 1),(lastColumn - 1)) << 4;
	d = getPiece((lastRow + 0),(lastColumn + 0)) << 6;
	possible[12] = points[a + b + c + d];

	// Add up wins
	winPoints = possible[0] + possible[1] + possible[2] + possible[3]
			+ possible[4] + possible[5] + possible[6] + possible[7]
			+ possible[8] + possible[9] + possible[10] + possible[11]
			+ possible[12];

	if (winPoints) {
		redPoints = winPoints & 255;
		bluePoints = (winPoints & 65280) >> 8;
		//printf ("winpoints: %d, win & number: %d, red: %d, blue: %d", winPoints, (winPoints & 65280) >> 8, redPoints, bluePoints);
		if (redPoints > bluePoints) {
			if (redPoints <= 5)
				return 0 - redPoints;
			for (i = 0; i < 13; i++) {
				if ((possible[i] & 255) > best) {
					best = possible[i] & 255;
				}
			}
			return 0 - best;
		} else if (redPoints < bluePoints) {
			if (bluePoints <= 5)
				return bluePoints;
			for (i = 0; i < 13; i++) {
				if ((possible[i] & 65280) >> 8 > best) {
					best = (possible[i] & 65280) >> 8;
				}
			}
			return best;
		} else {
			return 1;
		}
	}

	return 0;
}

void timeIswin(int times) {
	int i;
	clock_t t1, t2;
	t1 = clock();

	// loop until t2 gets a different value
	for (i = 0; i < times; i++)
		isWin(board, last_move);

	t2 = clock();

	printf("Ran isWin() %d times in %f s.\n", times, (double) (t2 - t1)
			/ CLOCKS_PER_SEC);
	printf("Average time %f µs.\n", (double) (t2 - t1) / CLOCKS_PER_SEC
			* 1000000 / (double) times);
}

// Prints out the array that defines points for piece combinations
// Points set out as 0000 0000 blue 0000 0000 red
void tempPrint() {
	int i, a, b, c, d, val;
	printf("int points[256] = { ");
	for (i = 0; i < 256; i++) {
		val = 0;
		a = i & 3;
		b = (i & 12) >> 2;
		c = (i & 48) >> 4;
		d = (i & 192) >> 6;
		if (a == 1 && b == 1 && c == 3 && d == 3)
			val = 5;
		if (a == 3 && b == 3 && c == 1 && d == 1)
			val = 5;
		if (a == 2 && b == 2 && c == 3 && d == 3)
			val = 5 << 8;
		if (a == 3 && b == 3 && c == 2 && d == 2)
			val = 5 << 8;

		if (a == 1 && b == 3 && c == 3 && d == 1)
			val = 4;
		if (a == 3 && b == 1 && c == 1 && d == 3)
			val = 4;
		if (a == 2 && b == 3 && c == 3 && d == 2)
			val = 4 << 8;
		if (a == 3 && b == 2 && c == 2 && d == 3)
			val = 4 << 8;

		if (a == 1 && b == 3 && c == 1 && d == 3)
			val = 3;
		if (a == 3 && b == 1 && c == 3 && d == 1)
			val = 3;
		if (a == 2 && b == 3 && c == 2 && d == 3)
			val = 3 << 8;
		if (a == 3 && b == 2 && c == 3 && d == 2)
			val = 3 << 8;

		printf("%d, ", val);
	}
	printf("};");
}

/**
 * Calls functions to read in board etc.
 */
int main(void) {
	//int col;
	//char p;

	readboard();

	//do
	//	col = random() % columns;
	//while (board[col * rows + rows - 1] != 's');

	//p = 'b';

	printBoard(board);
	//timeIswin(10000000);
	printf("%d\n", isWin(board, last_move));
	//tempPrint();
	freeboard();

	//printf("(%d,%c)", col+1, p);

	return 0;
}


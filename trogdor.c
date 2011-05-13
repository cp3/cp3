#include <stdio.h>
#include <sys/time.h>

#define SPACE	1
#define RED		2
#define BLUE	4
#define GREEN	8

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

/**
 * Reads in the board and other variables from standard in.
 */
void readboard(void) {
	int i, j;
	char tmpChar = 0;

	scanf("(%d,%d,%d,%d,%d,%d", &columns, &rows, &last_move, &total_time,
			&player_1_time, &last_move_time);
	last_move--; // First column is zero, not one
	board = (int *) calloc(sizeof(int), rows * columns);
	for (i = 0; i < columns; i++)
		for (j = 0; j < rows; j++) {
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
	printf("\n");
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
	for (i = 0; i < rows * columns; i++) {
		printf("%d", testBoard[i]);
	}
	printf("\n\n");
	for (i = rows - 1; i >= 0; i--) {
		for (j = 0; j < columns; j++) {
			printf("%d ", testBoard[i + j * rows]);
		}
		printf("\n");
	}
}

/**
 * Takes in an array of integers and the last column played
 * Returns an int value of points scored from the board.
 * Negative values are points for the other team.
 */
int isWin(int* testBoard, int lastColumn) {
	int i, j, k, lastRow = rows - 1, top, bot, left, right;
	// Total and maximal win points for red and blue
	int totalRed = 0, totalBlue = 0, maxRed = 0, maxBlue = 0;
	// Find height of last piece played
	for (i = 0; i < rows; i++) {
		//printf("%d ",testBoard[i+lastColumn*rows]);
		if (testBoard[i + lastColumn * rows] == SPACE) {
			lastRow = i - 1;
			break;
		}
	}
	printf("last piece played at (%d,%d)\n", lastColumn, lastRow);
	printf("Total of %d columns, %d rows\n", columns, rows);
	// Find boundaries to search, row 0 is bottom row, column 0 is left column
	top = lastRow + 3;
	bot = lastRow - 3;
	left = lastColumn - 3;
	right = lastColumn + 3;

	// Boundaries cannot be outside of map
	if (top > rows - 1)
		top = rows - 1;
	if (bot < 0)
		bot = 0;
	if (right > columns - 1)
		right = columns - 1;
	if (left < 0)
		left = 0;

	printf("Top: %d, Bottom: %d, Right: %d, Left: %d\n", top, bot, right, left);

	// Print board to be searched
	for (i = top; i >= bot; i--) {
		for (j = left; j <= right; j++) {
			printf("%d ", testBoard[i + j * rows]);
		}
		printf("\n");
	}

	// Search vertical
	printf("Checking vertical:\n");
	if (lastRow - bot == 3)
		for (i = 0; i < 4; i++)
			printf("%d ", testBoard[lastColumn * rows + lastRow - i]);
	printf("\n\n");

	// Search horizontal
	printf("Checking horizontal:\n");
	for (i = left; i <= right - 3; i++) {
		for (j = 0; j < 4; j++) {
			printf("%d ", testBoard[(i + j) * rows + lastRow]);
		}
		printf("\n");
	}
	printf("\n");

	// Search diagonal from top left to bottom right
	printf("Checking diagonal:\n");
	for (i = lastColumn, j = lastRow; i <= right && j >= bot; i++, j--) {
		if (i - 3 >= left && j + 3 <= top) {
			for (k = 3; k >= 0; k--) {
				printf("%d ", testBoard[(i - k) * rows + j + k]);
			}
			printf("\n");
		}
	}
	printf("\n");

	// Search diagonal from top right to bottom left
	printf("Checking diagonal:\n");
	for (i = lastColumn, j = lastRow; i >= left && j >= bot; i--, j--) {
		if (i + 3 <= right && j + 3 <= top) {
			for (k = 3; k >= 0; k--) {
				printf("%d ", testBoard[(i + k) * rows + j + k]);
			}
			printf("\n");
		}
	}
	printf("\n");

	return 0;
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
	isWin(board, last_move);

	freeboard();

	//printf("(%d,%c)", col+1, p);

	return 0;
}


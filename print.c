#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SPACE	0
#define RED		1
#define BLUE	2
#define GREEN	3
#define PADDING	6
#define MAX_DEPTH 2

// Macros
#define getPiece(r,c) board[skipPadding + r + c * rows]
#define boardSize(x) (x - PADDING);

// Function Prototypes

void readboard(void);
void freeboard(void);
void printboard(void);
int getTop(int column);
void addPiece(int col, int colour);
void remPiece(int col);
unsigned int burninate(int player, int depth, int origDepth);
int isWin(int lastColumn);

// Global variables
int *board;
int columns;
int rows;
int last_move;
int total_time;
int player_1_time;
int last_move_time;
int skipPadding;
int *columnHeight;
char pieces[4] = { 's', 'r', 'b', 'g' };
int almostPoints[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 256, 0, 0, 256, 512, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 512, 256, 0, 1,
		256, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 2, 0,
		0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 256, 0, 0, 256, 512, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 512, 0, 0, 0, 0, 0, 0, 0, 0, 512, 0, 0, 0, 0,
		0, 256, 256, 0, 0, 0, 0, 256, 0, 0, 0, 256, 0, 0, 0, 0, 0, 0, 0, 0, 2,
		0, 1, 0, 0, 512, 256, 0, 1, 256, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 256, 256, 0, 0, 0, 0, 256, 0, 0, 0, 256, 0, 0, 0, 0,
		2, 512, 0, 2, 0, 0, 0, 512, 0, 0, 0, 0, 0, 0, 0 };
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
	columnHeight = (int *) calloc(sizeof(int), columns);
	rows += PADDING; // Make board bigger for padding
	columns += PADDING;
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
					columnHeight[i - 3] = j - 2;
					board[i * rows + j] = RED;
					break;
				case 'b':
					columnHeight[i - 3] = j - 2;
					board[i * rows + j] = BLUE;
					break;
				case 'g':
					columnHeight[i - 3] = j - 2;
					board[i * rows + j] = GREEN;
				}
			}
		}
	if (last_move)
		last_move--; // First column is zero, not one
}

/**
 * Frees the board
 */
void freeboard(void) {
	free(board);
	free(columnHeight);
}

/**
 * Prints out the board to standard out.
 * For testing purposes.
 */
void printBoard() {
	int i, j;
	//	fprintf(stderr, "From input:\n");
	//
	//	for (i = 0; i < columns - PADDING; i++) {
	//		for (j = 0; j < rows - PADDING; j++) {
	//			printf("%c", pieces[getPiece(j,i)]);
	//		}
	//	}
	//	fprintf(stderr, "\nTop Pieces:\n");
	//	for (i = 0; i < columns - PADDING; i++) {
	//		printf("%d ", columnHeight[i]);
	//	}
	//	fprintf(stderr, "\n\n");
	//	for (i = rows - 1; i >= 0; i--) {
	//	 for (j = 0; j < columns; j++) {
	//	 printf("%c ", pieces[board[i + j * rows]]);
	//	 }
	//	 printf("\n");
	//	 }
	//	 printf("\n\n");
	for (i = rows - 7; i >= 0; i--) {
		for (j = 0; j < columns - PADDING; j++) {
			fprintf(stdout, "%c ", pieces[getPiece(i,j)]);
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n\n");
}

/**
 * Calls functions to read in board etc.
 */
int main(void) {
	int col, move;
	int totMoves = 0, i;
	char p;

	readboard();
	printBoard();

	return 0;
}

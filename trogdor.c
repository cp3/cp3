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
#define boardSize(x) (x - PADDING)
#define setReturnVal(future,points,column,played) ((future << 10) + (points << 6) + (column << 2) + played)
#define setReturnValScore(score,future,points,column,played) ((score << 14) + (future << 10) + (points << 6) + (column << 2) + played)
#define getScore(x) ((x >> 14) & 127)
#define getFuture(x) ((x >> 10) & 15)
#define getPoints(x) ((x >> 6) & 15)
#define getColumn(x) ((x >> 2) & 15)
#define getPlayed(x) (x & 3)

// Function Prototypes

void readboard(void);
void freeboard(void);
void printboard(void);
void printWin(void);
int getTop(int column);
void addPiece(int col, int colour);
void remPiece(int col);
int burninate(int player, int depth, int origDepth);
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
int almostPoints[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 256, 0, 0, 256, 256, 0,
		0, 0, 0, 0, 1, 0, 1, 0, 0, 256, 256, 0, 1, 256, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 512, 0, 0, 512, 512, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 512, 0, 0, 0, 0, 0, 0, 0, 0, 256, 0, 0, 0, 0, 0, 512, 512, 0, 0, 0, 0, 256, 0, 0, 0, 256, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 512, 512, 0, 2, 512, 0, 0, 2,
		0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 512, 512, 0, 0, 0, 0, 256, 0, 0, 0, 256, 0, 0, 0, 0, 2, 512, 0, 1, 0, 0, 0, 256, 0, 0, 0, 0, 0, 0, 0 };
int points[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 768, 0, 0, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0,
		3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1024, 0, 0, 768, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 1280, 0, 0, 0, 0, 0 };

/**
 * Reads in the board and other variables from standard in.
 */
void readboard(void) {
	int i, j;
	char tmpChar = 0;

	scanf("(%d,%d,%d,%d,%d,%d", &columns, &rows, &last_move, &total_time, &player_1_time, &last_move_time);
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
		for (j = 0; j < boardSize(columns); j++) {
			fprintf(stderr, "%c ", pieces[getPiece(i,j)]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n\n");
}

/**
 * Takes in the last column played
 * Returns an int value of points scored from the board.
 * Negative values are points for the other team.
 */
int isWin(int lastColumn) {
	int i, a, b, c, d;
	int lastRow;
	int left, right, top, bot;
	int redPoints = 0, bluePoints = 0, best = 0;
	//possible wins, values from -5 to 5
	int possible[13], winPoints;
	// Find height of last piece played
	lastRow = getTop(lastColumn) - 1;
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
	winPoints = possible[0] + possible[1] + possible[2] + possible[3] + possible[4] + possible[5] + possible[6] + possible[7] + possible[8] + possible[9] + possible[10]
			+ possible[11] + possible[12];

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
					if (best == 5)
						break;
				}
			}
			return 0 - best;
		} else if (redPoints < bluePoints) {
			if (bluePoints <= 5)
				return bluePoints;
			for (i = 0; i < 13; i++) {
				if ((possible[i] & 65280) >> 8 > best) {
					best = (possible[i] & 65280) >> 8;
					if (best == 5)
						break;
				}
			}
			return best;
		} else {
			return 1;
		}
	}

	return 0;
}

/**
 * Takes in the last column played
 * Returns an int value of near wins.
 * Negative values are points for the other team.
 */
int isAlmostWin(int lastColumn) {
	int a, b, c, d;
	int lastRow;
	int left, right, top, bot;
	int redPoints = 0, bluePoints = 0;
	//possible wins, values from -5 to 5
	int possible[13], winPoints;
	// Find height of last piece played
	lastRow = getTop(lastColumn) - 1;
	//printf("last piece played at (%d,%d)\n", lastColumn, lastRow);
	//printf("Total of %d columns, %d rows\n", columns, rows);
	// Find boundaries to search, row 0 is bottom row, column 0 is left column
	top = lastRow + 3;
	bot = lastRow - 3;
	left = lastColumn - 3;
	right = lastColumn + 3;

	//printf("Top: %d, Bottom: %d, Right: %d, Left: %d\n", top, bot, right, left);

	// Search vertical
	//	a = getPiece(lastRow,lastColumn);
	//	b = getPiece(lastRow-1,lastColumn) << 2;
	//	c = getPiece(lastRow-2,lastColumn) << 4;
	//	d = getPiece(lastRow-3,lastColumn) << 6;
	possible[0] = 0;

	// Search horizontal
	a = getPiece(lastRow,(left));
	b = getPiece(lastRow,(left + 1)) << 2;
	c = getPiece(lastRow,(left + 2)) << 4;
	d = getPiece(lastRow,(left + 3)) << 6;
	possible[1] = almostPoints[a + b + c + d];

	a = getPiece(lastRow,(left + 1));
	b = getPiece(lastRow,(left + 2)) << 2;
	c = getPiece(lastRow,(left + 3)) << 4;
	d = getPiece(lastRow,(left + 4)) << 6;
	possible[2] = almostPoints[a + b + c + d];

	a = getPiece(lastRow,(left + 2));
	b = getPiece(lastRow,(left + 3)) << 2;
	c = getPiece(lastRow,(left + 4)) << 4;
	d = getPiece(lastRow,(left + 5)) << 6;
	possible[3] = almostPoints[a + b + c + d];

	a = getPiece(lastRow,(left + 3));
	b = getPiece(lastRow,(left + 4)) << 2;
	c = getPiece(lastRow,(left + 5)) << 4;
	d = getPiece(lastRow,(left + 6)) << 6;
	possible[4] = almostPoints[a + b + c + d];

	// Search diagonal from top left to bottom right
	a = getPiece((lastRow + 0),(lastColumn - 0));
	b = getPiece((lastRow + 1),(lastColumn - 1)) << 2;
	c = getPiece((lastRow + 2),(lastColumn - 2)) << 4;
	d = getPiece((lastRow + 3),(lastColumn - 3)) << 6;
	possible[5] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 1),(lastColumn + 1));
	b = getPiece((lastRow + 0),(lastColumn - 0)) << 2;
	c = getPiece((lastRow + 1),(lastColumn - 1)) << 4;
	d = getPiece((lastRow + 2),(lastColumn - 2)) << 6;
	possible[6] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 2),(lastColumn + 2));
	b = getPiece((lastRow - 1),(lastColumn + 1)) << 2;
	c = getPiece((lastRow + 0),(lastColumn - 0)) << 4;
	d = getPiece((lastRow + 1),(lastColumn - 1)) << 6;
	possible[7] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 3),(lastColumn + 3));
	b = getPiece((lastRow - 2),(lastColumn + 2)) << 2;
	c = getPiece((lastRow - 1),(lastColumn + 1)) << 4;
	d = getPiece((lastRow + 0),(lastColumn - 0)) << 6;
	possible[8] = almostPoints[a + b + c + d];

	// Search diagonal from top right to bottom left
	a = getPiece((lastRow + 0),(lastColumn + 0));
	b = getPiece((lastRow + 1),(lastColumn + 1)) << 2;
	c = getPiece((lastRow + 2),(lastColumn + 2)) << 4;
	d = getPiece((lastRow + 3),(lastColumn + 3)) << 6;
	possible[9] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 1),(lastColumn - 1));
	b = getPiece((lastRow + 0),(lastColumn + 0)) << 2;
	c = getPiece((lastRow + 1),(lastColumn + 1)) << 4;
	d = getPiece((lastRow + 2),(lastColumn + 2)) << 6;
	possible[10] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 2),(lastColumn - 2));
	b = getPiece((lastRow - 1),(lastColumn - 1)) << 2;
	c = getPiece((lastRow + 0),(lastColumn + 0)) << 4;
	d = getPiece((lastRow + 1),(lastColumn + 1)) << 6;
	possible[11] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 3),(lastColumn - 3));
	b = getPiece((lastRow - 2),(lastColumn - 2)) << 2;
	c = getPiece((lastRow - 1),(lastColumn - 1)) << 4;
	d = getPiece((lastRow + 0),(lastColumn + 0)) << 6;
	possible[12] = almostPoints[a + b + c + d];

	// Add up wins
	winPoints = possible[0] + possible[1] + possible[2] + possible[3] + possible[4] + possible[5] + possible[6] + possible[7] + possible[8] + possible[9] + possible[10]
			+ possible[11] + possible[12];

	if (winPoints) {
		redPoints = winPoints & 255;
		bluePoints = (winPoints & 65280) >> 8;
		return bluePoints - redPoints;
	}

	return 0;
}

int getTop(int column) {
	int top, i;
	for (i = 0;; i++) {
		if (getPiece(i,column) == SPACE) {
			top = i;
			return top;
		}
	}
}

//Should make this function better
int columnWins() {
	int points = 0;
	int i, j;
	for (i = 0; i < boardSize(columns); i++) {
		if (columnHeight[i] >= boardSize(rows))
			continue;

		int placed = 1;
		int lastBlue = 0;
		int lastRed = 0;
		addPiece(i, RED);
		while (columnHeight[i] < boardSize(rows)) {
			int soFar = 0;
			int soFarR = 0;
			addPiece(i, GREEN);
			placed++;
			if (isWin(i) > 2) {
				points++;
				if (lastBlue) {
					points += 10;
					break;
				} else {
					soFar = 1;
				}
			} else if (isWin(i) < -2) {
				points--;
				if (lastRed) {
					points -= 10;
					break;
				} else {
					soFarR = 1;
				}
			}
			remPiece(i);
			addPiece(i, BLUE);
			if (isWin(i) > 2) {
				points++;
				if (lastBlue) {
					points += 10;
					break;
				} else {
					soFar = 1;
				}
			}
			lastBlue = soFar;
			lastRed = soFarR;
		}
		for (j = 0; j < placed; j++) {
			remPiece(i);
		}
	}
	//printBoard();
	//fprintf(stderr, "Points: %d\n",points);
	return 25 + points;
}

int burninate(int player, int depth, int origDepth) {
	int i, points, colour, future;
	int score = 0;
	int fourMove;
	int threeMove;
	int oneMove;
	int otherMove;
	int neutralMoves[21];
	int enemy = 1;
	int max = -6;
	if (player == RED) {
		enemy = -1;
	}
	int original = 0;
	if (depth == origDepth) {
		original = 1;
		future = 0;
	} else {
		future = origDepth - depth;
	}
	neutralMoves[0] = 0;

	colour = player;
	for (i = 0; i < boardSize(columns); i++) {
		if (columnHeight[i] >= boardSize(rows))
			continue;
		addPiece(i, colour);
		points = enemy * isWin(i);
		remPiece(i);
		if (points > max) {
			max = points;
			if (points == 5) {
				return setReturnVal(future,5,i,colour);
			}
			if (points == 4) {
				fourMove = setReturnVal(future,4,i,colour);
			}
			if (points == 3) {
				threeMove = setReturnVal(future,3,i,colour);
			}
			if (points == 1) {
				oneMove = setReturnVal(future,1,i,colour);
			}
		}
		if (points == 0) {
			if (future == 2) {
				score = columnWins();
				//fprintf(stderr, "%d\n",score);
			} else {
				score = 0;
			}
			neutralMoves[neutralMoves[0] + 1] = setReturnValScore(score,future,0,i,colour);
			neutralMoves[0]++;
		}
	}
	colour = GREEN;
	for (i = 0; i < boardSize(columns); i++) {
		if (columnHeight[i] >= boardSize(rows))
			continue;
		addPiece(i, colour);
		points = enemy * isWin(i);
		remPiece(i);
		if (points > max) {
			max = points;
			if (points == 5) {
				return setReturnVal(future,5,i,colour);
			}
			if (points == 4) {
				fourMove = setReturnVal(future,4,i,colour);
			}
			if (points == 3) {
				threeMove = setReturnVal(future,3,i,colour);
			}
			if (points == 1) {
				oneMove = setReturnVal(future,1,i,colour);
			}
		}
		if (points == 0) {
			if (future == 2) {
				score = columnWins();
				//fprintf(stderr, "%d\n",score);
			} else {
				score = 0;
			}
			neutralMoves[neutralMoves[0] + 1] = setReturnValScore(score,future,0,i,colour);
			neutralMoves[0]++;
		}
	}
	int bestLoss = 6;
	int farLoss = 0;
	int loss;

	otherMove = neutralMoves[1];
	int nonLosingMoves[21];
	nonLosingMoves[0] = 0;
	int opWorst = 100; // for opponent turn

	if (depth > 0 && max < 1 || original) {
		for (i = 0; i < neutralMoves[0]; i++) {
			int col = getColumn(neutralMoves[i + 1]);
			int p = getPlayed(neutralMoves[i + 1]);
			//fprintf(stderr,"Neutral move: %d, %c\n", col, pieces[p]);
			addPiece(col, p);
			int opponentTurn = burninate(3 - player, depth - 1, origDepth);
			int opponentScore = getPoints(opponentTurn);
			remPiece(col);

			if (opponentScore < 1) {
				if (future == 1 && getScore(opponentTurn) < opWorst) {
					//fprintf(stderr, "Found: %d\n", score);
					opWorst = getScore(opponentTurn);
				}
				if (original) {
					//fprintf(stderr, "Found: %d\n", getScore(opponentTurn));
					nonLosingMoves[nonLosingMoves[0] + 1] = setReturnValScore(getScore(opponentTurn),0,0,0,neutralMoves[i + 1]);
				} else {
					nonLosingMoves[nonLosingMoves[0] + 1] = neutralMoves[i + 1];
				}
				nonLosingMoves[0]++;
				//fprintf(stderr, "Neutral move: %d, %c Score %d\n", col, pieces[p], getScore(opponentTurn));
			} else if (opponentScore > 5) {
				//it's a win!
				points = opponentScore - 3;
				if (points > max) {
					max = points;
					if (points == 5) {
						return setReturnVal(getFuture(opponentTurn),5,col,p);
					}
					if (points == 4) {
						fourMove = setReturnVal(future,4,col,p);
					}
					if (points == 3) {
						threeMove = setReturnVal(future,3,col,p);
					}
					if (points == 1) {
						oneMove = setReturnVal(future,1,col,p);
					}
				}
			} else if (opponentScore > 0) {
				//it's a loss!
				if ((opponentScore <= bestLoss && getFuture(opponentTurn) >= farLoss) || (getFuture(opponentTurn) > farLoss) && getFuture(opponentTurn) <= 5) {
					bestLoss = opponentScore;
					farLoss = getFuture(opponentTurn);
					loss = setReturnVal(getFuture(opponentTurn),opponentScore+3,col,p);
					//
				}
			}

		}
	}
	if (max == 0 && nonLosingMoves[0] > 0) {
		if (original) {
			int centre;
			int centreX = boardSize(columns) / 2;
			int centreY = boardSize(rows) / 2;
			int closest = boardSize(columns);
			int best = -5;
			for (i = 0; i < nonLosingMoves[0]; i++) {
				int col = getColumn(nonLosingMoves[i + 1]);
				int p = getPlayed(nonLosingMoves[i + 1]);
				//addPiece(col, p);
				int score = getScore(nonLosingMoves[i + 1]);
				//int score = columnWins();
				int row = columnHeight[col];

				//remPiece(col);
				int current = abs(col - centreX) + 2 * abs(row - centreY);

				if (score > best || (score >= best && current < closest)) {
					closest = current;
					best = score;
					//fprintf(stderr, "Best score is %d with %d, %c\nCurrent: %d, Closest %d\n", best, col, pieces[p], current, closest);
					centre = setReturnVal(future,0,col,p);
				}

			}
			otherMove = centre;
		}
	} else {
		//we're gonna lose now... probably
		if (bestLoss < 6) {
			otherMove = loss;
		}
	}

	if (max == 4) {
		return fourMove;
	}
	if (max == 3) {
		return threeMove;
	}
	if (max == 1) {
		return oneMove;
	}
	//opponent wants our worst move
	if (future == 1) {
		otherMove = setReturnValScore(opWorst,0,0,0,otherMove);
		//fprintf(stderr, "Worst: %d\n", opWorst);
	}
	//No moves left!
	return otherMove;
}

void addPiece(int col, int colour) {
	getPiece(columnHeight[col],col) = colour;
	columnHeight[col]++;
}

void remPiece(int col) {
	getPiece(columnHeight[col]-1,col) = SPACE;
	columnHeight[col]--;
}

void testMacros() {
	int future, score, column, played, x;
	for (future = 0; future < 16; future++) {
		for (score = 0; score < 16; score++) {
			for (column = 0; column < 16; column++) {
				for (played = 0; played < 4; played++) {
					x = setReturnVal(future,score,column,played);
					if (future != getFuture(x) || score != getPoints(x) || column != getColumn(x) || played != getPlayed(x)) {
						printf("Broke on:\nfuture: %d\nscore: %d\ncolumn: %d\nplayed: %d\n\n", future, score, column, played);
					}

				}
			}
		}
	}
}

//void printWin() {
//	fprintf(stderr, "                     __               ) \n");
//	fprintf(stderr, "            _  \    | _\             Q)  / \n");
//	fprintf(stderr, "           / \  \   /  (            Q)  / \n");
//	fprintf(stderr, "          /_ |     / _/     \ /     )       /| \n");
//	fprintf(stderr, "   \      \- |     |/       .V.    _       / |______ \n");
//	fprintf(stderr, "    \      \_\_/---------_________/o\     /        / \n");
//	fprintf(stderr, "            ( |                      |   / /|__   /_____ \n");
//	fprintf(stderr, "         __/  |      _-____   /V-V-V-V    /   /     ___/ \n");
//	fprintf(stderr, "        (      |   v |     \  \^_^_^          \    <______ \n");
//	fprintf(stderr, "  ___    \  )   \    v \    \_____)     |\ |\ \    _______\ \n");
//	fprintf(stderr, "         ( / __/  \  vv  \              | \| \|    \ \n");
//	fprintf(stderr, "         /  \       \  vv  \            \       |\  \ \n");
//	fprintf(stderr, "        ( |  \ _      \      \           \  |\  | \  \ \n");
//	fprintf(stderr, "         \ \   _)        \     \          \ | \ |  \ | \n");
//	fprintf(stderr, "           \   _)         |     |          \|  \|   \| \n");
//	fprintf(stderr, "        /    \__)       |\|    | \n");
//	fprintf(stderr, "       /        ^ /\  |\|/    / \n");
//	fprintf(stderr, "                \\__\_|/    /         TROGDOR \n");
//	fprintf(stderr, "                 \_________/            the \n");
//	fprintf(stderr, "                      |  |           BURNiNATOR \n");
//	fprintf(stderr, "                      |  |____ \n");
//	fprintf(stderr, "                      |___ \n");
//}

/**
 * Calls functions to read in board etc.
 */
int main(void) {
	int col, move;
	int totMoves = 0, i;
	char p;
	int remColumns;
	int extra = 0;

	readboard();
	remColumns = boardSize(columns);
	//printBoard();

	for (i = 0; i < boardSize(columns); i++) {
		totMoves += columnHeight[i];
		if (columnHeight[i] == boardSize(rows))
			remColumns--;
	}
	//fprintf(stderr, "Columns: %d\n", remColumns);
	extra = (11 - remColumns) / 2; //look deeper

	if (totMoves == 0) {
		p = pieces[BLUE];
		col = 4;
	} else {
		move = burninate(BLUE, 4 + extra, 4 + extra);
		col = getColumn(move);
		p = pieces[getPlayed(move)];
	}

	freeboard();
	//testMacros();
	printf("(%d,%c)", col + 1, p);

	return 0;
}


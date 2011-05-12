#include <stdio.h>
#include <sys/time.h>

#define SPACE	1
#define RED		2
#define BLUE	4
#define GREEN	8

/******************************************************************************\
* Function Prototypes                                                          *
\******************************************************************************/
void readboard(void);
void freeboard(void);

/******************************************************************************\
* Global Variables                                                             *
\******************************************************************************/
int *board;
int columns;
int rows;
int last_move;
int total_time;
int player_1_time;
int last_move_time;

/******************************************************************************\
* readboard                                                                    *
\******************************************************************************/
void readboard(void)
{
	int i, j;
	char tmpChar = 0;

	scanf("(%d,%d,%d,%d,%d,%d", &columns, &rows, &last_move, &total_time, &player_1_time, &last_move_time);
	board = (int *)calloc(sizeof(int), rows * columns);
	for (i = 0; i < columns; i++)
		for (j = 0; j < rows; j++) {
			scanf(",%c", &tmpChar);

			switch(tmpChar) {
				case 's':
					board[i*rows+j] = SPACE;
					break;
				case 'r':
					board[i*rows+j] = RED;
					break;
				case 'b':
					board[i*rows+j] = BLUE;
					break;
				case 'g': board[i*rows+j] = GREEN;
			}
		}
		printf("\n");
} /* readboard */

/******************************************************************************\
* freeboard                                                                    *
\******************************************************************************/
void freeboard(void)
{
	free(board);
} /* freeboard */

/******************************************************************************\
* printBoard                                                                *
\******************************************************************************/
void printBoard(int* testBoard)
{
	int i,j;
	printf("From input:\n");
	for (i = 0; i < rows*columns; i++) {
		printf("%d", testBoard[i]);
	}
	printf("\n\n");
	for (i = rows-1; i >= 0; i--) {
		for (j = 0; j < columns; j++) {
			printf("%d ", testBoard[i+j*rows]);
		}
		printf("\n");
	}
} /* printBoard */

/******************************************************************************\
* isWin                                                                 *
\******************************************************************************/
int isWin(int* testBoard, int lastColumn)
{
	int i, j, k, lastRow = rows-1, top, bot, left, right;
	// Find height of last piece played
	for (i=0; i < rows; i++) {
		//printf("%d ",testBoard[i+lastColumn*rows]);
		if (testBoard[i+lastColumn*rows] == SPACE) {
			lastRow = i-1;
			break;
		}
	}
	printf("last piece played at (%d,%d)\n",lastColumn, lastRow);
	printf("Total of %d columns, %d rows\n",columns, rows);
	// Find boundaries to search, row 0 is bottom row, column 0 is left column
	top = lastRow + 3;
	bot = lastRow - 3;
	left = lastColumn - 3;
	right = lastColumn + 3;

	// Boundaries cannot be outside of map
	if (top > rows-1) top = rows-1;
	if (bot < 0) bot = 0;
	if (right > columns-1) right = columns-1;
	if (left < 0) left = 0;

	printf("Top: %d, Bottom: %d, Right: %d, Left: %d\n",top, bot, right, left);

	// Print board to be searched
	for (i = top; i >= bot; i--) {
		for (j = left; j <= right; j++) {
			printf("%d ", testBoard[i+j*rows]);
		}
		printf("\n");
	}

	// Search vertical
	printf("Checking vertical:\n");
	if (lastRow-bot == 3)
		for (i=0; i<4; i++)
			printf("%d ", testBoard[lastColumn*rows+lastRow-i]);
	printf("\n\n");

	// Search horizontal
	printf("Checking horizontal:\n");
	for (i=left; i<=right-3; i++) {
		for(j=0; j<4; j++) {
			printf("%d ", testBoard[(i+j)*rows+lastRow]);
		}
		printf("\n");
	}
	printf("\n");

	// Search diagonal from top left to bottom right
	printf("Checking diagonal:\n");
	for (i=lastColumn, j=lastRow; i<=right && j>=bot; i++, j--) {
		if (i-3 >= left && j+3 <= top) {
			for(k=3; k>=0; k--) {
				printf("%d ", testBoard[(i-k)*rows+j+k]);
			}
			printf("\n");
		}
	}
	printf("\n");

	// Search diagonal from top right to bottom left
	printf("Checking diagonal:\n");
	for (i=lastColumn, j=lastRow; i>=left && j>=bot; i--, j--) {
		if (i+3 <= right && j+3 <= top) {
			for(k=3; k>=0; k--) {
				printf("%d ", testBoard[(i+k)*rows+j+k]);
			}
			printf("\n");
		}
	}
	printf("\n");

	/*int horizontal = rows*(columns-3), vertical = columns*(rows-3), diagonal = 2*(rows-3)*(columns-3);
	char quads[horizontal+vertical+diagonal][4];
	printf("Horizontal: %d\nVertical %d\nDiagonal %d\n",horizontal, vertical, diagonal);
	// Do horizontal sections
	for (i = rows-1; i > 0; i--) {
		for (j = 0; j < columns-3; j++) {
			for (k = 0; k < 4; k++, l++) {
				//quads[l][k] = testBoard[i+(j+k)*rows];
				//printf("%c ", testBoard[i+(j+k)*rows]);
			}
			//printf("\n");
		}
		//printf("\n");
	}
	// TODO: Do vertical sections
	for (i = 0; i < columns; i++) {
		for (j = 0; j < rows-3; j++) {
			for (k = 0; k < 4; k++, l++) {
				//quads[l][k] = testBoard[k+j+i*rows];
				printf("%c ", testBoard[k+j+i*rows]);
				break;
			}
			printf("\n");
		}
		printf("\n");
	}*/
	return 0;
	// TODO: Do diagonal sections
} /* isWin */

/******************************************************************************\
* main                                                                         *
\******************************************************************************/
int main(void)
{
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
} /* main */


#include <stdio.h>
#include <sys/time.h>

/******************************************************************************\
* Function Prototypes                                                          *
\******************************************************************************/
void readboard(void);
void freeboard(void);

/******************************************************************************\
* Global Variables                                                             *
\******************************************************************************/
char *board;
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

	scanf("(%d,%d,%d,%d,%d,%d", &columns, &rows, &last_move, &total_time, &player_1_time, &last_move_time);
	board = (char *)calloc(sizeof(char), rows * columns);
	for (i = 0; i < columns; i++)
		for (j = 0; j < rows; j++)
			scanf(",%c", &board[i*rows+j]);
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
int printBoard(char* testBoard)
{
	int i,j;
	printf("From input:\n");
	for (i = 0; i < rows*columns; i++) {
		printf("%c", testBoard[i]);
	}
	printf("\n\n");
	for (i = rows-1; i > 0; i--) {
		for (j = 0; j < columns; j++) {
			printf("%c ", testBoard[i+j*rows]);
		}
		printf("\n");
	}

} /* printBoard */

/******************************************************************************\
* isWin                                                                 *
\******************************************************************************/
int isWin(char* testBoard)
{
	int i,j,k,l = 0;
	int horizontal = rows*(columns-3), vertical = columns*(rows-3), diagonal = 2*(rows-3)*(columns-3);
	char quads[horizontal+vertical+diagonal][4];
	printf("Horizontal: %d\nVertical %d\nDiagonal %d\n",horizontal, vertical, diagonal);
	// Do horizontal sections
	for (i = rows-1; i > 0; i--) {
		for (j = 0; j < columns-3; j++) {
			for (k = 0; k < 4; k++, l++) {
				quads[l][k]
				printf("%c ", testBoard[i+(j+k)*rows]);
			}
			printf("\n");
		}
		printf("\n");
	}
	// TODO: Do vertical sections
	// TODO: Do diagonal sections
} /* isWin */

/******************************************************************************\
* main                                                                         *
\******************************************************************************/
int main(void)
{
	int col;
	char p;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	srandom(getpid() ^ ~getuid() ^ tv.tv_sec ^ tv.tv_usec);

	readboard();
	
	do
		col = random() % columns;
	while (board[col * rows + rows - 1] != 's');

	if (random() % 8)
		p = 'b';
	else
		p = 'g';

	printBoard(board);
	isWin(board);

	freeboard();

	printf("(%d,%c)", col+1, p);

	return 0;
} /* main */


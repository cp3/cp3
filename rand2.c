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
int last_column;
int total_time;
int player_1_time;
int last_move_time;

/******************************************************************************\
* readboard                                                                    *
\******************************************************************************/
void readboard(void)
{
	int i, j;
	char c, lc;

	/* Get the start of board character */
	while ((c = getchar()) != '(') ;

	/* Get the number of columns */
	columns = 0;
	while ((c = getchar()) != ',')
	{
		if ((c >= '0') && (c <= '9'))
			columns = (columns * 10) + c - '0';
	} /* while */

	/* Get the number of rows */
	rows = 0;
	while ((c = getchar()) != ',')
	{
		if ((c >= '0') && (c <= '9'))
			rows = (rows * 10) + c - '0';
	} /* while */

	/* Get the last move */
	last_column = 0;
	while ((c = getchar()) != ',')
	{
		if ((c >= '0') && (c <= '9'))
			last_column = (last_column * 10) + c - '0';
	} /* while */

	/* Get the total game time */
	total_time = 0;
	while ((c = getchar()) != ',')
	{
		if ((c >= '0') && (c <= '9'))
			total_time = (total_time * 10) + c - '0';
	} /* while */

	/* Get the player 1 time */
	player_1_time = 0;
	while ((c = getchar()) != ',')
	{
		if ((c >= '0') && (c <= '9'))
			player_1_time = (player_1_time * 10) + c - '0';
	} /* while */

	/* Get the last move time */
	last_move_time = 0;
	while ((c = getchar()) != ',')
	{
		if ((c >= '0') && (c <= '9'))
			last_move_time = (last_move_time * 10) + c - '0';
	} /* while */

	/* Allocate the appropriate space for the board */
	board = (char *)calloc(sizeof(char), rows * columns);

	/* Get the board contents */
	i = 0;
	while ((c = getchar()) != ')')
	{
		if (c == ',')
			board[i++] = lc;
		else if (c != ' ')
			lc = c;
	} /* while */

	/* Check to make sure everything is valid */
	board[i++] = lc;
	if (i != rows * columns)
		fprintf(stderr, "ERROR, #pieces does not match board size\n");
} /* readboard */

/******************************************************************************\
* freeboard                                                                    *
\******************************************************************************/
void freeboard(void)
{
	free(board);
} /* freeboard */

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

	freeboard();

	printf("(%d, %c)", col+1, p);

	return 0;
} /* main */


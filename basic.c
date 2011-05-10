#include <stdio.h>
#include <sys/time.h>
#include <values.h>

/******************************************************************************\
* Basic AI implementation                                                      *
*                                                                              *
* Follows information from:                                                    *
*      http://www.seanet.com/~brucemo/topics/topics.htm                        *
\******************************************************************************/

/******************************************************************************\
* Global Defines                                                               *
\******************************************************************************/
#define INFINITY MAXINT
#define RED      'r'
#define BLUE     'b'
#define GREEN    'g'
#define SPACE    's'

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

int maxdepthval;
int column;
char piece;

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
* MinMax                                                                       *
\******************************************************************************/
int MinMax(int depth)
{
/* if (SideToMove() == WHITE)    // White is the "maximizing" player.         */
/*    return Max(depth);                                                      */
/* else                          // Black is the "minimizing" player.         */
/*    return Min(depth);                                                      */
/* Blue always plays first, and since we want to maximize its score...        */
	maxdepthval = depth;
	Max(depth);
} /* MinMax */

/******************************************************************************\
* Evaluate                                                                     *
\******************************************************************************/
int Evaluate()
{
	return random();
} /* Evaluate */

/******************************************************************************\
* AddToBoard                                                                   *
\******************************************************************************/
void AddToBoard(int column, char piece)
{
	int row;

	for (row = rows - 1; board[column * rows + row] == SPACE; row--) ;
	row++;

	board[column * rows + row] = piece;
}

/******************************************************************************\
* RemoveFromBoard                                                              *
\******************************************************************************/
void RemoveFromBoard(int column)
{
	int row;

	for (row = rows - 1; board[column * rows + row] == SPACE; row--) ;

	board[column * rows + row] = SPACE;
}

/******************************************************************************\
* Max                                                                          *
\******************************************************************************/
int Max(int depth)
{
/* int best = -INFINITY;                                                      */
	int best = -INFINITY;
	int val;
	int *legal_columns;
	char *legal_pieces;
	int max_legal;
	int i;

/* if (depth <= 0)                                                            */
/*    return Evaluate();                                                      */
	if (depth <= 0)
		return Evaluate();

/* GenerateLegalMoves();                                                      */
	max_legal = 0;
	legal_columns = (int*)calloc(columns*2,sizeof(int));
	if (legal_columns == NULL)
		return best;
	legal_pieces = (char*)calloc(columns*2,sizeof(char));
	if (legal_pieces == NULL)
	{
		free(legal_columns);
		return best;
	}
	for (i = 0; i < columns; i++)
		if (board[i * rows + rows - 1] == SPACE)
		{
			legal_columns[max_legal] = i;
			legal_pieces[max_legal++] = BLUE;
			legal_columns[max_legal] = i;
			legal_pieces[max_legal++] = GREEN;
		}

/* while (MovesLeft()) {                                                      */
	for (i = 0; i < max_legal; i++) {

/*    MakeNextMove();                                                         */
		AddToBoard(legal_columns[i], legal_pieces[i]);

/*    val = Min(depth - 1);                                                   */
		val = Min(depth - 1);

/*    UnmakeMove();                                                           */
		RemoveFromBoard(legal_columns[i]);

/*    if (val > best)                                                         */
/*       best = val;                                                          */
		if (val > best)
		{
			best = val;
			if (depth == maxdepthval)
			{
				piece = legal_pieces[i];
				column = legal_columns[i];
			}
		}
/* }                                                                          */
	}

	free(legal_columns);
	free(legal_pieces);

/* return best;                                                               */
	return best;
} /* Max */

/******************************************************************************\
* Min                                                                          *
\******************************************************************************/
int Min(int depth)
{
/* int best = INFINITY;  // <-- Note that this is different than in "Max".    */
	int best = INFINITY;
	int val;
	int *legal_columns;
	char *legal_pieces;
	int max_legal;
	int i;

/* if (depth <= 0)                                                            */
/*    return Evaluate();                                                      */
	if (depth <= 0)
		return Evaluate();

/* GenerateLegalMoves();                                                      */
	max_legal = 0;
	legal_columns = (int*)calloc(columns*2,sizeof(int));
	if (legal_columns == NULL)
		return best;
	legal_pieces = (char*)calloc(columns*2,sizeof(char));
	if (legal_pieces == NULL)
	{
		free(legal_columns);
		return best;
	}
	for (i = 0; i < columns; i++)
		if (board[i * rows + rows - 1] == SPACE)
		{
			legal_columns[max_legal] = i;
			legal_pieces[max_legal++] = RED;
			legal_columns[max_legal] = i;
			legal_pieces[max_legal++] = GREEN;
		}

/* while (MovesLeft()) {                                                      */
	for (i = 0; i < max_legal; i++) {

/*    MakeNextMove();                                                         */
		AddToBoard(legal_columns[i], legal_pieces[i]);

/*    val = Max(depth - 1);                                                   */
		val = Max(depth - 1);

/*    UnmakeMove();                                                           */
		RemoveFromBoard(legal_columns[i]);

/*    if (val < best)  // <-- Note that this is different than in "Max".      */
/*       best = val;                                                          */
		if (val < best)
			best = val;
/* }                                                                          */
	}

	free(legal_columns);
	free(legal_pieces);

/* return best;                                                               */
	return best;
} /* Min */

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

	MinMax(3);

	freeboard();

	printf("(%d, %c)", column+1, piece);

	return 0;
} /* main */


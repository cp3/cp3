/******************************************************************************\
* ai                                                                           *
\******************************************************************************/
MOVE ai(PIECE turn, char *pai)
{
	MOVE mv;
	int i, j, pid, fdi[2], fdo[2], timer;
	char receive[81];
	char *str;
	char *gets();
	int c = 0;
	char p = '!';
	bool running;

	if ((pipe(fdi) != 0) || (pipe(fdo) != 0)) {
		fprintf(stderr, "ERROR: Pipe creation problem\n");
		exit(-1);
	} /* if */

	i = 0;
	while ((pid=fork()) == -1) /* try a few times before giving up */
		if (++i > 5 ) {
			fprintf(stderr, "ERROR: Fork failed after 5 attempts\n");
			exit(-1);
		} else sleep(5);

	if (pid == 0)
	{ /* CHILD process */
		close(fdo[0]); /* not using pipe read fd=3 */
		close(fdi[1]); /* not using pipe write fd=4 */
		close(0); /* redirecting stdin fd=0 */
		close(1); /* redirecting stdout fd=1 */
		i = dup(fdi[0]); /* duplicate pipe read with stdin i.e. 3=0 */
		j = dup(fdo[1]);
		close(fdi[0]); /* close pipe read after use fd=3 */
		close(fdo[1]); /* close pipe write after use fd=4 */
		execlp(pai, "ai", NULL);
		exit(0);
	}
	else
	{ /* Parent process */
		close(fdi[0]); /* not using pipe read fd=3 */
		close(fdo[1]); /* not using pipe write fd=4 */
		str = boardline(turn);
		write(fdi[1], str, strlen(str));
		free(str);
		timer = 0;
		do {
			sleep(1);
			running = 0 == waitpid(pid, NULL, WNOHANG);
		} while ((++timer < movetime) && running);

		if (running)
			kill(pid, 9);

		i = read(fdo[0], receive, 80);
		receive[i] = '\0';
		close(fdo[0]); /* close pipe read after use fd=3 */
		close(fdi[1]); /* close pipe write after use fd=4 */
	}

	if (receive[0] == '\0')
		strcpy(receive, "TIMEOUT");
	else
		sscanf(receive, "(%d, %c)", &c, &p);

	if (displevel >= 2) fprintf(output, "%d\t%s\t", timer, receive);

	mv.val = timer;
	mv.col = c;
	switch (p)
	{
		case 'r':
		case 'R':
			mv.p = turn;
			break;
		case 'g':
		case 'G':
			mv.p = green;
			break;
		default:
			mv.p = open;
			mv.col = 0;
			break;
	} /* switch */

	return mv;
} /* ai */


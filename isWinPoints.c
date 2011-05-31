#include <stdio.h>

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
//		if (a == 1 && b == 1 && c == 3 && d == 3)
//			val = 5;
//		if (a == 3 && b == 3 && c == 1 && d == 1)
//			val = 5;
//		if (a == 2 && b == 2 && c == 3 && d == 3)
//			val = 5 << 8;
//		if (a == 3 && b == 3 && c == 2 && d == 2)
//			val = 5 << 8;
//
//		if (a == 1 && b == 3 && c == 3 && d == 1)
//			val = 4;
//		if (a == 3 && b == 1 && c == 1 && d == 3)
//			val = 4;
//		if (a == 2 && b == 3 && c == 3 && d == 2)
//			val = 4 << 8;
//		if (a == 3 && b == 2 && c == 2 && d == 3)
//			val = 4 << 8;
//
//		if (a == 1 && b == 3 && c == 1 && d == 3)
//			val = 3;
//		if (a == 3 && b == 1 && c == 3 && d == 1)
//			val = 3;
//		if (a == 2 && b == 3 && c == 2 && d == 3)
//			val = 3 << 8;
//		if (a == 3 && b == 2 && c == 3 && d == 2)
//			val = 3 << 8;

		// Almost wins
		if (a == 0 && b == 1 && c == 3 && d == 3)
			val = 1;
		if (a == 1 && b == 0 && c == 3 && d == 3)
			val = 2;
		if (a == 1 && b == 1 && c == 0 && d == 3)
			val = 2;
		if (a == 1 && b == 1 && c == 3 && d == 0)
			val = 1;

		if (a == 0 && b == 3 && c == 1 && d == 1)
			val = 1;
		if (a == 3 && b == 0 && c == 1 && d == 1)
			val = 2;
		if (a == 3 && b == 3 && c == 0 && d == 1)
			val = 2;
		if (a == 3 && b == 3 && c == 1 && d == 0)
			val = 1;

		if (a == 0 && b == 2 && c == 3 && d == 3)
			val = 1 << 8;
		if (a == 2 && b == 0 && c == 3 && d == 3)
			val = 2 << 8;
		if (a == 2 && b == 2 && c == 0 && d == 3)
			val = 2 << 8;
		if (a == 2 && b == 2 && c == 3 && d == 0)
			val = 1 << 8;

		if (a == 0 && b == 3 && c == 2 && d == 2)
			val = 1 << 8;
		if (a == 3 && b == 0 && c == 2 && d == 2)
			val = 2 << 8;
		if (a == 3 && b == 3 && c == 0 && d == 2)
			val = 2 << 8;
		if (a == 3 && b == 3 && c == 2 && d == 0)
			val = 1 << 8;

		if (a == 0 && b == 3 && c == 3 && d == 1)
			val = 1;
		if (a == 1 && b == 0 && c == 3 && d == 1)
			val = 2;
		if (a == 1 && b == 3 && c == 0 && d == 1)
			val = 2;
		if (a == 1 && b == 3 && c == 3 && d == 0)
			val = 1;

		if (a == 0 && b == 1 && c == 1 && d == 3)
			val = 1;
		if (a == 3 && b == 0 && c == 1 && d == 3)
			val = 2;
		if (a == 3 && b == 1 && c == 0 && d == 3)
			val = 2;
		if (a == 3 && b == 1 && c == 1 && d == 0)
			val = 1;

		if (a == 0 && b == 3 && c == 3 && d == 2)
			val = 1 << 8;
		if (a == 2 && b == 0 && c == 3 && d == 2)
			val = 2 << 8;
		if (a == 2 && b == 3 && c == 0 && d == 2)
			val = 2 << 8;
		if (a == 2 && b == 3 && c == 3 && d == 0)
			val = 1 << 8;

		if (a == 0 && b == 2 && c == 2 && d == 3)
			val = 1 << 8;
		if (a == 3 && b == 0 && c == 2 && d == 3)
			val = 2 << 8;
		if (a == 3 && b == 2 && c == 0 && d == 3)
			val = 2 << 8;
		if (a == 3 && b == 2 && c == 2 && d == 0)
			val = 1 << 8;

		if (a == 0 && b == 3 && c == 1 && d == 3)
			val = 1;
		if (a == 1 && b == 0 && c == 1 && d == 3)
			val = 2;
		if (a == 1 && b == 3 && c == 0 && d == 3)
			val = 2;
		if (a == 1 && b == 3 && c == 1 && d == 0)
			val = 1;

		if (a == 0 && b == 1 && c == 3 && d == 1)
			val = 1;
		if (a == 3 && b == 0 && c == 3 && d == 1)
			val = 2;
		if (a == 3 && b == 1 && c == 0 && d == 1)
			val = 2;
		if (a == 3 && b == 1 && c == 3 && d == 0)
			val = 1;

		if (a == 0 && b == 3 && c == 2 && d == 3)
			val = 1 << 8;
		if (a == 2 && b == 0 && c == 2 && d == 3)
			val = 2 << 8;
		if (a == 2 && b == 3 && c == 0 && d == 3)
			val = 2 << 8;
		if (a == 2 && b == 3 && c == 2 && d == 0)
			val = 1 << 8;

		if (a == 0 && b == 2 && c == 3 && d == 2)
			val = 1 << 8;
		if (a == 3 && b == 0 && c == 3 && d == 2)
			val = 2 << 8;
		if (a == 3 && b == 2 && c == 0 && d == 2)
			val = 2 << 8;
		if (a == 3 && b == 2 && c == 3 && d == 0)
			val = 1 << 8;

		printf("%d, ", val);
	}
	printf("};");
}

int main(void) {
	tempPrint();
}

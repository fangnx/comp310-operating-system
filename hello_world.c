#include <stdio.h>
#include <stdio.h>
#include <time.h>

int isEven(int n);

int main() {
	srand(time(NULL));
	int r = rand();
	if (isEven(r)) {
		printf("%i is even\n", r);
	} else {
        printf("%i is odd\n", r);
    }

    return 0;
}

int isEven(int n) {
    return n % 2 == 0;
}

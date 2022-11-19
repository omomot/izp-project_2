all: cluster.c
	gcc -std=c99 -Wall -Wextra -Werror -DNDEBUG cluster.c -o cluster -lm
debug: cluster.c
	gcc -std=c99 -Wall -Wextra -Werror cluster.c -o cluster -lm
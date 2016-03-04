all: array

array:
	g++ -std=c++11 -Wall -Wextra -pedantic test.cpp -o array

clean:
	rm -f ~* array
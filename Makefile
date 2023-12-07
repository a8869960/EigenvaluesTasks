FLAGS=-O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -c

all: main.o matrix_input.o matrix_operations.o matrix_output.o results.o basic_functions.o eigenvalues.o
	g++ main.o matrix_input.o matrix_operations.o matrix_output.o results.o basic_functions.o eigenvalues.o

main.o: main.cpp functions.h
	g++ $(FLAGS) main.cpp

matrix_input.o: matrix_input.cpp functions.h
	g++ $(FLAGS) matrix_input.cpp

matrix_operations.o: matrix_operations.cpp functions.h
	g++ $(FLAGS) matrix_operations.cpp

matrix_output.o: matrix_output.cpp functions.h
	g++ $(FLAGS) matrix_output.cpp

results.o: results.cpp functions.h
	g++ $(FLAGS) results.cpp

basic_functions.o:basic_functions.cpp functions.h
	g++ $(FLAGS) basic_functions.cpp

eigenvalues.o: eigenvalues.cpp functions.h
	g++ $(FLAGS) eigenvalues.cpp

clean:
	rm -f *.out *.o *.gch

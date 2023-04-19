FLAGS = -Wunused -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual

all: a.out

a.out: main.o point.o time_now.o solve.o check_input.o get_data.o
	g++ main.o point.o time_now.o solve.o check_input.o get_data.o -o a.out

main.o: main.cpp functions.h
	g++ $(FLAGS) -c main.cpp
point.o: point.cpp point.h
	g++ $(FLAGS) -c point.cpp
time_now.o: time_now.cpp functions.h
	g++ $(FLAGS) -c time_now.cpp
solve.o: solve.cpp functions.h
	g++ $(FLAGS) -c solve.cpp
check_input.o: check_input.cpp functions.h
	g++ $(FLAGS) -c check_input.cpp
get_data.o: get_data.cpp functions.h
	g++ $(FLAGS) -c get_data.cpp
clean:
	rm -rf *.0 a.out

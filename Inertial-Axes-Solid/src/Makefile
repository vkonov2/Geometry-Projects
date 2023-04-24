FLAGS = -O3 -ffast-math

all: axes.out

axes.out: main.o point.o time_now.o solve.o get_data.o eigen.o yakobi.o check.o
	g++ main.o point.o time_now.o solve.o get_data.o eigen.o yakobi.o check.o -o axes.out

main.o: main.cpp functions.h
	g++ $(FLAGS) -c main.cpp
point.o: point.cpp point.h
	g++ $(FLAGS) -c point.cpp
time_now.o: time_now.cpp functions.h
	g++ $(FLAGS) -c time_now.cpp
solve.o: solve.cpp functions.h
	g++ $(FLAGS) -c solve.cpp
get_data.o: get_data.cpp functions.h
	g++ $(FLAGS) -c get_data.cpp
area.o: area.cpp functions.h
	g++ $(FLAGS) -c area.cpp
eigen.o: eigen.cpp functions.h
	g++ $(FLAGS) -c eigen.cpp
yakobi.o: yakobi.cpp functions.h
	g++ $(FLAGS) -c yakobi.cpp
check.o: check.cpp functions.h
	g++ $(FLAGS) -c check.cpp	
clean:
	rm -rf *.0 axes.out

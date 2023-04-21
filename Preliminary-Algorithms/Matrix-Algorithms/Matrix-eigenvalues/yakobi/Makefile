FLAGS = -O3

all: a.out

a.out: main.o time_now.o solve.o print.o norm.o formula.o solution_alloc.o yakobi.o
	g++ main.o time_now.o solve.o print.o norm.o formula.o solution_alloc.o yakobi.o -o a.out

main.o: main.cpp functions.h
	g++ $(FLAGS) -c main.cpp
yakobi.o: yakobi.cpp functions.h
	g++ $(FLAGS) -c yakobi.cpp		
print.o: print.cpp functions.h
	g++ $(FLAGS) -c print.cpp
norm.o: norm.cpp functions.h
	g++ $(FLAGS) -c norm.cpp
formula.o: formula.cpp functions.h
	g++ $(FLAGS) -c formula.cpp
solution_alloc.o: solution_alloc.cpp functions.h
	g++ $(FLAGS) -c solution_alloc.cpp
time_now.o: time_now.cpp functions.h
	g++ $(FLAGS) -c time_now.cpp
solve.o: solve.cpp functions.h
	g++ $(FLAGS) -c solve.cpp
clean:
	rm -rf *.0 a.out

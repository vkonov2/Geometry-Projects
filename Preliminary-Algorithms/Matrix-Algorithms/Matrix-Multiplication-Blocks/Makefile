all: gauss

gauss: main.o _max.o print.o check.o det.o norm.o formula.o inverse.o solution.o swap_str.o mult.o time_now.o
	g++ main.o _max.o print.o check.o det.o formula.o inverse.o solution.o swap_str.o mult.o time_now.o norm.o -o gauss

main.o: main.cpp
	g++ -c main.cpp
_max.o: _max.cpp
	g++ -c _max.cpp
print.o: print.cpp
	g++ -c print.cpp
check.o:  check.cpp
	g++ -c check.cpp
det.o: det.cpp
	g++ -c det.cpp
norm.o: norm.cpp
	g++ -c norm.cpp
formula.o: formula.cpp
	g++ -c formula.cpp
inverse.o: inverse.cpp
	g++ -c inverse.cpp
solution.o: solution.cpp
	g++ -c solution.cpp
swap_str.o: swap_str.cpp
	g++ -c swap_str.cpp
mult.o: mult.cpp
	g++ -c 	mult.cpp
time_now.o: time_now.cpp
	g++ -c 	time_now.cpp
clean:
	rm -rf *.0 gauss

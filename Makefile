CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17

SOURCES := $(wildcard day*.cpp)
TARGETS := $(basename ${SOURCES})

.PHONY: clean all

all:
	for target in $(TARGETS); do \
        $(MAKE) -B $$target; \
    done

clean:
	rm day[0-2][0-9]

day01: day01.cpp
	${CXX} ${CXXFLAGS} day01.cpp -o day01

day02: day02.cpp
	${CXX} ${CXXFLAGS} day02.cpp -o day02

day03: day03.cpp
	${CXX} ${CXXFLAGS} day03.cpp -o day03

day04: day04.cpp
	${CXX} ${CXXFLAGS} day04.cpp -o day04

day05: day05.cpp
	${CXX} ${CXXFLAGS} day05.cpp -o day05

day06: day06.cpp
	${CXX} ${CXXFLAGS} day06.cpp -o day06

day07: day07.cpp
	${CXX} ${CXXFLAGS} day07.cpp -o day07

day08: day08.cpp
	${CXX} ${CXXFLAGS} day08.cpp -o day08

day09: day09.cpp
	${CXX} ${CXXFLAGS} day09.cpp -o day09

day10: day10.cpp
	${CXX} ${CXXFLAGS} day10.cpp -o day10

day11: day11.cpp
	${CXX} ${CXXFLAGS} day11.cpp -o day11

day12: day12.cpp
	${CXX} ${CXXFLAGS} day12.cpp -o day12

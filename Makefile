CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17

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

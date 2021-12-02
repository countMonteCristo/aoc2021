CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++1z

day01: day01.cpp
	${CXX} ${CXXFLAGS} day01.cpp -o day01

day02: day02.cpp
	${CXX} ${CXXFLAGS} day02.cpp -o day02

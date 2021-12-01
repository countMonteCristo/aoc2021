CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic

day01: day01.cpp
	${CXX} ${CXXFLAGS} day01.cpp -o day01

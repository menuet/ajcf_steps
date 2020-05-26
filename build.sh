
g++ -c calc.cpp

g++ -c calcsuper.cpp

g++ -c test.cpp

g++ -o exectest test.o calc.o calcsuper.o -std=c++11

./exectest

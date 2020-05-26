
g++ -c calc.cpp
g++ -c calcsuper.cpp

# lib statique
ar rcs libcalcstatic.a calc.o calcsuper.o
# fournit aussi calc.hpp et calcsuper.hpp

g++ -shared -o libcalcshared.so calc.o calcsuper.o

-------------------------------------------

g++ -c test.cpp

g++ -o exectest test.o -L. -lcalcstatic
g++ -o exectest test.o -L. -lcalcshared

------------------------------------------

./exectest

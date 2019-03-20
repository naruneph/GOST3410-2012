genpkey_objects = rand.o genpkey.o

all: genpkey

genpkey: $(genpkey_objects)
	g++ -std=c++11 -Wall -o genpkey $(genpkey_objects)

rand: rand.cpp
genpkey: genpkey.cpp
.cpp.o:
	g++ -c -o $@ -std=c++11 $<

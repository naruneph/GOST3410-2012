all: genpkey convpkey sign verify

genpkey: genpkey.cpp
	g++ -Wall -O2 -std=c++11 -o genpkey genpkey.cpp
convpkey: convpkey.cpp
	g++ -Wall -O2 -std=c++11 -o convpkey convpkey.cpp
sign: sign.cpp
	g++ -Wall -O2 -std=c++11 -o sign sign.cpp
verify: verify.cpp
	g++ -Wall -O2 -std=c++11 -o verify verify.cpp


all: main 

main:
	g++ -std=c++11 -o main main.cpp
SAP:
	g++ -std=c++11 -o SAP SAP.cpp

clean:
	rm -rf main SAP

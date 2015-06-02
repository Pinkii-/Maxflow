all: main SAP

main:
	g++ -std=c++11 -o main main.cpp

SAP:
	g++ -o SAP SAP.cpp

clean:
	rm -rf main SAP

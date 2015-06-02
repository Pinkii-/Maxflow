all: main SAP

main:
	g++ -o main main.cpp

SAP:
	g++ -o SAP SAP.cpp

clean:
	rm -rf main SAP
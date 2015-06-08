#include <iostream>
using namespace std;

int main(){
	
	for(int i = 0; i < 72; ++i){
		float T = 0;
		for(int j = 0; j < 10; ++j){
			float t = 0;
			cin >> t;
			T += t;
		}
		cout << 2+(i/9) << "_" << (i%9) << " & " << T/10 << endl;
	}
}
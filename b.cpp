#include<bits/stdc++.h>

using namespace std;

int main(){ 
	vector<int> I = {0};
	cout << 0 << ' ';
	for(int i = 1; i < 10000; ++i){
		set<int> S;
		for(int j = 0; j < i - 1; ++j) S.insert(I[j] ^ I[i - j - 2]);
		int mn = 0;
		while(S.count(mn))mn++;
		I.push_back(mn);
		if(mn == 0) cout << i << ' ';
	}
	cout << endl;
	/*
	vector<int> L = I;
	for(int i = 1; i < 10000; ++i) {
		set<int> S;
		for(int j = 0; j < L[i]; ++j) S.insert(j);
		for(int j = 1; j <= i; ++j) S.insert(L[i - j] - j);
		int mn = 0;
		while(S.count(mn))mn++;
		L[i] = mn;
	}
	for(int i = 84; i <= 117; ++i) cout << L[i] << ' ';
	cout << '\n';
	*/
}

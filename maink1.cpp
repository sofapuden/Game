#include<bits/stdc++.h>
#include"game.h"

using namespace std;

typedef long long ll;

vector<Game> Game::games;
unordered_map<string, string> Game::conv;
unordered_map<int, int> dp;
unordered_map<int, Game> number;

int f(int k) {
	if(dp.contains(k)) return dp[k];
	vector<long long> L, R;
	for(int i = 0; i < k - 1; ++i) {
		L.push_back((Game::games[f(i)] + Game::games[f(k - i - 2)]).id2);
		R.push_back((Game::games[f(i)] + Game::games[f(k - i - 2)]).id2);
	}
	Game ret = Game::create(L, R);
	return dp[k] = ret.id2;
}

void init() {
	Game zero = Game::create({}, {});
	Game::conv[to_string(zero)] = "0";
	number[0] = zero;
	Game ls = zero;
	for(int i = 1; i <= 100; ++i) {
		ls = Game::create({ls.id2}, {});
		number[i] = ls;
		Game::conv[to_string(ls)] = to_string(i);
	}
	ls = zero;
	for(int i = 1; i <= 100; ++i) {
		ls = Game::create({}, {ls.id2});
		number[-i] = ls;
		Game::conv[to_string(ls)] = to_string(-i);
	}
	vector<long long> D = {zero.id2};
	Game star = Game::create(D, D);
	Game::conv[to_string(star)] = "*";
	for(int i = 2; i <= 10; ++i) {
		D.push_back(star.id2);
		star = Game::create({D}, {D});
		Game::conv[to_string(star)] = "*" + to_string(i);
	}
}

int main() {
	Game::games.reserve(1000000);
	init();
	int k; cin >> k;
	if(k == -1) {
		for(int i = 0; i < 1000; ++i) cout << "GAME " << i << " -> " <<  Game::games[f(i)] << endl;
	}
	else {
		cout << Game::games[f(k)] << '\n';
	}
}

#include<bits/stdc++.h>
#include"game.h"

using namespace std;

vector<Game> Game::games;
unordered_map<string, string> Game::conv;

unordered_map<long long, Game> number;
unordered_map<long long, long long> dpf, dpg;

long long f(long long k);
long long g(long long k);

long long f(long long k) {
	if(dpf.contains(k)) return dpf[k];
	vector<long long> L, R;
	for(int i = 0; i < k - 1; ++i) {
		if(i & 1) {
			R.push_back((Game::games[f(i)] + Game::games[g(k - i - 2)]).id2);
			L.push_back((Game::games[f(i)] + Game::games[g(k - i - 2)]).id2);
		}
		else {
			R.push_back((Game::games[f(i)] + Game::games[f(k - i - 2)]).id2);
			L.push_back((Game::games[f(i)] + Game::games[f(k - i - 2)]).id2);
			L.push_back((Game::games[f(i)] + Game::games[g(k - i - 1)]).id2);
		}
	}
	if(k & 1) L.push_back(f(k - 1));
	
	Game ret = Game::create(L, R);
	return dpf[k] = ret.id2;
}

long long g(long long k) {
	if(dpg.contains(k)) return dpg[k];
	vector<long long> L, R;
	for(int i = 0; i < k - 1; ++i) {
		if(i & 1) {
			R.push_back((Game::games[g(i)] + Game::games[f(k - i - 2)]).id2);
			L.push_back((Game::games[g(i)] + Game::games[f(k - i - 2)]).id2);
			L.push_back((Game::games[g(i)] + Game::games[g(k - i - 1)]).id2);
		}
		else {
			R.push_back((Game::games[g(i)] + Game::games[g(k - i - 2)]).id2);
			L.push_back((Game::games[g(i)] + Game::games[g(k - i - 2)]).id2);
		}
	}
	Game ret = Game::create(L, R);
	return dpg[k] = ret.id2;
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
	for(int i = 0; i < 1000; ++i) {
		cout << "GAME " << i << " -> " << Game::games[f(i)];
		if(i >= 14) cout << ' ' << (Game::games[f(i)] == Game::games[f(i - 14)] + number[1]) << endl;
		else cout << endl;
	}
}

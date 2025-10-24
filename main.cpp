#include<bits/stdc++.h>
#include"game.h"

using namespace std;

typedef long long ll;

vector<Game> Game::games;
unordered_map<string, string> Game::conv;
unordered_map<int, int> dp;
unordered_map<int, Game> number;

long long f(int mask, vector<vector<vector<int>>> pos) {
	if(dp.contains(mask)) return dp[mask];
	int n = pos.size();
	int m = pos[0].size();
	int k = pos[0][0].size();
	auto g = [&](int x, int y, int z) -> ll {
		return 1ll << (x * m * k + y * k + z);
	};
	vector<long long> L, R;

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			for(int l = 0; l < k; ++l) {
				if(i + 1 < n && !pos[i][j][l] && !pos[i + 1][j][l]) {
					int cu = g(i, j, l) | g(i + 1, j, l);
					assert((mask & cu) == 0);
					pos[i][j][l] = pos[i + 1][j][l] = 1;
					auto z = f(mask | cu, pos);
					L.push_back(z);
					R.push_back(z);
					pos[i][j][l] = pos[i + 1][j][l] = 0;
				}
				if(j + 1 < m && !pos[i][j][l] && !pos[i][j + 1][l]) {
					int cu = g(i, j, l) | g(i, j + 1, l);
					assert((mask & cu) == 0);
					pos[i][j][l] = pos[i][j + 1][l] = 1;
					auto z = f(mask | cu, pos);
					L.push_back(z);
					R.push_back(z);
					pos[i][j][l] = pos[i][j + 1][l] = 0;
				}
				if(l + 1 < k && !pos[i][j][l] && !pos[i][j][l + 1]) {
					int cu = g(i, j, l) | g(i, j, l + 1);
					assert((mask & cu) == 0);
					pos[i][j][l] = pos[i][j][l + 1] = 1;
					auto z = f(mask | cu, pos);
					L.push_back(z);
					R.push_back(z);
					pos[i][j][l] = pos[i][j][l + 1] = 0;
				}
			}
		}
	}
	Game ret = Game::create(L, R);
	return dp[mask] = ret.id2;
}

int solve(vector<vector<vector<int>>> ini) {
	dp.clear();
	ll mask = 0;
	int n = ini.size();
	int m = ini[0].size();
	int k = ini[0][0].size();
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			for(int l = 0; l < k; ++l) {
				if(ini[i][j][l]) mask |= 1ll << (i * m * k + j * k + l);
			}
		}
	}
	return f(mask, ini);
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
	int n, m, k; cin >> n >> m >> k;
	vector<vector<vector<int>>> arena(n, vector<vector<int>> (m, vector<int> (k)));
	for(auto &x : arena) for(auto &y : x) for(auto &z : y) cin >> z;
	if(n == 1) {
		auto A = Game::games[solve(arena)];
		arena.push_back(arena[0]);
		int uA = 20, lA = -20;
		for(int i = 20; i >= -20; --i) {
			if(number[i] >= A) uA = min(uA, i);
			if(number[i] <= A) lA = max(lA, i);
		}
		auto B = Game::games[solve(arena)];
		int uB = 20, lB = -20;
		for(int i = 20; i >= -20; --i) {
			if(number[i] >= B) uB = min(uB, i);
			if(number[i] <= B) lB = max(lB, i);
		}
		arena.push_back(arena[0]);
		auto C = Game::games[solve(arena)];
		int uC = 20, lC = -20;
		for(int i = 20; i >= -20; --i) {
			if(number[i] >= C) uC = min(uC, i);
			if(number[i] <= C) lC = max(lC, i);
		}
		cout << "1h -> " << A << " | lower_bound -> " << lA << " | upper_bound -> " << uA <<'\n';
		cout << "2h -> " << B << " | lower_bound -> " << lB << " | upper_bound -> " << uB <<'\n';
		// cout << "3h -> " << C << " | lower_bound -> " << lC << " | upper_bound -> " << uC <<'\n';
	}
	else {
		auto A = Game::games[solve(arena)];
		int uA = 20, lA = -20;
		for(int i = 20; i >= -20; --i) {
			if(number[i] >= A) uA = min(uA, i);
			if(number[i] <= A) lA = max(lA, i);
		}
		cout << A << " | lower_bound -> " << lA << " | upper_bound -> " << uA <<'\n';
	}
}

#include<bits/stdc++.h>
#include"game.h"

using namespace std;

typedef long long ll;

vector<Game> Game::games;
unordered_map<string, string> Game::conv;
unordered_map<int, int> dp;
unordered_map<int, Game> number;
unordered_map<int, Game> star_number;
unordered_map<long long, long long> dpf, dpg;

void init() {
	Game zero = Game::create({}, {});
	Game::conv[to_string(zero)] = "0";
	number[0] = zero;
	star_number[0] = zero;
	Game ls = zero;
	for(int i = 1; i <= 200; ++i) {
		ls = Game::create({ls.id2}, {});
		number[i] = ls;
		Game::conv[to_string(ls)] = to_string(i);
	}
	cout << "STEP 1 DONE" << endl;
	ls = zero;
	for(int i = 1; i <= 200; ++i) {
		ls = Game::create({}, {ls.id2});
		number[-i] = ls;
		Game::conv[to_string(ls)] = to_string(-i);
	}
	cout << "STEP 2 DONE" << endl;
	vector<long long> D = {zero.id2};
	Game star = Game::create(D, D);
	star_number[1] = star;
	Game::conv[to_string(star)] = "*";
	for(int i = 2; i <= 10; ++i) {
		D.push_back(star.id2);
		star = Game::create({D}, {D});
		star_number[i] = star;
		Game::conv[to_string(star)] = "*" + to_string(i);
	}
}

int solve_n1m1(long long k) {
	if(dp.contains(k)) return dp[k];
	vector<long long> L, R;
	for(int i = 0; i < k - 1; ++i) {
		L.push_back((Game::games[solve_n1m1(i)] + Game::games[solve_n1m1(k - i - 2)]).id2);
		R.push_back((Game::games[solve_n1m1(i)] + Game::games[solve_n1m1(k - i - 2)]).id2);
	}
	Game ret = Game::create(L, R);
	return dp[k] = ret.id2;
}

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

int solve_n2m1(long long k) {
	return f(k);
}

Game approx(long long n, long long k) {
	vector<vector<int>> T((n + 1) / 2, vector<int> (k + 1, 0));

	std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, n / 2);

	for(int i = 1; i <= k; i += 2) T[distrib(gen)][i] = 1;

	vector<vector<vector<int>>> memo((n + 1) / 2, (vector<vector<int>> (k + 2, vector<int> (k + 2, -1))));
	auto find = [&](auto &&self, int id, int l, int r) -> int { 
		if(~memo[id][l][r]) return memo[id][l][r];
		vector<long long> L, R;
		for(int i = l; i <= r; ++i) {
			if (!T[id][i] && (i & 1)) L.push_back((Game::games[self(self, id, l, i - 1)] + Game::games[self(self, id, i + 1, r)]).id2);
			if(i < r) {
				L.push_back((Game::games[self(self, id, l, i - 1)] + Game::games[self(self, id, i + 2, r)]).id2);
				R.push_back((Game::games[self(self, id, l, i - 1)] + Game::games[self(self, id, i + 2, r)]).id2);
			}
		}
		Game res = Game::create(L, R);
		return memo[id][l][r] = res.id2;
	};

	Game ret = number[0];

	for(int i = 0; i < (n + 1) / 2; ++i) {
		cerr << i << endl;
		ret += Game::games[find(find, i, 1, k)];
	}
	return ret;
}

int main() {
	Game::games.reserve(1000000);
	cerr << "INIT START" << endl;
	init();
	cerr << "INIT END" << endl;
	long long n, m, k; cin >> n >> m >> k;
	bool szI = 1 & ((n + 1) / 2) & ((m + 1) / 2) & ((k + 1) / 2);
	if(k == 1) {
		if(m == 1) cout << n / 2 << '\n';
		else if(n == 1) cout << - m / 2 << '\n';
		else cout << star_number[szI] << '\n';
	}
	else {
		if(n >= 2 && m >= 2) cout << star_number[szI] << '\n';
		else if(n == 1 && m == 1) {
			if(k <= 86) cout << Game::games[solve_n1m1(k)] << '\n';
			else cout << Game::games[solve_n1m1((k - 53) % 34 + 53)] << '\n';
		}
		else if(~n & 1) {
			Game ret = number[0];
			for(int i = 0; i < n / 2; ++i) ret += Game::games[solve_n2m1(k % 14)];
			if((n / 2) * (k / 14) < 200) {
				ret += number[(n / 2) * (k / 14)];
				cout << ret << '\n';
			}
			else {
				cout << ret << " + " << (n / 2) * (k / 14) << '\n';
			}
		}
		else if(~m & 1) {
			Game ret = number[0];
			for(int i = 0; i < m / 2; ++i) ret -= Game::games[solve_n2m1(k % 14)];
			if((m / 2) * (k / 14) < 200) {
				ret -= number[(m / 2) * (k / 14)];
				cout << ret << '\n';
			}
			else {
				cout << ret << " - " << (m / 2) * (k / 14) << '\n';
			}
		}
		else {
			if(k == 2) cout << star_number[szI] << '\n';
			else {
				if(n > 1) {
					Game lw = number[0];
					for(int i = 0; i < n / 2; ++i) lw += Game::games[solve_n2m1(k % 14)];
					if(k <= 86) lw += Game::games[solve_n1m1(k)];
					else lw += Game::games[solve_n1m1((k - 53) % 34 + 53)];
					Game up = number[0];
					for(int i = 0; i < (n + 1) / 2; ++i) up += Game::games[solve_n2m1(k % 14)];

					if(((n + 1) / 2) * (k / 14) >= 200) {
						cout << "Lower bound -> " << lw << " + " << (n / 2) * (k / 14) << '\n';
						cout << "Upper bound -> " << up << " + " << ((n + 1) / 2) * (k / 14) << '\n';
					}
					else {
						lw += number[(n / 2) * (k / 14)];
						up += number[((n + 1) / 2) * (k / 14)];
						int iterations;
						cout << "How many iterations to find better lower bound?" << endl;
						cin >> iterations;
						for(int _ = 0; _ < iterations; ++_) {
							Game cu = approx(n, k);
							if(cu > lw) {
								cout << "FOUND BETTER LOWER BOUND\n";
								lw = cu;
							}
						}
						cout << "Lower bound -> " << lw << '\n';
						cout << "Upper bound -> " << up << '\n';
					}
				}
				else {
					Game up = number[0];
					for(int i = 0; i < m / 2; ++i) up -= Game::games[solve_n2m1(k % 14)];
					if(k <= 86) up -= Game::games[solve_n1m1(k)];
					else up -= Game::games[solve_n1m1((k - 53) % 34 + 53)];
					Game lw = number[0];
					for(int i = 0; i < (m + 1) / 2; ++i) lw -= Game::games[solve_n2m1(k % 14)];

					if(((m + 1) / 2) * (k / 14) >= 200) {
						cout << "Lower bound -> " << lw << " + " << (m / 2) * (k / 14) << '\n';
						cout << "Upper bound -> " << up << " + " << ((m + 1) / 2) * (k / 14) << '\n';
					}
					else {
						up -= number[(m / 2) * (k / 14)];
						lw -= number[((m + 1) / 2) * (k / 14)];
						int iterations;
						cout << "How many iterations to find better upper bound?" << endl;
						cin >> iterations;
						for(int _ = 0; _ < iterations; ++_) {
							Game cu = -approx(m, k);
							if(cu < up) {
								cout << "FOUND BETTER UPPER BOUND\n";
								up = cu;
							}
						}
						cout << "Lower bound -> " << lw << '\n';
						cout << "Upper bound -> " << up << '\n';
					}
				}
			}
		}
	}
}


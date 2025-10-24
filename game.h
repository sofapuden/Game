#include<bits/stdc++.h>

using namespace std;

struct Game {
	long long id, sz, id2;
    vector<long long> L, R;
	static vector<Game> games;
	static unordered_map<string, string> conv;

	static Game create(const vector<long long> &_L = {}, const vector<long long> &_R = {}) {
		Game g;
		static long long cnt = 0;

		g.sz = 1;
		g.L = _L;
		g.R = _R;
		g.id = cnt++;

		return games[g.reduce()];
	}

    // Negation
    Game operator -() const { 
		static unordered_map<long long, long long> memo;
		if(memo.contains(id)) return games[memo[id]];
		vector<long long> cuL, cuR;
        for (auto &x : L) cuR.push_back((-games[x]).id2);
        for (auto &x : R) cuL.push_back((-games[x]).id2);
        Game res = Game::create(cuL, cuR);
		memo[id2] = res.id2;
		memo[res.id2] = id2;
        return res;
    }

    // Addition
    Game &operator += (const Game &o) {
		static map<pair<long long, long long>, long long> memo;
		if(memo.contains(make_pair(id, o.id))) return *this = games[memo[make_pair(id, o.id)]];
        vector<long long> cuL, cuR;
        for (auto const &x : o.L) cuL.push_back(((*this) + games[x]).id2);
        for (auto const &x : L) cuL.push_back((games[x] + o).id2);
        for (auto const &x : o.R) cuR.push_back(((*this) + games[x]).id2);
        for (auto const &x : R) cuR.push_back((games[x] + o).id2);
		Game res = Game::create(cuL, cuR);
		memo[make_pair(id, o.id)] = res.id2;
		return *this = res;
    }

    Game &operator -= (const Game &o) { return *this += -o; }

    friend Game operator + (Game a, const Game &b) { return a += b; }
    friend Game operator - (Game a, const Game &b) { return a -= b; }

	friend bool operator >= (const Game &a, const Game &b) {
		static map<pair<long long, long long>, bool> memo;
		if(memo.contains(make_pair(a.id, b.id))) return memo[make_pair(a.id, b.id)];
		auto &mem = memo[make_pair(a.id, b.id)];
		for(auto x : a.R) if(games[x] <= b) return mem = false;
		for(auto x : b.L) if(a <= games[x]) return mem = false;
		return mem = true;
	}

    friend bool operator <= (const Game &a, const Game &b) {
		return b >= a;
    }

	friend bool operator > (const Game &a, const Game &b) {
		return a >= b && a != b;
	}

	friend bool operator < (const Game &a, const Game &b) {
		return b > a;
	}

    friend bool fuzzy(const Game &a, const Game &b) {
        return !(a >= b) && !(b >= a);
    }

	friend bool operator == (const Game &a, const Game &b) {
		return (a >= b) && (b >= a);
	}

    friend string to_string(Game a) { 
		string out = "{";
		for(size_t i = 0; i < a.L.size(); ++i) {
			out += to_string(games[a.L[i]]);
			if(i + 1 != a.L.size()) out += ", ";
		}
		out += " | ";
		for(size_t i = 0; i < a.R.size(); ++i) {
			out += to_string(games[a.R[i]]);
			if(i + 1 != a.R.size()) out += ", ";
		}
		out += "}";
		if(conv.contains(out)) return conv[out];
		return out;
	}
    friend ostream &operator << (ostream &s, const Game &a) { 
		s << to_string(a); return s; 
	}

    long long reduce() {

		for(size_t i = 0; i < games.size(); ++i) {
			if(*this == games[i]) {
				return i;
			}
		}

		games.emplace_back();
		auto &g = games.back();
		g.id = id;
		g.id2 = (int)games.size() - 1;

		vector<bool> actL(L.size(), 1), actR(R.size(), 1);
		for(size_t i = 0; i < L.size(); ++i) {
			for(size_t j = i + 1; j < L.size(); ++j) {
				if(games[L[j]] >= games[L[i]]) {
					actL[i] = 0;
					break;
				}
				else if(games[L[i]] >= games[L[j]]) {
					actL[j] = 0;
				}
			}
		}

		for(size_t i = 0; i < R.size(); ++i) {
			for(size_t j = i + 1; j < R.size(); ++j) {
				if(games[R[j]] <= games[R[i]]) {
					actR[i] = 0;
					break;
				}
				else if(games[R[i]] <= games[R[j]]) {
					actR[j] = 0;
				}
			}
		}

		g.sz = 1;
		for(size_t i = 0; i < actL.size(); ++i) {
			if(actL[i]) {
				g.L.push_back(L[i]);
				g.sz += games[L[i]].sz;
			}
		}
		for(size_t i = 0; i < actR.size(); ++i) {
			if(actR[i]) {
				g.R.push_back(R[i]);
				g.sz += games[R[i]].sz;
			}
		}

		return g.id2;
	}
};


#include<bits/stdc++.h>

using namespace std;

struct Game {
	int id, sz;
    vector<Game> L, R;
	static vector<Game> games;
	static unordered_map<string, string> conv;

    Game(vector<Game> _L = {}, vector<Game> _R = {}) : L(_L), R(_R) { 
		sz = 1;
		for(auto x : L) sz += x.sz;
		for(auto x : R) sz += x.sz;
		id = games.size();
		games.push_back(*this);
	}

    // Negation
    Game operator -() const { 
		static unordered_map<int, int> memo;
		if(memo.contains(id)) return games[memo[id]];
        Game res(R, L);
        for (auto &x : res.L) x = -x;
        for (auto &x : res.R) x = -x;
		memo[id] = res.id;
		memo[res.id] = id;
        return res;
    }

    // Addition
    Game &operator += (const Game &o) {
		static map<pair<int, int>, int> memo;
		if(memo.contains(make_pair(id, o.id))) return *this = games[memo[make_pair(id, o.id)]];
        vector<Game> cuL, cuR;
        for (auto &x : o.L) cuL.push_back(*this + x);
        for (auto &x : L) cuL.push_back(x + o);
        for (auto &x : o.R) cuR.push_back(*this + x);
        for (auto &x : R) cuR.push_back(x + o);
		Game res(cuL, cuR);
		memo[make_pair(id, o.id)] = res.id;
		return *this = res;
    }
    Game &operator -= (const Game &o) { return *this += -o; }

	Game &operator *= (const Game &o) {
		vector<Game> cuL, cuR;
		for(auto xl : L) for(auto yl : o.L) cuL.push_back(xl * o + (*this) * yl - xl * yl);
		for(auto xr : R) for(auto yr : o.R) cuL.push_back(xr * o + (*this) * yr - xr * yr);
		for(auto xl : L) for(auto yr : o.R) cuR.push_back(xl * o + (*this) * yr - xl * yr);
		for(auto xr : R) for(auto yl : o.L) cuR.push_back(xr * o + (*this) * yl - xr * yl);
		swap(cuL, L);
		swap(cuR, R);
		sz = 1;
		for(auto x : L) sz += x.sz;
		for(auto x : R) sz += x.sz;
		return *this;
	}

    friend Game operator + (Game a, const Game &b) { return a += b; }
    friend Game operator - (Game a, const Game &b) { return a -= b; }
    friend Game operator * (Game a, const Game &b) { return a *= b; }

	friend bool operator >= (const Game &a, const Game &b) {
		static map<pair<int, int>, bool> memo;
		if(memo.contains(make_pair(a.id, b.id))) return memo[make_pair(a.id, b.id)];
		auto &mem = memo[make_pair(a.id, b.id)];
		for(auto x : a.R) if(x <= b) return mem = false;
		for(auto x : b.L) if(a <= x) return mem = false;
		return mem = true;
	}

    friend bool operator <= (const Game &a, const Game &b) {
		return b >= a;
    }

    friend bool fuzzy(const Game &a, const Game &b) {
        return !(a >= b) && !(b >= a);
    }

	friend bool operator == (const Game &a, const Game &b) {
		return (a >= b) && (b >= a);
	}

    friend string to_string(Game a) { 
		// a.reduce();
		string out = "{";
		for(size_t i = 0; i < a.L.size(); ++i) {
			out += to_string(a.L[i]);
			if(i + 1 != a.L.size())out += ", ";
		}
		out += " | ";
		for(size_t i = 0; i < a.R.size(); ++i) {
			out += to_string(a.R[i]);
			if(i + 1 != a.R.size())out += ", ";
		}
		out += "}";
		if(conv.contains(out)) return conv[out];
		return out;
	}
    friend ostream &operator << (ostream &s, const Game &a) { 
		s << to_string(a); return s; 
	}

    void reduce() {
		vector<bool> actL(L.size(), 1), actR(R.size(), 1);
		for(size_t i = 0; i < L.size(); ++i) {
			for(size_t j = i + 1; j < L.size(); ++j) {
				if(L[j] >= L[i]) {
					actL[i] = 0;
					break;
				}
				else if(L[i] >= L[j]) {
					actL[j] = 0;
				}
			}
		}
		for(size_t i = 0; i < R.size(); ++i) {
			for(size_t j = i + 1; j < R.size(); ++j) {
				if(R[j] <= R[i]) {
					actR[i] = 0;
					break;
				}
				else if(R[i] <= R[j]) {
					actR[j] = 0;
				}
			}
		}
		vector<Game> cuL, cuR;
		sz = 1;
		for(size_t i = 0; i < actL.size(); ++i) {
			if(actL[i]) {
				cuL.push_back(L[i]);
				sz += L[i].sz;
			}
		}
		for(size_t i = 0; i < actR.size(); ++i) {
			if(actR[i]) {
				cuR.push_back(R[i]);
				sz += R[i].sz;
			}
		}
		swap(cuL, L);
		swap(cuR, R);
		games[id] = *this;
		for(size_t i = 0; i < games.size(); ++i) {
			if((sz > games[i].sz) && (*this == games[i])) {
				*this = games[i];
			}
		}
	}
};


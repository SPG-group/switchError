/*Copyright (C) 2015 Olivier Delaneau, Halit Ongen, Emmanouil T. Dermitzakis
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef _RANKER_H
#define _RANKER_H

#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

using std::vector;
using std::string;

namespace myranker {

	template <class T>
	class lt {
		public:
		static int compare(T a, T b) {
			return(a < b);
		}
	};

	template <class T>
    class gt {
		public:
		static int compare(T a, T b) {
			return(a > b);
		}
	};

    template <class T, class C>
    class ranker {
    	private:
    	const T* p;
    	uint32_t sz;

    	public:
    	ranker(const vector<T>& v) : p(&v[0]), sz(v.size()) {
    	}

    	ranker(const T* tp, uint32_t s) : p(tp), sz(s) {
    	}

    	int operator()(uint32_t i1, uint32_t i2) const {
    		return(C::compare(p[i1],p[i2]));
    	}

    	template <class S>
    	void get_orders(vector<S>& w) const {
    		w.resize(sz);
    		w.front() = 0;
    		for (typename vector<S>::iterator i = w.begin(); i != w.end() - 1; ++i) *(i + 1) = *i + 1;
    		std::sort(w.begin(), w.end(), *this);
    	}

    	template <class S>
    	void get_partial_orders(vector<S>& w, uint32_t num) const {
    		if (num > sz) num = sz;
    		w.resize(sz);
    		w.front() = 0;
    		for (typename vector<S>::iterator i = w.begin(); i != w.end() - 1; ++i) *(i + 1) = *i + 1;
    		std::partial_sort(w.begin(), w.begin() + num, w.end(), *this);
    		w.resize(num);
    	}

    	template <class S>
    	void get_ranks(vector<S>& w, const string& method) const {
    		w.resize(sz);
    		vector<uint32_t> tmp(w.size());
    		get_orders(tmp);
    		if (method == "average") {
    			for (uint32_t c = 0, reps; c < w.size(); c += reps) {
    				reps = 1;
    				while (c + reps < w.size() && p[tmp[c]] == p[tmp[c + reps]]) ++reps;
    				for (uint32_t k = 0; k < reps; ++k) w[tmp[c + k]] = S(2 * c + reps - 1) / 2 + 1;
    			}
    		} else if (method == "min") {
    			for (uint32_t c = 0, reps; c < w.size(); c += reps) {
    				reps = 1;
    				while (c + reps < w.size() && p[tmp[c]] == p[tmp[c + reps]]) ++reps;
    				for (uint32_t k = 0; k < reps; ++k) w[tmp[c + k]] = c + 1;
    			}
    		} else if (method == "max") {
    			for (uint32_t c = 0, reps; c < w.size(); c += reps) {
    				reps = 1;
    				while (c + reps < w.size() && p[tmp[c]] == p[tmp[c + reps]]) ++reps;
    				for (uint32_t k = 0; k < reps; ++k) w[tmp[c + k]] = c + reps;
    			}
    		} else // default
    			for (uint32_t c = 0; c < w.size(); ++c) w[tmp[c]] = c + 1;
    	}

    	template <class S>
    	void get_partial_ranks(vector<S>& w, const string& method, size_t num) const {
    		if (num > sz) num = sz;
    		vector<uint32_t> tmp(sz);
    		get_partial_orders(tmp, num);
    		w.resize(sz);
    		fill(w.begin(), w.end(), 0);
    		if (method == "average") {
    			for (uint32_t c = 0, reps; c < num; c += reps) { reps = 1;
    			while (c + reps < num && p[tmp[c]] == p[tmp[c + reps]]) ++reps;
    			for (uint32_t k = 0; k < reps; ++k)
    				w[tmp[c + k]] = S(2 * c + reps - 1) / 2 + 1;
    			}
    		} else if (method == "min") {
    			for (uint32_t c = 0, reps; c < num; c += reps) { reps = 1;
    			while (c + reps < num && p[tmp[c]] == p[tmp[c + reps]]) ++reps;
    			for (uint32_t k = 0; k < reps; ++k) w[tmp[c + k]] = c + 1;
    			}
    		} else if (method == "max") {
    			for (uint32_t c = 0, reps; c < num; c += reps) { reps = 1;
    			while (c + reps < num && p[tmp[c]] == p[tmp[c + reps]]) ++reps;
    			for (uint32_t k = 0; k < reps; ++k) w[tmp[c + k]] = c + reps;
    			}
    		} else // default
    			for (uint32_t c = 0; c < num; ++c) w[tmp[c]] = c + 1;
    	}
	};

	template <class T, class S>
	inline void rank(const vector<T>& v, vector<S>& w, const string& method = "average") {
		ranker<T, lt<T> > r(v); r.get_ranks(w, method);
	}

    template <class T, class S>
    inline void rank(const T* d, uint32_t size, vector<S>& w, const string& method = "average") {
    	ranker<T, lt<T> > r(d, size); r.get_ranks(w, method);
    }

    template <class T, class S>
    inline void partial_rank(const vector<T>& v, vector<S>& w, uint32_t num, const string& method = "average") {
    	ranker<T, lt<T> > r(v); r.get_partial_ranks(w, method, num);
    }

    template <class T, class S>
    inline void partial_rank(const T* d, uint32_t size, vector<S>& w, uint32_t num, const string& method = "average") {
    	ranker<T, lt<T> > r(d, size); r.get_partial_ranks(w, method, num);
    }

    template <class T, class S>
    inline void order(const vector<T>& v, vector<S>& w) {
    	ranker<T, lt<T> > r(v); r.get_orders(w);
    }

    template <class T, class S>
    inline void order(const T* d, uint32_t size, vector<S>& w) {
    	ranker<T, lt<T> > r(d, size); r.get_orders(w);
    }

    template <class T, class S>
    inline void partial_order(const vector<T>& v, vector<S>& w, uint32_t num) {
    	ranker<T, lt<T> > r(v); r.get_partial_orders(w, num);
    }

    template <class T, class S>
    inline void partial_order(const T* d, uint32_t size, vector<S>& w, uint32_t num) {
    	ranker<T, lt<T> > r(d, size); r.get_partial_orders(w, num);
    }

    template <class T, class S>
    inline void rankhigh(const vector<T>& v, vector<S>& w, const string& method = "average") {
    	ranker<T, gt<T> > r(v); r.get_ranks(w, method);
    }

    template <class T, class S>
    inline void rankhigh(const T* d, uint32_t size, vector<S>& w, const string& method = "average") {
    	ranker<T, gt<T> > r(d, size); r.get_ranks(w, method);
    }

    template <class T, class S>
    inline void partial_rankhigh(const vector<T>& v, vector<S>& w, uint32_t num, const string& method = "average") {
    	ranker<T, gt<T> > r(v); r.get_partial_ranks(w, method, num);
    }

    template <class T, class S>
    inline void partial_rankhigh(const T* d, uint32_t size, vector<S>& w, uint32_t num, const string& method = "average") {
    	ranker<T, gt<T> > r(d, size); r.get_partial_ranks(w, method, num);
    }

    template <class T, class S>
    inline void orderhigh(const vector<T>& v, vector<S>& w) {
    	ranker<T, gt<T> > r(v); r.get_orders(w);
    }

    template <class T, class S>
    inline void orderhigh(const T* d, uint32_t size, vector<S>& w) {
    	ranker<T, gt<T> > r(d, size); r.get_orders(w);
    }

    template <class T, class S>
    inline void partial_orderhigh(const vector<T>& v, vector<S>& w, uint32_t num) {
    	ranker<T, gt<T> > r(v); r.get_partial_orders(w, num);
    }

    template <class T, class S>
    inline void partial_orderhigh(const T* d, uint32_t size, vector<S>& w, uint32_t num) {
    	ranker<T, gt<T> > r(d, size); r.get_partial_orders(w, num);
    }

    template <class T>
    inline T quantile(const T* d, const uint32_t size, const double q) {
		if (size == 0) return T(0);
		if (size == 1) return d[0];
		if (q <= 0) return *std::min_element(d, d + size);
		if (q >= 1) return *std::max_element(d, d + size);

		double pos = (size - 1) * q;
		uint32_t ind = uint32_t(pos);
		double delta = pos - ind;
		vector<T> w(size); std::copy(d, d + size, w.begin());
		std::nth_element(w.begin(), w.begin() + ind, w.end());
		T i1 = *(w.begin() + ind);
		T i2 = *std::min_element(w.begin() + ind + 1, w.end());
		return i1 * (1.0 - delta) + i2 * delta;
    }

    template <class T>
    inline T quantile(const vector<T>& v, const double q) {
    	return quantile(&v[0], v.size(), q);
    }
};

#endif

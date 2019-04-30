#include "data.h"

void data::typingError() {
	vrb.title("Typing error calculations");
	int nerr = 0, ntot = 0;
	for (int i = 0 ; i < vec_names.size() ; i ++) {
		if (hap1[i].size() > 0) {
			for (int l = 0 ; l < gen1[i].size() ; l ++) {
				bool t0 = gen1[i][l];
				bool t1 = gen2[i][l];
				bool h0 = hap1[i][l];
				bool h1 = hap2[i][l];
				bool mi = miss[i][l];
				terrors[i][l] = !mi && (t0+t1) != (h0+h1);
				nerr += terrors[i][l];
				ntot += !mi;
			}
		}
	}
	vrb.bullet("TER = " + stb.str(nerr * 100.0 / ntot, 4) + "%");
}

void data::switchError(float min_maf) {
	vrb.title("Switch error calculations");
	int nerr = 0, ntot = 0;
	for (int i = 0 ; i < vec_names.size() ; i ++) {
		if (hap1[i].size() > 0) {
			vector < int > vidx;
			for (int l = 0 ; l < gen1[i].size() ; l ++) {
				if (pknown[i][l] && psolved[i][l] && maf[l] >= min_maf) {
					bool curr_t0 = gen1[i][l];
					bool curr_t1 = gen2[i][l];
					bool curr_h0 = hap1[i][l];
					bool curr_h1 = hap2[i][l];
					int curr_ps = psets[i][l];

					int pidx = -1;
					for (int pl = vidx.size() - 1 ; pl >= 0 && pidx < 0; pl --) if (psets[i][vidx[pl]] == curr_ps) pidx = vidx[pl];

					if (pidx >= 0) {
						bool prev_t0 = gen1[i][pidx];
						bool prev_t1 = gen2[i][pidx];
						bool prev_h0 = hap1[i][pidx];
						bool prev_h1 = hap2[i][pidx];
						serrors[i][l] = ((prev_t0==curr_t0) != (prev_h0==curr_h0));
						nerr += serrors[i][l];
						//cout << prev_t0 << curr_t0 << " " << prev_t1 << curr_t1 << " " << prev_h0 << curr_h0 << " " << prev_h1 << curr_h1 << "  = " << (int)serrors[i][l] << " "  << pidx << " " << l << " " << vidx.size() << " " << vidx.back() << " " << curr_ps<< endl;
						ntot ++;
					}
					vidx.push_back(l);
				}
			}
		}
	}
	vrb.bullet("SER = " + stb.str(nerr * 100.0 / ntot, 4) + "%");
}



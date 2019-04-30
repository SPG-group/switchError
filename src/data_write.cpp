#include "data.h"

//1. SER TER BY INDIVIDUAL
//2. SER TER BY VARIANT
//3. SER MATRIX

void data::writeReport(string prefix) {
	//1. SER TER BY INDIVIDUAL
	output_file fd1 (prefix + ".iser");
	for (int i = 0 ; i < vec_names.size() ; i ++) {
		int minc = 0, mcor = 0;
		int sinc = 0, scor = 0;
		int tinc = 0, tcor = 0;
		for (int l = 0 ; l < merrors[i].size() ; l ++) merrors[i][l]?minc++:mcor++;
		for (int l = 0 ; l < terrors[i].size() ; l ++) terrors[i][l]?tinc++:tcor++;
		for (int l = 0 ; l < serrors[i].size() ; l ++) if (serrors[i][l] >=0) serrors[i][l]?sinc++:scor++;
		if (nknown[i] > 0) fd1 << i << " " << vec_names[i] << " " << minc << " " << mcor << " " << tinc << " " << tcor << " " << sinc << " " << scor << endl;
	}
	fd1.close();

	//2. SER TER BY VARIANT
	output_file fd2 (prefix + ".vser");
	for (int l = 0 ; l < mac.size() ; l ++) {
		int minc = 0, mcor = 0;
		int sinc = 0, scor = 0;
		int tinc = 0, tcor = 0;
		for (int i = 0 ; i < vec_names.size() ; i ++) {
			merrors[i][l]?minc++:mcor++;
			if (terrors[i].size() > 0) terrors[i][l]?tinc++:tcor++;
			if (serrors[i].size() > 0 && serrors[i][l] >= 0) serrors[i][l]?sinc++:scor++;
		}
		fd2 << l << " " << pos[l] << " " << mac[l] << " " << maf[l] << " " << minc << " " << mcor << " " << tinc << " " << tcor << " " << sinc << " " << scor << endl;
	}
	fd2.close();

	//3. SER MATRIX
	output_file fd3 (prefix + ".mser.gz");
	for (int i = 0 ; i < vec_names.size() ; i ++) {
		if (serrors[i].size() > 0 && nknown[i] > 0) {
			fd3 << vec_names[i] << " " << pos[0] << endl;
			for (int l = 0 ; l < serrors[i].size() ; l ++) if (serrors[i][l] > 0) fd3 << vec_names[i] << " " << pos[l] << endl;
			fd3 << vec_names[i] << " " << pos.back() << endl;
		}
	}
	fd3.close();
}

#include "data.h"

int main (int argc, char ** argv) {
	string reg = "", gen = "", hap = "", fam ="", ps="", out="", maf="0.0";
	for (int a = 1 ; a < argc ; a++) {
		if (strcmp(argv[a], "--gen") == 0) gen = string(argv[a+1]);
		if (strcmp(argv[a], "--hap") == 0) hap = string(argv[a+1]);
		if (strcmp(argv[a], "--fam") == 0) fam = string(argv[a+1]);
		if (strcmp(argv[a], "--reg") == 0) reg = string(argv[a+1]);
		if (strcmp(argv[a], "--ps")  == 0) ps  = string(argv[a+1]);
		if (strcmp(argv[a], "--out") == 0) out = string(argv[a+1]);
		if (strcmp(argv[a], "--maf") == 0) maf = string(argv[a+1]);
	}
	assert(gen != ""); assert(hap != ""); assert(reg != "");assert(out != "");

	data D;
	D.readGenotypesAndHaplotypes(gen, hap, reg);
	if (ps != "" && ps != "NA") D.readPhaseSets(ps);
	if (fam != "") {
		D.readPedigrees(fam);
		D.solvePedigrees();
	} else D.fixPhase();

	D.nknown = vector < int >(D.vec_names.size(), 0);
	for (int i = 0 ; i < D.vec_names.size() ; i ++) for (int l = 0 ; l < D.pos.size() ; l ++) D.nknown[i] += D.pknown[i][l];


	D.typingError();
	D.switchError(atof(maf.c_str()));

	D.writeReport(out);
}

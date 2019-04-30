#ifndef _DATA_H
#define _DATA_H

#include "otools.h"

class data {
public:
	//IND
	vector < string > vec_names;			//samples ids in vector
	map < string, int > map_names;			//samples ids in map

	//VARIANT
	vector < int > mac;
	vector < float > maf;
	vector < int > pos;
	vector < bool > mask;

	//FAMILY
	vector < int > fathers;					//father ids
	vector < int > mothers;					//mother ids

	//GENOTYPES
	vector < vector < bool > > gen1;
	vector < vector < bool > > gen2;
	vector < vector < bool > > hap1;
	vector < vector < bool > > hap2;
	vector < vector < bool > > miss;
	vector < vector < bool > > pknown;
	vector < int > nknown;

	//HAPLOTYPES
	vector < vector < int > > psets;
	vector < vector < bool > > psolved;

	//ERRORS
	vector < vector < char > > serrors;
	vector < vector < bool > > terrors;
	vector < vector < bool > > merrors;

	data();
	~data();

	void readPedigrees(string);
	void readGenotypesAndHaplotypes(string, string, string);
	void readPhaseSets(string);

	bool solveTrio(int locus, int cidx, int fidx, int midx);
	bool solveDuoFather(int locus, int cidx, int pidx);
	bool solveDuoMother(int locus, int cidx, int pidx);
	void solvePedigrees();

	void typingError();
	void switchError(float);

	void fixPhase();
	void writeReport(string);
};

#endif

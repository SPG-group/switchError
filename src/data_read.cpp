#define _DECLARE_TOOLBOX_HERE
#include "data.h"

void data::readPedigrees(string fped) {
	string buffer;
	vector < string > str;
	vrb.title("Reading pedigrees in [" + fped + "]");
	input_file fd (fped);
	if (fd.fail()) vrb.error("Cannot open file!");
	int n_unr = 0, n_duo = 0, n_tri = 0;
	while (getline(fd, buffer)) {
		stb.split(buffer, str);
		map < string, int > :: iterator itC = map_names.find(str[0]);
		map < string, int > :: iterator itF = map_names.find(str[1]);
		map < string, int > :: iterator itM = map_names.find(str[2]);
		if (itC != map_names.end()) {
			if (itF != map_names.end()) fathers[itC->second] = itF->second;
			else fathers[itC->second] = -1;
			if (itM != map_names.end()) mothers[itC->second] = itM->second;
			else mothers[itC->second] = -1;
			int type = (fathers[itC->second] >= 0) + (mothers[itC->second] >= 0);
			switch (type) {
				case 2: n_tri ++; break;
				case 1: n_duo ++; break;
				default: n_tri ++; break;
			}
		}
	}
	vrb.bullet("#trios = " + stb.str(n_tri));
	vrb.bullet("#duos = " + stb.str(n_duo));
	vrb.bullet("#unrelateds = " + stb.str(n_unr));
}

void data::readPhaseSets(string fps) {
	string buffer;
	vector < string > str;
	vrb.title("Reading phase sets in [" + fps + "]");

	int idx = -1, l0 = 0, l1 = 0;
	for (int i = 0 ; i < hap1.size() && idx < 0 ; i ++) if (hap1[i].size() > 0) idx = i;
	assert(idx >= 0);

	input_file fd (fps);
	if (fd.fail()) vrb.error("Cannot open file!");
	while (getline(fd, buffer)) {
		if (mask[l0]) {
			if (buffer != "NA" && psolved[idx][l1]) psets[idx][l1] = atoi(buffer.c_str());
			else psolved[idx][l1] = false;
			l1++;
		}
		l0++;
	}
	fd.close();
}

void data::readGenotypesAndHaplotypes(string fgen, string fhap, string region) {
	vrb.title("Reading genotypes in ["  + fgen + "] and haplotypes in [" + fhap + "]");
	bcf_srs_t * sr =  bcf_sr_init();
	sr->collapse = COLLAPSE_NONE;
	if (bcf_sr_set_regions(sr, region.c_str(), 0) == -1) vrb.error("Impossible to jump to region [" + region + "]");
	else vrb.bullet("Jump to region [" + region + "] done");

	//Read headers
	if(!(bcf_sr_add_reader (sr, fgen.c_str()))) vrb.error("Impossible to read header of [" + fgen + "]");
	if(!(bcf_sr_add_reader (sr, fhap.c_str()))) vrb.error("Impossible to read header of [" + fhap + "]");

	//Genotype ids processing
	int n_samples_gen = bcf_hdr_nsamples(sr->readers[0].header);
	for (int i = 0 ; i < n_samples_gen ; i ++) {
		string sample_id = string(sr->readers[0].header->samples[i]);
		map_names.insert(pair < string, int > (sample_id, vec_names.size()));
		vec_names.push_back(sample_id);
		fathers.push_back(-1);
		mothers.push_back(-1);
	}
	vrb.bullet("#genotyped samples = " + stb.str(vec_names.size()));

	//Haplotype ids processing
	int n_samples_hap = bcf_hdr_nsamples(sr->readers[1].header);
	vector < int > indexes_hap = vector < int > (n_samples_hap, -1);
	int n_overlap = 0;
	for (int i = 0 ; i < n_samples_hap ; i ++) {
		string sample_id = string(sr->readers[1].header->samples[i]);
		map < string, int > :: iterator itM = map_names.find(sample_id);
		if (itM != map_names.end()) {
			indexes_hap[i] = itM->second;
			n_overlap ++;
		}
	}
	vrb.bullet("#haplotyped samples = " + stb.str(n_samples_hap));
	vrb.bullet("#overlaps = " + stb.str(n_overlap));

	//Memory allocation
	gen1 = vector < vector < bool > > (vec_names.size());
	gen2 = vector < vector < bool > > (vec_names.size());
	hap1 = vector < vector < bool > > (vec_names.size());
	hap2 = vector < vector < bool > > (vec_names.size());
	miss = vector < vector < bool > > (vec_names.size());
	psets = vector < vector < int > > (vec_names.size());
	pknown  = vector < vector < bool > > (vec_names.size());
	psolved  = vector < vector < bool > > (vec_names.size());
	serrors = vector < vector < char > > (vec_names.size());
	terrors = vector < vector < bool > > (vec_names.size());
	merrors = vector < vector < bool > > (vec_names.size());

	//Read genotype and haplotype data
	int nset = 0, *gt_arr_gen = NULL, *gt_arr_hap = NULL, ngt_arr_gen = 0, ngt_arr_hap = 0;
	int nps, *ps_arr = NULL, nps_arr = 0;
	int n_variant_tot = 0, n_variant_set = 0, n_variant_bia = 0;
	bcf1_t * line_gen, * line_hap;
	while ((nset = bcf_sr_next_line (sr))) {
		line_gen =  bcf_sr_get_line(sr, 0);
		line_hap =  bcf_sr_get_line(sr, 1);
		if (line_hap != NULL) mask.push_back(false);
		if (nset == 2) {
			if (line_gen->n_allele == 2) {
				mask.back() = true;
				//Extract variant
				bcf_unpack(line_gen, BCF_UN_STR);
				pos.push_back(line_gen->pos + 1);
				//Extract genotypes
				bcf_get_genotypes(sr->readers[0].header, line_gen, &gt_arr_gen, &ngt_arr_gen);
				for(int h = 0 ; h < 2 * n_samples_gen ; h += 2) {
					bool a0 = bcf_gt_allele(gt_arr_gen[h+0])!=0;
					bool a1 = bcf_gt_allele(gt_arr_gen[h+1])!=0;
					gen1[h/2].push_back(bcf_gt_allele(gt_arr_gen[h+0])!=0);
					gen2[h/2].push_back(bcf_gt_allele(gt_arr_gen[h+1])!=0);
					miss[h/2].push_back(gt_arr_gen[h+0] == bcf_gt_missing || gt_arr_gen[h+1] == bcf_gt_missing);
					pknown[h/2].push_back(false);
					merrors[h/2].push_back(false);
				}

				//Extract haplotypes
				int cref = 0, calt = 0, ctot = 0;
				bcf_get_genotypes(sr->readers[1].header, line_hap, &gt_arr_hap, &ngt_arr_hap);
				nps = bcf_get_format_int32(sr->readers[1].header, line_hap, "PS", &ps_arr, &nps_arr);
				for(int h = 0 ; h < 2 * n_samples_hap ; h += 2) {
					int index = indexes_hap[h/2];
					bool a0 = bcf_gt_allele(gt_arr_hap[h+0])!=0;
					bool a1 = bcf_gt_allele(gt_arr_hap[h+1])!=0;
					if (index >= 0) {
						bool mi = gt_arr_hap[h+0] == bcf_gt_missing || gt_arr_hap[h+1] == bcf_gt_missing;
						bool ph = bcf_gt_is_phased(gt_arr_hap[h+0]) || bcf_gt_is_phased(gt_arr_hap[h+1]);
						bool he = a0 != a1;
						hap1[index].push_back(a0);
						hap2[index].push_back(a1);
						psolved[index].push_back(!mi && he && ph);
						psets[index].push_back(0);
						if (nps == n_samples_hap && ps_arr[h/2] != bcf_int32_missing) psets[index].back() = ps_arr[h/2];
						serrors[index].push_back(-1);
						terrors[index].push_back(false);
					}
					cref += !a0 + !a1;
					calt += a0 + a1;
					ctot += 2;
				}
				mac.push_back(min(cref, calt));
				maf.push_back(min(cref, calt) * 1.0 / ctot);
				n_variant_bia ++;
			}
			n_variant_set ++;
		}
		n_variant_tot ++;
	}

	vrb.bullet("#variants: total = " + stb.str(n_variant_tot) + " / set = " + stb.str(n_variant_set) + " / bi-allelic = " + stb.str(n_variant_bia));;
	free(gt_arr_gen);
	free(gt_arr_hap);
	bcf_sr_destroy(sr);
}

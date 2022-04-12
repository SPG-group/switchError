# switchError
Compute switch error rates

This code was written by Olivier Delaneau with build environment and documentation contributions from Arjun Biddanda.

## Building the executable

The environment to build the software can be conducted by `conda env create -f env.yaml`. You can then subsequently run `conda activate switchError` and `make`, which should build the executable for you in `bin/switchError`.

## Input File Definitions

* "--gen": BCF formatted file of genotyped individuals [required]
* "--hap": BCF formatted file of individuals with estimated haplotypes [required] 
* "--fam": PLINK formatted FAM file for evaluating IDs 
* "--reg": BED-formatted region string for evaluating switch error rates  [required] 
* "--ps": Phase set information (more information [here](http://mathgen.stats.ox.ac.uk/genetics_software/shapeit/shapeit.html#readaware))
* "--out": output prefix, the outputfiles have extensions `*.iser`, `*.vser`,`*.mser.gz`  [required]
* "--maf": minor allele frequency cutoff

## Output File Definitions

1. `*.iser`

The columns in order are:
  * individual index
  * individual name
  * number of mendel errors
  * number of opportunities for mendel errors
  * number of typing errors
  * number of opportunities for typing errors
  * number of switch errors 
  * number of opportunities for switch errors

2. `*.vser`
  * variant-level index
  * position of variant
  * minor allele count
  * minor allele frequency 
  * number of mendel errors
  * number of opportunities for mendel errors
  * number of typing errors
  * number of opportunities for typing errors
  * number of switch errors 
  * number of opportunities for switch errors

Across both settings, we largely want to look at how to use the last two columns and their distribution across individuals. 

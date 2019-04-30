##########################################
# SET CORRECTLY THESE 6 PATHS TO COMPILE #
##########################################
BOOST_INC=
BOOST_LIB=
RMATH_INC=
RMATH_LIB=
HTSLD_INC=
HTSLD_LIB=

#COMPILER MODE C++11
CXX=g++ -std=c++0x

#COMPILER FLAGS
CXXFLAG_REL=-O2
CXXFLAG_DBG=-g
CXXFLAG_WRN=-Wall -Wextra -Wno-sign-compare -Wno-unused-local-typedefs -Wno-deprecated -Wno-unused-parameter

#BASE LIBRARIES
LIB_FLAGS=-lz -lbz2 -lm -lpthread -llzma

#FILE LISTS
BFILE=bin/swithError
HFILE=$(shell find src -name *.h)
TFILE=$(shell find lib -name *.h)
CFILE=$(shell find src -name *.cpp)
OFILE=$(shell for file in `find src -name *.cpp`; do echo obj/$$(basename $$file .cpp).o; done)
VPATH=$(shell for file in `find src -name *.cpp`; do echo $$(dirname $$file); done)

#DEFAULT VERSION (I.E. UNIGE DESKTOP RELEASE VERSION)
all: desktop

#UNIGE DESKTOP RELEASE VERSION
desktop: RMATH_INC=$(HOME)/Tools/R-3.5.1/src/include
desktop: RMATH_LIB=$(HOME)/Tools/R-3.5.1/src/nmath/standalone
desktop: HTSLD_INC=$(HOME)/Tools/htslib-1.9
desktop: HTSLD_LIB=$(HOME)/Tools/htslib-1.9
desktop: BOOST_INC=/usr/include
desktop: BOOST_LIB=/usr/lib/x86_64-linux-gnu
desktop: CXXFLAG=$(CXXFLAG_REL) $(CXXFLAG_WRN)
desktop: IFLAG=-Ilib/OTools -Ilib -I$(RMATH_INC) -I$(HTSLD_INC) -I$(BOOST_INC)
desktop: LIB_FILES=$(RMATH_LIB)/libRmath.a $(HTSLD_LIB)/libhts.a $(BOOST_LIB)/libboost_iostreams.a $(BOOST_LIB)/libboost_program_options.a
desktop: LDFLAG=$(CXXFLAG_REL)
desktop: $(BFILE)

#UNIL SERVER RELEASE VERSION
dcb2: LIB_FLAGS=-lz -lbz2 -lm -lpthread -lcurl -lcrypto -lssl -llzma
dcb2: RMATH_INC=/software/R/3.4.2/include
dcb2: RMATH_LIB=/software/R/3.4.2/lib64
dcb2: HTSLD_INC=/home/oldelan/lib/htslib-1.7
dcb2: HTSLD_LIB=/home/oldelan/lib/htslib-1.7
dcb2: BOOST_INC=/software/include
dcb2: BOOST_LIB=/software/lib64
dcb2: CXXFLAG=$(CXXFLAG_REL)
dcb2: IFLAG=-Ilib -Ilib/OTools -I$(HTSLD_INC) -I$(BOOST_INC) -I$(RMATH_INC)
dcb2: LIB_FILES=$(HTSLD_LIB)/libhts.a $(BOOST_LIB)/libboost_iostreams.a $(BOOST_LIB)/libboost_program_options.a
dcb2: LDFLAG=$(CXXFLAG_REL)
dcb2: $(BFILE)


#COMPILATION RULES
$(BFILE): $(OFILE)
	$(CXX) $^ $(LIB_FILES) -o $@ $(LIB_FLAGS) $(LDFLAG)

obj/switchError.o: src/switchError.cpp $(HFILE) $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

obj/data_mendel.o: src/data_mendel.cpp src/data.h $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

obj/data_write.o: src/data_write.cpp src/data.h $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

obj/data_read.o: src/data_read.cpp src/data.h $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

obj/data_switch.o: src/data_switch.cpp src/data.h $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

obj/data_base.o: src/data_base.cpp src/data.h $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

clean: 
	rm -f obj/*.o $(BFILE)

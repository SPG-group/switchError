##########################################
# SET CORRECTLY THESE 6 PATHS TO COMPILE #
##########################################
BOOST_INC=$(CONDA_PREFIX)/include
BOOST_LIB=$(CONDA_PREFIX)/lib
RMATH_INC=$(CONDA_PREFIX)/include
RMATH_LIB=$(CONDA_PREFIX)/lib
HTSLD_INC=$(CONDA_PREFIX)/include
HTSLD_LIB=$(CONDA_PREFIX)/lib

#COMPILER MODE C++11
CXX=g++ -std=c++0x

#COMPILER FLAGS
CXXFLAG_REL=-O2
CXXFLAG_DBG=-g
CXXFLAG_WRN=-Wall -Wextra -Wno-sign-compare -Wno-unused-local-typedefs -Wno-deprecated -Wno-unused-parameter

#BASE LIBRARIES
LIB_FLAGS=-lz -lbz2 -lm -lpthread -llzma  

#FILE LISTS
BFILE=bin/switchError
HFILE=$(shell find src -name *.h)
TFILE=$(shell find lib -name *.h)
CFILE=$(shell find src -name *.cpp)
OFILE=$(shell for file in `find src -name *.cpp`; do echo obj/$$(basename $$file .cpp).o; done)
VPATH=$(shell for file in `find src -name *.cpp`; do echo $$(dirname $$file); done)

#DEFAULT VERSION 
all: desktop

#UNIGE DESKTOP RELEASE VERSION
desktop: CXXFLAG=$(CXXFLAG_REL) $(CXXFLAG_WRN)
desktop: IFLAG=-Ilib/OTools -Ilib -I$(RMATH_INC) -I$(HTSLD_INC) -I$(BOOST_INC)
desktop: LIB_FILES=-L$(RMATH_LIB)/ -lRmath  -lboost_iostreams -lboost_program_options -lhts
desktop: LDFLAG=$(CXXFLAG_REL)
desktop: $(BFILE)


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

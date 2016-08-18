CC   = gcc
CXX  = g++

LIBNAME = reader.a
all : $(LIBNAME)
SRC = $(wildcard *.cpp) 
TGT = $(SRC:.cpp=.o)
INCLUDE  =  -I./ -I../include -I./tools/libusb-0.1.8

SHAREDLIB =  ../lib/libusb.a 

$(SRC) : 
	@touch $@
%.o : %.cpp
	$(CXX) -c $? $(INCLUDE)
	
$(LIBNAME) : $(TGT)
	ar crv -o $@ $(TGT)
	cp $(LIBNAME) ../lib/
clean:
	rm -f *.o $(LIBNAME)

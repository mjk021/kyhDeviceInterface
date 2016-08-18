CC   = gcc
CXX  = g++

LIBNAME = cos.a
all : $(LIBNAME)
SRC = $(wildcard *.cpp) 
TGT = $(SRC:.cpp=.o)
INCLUDE  =  -I./ -I../include
LIBS = ../lib/reader.a


$(SRC) : 
	@touch $@
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@ $(INCLUDE)
	
$(LIBNAME) :$(LIBS)  $(TGT) 
	ar crv -o $(LIBNAME) $(TGT)
	cp $(LIBNAME) ../lib/
clean:
	rm -f *.o $(LIBNAME)

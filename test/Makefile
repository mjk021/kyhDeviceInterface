SOURCE = main.c 
INCLUDE  =  -I./ -I../include
LIBS =  ../lib/cos.a ../lib/reader.a ../lib/libusb.a
all:
	gcc $(SOURCE) $(LIBS) $(INCLUDE) -lstdc++ -lpthread -o test 
clean:
	rm test

POBJS		= Data.o Module.o RoutingTable.o Audio.o Delay.o DAC.o ADC.o Coordinator.o MToken.o Envelope.o Sine.o AudioSource.o ModuleController.o ModuleList.o Serial.o XBeeController.o Tile.o AudioClock.o AudioClock.o Server.o
CFLAGS = -I ../include -I../include/lo -I../include/rtsosc

CC          	= g++

.cpp.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean veryclean

all:	librtsosc.a

clean:
	rm -f $(POBJS)   
	rm -f librtsosc.a;

veryclean:
	rm -f $(POBJS)   
	rm -f librtsosc.a;
	cd ../lib; rm -f librtsosc.a;

librtsosc.a:	$(POBJS)
	ar -rc librtsosc.a $(POBJS)
	ranlib  librtsosc.a

install:	librtsosc.a
	cp librtsosc.a ../lib

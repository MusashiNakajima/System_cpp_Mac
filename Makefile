INCLUDES 	= -Iinclude -I./include/lo
LIBS 		= -Llib -llo -lportaudio -lpm -lpt
FRAMEWORKS	= -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices -framework CoreMIDI
SYSTEM		= main.o Data.o Module.o RoutingTable.o Audio.o Delay.o DAC.o ADC.o Coordinator.o MToken.o
MIDI		= MIDIMain.o
CC          	= g++
PROGS		= System midi

.PHONY: liblo pmmac porttime clean

all:	liblo pmmac porttime System midi

liblo:
	cd liblo; make; make install;

pmmac:
	cd pm_mac; make; make install;

porttime:
	cd porttime; make; make install;

.cpp.o:
	$(CC) $(INCLUDES) -c $<

midi: $(MIDI)
	$(CC) -o $@ $(MIDI) $(LIBS) $(INCLUDES) $(FRAMEWORKS)

System: $(SYSTEM)
	$(CC) -o $@ $(SYSTEM) $(LIBS) $(INCLUDES) $(FRAMEWORKS)
clean:
	rm -f *.o   
	rm -f $(PROGS)
	cd liblo; make clean; cd ../
	cd pm_mac; make clean; cd ../
	cd porttime; make clean; cd ../

	
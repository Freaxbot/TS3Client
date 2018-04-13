PROGRAM = test

INCLUDEDIRS = \
	-I/usr/local/include

LIBDIRS = \
	-L/usr/local/lib

LIBS = -ltomcrypt -ltommath

CXXSOURCES = client.cc PacketHandler.cc crypto.cc Packet.cc Util.cc header/client.h header/crypto.h header/Packet.h header/PacketHandler.h header/Util.h
CXXREMOVE = client.o crypto.o Packet.o PacketHandler.o Util.o 
CXXOBJECTS = $(CXXSOURCES:.cc=.o)  # expands to list of object files
CXXFLAGS = -DESRI_UNIX $(INCLUDEDIRS) -DUSE_LTM -DLTM_DESC
CXX = g++

LDFLAGS = $(LIBDIRS) $(LIBS)

main: $(PROGRAM)

$(PROGRAM): $(CXXOBJECTS) 
	$(CXX) -o $@ $(CXXOBJECTS) $(CXXFLAGS) $(LDFLAGS)

clean:
	$(RM) -f $(CXXREMOVE) $(PROGRAM)

CPP = g++

SERVER = SERVER
CLIENT = CLIENT
TARGET_SERVER = startServer
OBJECT_SERVER = startServer.o
LIB_SERVER    = tcpServer
LLIB_SERVER   = -ltcpServer
TARGET_CLIENT = startClient
OBJECT_CLIENT = startClient.o
LIB_CLIENT    = tcpClient
LLIB_CLIENT   = -ltcpClient
TARGETS = $(TARGET_SERVER) $(TARGET_CLIENT)
OBJECTS = $(OBJECT_SERVER) $(OBJECT_CLIENT)
LIBS = lib$(LIB_SERVER).so lib$(LIB_CLIENT).so

all: clean $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(OBJECT_SERVER) $(LIB_SERVER)
	$(CPP) -o $@ $< -L . $(LLIB_SERVER) -lpthread

$(TARGET_CLIENT): $(OBJECT_CLIENT) $(LIB_CLIENT)
	$(CPP) -o $@ $< -L . $(LLIB_CLIENT)

$(OBJECT_SERVER): %.o: %.cpp
	$(CPP) -c -g -o $@ $<

$(OBJECT_CLIENT): %.o: %.cpp
	$(CPP) -c -g -o $@ $<

$(LIB_SERVER): %: %.cpp
	$(CPP) -fPIC -shared -o lib$@.so $< -lpthread

$(LIB_CLIENT): %: %.cpp
	$(CPP) -fPIC -shared -o lib$@.so $<

.PHONY: clean
.INTERMEDIATE: $(OBJECTS)
clean:
	-rm $(TARGETS)
	-rm $(LIBS)

.PHONY: clean
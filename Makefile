CC=g++
CFLAGS = -c -Wall -std=c++14 -fPIC
LDFLAGS = -lfcgi -lpthread -lcurl

SOURCES = framework/Application.cpp\
    framework/request/Request.cpp
	
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = onyx

all: create_required_dirs $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -shared -o build/libonyx.so $(OBJECTS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(OBJECTS)
	
create_required_dirs:
	@if [ ! -d build ]; then mkdir build; fi

OWNER_USER = myduomilia

install:
	cp build/libonyx.so /usr/lib/
	@if [ ! -d /usr/include/onyx ]; then mkdir /usr/include/onyx; fi
	@if [ ! -d /var/log/onyx ]; then mkdir /var/log/onyx; fi
	cp framework/Application.h /usr/include/onyx/
	cp framework/request/Request.h /usr/include/onyx/
	cp framework/response/BaseResponse.h /usr/include/onyx/
	cp framework/response/JsonResponse.h /usr/include/onyx/
	cp -r framework/common /usr/include/onyx/
	sudo ldconfig
	
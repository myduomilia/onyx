CC=g++
CFLAGS = -c -Wall -std=c++14 -fPIC
LDFLAGS = -lfcgi -lpthread -lcurl -lssl -lcrypto

SOURCES = framework/dispatcher/Dispatcher.cpp\
    framework/token/Token.cpp\
    framework/param/Param.cpp\
    framework/cookie/Cookie.cpp\
    framework/object/ONObject.cpp\
    framework/handlers/404.cpp\
    framework/handlers/403.cpp\
    framework/session/Session.cpp\
    framework/security/Security.cpp\
    framework/Application.cpp
    
	
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

install:
	cp build/libonyx.so /usr/lib/
	@if [ ! -d /usr/include/onyx ]; then mkdir /usr/include/onyx; fi
	@if [ ! -d /usr/include/onyx/dispatcher ]; then mkdir /usr/include/onyx/dispatcher; fi
	@if [ ! -d /usr/include/onyx/exception ]; then mkdir /usr/include/onyx/exception; fi
	@if [ ! -d /usr/include/onyx/request ]; then mkdir /usr/include/onyx/request; fi
	@if [ ! -d /usr/include/onyx/response ]; then mkdir /usr/include/onyx/response; fi
	@if [ ! -d /usr/include/onyx/token ]; then mkdir /usr/include/onyx/token; fi
	@if [ ! -d /usr/include/onyx/param ]; then mkdir /usr/include/onyx/param; fi
	@if [ ! -d /usr/include/onyx/object ]; then mkdir /usr/include/onyx/object; fi
	@if [ ! -d /usr/include/onyx/cookie ]; then mkdir /usr/include/onyx/cookie; fi
	@if [ ! -d /usr/include/onyx/session ]; then mkdir /usr/include/onyx/session; fi
	@if [ ! -d /usr/include/onyx/handlers ]; then mkdir /usr/include/onyx/handlers; fi
	@if [ ! -d /usr/include/onyx/security ]; then mkdir /usr/include/onyx/security; fi
	@if [ ! -d /var/log/onyx ]; then mkdir /var/log/onyx; fi
	cp framework/Application.h /usr/include/onyx/
	cp framework/dispatcher/Dispatcher.h /usr/include/onyx/dispatcher/
	cp framework/exception/Exception.h /usr/include/onyx/exception/
	cp framework/request/Request.h /usr/include/onyx/request/
	cp framework/response/BaseResponse.h /usr/include/onyx/response/
	cp framework/response/JsonResponse.h /usr/include/onyx/response/
	cp framework/response/HtmlResponse.h /usr/include/onyx/response/
	cp framework/response/RedirectResponse.h /usr/include/onyx/response/
	cp framework/session/Session.h /usr/include/onyx/session/
	cp framework/security/Security.h /usr/include/onyx/security/
	cp framework/token/Token.h /usr/include/onyx/token/
	cp framework/param/Param.h /usr/include/onyx/param/
	cp framework/cookie/Cookie.h /usr/include/onyx/cookie/
	cp framework/object/ONObject.h /usr/include/onyx/object/
	cp framework/handlers/404.h /usr/include/onyx/handlers/
	cp framework/handlers/403.h /usr/include/onyx/handlers/
	cp -r framework/common /usr/include/onyx/
	sudo ldconfig
	
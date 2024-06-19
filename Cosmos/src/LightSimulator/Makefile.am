lib_LTLIBRARIES = libClientSimLight.la

libClientSimLight_la_SOURCES = \
	clientsimLight.cpp \
	Event.cpp  Event.hpp \
	EventsQueueLight.cpp EventsQueueLight.hpp \
	SimLight.cpp SimLight.hpp \
        Print.h Print.cpp \
	SerialPort.cpp SerialPort.h \
	SocketPort.h SocketPort.cpp \
	spnLight.hpp

includes_HEADERS = spnLight.hpp macro.hpp Print.h
includesdir = $(includedir)
AM_CPPFLAGS = -Wall -Wextra  -Wno-unused-local-typedef  -Wno-unused-parameter $(BOOST_CPPFLAGS)

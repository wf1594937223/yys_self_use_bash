CXX = g++
CXXFLAGS = -Wall -std=c++17
EXTRA = -fexec-charset=GBK -finput-charset=UTF-8

TARGET = fei.exe

all: $(TARGET)

$(TARGET): fei.cpp
	$(CXX) $(CXXFLAGS) fei.cpp -o $(TARGET) $(EXTRA)

clean:
	rm -f $(TARGET)

.PHONY: all clean
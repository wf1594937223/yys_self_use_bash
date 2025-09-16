CXX = g++
CXXFLAGS = -Wall -std=c++17 -static -static-libgcc -static-libstdc++
LDFLAGS = -lgdi32
EXTRA = -fexec-charset=GBK -finput-charset=UTF-8

TARGET = fei.exe
SRC = fei.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(EXTRA) $(LDFLAGS)

clean:
	-del $(TARGET)

.PHONY: all clean

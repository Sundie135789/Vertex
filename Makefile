CXX = g++
TARGET = main
SRCS = main.cpp variable.cpp

$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

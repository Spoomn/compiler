# clang++ -Wall -g -o myProgram Main.cpp Scanner.cpp StateMachine.cpp Token.cpp
# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -Wall -g -stdlib=libc++ -std=c++11

# Output program name
TARGET = myProgram

# Source files
SRCS = Main.cpp Token.cpp StateMachine.cpp Scanner.cpp Symbol.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule (compiles the program)
all: $(TARGET)

# Link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp files into .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Clean rule (removes object files and executable)
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything from scratch
rebuild: clean all
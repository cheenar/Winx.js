CXX = clang++
CXXFLAGS = -std=c++17 -O3 -Wall -pedantic
INCLUDES = -I./include/v8 -I./include/uv -I./include/toml++
LIBS = ./lib/v8/libv8_monolith.a ./lib/libuv/libuv.a
SRCDIR = src
OBJDIR = obj
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
TARGET = out/winx

.PHONY: clean

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
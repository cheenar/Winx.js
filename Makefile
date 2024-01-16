CXX = clang++
CXXFLAGS = -std=c++17 -O3 -Wall -pedantic
INCLUDES = -I./include/toml++ -I./include/v8 -I./include/uv -I./include
LIBS = ./lib/v8/libv8_monolith.a ./lib/libuv/libuv.a 
SRCDIR = src
OBJDIR = obj
SRCS = $(shell find $(SRCDIR)/ -type f -name '*.cpp')
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
TARGET = out/winx

.PHONY: clean

embedded_winx_polyfill: $(TARGET)
	@xxd -i $(SRCDIR)/polyfills/Winx.js > $(SRCDIR)/embedded_winx_polyfill.h

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(TARGET)

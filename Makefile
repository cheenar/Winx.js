CXX = clang++
CXXFLAGS = -std=c++17 -O3 -Wall -pedantic # -fsanitize=address
INCLUDES = -I./include/toml++ -I./include/v8 -I./include/uv -I./include
LIBS = ./lib/v8/libv8_monolith.a ./lib/libuv/libuv.a -shared-libasan
SRCDIR = src
OBJDIR = obj
SRCS = $(shell find $(SRCDIR)/ -type f -name '*.cpp')
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
TARGET = out/winx

ifdef MEM_SAFETY
	CXXFLAGS += -fsanitize=address
endif

.PHONY: clean

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp embedded_winx_polyfill
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

embedded_winx_polyfill: bundle_winx
	@xxd -i polyfills/Winx.js > $(SRCDIR)/embedded_winx_polyfill.h

bundle_winx:
	@./winx_bundler.py

clean:
	@rm -rf $(OBJDIR) $(TARGET)
	@rm $(SRCDIR)/embedded_winx_polyfill.h
	@rm polyfills/Winx.js

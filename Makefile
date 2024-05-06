#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

EXE = demo_app
SRCS = main.cpp DebugLog.cpp Client.cpp GuiWrapper.cpp
# SRCS = legacy.cpp
IMGUI = imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp
BACKENDS = imgui_impl_glfw.cpp imgui_impl_opengl3.cpp
LIBS_SRCS = Critical.cpp Buffer.cpp utils.cpp

SRCS_DIR = src/
IMGUI_DIR = imgui/
BACKENDS_DIR = backends/
INCS_DIR = inc/
LIBS_DIR = lib/
LIBS_INCS_DIR = $(LIBS_DIR)inc/
LIBS_SRCS_DIR = $(LIBS_DIR)src/
OBJS_DIR = obj/

SOURCES = $(addprefix $(SRCS_DIR), $(SRCS))
SOURCES += $(addprefix $(IMGUI_DIR), $(IMGUI))
SOURCES += $(addprefix $(BACKENDS_DIR), $(BACKENDS))
SOURCES += $(addprefix $(LIBS_SRCS_DIR), $(SRCS))

OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))
OBJS += $(addprefix $(OBJS_DIR), $(IMGUI:.cpp=.o))
OBJS += $(addprefix $(OBJS_DIR), $(BACKENDS:.cpp=.o))
OBJS += $(addprefix $(OBJS_DIR), $(LIBS_SRCS:.cpp=.o))

UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++17 -I$(IMGUI_DIR) -I$(BACKENDS_DIR) -I$(INCS_DIR) -I$(LIBS_INCS_DIR) -I$(LIBS_SRCS_DIR)
CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(OBJS_DIR)%.o:$(SRCS_DIR)%.cpp
	echo $(SOURCES)
	@test -d $(OBJS_DIR) || mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_DIR)%.o:$(IMGUI_DIR)%.cpp
	@test -d $(OBJS_DIR) || mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_DIR)%.o:$(BACKENDS_DIR)%.cpp
	@test -d $(OBJS_DIR) || mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_DIR)%.o:$(LIBS_SRCS_DIR)%.cpp
	@test -d $(OBJS_DIR) || mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -rf $(EXE) $(OBJS_DIR) *.ini

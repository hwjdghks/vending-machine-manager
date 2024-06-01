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
# SERVER = server

CLIENT_SRCS = main.cpp
# SERVER_SRCS = Server.cpp
SRCS = DebugLog.cpp Client.cpp Socket.cpp Page.cpp Shelf.cpp CashTray.cpp VendingMachine.cpp Program.cpp
IMGUI_SRCS = imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp
BACKENDS_SRCS = imgui_impl_glfw.cpp imgui_impl_opengl3.cpp
LIBS_SRCS = Buffer.cpp utils.cpp
SRCS_DIR = src/
IMGUI_DIR = imgui/
BACKENDS_DIR = backends/
INCS_DIR = inc/
LIBS_DIR = lib/
LIBS_INCS_DIR = $(LIBS_DIR)inc/
LIBS_SRCS_DIR = $(LIBS_DIR)src/
OBJS_DIR = obj/

OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))
OBJS += $(addprefix $(OBJS_DIR), $(IMGUI_SRCS:.cpp=.o))
OBJS += $(addprefix $(OBJS_DIR), $(BACKENDS_SRCS:.cpp=.o))
OBJS += $(addprefix $(OBJS_DIR), $(LIBS_SRCS:.cpp=.o))

C_OBJS = $(addprefix $(OBJS_DIR), $(CLIENT_SRCS:.cpp=.o))
C_OBJS += $(OBJS)

# S_OBJS = $(addprefix $(OBJS_DIR), $(SERVER_SRCS:.cpp=.o))
# S_OBJS += $(OBJS)

UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++17 -I$(IMGUI_DIR) -I$(BACKENDS_DIR) -I$(INCS_DIR) -I$(LIBS_INCS_DIR) -I$(LIBS_SRCS_DIR)
CXXFLAGS += -g -Wall -Wformat -pthread
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

all: $(EXE) $(SERVER)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(C_OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

# $(SERVER): $(S_OBJS)
# 	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

$(OBJS_DIR)%.o:$(SRCS_DIR)%.cpp
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

clean:
	rm -rf $(EXE) $(SERVER) $(OBJS_DIR) *.ini

re:
	make clean
	make
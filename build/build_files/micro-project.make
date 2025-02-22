# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq ($(shell echo "test"), "test")
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

ifeq ($(origin CC), default)
  CC = gcc
endif
ifeq ($(origin CXX), default)
  CXX = g++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
INCLUDES += -I../../src -I../../include -I../external/raylib-master/src -I../external/raylib-master/src/external -I../external/raylib-master/src/external/glfw/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/x64/Debug/micro-project
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -g -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),debug_x86)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/x86/Debug/micro-project
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -g -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/ARM64/Debug/micro-project
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -g -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug/libraylib.a
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_x64)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/x64/Release/micro-project
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -O2 -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),release_x86)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/x86/Release/micro-project
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -O2 -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

else ifeq ($(config),release_arm64)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/ARM64/Release/micro-project
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -O2 -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -s

else ifeq ($(config),debug_rgfw_x64)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/x64/Debug_RGFW/micro-project
DEFINES += -DDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -g -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -g -std=c++17
LIBS += ../../bin/Debug_RGFW/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug_RGFW/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),debug_rgfw_x86)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/x86/Debug_RGFW/micro-project
DEFINES += -DDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -g -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -g -std=c++17
LIBS += ../../bin/Debug_RGFW/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug_RGFW/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),debug_rgfw_arm64)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/ARM64/Debug_RGFW/micro-project
DEFINES += -DDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -g -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -g -std=c++17
LIBS += ../../bin/Debug_RGFW/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug_RGFW/libraylib.a
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_rgfw_x64)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/x64/Release_RGFW/micro-project
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -O2 -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release_RGFW/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release_RGFW/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),release_rgfw_x86)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/x86/Release_RGFW/micro-project
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -O2 -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release_RGFW/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release_RGFW/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

else ifeq ($(config),release_rgfw_arm64)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/micro-project
OBJDIR = obj/ARM64/Release_RGFW/micro-project
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -O2 -std=c17
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release_RGFW/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release_RGFW/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/main.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking micro-project
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning micro-project
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/main.o: ../../src/main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_x64)
  micro_project_config = debug_x64
  raylib_config = debug_x64

else ifeq ($(config),debug_x86)
  micro_project_config = debug_x86
  raylib_config = debug_x86

else ifeq ($(config),debug_arm64)
  micro_project_config = debug_arm64
  raylib_config = debug_arm64

else ifeq ($(config),release_x64)
  micro_project_config = release_x64
  raylib_config = release_x64

else ifeq ($(config),release_x86)
  micro_project_config = release_x86
  raylib_config = release_x86

else ifeq ($(config),release_arm64)
  micro_project_config = release_arm64
  raylib_config = release_arm64

else ifeq ($(config),debug_rgfw_x64)
  micro_project_config = debug_rgfw_x64
  raylib_config = debug_rgfw_x64

else ifeq ($(config),debug_rgfw_x86)
  micro_project_config = debug_rgfw_x86
  raylib_config = debug_rgfw_x86

else ifeq ($(config),debug_rgfw_arm64)
  micro_project_config = debug_rgfw_arm64
  raylib_config = debug_rgfw_arm64

else ifeq ($(config),release_rgfw_x64)
  micro_project_config = release_rgfw_x64
  raylib_config = release_rgfw_x64

else ifeq ($(config),release_rgfw_x86)
  micro_project_config = release_rgfw_x86
  raylib_config = release_rgfw_x86

else ifeq ($(config),release_rgfw_arm64)
  micro_project_config = release_rgfw_arm64
  raylib_config = release_rgfw_arm64

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := micro-project raylib

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

micro-project: raylib
ifneq (,$(micro_project_config))
	@echo "==== Building micro-project ($(micro_project_config)) ===="
	@${MAKE} --no-print-directory -C build/build_files -f micro-project.make config=$(micro_project_config)
endif

raylib:
ifneq (,$(raylib_config))
	@echo "==== Building raylib ($(raylib_config)) ===="
	@${MAKE} --no-print-directory -C build/build_files -f raylib.make config=$(raylib_config)
endif

clean:
	@${MAKE} --no-print-directory -C build/build_files -f micro-project.make clean
	@${MAKE} --no-print-directory -C build/build_files -f raylib.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_x64"
	@echo "  debug_x86"
	@echo "  debug_arm64"
	@echo "  release_x64"
	@echo "  release_x86"
	@echo "  release_arm64"
	@echo "  debug_rgfw_x64"
	@echo "  debug_rgfw_x86"
	@echo "  debug_rgfw_arm64"
	@echo "  release_rgfw_x64"
	@echo "  release_rgfw_x86"
	@echo "  release_rgfw_arm64"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   micro-project"
	@echo "   raylib"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"

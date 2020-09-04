# Makefile for QEMU.

ifneq ($(words $(subst :, ,$(CURDIR))), 1)
  $(error main directory cannot contain spaces nor colons)
endif

# Always point to the root of the build tree (needs GNU make).
BUILD_DIR=$(CURDIR)

# Before including a proper config-host.mak, assume we are in the source tree
SRC_PATH=.

UNCHECKED_GOALS := %clean TAGS cscope ctags dist \
    help check-help print-% \
    docker docker-% vm-help vm-test vm-build-%

# All following code might depend on configuration variables
ifneq ($(wildcard config-host.mak),)
# Put the all: rule here so that config-host.mak can contain dependencies.
all:
include config-host.mak

git-submodule-update:

.PHONY: git-submodule-update

git_module_status := $(shell \
  cd '$(SRC_PATH)' && \
  GIT="$(GIT)" ./scripts/git-submodule.sh status $(GIT_SUBMODULES); \
  echo $$?; \
)

ifeq (1,$(git_module_status))
ifeq (no,$(GIT_UPDATE))
git-submodule-update:
	$(call quiet-command, \
            echo && \
            echo "GIT submodule checkout is out of date. Please run" && \
            echo "  scripts/git-submodule.sh update $(GIT_SUBMODULES)" && \
            echo "from the source directory checkout $(SRC_PATH)" && \
            echo && \
            exit 1)
else
git-submodule-update:
	$(call quiet-command, \
          (cd $(SRC_PATH) && GIT="$(GIT)" ./scripts/git-submodule.sh update $(GIT_SUBMODULES)), \
          "GIT","$(GIT_SUBMODULES)")
endif
endif

export NINJA=./ninjatool

# Running meson regenerates both build.ninja and ninjatool, and that is
# enough to prime the rest of the build.
ninjatool: build.ninja

Makefile.ninja: build.ninja ninjatool
	./ninjatool -t ninja2make --omit clean dist uninstall cscope TAGS ctags < $< > $@
-include Makefile.ninja

${ninja-targets-c_COMPILER} ${ninja-targets-cpp_COMPILER}: .var.command += -MP

# If MESON is empty, the rule will be re-evaluated after Makefiles are
# reread (and MESON won't be empty anymore).
ifneq ($(MESON),)
Makefile.mtest: build.ninja scripts/mtest2make.py
	$(MESON) introspect --tests | $(PYTHON) scripts/mtest2make.py > $@
-include Makefile.mtest
endif

.git-submodule-status: git-submodule-update config-host.mak

# Check that we're not trying to do an out-of-tree build from
# a tree that's been used for an in-tree build.
ifneq ($(realpath $(SRC_PATH)),$(realpath .))
ifneq ($(wildcard $(SRC_PATH)/config-host.mak),)
$(error This is an out of tree build but your source tree ($(SRC_PATH)) \
seems to have been used for an in-tree build. You can fix this by running \
"$(MAKE) distclean && rm -rf *-linux-user *-softmmu" in your source tree)
endif
endif

CONFIG_SOFTMMU := $(if $(filter %-softmmu,$(TARGET_DIRS)),y)
CONFIG_USER_ONLY := $(if $(filter %-user,$(TARGET_DIRS)),y)
CONFIG_XEN := $(CONFIG_XEN_BACKEND)
CONFIG_ALL=y
-include config-all-devices.mak
-include config-all-disas.mak

config-host.mak: $(SRC_PATH)/configure $(SRC_PATH)/pc-bios $(SRC_PATH)/VERSION
	@echo $@ is out-of-date, running configure
	@if test -f meson-private/coredata.dat; then \
	  ./config.status --skip-meson; \
	else \
	  ./config.status; \
	fi

# Force configure to re-run if the API symbols are updated
ifeq ($(CONFIG_PLUGIN),y)
config-host.mak: $(SRC_PATH)/plugins/qemu-plugins.symbols
endif

else
config-host.mak:
ifneq ($(filter-out $(UNCHECKED_GOALS),$(MAKECMDGOALS)),$(if $(MAKECMDGOALS),,fail))
	@echo "Please call configure before running make!"
	@exit 1
endif
endif

# Only needed in case Makefile.ninja does not exist.
.PHONY: ninja-clean ninja-distclean clean-ctlist
clean-ctlist:
ninja-clean::
ninja-distclean::
build.ninja: config-host.mak

include $(SRC_PATH)/rules.mak

# lor is defined in rules.mak
CONFIG_BLOCK := $(call lor,$(CONFIG_SOFTMMU),$(CONFIG_TOOLS))

generated-files-y += .git-submodule-status

# Don't try to regenerate Makefile or configure
# We don't generate any of them
Makefile: ;
configure: ;

.PHONY: all clean cscope distclean install \
	recurse-all dist msi FORCE

$(call set-vpath, $(SRC_PATH))

LIBS+=-lz $(LIBS_TOOLS)

SUBDIR_MAKEFLAGS=$(if $(V),,--no-print-directory --quiet) BUILD_DIR=$(BUILD_DIR)

ifneq ($(wildcard config-host.mak),)
include $(SRC_PATH)/Makefile.objs
endif

include $(SRC_PATH)/tests/Makefile.include

all: recurse-all
Makefile: $(addsuffix /all, $(SUBDIRS))

# LIBFDT_lib="": avoid breaking existing trees with objects requiring -fPIC
DTC_MAKE_ARGS=-I$(SRC_PATH)/dtc VPATH=$(SRC_PATH)/dtc -C dtc V="$(V)" LIBFDT_lib=""
DTC_CFLAGS=$(CFLAGS) $(QEMU_CFLAGS)
DTC_CPPFLAGS=-I$(SRC_PATH)/dtc/libfdt

.PHONY: dtc/all
dtc/all: .git-submodule-status dtc/libfdt
	$(call quiet-command,$(MAKE) $(DTC_MAKE_ARGS) CPPFLAGS="$(DTC_CPPFLAGS)" CFLAGS="$(DTC_CFLAGS)" LDFLAGS="$(QEMU_LDFLAGS)" ARFLAGS="$(ARFLAGS)" CC="$(CC)" AR="$(AR)" LD="$(LD)" $(SUBDIR_MAKEFLAGS) libfdt,)

dtc/%: .git-submodule-status
	@mkdir -p $@

# Overriding CFLAGS causes us to lose defines added in the sub-makefile.
# Not overriding CFLAGS leads to mis-matches between compilation modes.
# Therefore we replicate some of the logic in the sub-makefile.
# Remove all the extra -Warning flags that QEMU uses that Capstone doesn't;
# no need to annoy QEMU developers with such things.
CAP_CFLAGS = $(patsubst -W%,,$(CFLAGS) $(QEMU_CFLAGS))
CAP_CFLAGS += -DCAPSTONE_USE_SYS_DYN_MEM
CAP_CFLAGS += -DCAPSTONE_HAS_ARM
CAP_CFLAGS += -DCAPSTONE_HAS_ARM64
CAP_CFLAGS += -DCAPSTONE_HAS_POWERPC
CAP_CFLAGS += -DCAPSTONE_HAS_X86

.PHONY: capstone/all
capstone/all: .git-submodule-status
	$(call quiet-command,$(MAKE) -C $(SRC_PATH)/capstone CAPSTONE_SHARED=no BUILDDIR="$(BUILD_DIR)/capstone" CC="$(CC)" AR="$(AR)" LD="$(LD)" RANLIB="$(RANLIB)" CFLAGS="$(CAP_CFLAGS)" $(SUBDIR_MAKEFLAGS) $(BUILD_DIR)/capstone/$(LIBCAPSTONE))

.PHONY: slirp/all
slirp/all: .git-submodule-status
	$(call quiet-command,$(MAKE) -C $(SRC_PATH)/slirp		\
		BUILD_DIR="$(BUILD_DIR)/slirp" 			\
		PKG_CONFIG="$(PKG_CONFIG)" 				\
		CC="$(CC)" AR="$(AR)" 	LD="$(LD)" RANLIB="$(RANLIB)"	\
		CFLAGS="$(QEMU_CFLAGS) $(CFLAGS)" LDFLAGS="$(QEMU_LDFLAGS)")

ROM_DIRS = $(addprefix pc-bios/, $(ROMS))
ROM_DIRS_RULES=$(foreach t, all clean, $(addsuffix /$(t), $(ROM_DIRS)))
# Only keep -O and -g cflags
.PHONY: $(ROM_DIRS_RULES)
$(ROM_DIRS_RULES):
	$(call quiet-command,$(MAKE) $(SUBDIR_MAKEFLAGS) -C $(dir $@) V="$(V)" TARGET_DIR="$(dir $@)" $(notdir $@),)

.PHONY: recurse-all recurse-clean
recurse-all: $(addsuffix /all, $(ROM_DIRS))
recurse-clean: $(addsuffix /clean, $(ROM_DIRS))

######################################################################

clean: recurse-clean ninja-clean clean-ctlist
	if test -f ninjatool; then ./ninjatool $(if $(V),-v,) -t clean; fi
# avoid old build problems by removing potentially incorrect old files
	rm -f config.mak op-i386.h opc-i386.h gen-op-i386.h op-arm.h opc-arm.h gen-op-arm.h
	find . \( -name '*.so' -o -name '*.dll' -o -name '*.[oda]' \) -type f \
		! -path ./roms/edk2/ArmPkg/Library/GccLto/liblto-aarch64.a \
		! -path ./roms/edk2/ArmPkg/Library/GccLto/liblto-arm.a \
		! -path ./roms/edk2/BaseTools/Source/Python/UPT/Dll/sqlite3.dll \
		-exec rm {} +
	rm -f TAGS cscope.* *.pod *~ */*~
	rm -f fsdev/*.pod scsi/*.pod
	rm -f $(foreach f,$(generated-files-y),$(f) $(f)-timestamp)

VERSION = $(shell cat $(SRC_PATH)/VERSION)

dist: qemu-$(VERSION).tar.bz2

qemu-%.tar.bz2:
	$(SRC_PATH)/scripts/make-release "$(SRC_PATH)" "$(patsubst qemu-%.tar.bz2,%,$@)"

distclean: clean ninja-distclean
	-test -f ninjatool && ./ninjatool $(if $(V),-v,) -t clean -g
	rm -f config-host.mak config-host.h*
	rm -f tests/tcg/config-*.mak
	rm -f config-all-disas.mak config.status
	rm -f tests/qemu-iotests/common.env
	rm -f roms/seabios/config.mak roms/vgabios/config.mak
	rm -f qemu-plugins-ld.symbols qemu-plugins-ld64.symbols
	rm -f *-config-target.h *-config-devices.mak *-config-devices.h
	rm -rf meson-private meson-logs meson-info compile_commands.json
	rm -f Makefile.ninja ninjatool ninjatool.stamp Makefile.mtest
	rm -f config.log
	rm -f linux-headers/asm
	rm -Rf .sdk

.PHONY: ctags
ctags:
	rm -f tags
	find "$(SRC_PATH)" -name '*.[hc]' -exec ctags --append {} +

.PHONY: TAGS
TAGS:
	rm -f TAGS
	find "$(SRC_PATH)" -name '*.[hc]' -exec etags --append {} +

.PHONY: cscope
cscope:
	rm -f "$(SRC_PATH)"/cscope.*
	find "$(SRC_PATH)/" -name "*.[chsS]" -print | sed -e 's,^\./,,' > "$(SRC_PATH)/cscope.files"
	cscope -b -i"$(SRC_PATH)/cscope.files"

# Needed by "meson install"
export DESTDIR

# Add a dependency on the generated files, so that they are always
# rebuilt before other object files
ifneq ($(wildcard config-host.mak),)
ifneq ($(filter-out $(UNCHECKED_GOALS),$(MAKECMDGOALS)),$(if $(MAKECMDGOALS),,fail))
Makefile: $(generated-files-y)
endif
endif

# Include automatically generated dependency files
# Dependencies in Makefile.objs files come from our recursive subdir rules
-include $(wildcard *.d tests/*.d)

include $(SRC_PATH)/tests/docker/Makefile.include
include $(SRC_PATH)/tests/vm/Makefile.include

print-help-run = printf "  %-30s - %s\\n" "$1" "$2"
print-help = $(quiet-@)$(call print-help-run,$1,$2)

.PHONY: help
help:
	@echo  'Generic targets:'
	$(call print-help,all,Build all)
	$(call print-help,dir/file.o,Build specified target only)
	$(call print-help,install,Install QEMU, documentation and tools)
	$(call print-help,ctags/TAGS,Generate tags file for editors)
	$(call print-help,cscope,Generate cscope index)
	$(call print-help,sparse,Run sparse on the QEMU source)
	@echo  ''
	@echo  'Cleaning targets:'
	$(call print-help,clean,Remove most generated files but keep the config)
	$(call print-help,distclean,Remove all generated files)
	$(call print-help,dist,Build a distributable tarball)
	@echo  ''
	@echo  'Test targets:'
	$(call print-help,check,Run all tests (check-help for details))
	$(call print-help,docker,Help about targets running tests inside containers)
	$(call print-help,vm-help,Help about targets running tests inside VM)
	@echo  ''
	@echo  'Documentation targets:'
	$(call print-help,html info pdf txt man,Build documentation in specified format)
	@echo  ''
ifdef CONFIG_WIN32
	@echo  'Windows targets:'
	$(call print-help,installer,Build NSIS-based installer for QEMU)
ifdef QEMU_GA_MSI_ENABLED
	$(call print-help,msi,Build MSI-based installer for qemu-ga)
endif
	@echo  ''
endif
	$(call print-help,$(MAKE) [targets],(quiet build, default))
	$(call print-help,$(MAKE) V=1 [targets],(verbose build))

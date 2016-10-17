
# Directory names
src_dir := src
out_dir := out

# C compiler
CC := i386-elf-gcc
CFLAGS := -Wall -Wextra -O2 -ffreestanding -I"$(src_dir)/include"
LDFLAGS := -nostdlib -lgcc

no-dep-targets = clean

# Helper macros
mkdirp = @dir=$(dir $@); [ -d "$$dir" ] || mkdir -p "$$dir"

# Build the complete OS image by default
all: $(out_dir)/Image

# That's easy, we just need to delete the output directory
.PHONY: clean
clean:
	rm -rf $(out_dir)

# Run the image in qemu
.PHONY: run
run: $(out_dir)/Image
	qemu-system-i386 -kernel $(out_dir)/Image

# These directories will be searched for a Makefile
MODULES := boot kernel lib

# Set up variables for included Makefiles
libs :=

# Include sub-Makefiles
include $(patsubst %, $(src_dir)/%/Makefile,$(MODULES))
#
objs := $(patsubst %,$(out_dir)/%,$(objs))
obj-dirs := $(dir $(objs))

# Build a .c file to an object
# Also generates a corresponding .d file for faster building
%.o: %.c
$(out_dir)/%.o: $(src_dir)/%.c $(out_dir)/%.d
	$(mkdirp)
	$(CC) -MT "$@" -MMD -MP -MF "$(out_dir)/$*.Td" $(CFLAGS) -o "$@" -c "$<"
	mv -f "$(out_dir)/$*.Td" "$(out_dir)/$*.d"

# Build a .S file to an object
# Also generates a corresponding .d file for faster building
%.o: %.S
$(out_dir)/%.o: $(src_dir)/%.S $(out_dir)/%.d
	$(mkdirp)
	$(CC) -MT "$@" -MMD -MP -MF "$(out_dir)/$*.Td" $(CFLAGS) -o "$@" -c "$<"
	mv -f "$(out_dir)/$*.Td" "$(out_dir)/$*.d"

# Link the kernel
$(out_dir)/Image: $(src_dir)/linker.ld $(objs)
	$(mkdirp)
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(src_dir)/linker.ld -o $@ $(libs) $(objs)

#
$(out_dir)/%.d: ;
.PRECIOUS: $(out_dir)/%.d

# Include .d files if needed
deps = $(patsubst %.o,%.d,$(objs))
ifeq ($(filter $(MAKECMDGOALS),$(no-dep-targets)),)
-include $(deps)
endif

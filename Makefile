
# Directory names
src_dir := src
out_dir := out

# C compiler
CC := i386-elf-gcc
CFLAGS := -Wall -Wextra -O2 -ffreestanding -I"$(src_dir)/include"
LDFLAGS := -nostdlib -lgcc

# Targets that don't need dependency files (.d) (e.g. clean)
# This allows us to only include them when they're actually needed
# so we are not building unnecessary stuff
no-dep-targets = clean

# Helper macros
# Recursively create the directory for the current target
# Usage: $(mkdirp)
mkdirp = @dir=$(dir $@); [ -d "$$dir" ] || mkdir -p "$$dir"

# Build the complete OS image by default
.PHONY: all
all: $(out_dir)/Image

# That's easy, we just need to delete the output directory
.PHONY: clean
clean:
	rm -rf $(out_dir)

# Run the image in qemu
.PHONY: run
run: $(out_dir)/Image
	qemu-system-i386 -kernel $(out_dir)/Image

$(out_dir)/Image.iso: $(out_dir)/Image $(src_dir)/grub.cfg
	$(mkdirp)
	mkdir -p $(out_dir)/iso/boot/grub/
	cp $(out_dir)/Image $(out_dir)/iso/boot/
	cp $(src_dir)/grub.cfg $(out_dir)/iso/boot/grub/
	grub-mkrescue -o $@ $(out_dir)/iso

# These directories will be searched for a Makefile
MODULES := boot kernel lib drivers

# Set up variables for included Makefiles
libs :=

# Include sub-Makefiles
-include $(patsubst %, $(src_dir)/%/Makefile,$(MODULES))
# Prepend the output directory to all object file paths
objs := $(addprefix $(out_dir)/,$(objs))

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

# Link the final kernel image
$(out_dir)/Image: $(src_dir)/linker.ld $(objs)
	$(mkdirp)
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(src_dir)/linker.ld -o $@ $(libs) $(objs)

#
$(out_dir)/%.d: ;
# Mark the dependency files as precious, so they won't get deleted when
# make is interrupted (we are using intermediary files (.Td))
.PRECIOUS: $(out_dir)/%.d

# Include .d files only if needed
# This is rather complicated as make does not have proper a
# proper logical or function
no-deps :=
# Include if no targets given
ifneq ($(MAKECMDGOALS),)
# Don't include if only no-dep targets are given
ifeq ($(filter-out $(no-dep-targets),$(MAKECMDGOALS)),)
no-deps := 1
endif
endif
# Only include if no-deps is not set
ifeq ($(no-deps),)
# For every .o file include a .d file
-include $(objs:.o,.d)
endif

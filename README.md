
# Rubidium

Operating system. "Rubidium" is just its code name.
I (hopefully) will come up with a better name in the future (name suggestions are welcome).

## Building

This requires a cross-compiled `gcc` (plus `libgcc`) for `i386-elf`.

```bash
make
# If you want an ISO image with GRUB as the bootloader:
# this generates `Image.iso` in the output directory
make iso
```

## Running

Currently, the OS can only be run with qemu. You need `qemu-system-i386`.

```bash
make run
```

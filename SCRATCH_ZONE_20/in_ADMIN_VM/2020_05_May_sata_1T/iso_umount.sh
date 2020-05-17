#!/bin/bash

# boot_zilla # VM += BOOT_iso as DVD=VM.dvd

# ISO=clonezilla-live-2.6.6-15-i686.iso	# unused # you qemu on 32 bit ?
  ZIP=clonezilla-live-2.6.6-15-amd64.zip # unused

  ISO=clonezilla-live-2.6.6-15-amd64.iso # AMD64 running QEMU over ISO ROM

  OPTS="$OPTS -fda fat:floppy:DIR1"
  OPTS="$OPTS -fda fat:floppy:DIR1"

  sudo umount iso
  # ask_do sudo umount $ISO


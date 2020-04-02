#!/bin/bash

image_iso=image.iso
image_iso=~/G//tmp/THE_LIVES_OF_OTHERS.iso
image_iso=ubuntu-19.10-desktop-amd64.iso
image_iso=image.iso
image_iso="${1:-$image_iso}"

# MODULE # ${ image_iso } # bash FAILS with "bad substitution" error
# MODULE # ${image_iso} # bash SYNTAX NO_SP in dollar_braces

# the rest you can hardcode edit

dev_dvd=/dev/dvd
dev_dvd=/dev/sr0

OPT_DRY_RUN=-dry-run
OPT_DRY_RUN=""


OPT_SPEED="-speed=1" # double speed on D620?
OPT_SPEED="" # quad speed on D620?

du -h ${image_iso}

ask_do sudo \
  growisofs -dvd-compat ${OPT_DRY_RUN} ${OPT_SPEED} -Z ${dev_dvd}=${image_iso}

ask_do eject

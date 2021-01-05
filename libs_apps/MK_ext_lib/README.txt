MK_ext_lib/README.txt

	# TODO LGX=LGX of { LGX WIN32 ARM HANDSET } 
	# TODO E6320=$(shell uname -n)

	LGX=LGX of { LGX WIN32 ARM HANDSET } 
	box_name=$(shell uname -n)

        # seems ~ does not work in Makefiles # $(HOME)
        DIR_libs_apps=$(HOME)/YEAR/src/libs_apps
        DIR_libs_apps_build=$(DIR_libs_apps)/build_$(LGX)_$(box_name)

        INCLS += -I$(DIR_libs_apps)/include

	# vector to specific version # VFS lib => $(EXPR ARGV) # EXPR
        LINKS += -Wl,--rpath=$(DIR_libs_apps_build)/lib

	# everything is output in the . directory .o .so .exec
	# so runtime looking here gets latest

        LINKS += -Wl,--rpath=$(DIR_libs_apps_build)/
        LINKS += -L$(DIR_libs_apps_build)
        LINKS += -lbase


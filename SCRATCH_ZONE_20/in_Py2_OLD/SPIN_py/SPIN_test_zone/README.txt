This is tests

	You definitely need SPIN_py to compile before coming here
	You probably need /usr/local/lib/libsp16-1.3.so INSTALLED
	or whatever sucessfully linked. ld.so 

	You do NOT need spin_py.so nor the base/*.py files installed
	into the standard Python DIRS, which is why this ./tests/. dir
	exists, but you WILL need the files mentioned in
	etc_sgml/some_dir_catalog2 to exist, (see also spin_eh_base.py)

	LOTS of errors usually means ERROR-DTD-NOT-FOUND.

README.txt

etc_sgml/

	fakes an installed catalog (for me anyway)
	without actually distributing OpenSP/HTML4.soc

lib_base/

	Files that you MIGHT install into standard python dirs

lib_plus/

	Files that you would consider standard application libs

test_number2/

	build an HTML document from (1) pieces, and number the
	headings (only a little care is needed to setup in the first
	place, and then maintain the HTML file).

	That should then re-number, after you've edited the generated.

test_gen_2_LGPL/

	generate the FOSS download directory index
	it lists lots of .tgz files with a desc
	http://www.information-cascade.co.uk/LGPL/index1.html

/src/SPIN_py/SPIN_build_16_so_

#ifndef sha_md_H
#define sha_md_H

#include "obj_ref.h" // not obscured
#include "evp_md_calculator.h" // init_sha512();
// #include "sha.h" // not obscured


class sha_md_CALC : public md_calculator_base {
 public:
	virtual ~sha_md_CALC();
	sha_md_CALC() {}
};

class sha_512_md_CALC : public sha_md_CALC {
 public:
	~sha_512_md_CALC() {
	}
	sha_512_md_CALC() {
	}

	bool virtual Init() {
		return init_sha512();
	//	int t = SHA512_Init(CTX);
		return WARN("RETVAL TODO");
	}

	bool virtual Update( const void *data, size_t len) {
	//	int t = SHA512_Update(CTX, data, len);
		return WARN("RETVAL TODO");
	}

	bool Final( unsigned char *md ) { // must be STR0 not binary
	//	int t = SHA512_Final(md, CTX );
		return WARN("RETVAL TODO");
	}


};

#if 0
	as file is copied to remote
	also compute SHA1 of entire file
	also compute SHA1 of each 1 GB
	then put those values into an SQLITE TREE

		FILE_DBID LOCN_DBID FILE_NAME 

		FILE_SHA1 // of entire file not 

		STAT_nbytes
		STAT_mtime
		STAT_

		LIST
		 FILE_DBID
		 PART_POSN
		 PART_SIZE
		 PART_SHA1	_as_BLOB_20_BYTES

#endif

#endif

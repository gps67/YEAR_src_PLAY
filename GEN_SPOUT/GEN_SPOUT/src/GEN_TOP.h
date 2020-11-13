#ifndef GEN_TOP_H
#define GEN_TOP_H

namespace SPOUT {

   // forward declarations
   struct GEN_DOC_ARGS_t;
   struct GEN_PAGE_t;
   struct GEN_PS_t; // forward // more code to move around
   struct GEN_TOP_t;

    // DOC_ARGS used in file_header
    // += fonts used
    //
    struct GEN_DOC_ARGS_t {

	 buffer2 DOC_Title; // easier than STR1
	 buffer2 DOC_Creator; // SPOUT
	 buffer2 DOC_Creator_version; // -1
	 buffer2 DOC_; // easier than STR1

	int pages;

    };

    // PAGE variables and FGBG and ...
    //
    struct GEN_PAGE_t {

	int page_no;

	// SURFACE ATTRS xy 

	// not all pages are same
	// but when they are default, they are ... page_wh
	XY_t page_wh;

	// GC PEN

	// _DRAW_ has _FGBG_
	// generally easier to set single global FGBG.GC.rgb (whatever)

	ink_rgb rgb;

	// COMMON USE path builder // vars

	bool in_page;

    }; 

#if 0
   // unused
   // this is the new SPOUT
   // the alternative to everything being in GEN_PS_t
   // but that is a good place for stuff
   //
   struct GEN_TOP_t {
 //
	GEN_DOC_ARGS_t & DOC_ARGS;
	GEN_PAGE_t & PAGE;
	GEN_PS_t & PS;
 //
	GEN_TOP_t(
		GEN_DOC_ARGS_t & DOC_ARGS_,
		GEN_PAGE_t & PAGE_,
		GEN_PS_t & PS_
	)
	: DOC_ARGS( DOC_ARGS_ )
	, PAGE( PAGE_ )
	, PS( PS_ )
	{
		// CTOR
	}
   }; // class
#endif

}; // namespace

#endif



#include "SPIN_tree_print.h"
// #include "Python.h"
#include "buffer/misc.h"
#include "buffer/blk1.h"
#include "buffer/buffer1.h"

#include "spin_py_mod.h"
#include "Python.h"
// #include "SPIN_tree_print.h"
#include "str_obj.h"

#include "elem_info/elem_info.h"
#include "elem_obj/elem_obj.h"
#include "sdata_obj/sdata_obj.h"

extern PyMethodDef SPIN_tree_print_methods[]; // forward
class SPIN_tree_print;


/*
	Silly attempt to move things around and add some inline hooks
*/

#include "P_utils.h"	// Python Utils
#include "utf8_iterator.h"

/*!

	SPIN_tree print
		its still all wrong - valid ok - fmt layout like w_text ?

	SPIN_tree print

		reads from utf8 tree

		writes to utf8 file

		indents nested xxML structure

		zero-indents <PRE>text</PRE>

		wraps data at APPROX wrap_pos

		does NOT wrap a multi-part-word (MAY start a new line?)

	part_word_mode:

		flush data to word start (+- boln)
		collect all subsequent WITHOUT wrap to any length line
	end:
		if fits into same line, flush send
		else send CRLF, indent, flush

	PROBS:
		generating a .php file requires cset-encoding-zone

	TODO:
		sdata, PI, comment, ...

	TODO:
		use app_link ... though SP is (probably) nolonger active

	TODO:
		part_word is still vapour

*/
struct SPIN_tree_print : PY_Object_base
{
	refhold downstream;	/* buffer sends it on */
	buffer1 buff;           /* buffer collects it */
	int lo;			/* reset to this initial size */
	int hi;			/* force flush when growing beyond this */
	int xpos;		/* current column */
	int depth;		/* indentation level */
	bool boln;		/* beginning of line */
	bool wrap;		/* is_pre_mode */
	int wrap_pos;		/* column to wrap at */
	bool ok;		/* error occured, and reported */

	bool part_word_mode;	/* filling buffer without poss CRLF wrap */
	int part_word_mode_left; /* xpos already flushed if not boln */

	bool mid_gap;		/* putc_prev == SP or boln or ... */
	bool keep_gap;		/* putc_prev == SP or boln or ... */
	bool queued_space;	/* putc_prev == SP or boln or ... */
	bool queued_eoln;	/* putc_prev == LF or boln or ... */

	P_util P;	// retvals placed here rather than each function

	/*!
		Create a new tree print using downstream
	*/
	SPIN_tree_print(
		PyObject *downstream_,
		int lo_,
		int hi_
	)
	: PY_Object_base( &SPIN_tree_print_Type )
	, buff( lo_ ) 
	, lo( lo_ )
	, hi( hi_ )
	, xpos( 0 )
	, depth( -1 )
	, boln( true )
	, wrap( true )
	, wrap_pos( 74 )
	, part_word_mode( false )
	, part_word_mode_left( 0 )
	, mid_gap( false )
	, keep_gap( false )
	, queued_space( false )
	, ok( true )
	{
		if( !downstream_ ) {
			PY_Static::TypeError("new_SPIN_tree_printer: downstream is NULL");
			// throw // return NULL;
		}
		downstream.claim_this( downstream_ );
	}

	/*!
		Destructor - auto flush
	*/
	~SPIN_tree_print()
	{
		print_CRLF();
		flush2();
	}

	/*!
		A PyErr has been set, remember and return false
	*/
	bool fail_reported()
	{
		ok = false;
		return false;
	}

	/*!
		Print a CR and an LF, or just a LF (edit code to pick which)
	*/
	void print_CRLF()
	{
		// buff.put_byte('\r');
		buff.put_byte('\n');
		mid_gap = true;
		xpos = 0;
	}

	/*!
		next output appears on own line, but dont add blank lines
	*/
	void own_line()
	{
		if(!queued_eoln) queue_eoln();
	}

	/*!
		If it is the boln, add depth spaces
	*/
	void boln_indent()
	{
		if( !boln ) return;
		boln = false;
		for( int i=0; i<depth; i++ )
		{
			buff.put_byte(' ');
			xpos ++;
		}
		mid_gap = true;
		queued_space = false;
	}

	/*!
		add (or absorb?) spaces
	*/
	void element_might_be_gap(
		SPIN_elem_info * elem_info,
		int pos
	)
	{
		if( !wrap ) return;
		/*
			pos is one of these:
			1<TAG ...>2 ..... 3</TAG>4
			1<TAG/>4
		*/
		if( elem_info -> element_breaks_word )
		{
		// element IS a gap, allow a space (at any / all of 1 2 3 4)
		 wrap_point();
		 switch(pos)
		 {
		  case 1:
			/*
				if is boln here, maybe fit all on one line
				if it wont fit, WANT matching boln at [3]

				CALLER would need to look: are there
				multiple child nodes (not text flow)
				or does text flow exceed 1 line
				IF SO: tag on own line
				IF NOT: <T1><T2> text on 1 line text </T2></T1>

				OPTIMISER: splice CRLF_Indent markers,
				then overwrite with actual gap settings,
				and process (expand or skip) on output
			*/
			// element IS a gap, add a space before <
			queue_space_wrappable();
		  break;
		  case 2:
			// migrate spaces out left, pretend SPACE seen
			mid_gap = true;
		  break;
		  case 3:
			// migrate space out right, eat space
			queue_cancel_gap();
		  break;
		  case 4:
			// force space after </TAG>
			queue_space_wrappable();
		  break;
		 }
		} else {
		 // element is NOT a gap, cant add any fake spaces, unless already one
		 switch(pos)
		 {
		  case 1:
			if( mid_gap ) keep_gap = true; // but keep queued SP
		  break;
		  case 2:
			if( keep_gap ){
				mid_gap = true; // but no extra spaces
			}
			keep_gap = false; // done
		  break;
		  case 3:
			if( mid_gap ) {
				queue_cancel_gap();
				keep_gap = true;
			}
		  break;
		  case 4:
			if( keep_gap ) {
				keep_gap = true;
				queue_space_wrappable();
			}
		  break;
		 }
		}

		if( mid_gap ) wrap_point();
mid_gap = true;
	}

	/*!
		put_byte(' ') - but not yet (dropped if EOLN)

		The point of mid_gap is that it can persist (somehow)
		when the space is flushed or dropped. EG element_breaks_word
		and space migration.
	*/
	void queue_space_wrappable()
	{
		if( mid_gap ) return;
		mid_gap = true;
		queued_space = true;
		if( wrap ) wrap_point();
	}

	/*!
		put_byte('\n') - maybe with CRLF
	*/
	void queue_eoln()
	{
		if(!wrap) pre_non_space_fn();
		queued_eoln = true;
		queued_space = false;
		mid_gap = true;
		boln = true;
	}

	/*!
		eat queued spaces
	*/
	void queue_cancel_gap()
	{
		mid_gap = false;
		queued_eoln = false;
		queued_space = false;
	}

	/*!
		a non space is about to be printed, are there
		any queued spaced to send first?

		leave flags as if in a non-space
	*/
	void pre_non_space()
	{
		if( queued_eoln || queued_space || mid_gap ) pre_non_space_fn();
	}

	/*!
		if( queued_space ) put_byte(' ') - NOW
	*/
	void pre_non_space_fn()
	{
		mid_gap = false;
		boln = false;
		if( queued_eoln )
		{
			queued_eoln = false;
			queued_space = false;
			// newline with indent
			print_CRLF();
			mid_gap = false; // this confused by print_CRLF
			for( int i=0; i<depth; i++ )
			{
				buff.put_byte(' ');
				xpos ++;
			}
			return;
		}
		if( queued_space )
		{
			queued_space = false;
			buff.put_byte(' ');
			xpos ++;
		}
	}

	/*!
		The buffer should not grow excessively, but should
		only be flushed at convenient points.

		Call this function when you think its a good idea
		(retain trailing space/s ??) Typically at the
		end of every close tag.
	*/
	bool possible_flush_point()
	{
		// maybe catch a any sub-error and continue ?
		if(( buff.nbytes_used + 100 ) >= hi )  {
			return flush();
		} else {
			// debug - call it anyway
			// return flush();
		}
		return true;
	}

	/*!
		Send all buffered output downstream (but dont tell it
		to flush - if its also buffered).

		Downstream receives (PY) write( pystr ), which is presumed
		to not do any CRLF or UTF convertions.
	*/
	bool flush()
	{
		if( !buff.nbytes_used ) return true; // OK

		// create the String for the next (Python) device
		PyObject * string;
		string = PyString_FromStringAndSize(
		(STR0)	buff.buff,
			buff.nbytes_used);
		if(!string) {
			return false;
		}
		// clear the buffer but dont reclaim space 
		buff.nbytes_used = 0; //leave putc_prev
		// buff.resize( self->lo );

		return P.call_method( downstream, "write", string );
		// throw return value away
	}

	/*!
		flush() and tell downstream to flush too.
	*/
	bool flush2()
	{
		if(!flush()) return false;
		return call_downstream_flush2(); // always suceeds!
	}

	/*!
		Call flush2() on downstream.

		If that fails (PROBABLY) because of no such method,
		try calling flush() on downstream. If that fails (PROBABLY)
		because of no such method, OK return true.


		This allows downstream to be any Python Stream, or
		user-defines object that recognises write(str);flush2();flush();
		but doesnt complain if its a simpler thing.

		This buffer has already been flushed.
	*/
	bool call_downstream_flush2()
	{
		/*
			downstream.flush2() cascades further
			OK if downstream doesnt do it
		*/
		if( P.call_method( downstream, "flush2" ) )
			return true;
		PyErr_Clear();

		/*
			downstream.flush() cascades a bit further
			OK is fownstream doesnt do it
		*/
		
		if( P.call_method( downstream, "flush" ) )
			return true;
		PyErr_Clear();

		/*
			presumably downstream doesnt need (or provide) flush
		*/
		return true;
	}

	/*!
		Put str into buff without any convertion, but do clear gap;
	*/
	bool put_plain( const char * str )
	{
		pre_non_space();
		int len = strlen( str );
		return buff.put_nn_bytes( len, (char *) str );
	}

	/*!
		Call wrap_point every time you see a space and ignore the
		return bool.

		This is a bit primitive, it simply leaves 10, but the
		improved version should record the position, and go back
		to it when the line is broken. That would split a line
		AFTER a word has been written that crosses the line.
		Plus handle very long (URL) words. (Thats what own_line
		does).


		TODO:
	*/
	bool wrap_point()
	{
		// should record xpos of previous space
		// should check at each word edge
		if(!wrap) return false;
		if( (xpos+10) < wrap_pos ) return false;
		own_line();
		return true;
	}


	/*!
		Convert a single char into MINIMALISTIC sdata_char &lt;

		More advanced code, (eg nbsp) should do their own convertions

		MAYBE: ' -- &apos;
		MAYBE: " -- &quot;
		at least in attribute values?
	*/
	void putc_into_ml( char c )
	{
		if( unsigned(c) <= ' ' ) 
		{
		 if( !wrap )
		 {
			// SP CR LF TAB and all others ...
			buff.put_byte( c );
		 } else
		 {
			queue_space_wrappable();
		 }
		 return;
		}
		pre_non_space();
		xpos ++; 	// ignore UTF being more than 1, also amp
		switch( c )
		{
		 case '<':
			put_plain( "&lt;");
		 break;
		 case '>':
			put_plain( "&gt;");
		 break;
		 case '&':
			put_plain( "&amp;");
		 break;
		 default:
			buff.put_byte( c );
		}
	}

	/*!
		Write str/len into the buffer, converting from 8859
		to the buffers (UTF8) char-set-encoding, adding &lt;
		convertion for the BASE set (amp, lt, gt).

		TODO: This doesnt do any such thing, but you can call
		it now, and when it does, it will.
	*/
	bool text_8859( const char * str, int len )
	{
		return text_utf8( str, len );
	}

	/*!
		Write str/len into the buffer, converting from UTF8
		into UTF8. Each char is &amp; encoded into ML.

		If the chars are more than the basic AMP, LT, GT, you should
		write the sdata_char separatately (split left mid right)
	*/
	bool text_utf8( const char * str, int len )
	{
		bool ok = buff.get_space( len );
		for( int i=0; i<len; i++ )
		{
			putc_into_ml( str[i] );
		}
		return ok;
	}

	/*!
		Write an 8859 pystr into the buffer, converting
		it to UFT (if necessary) and adding ML encoding for LT;
	*/
	bool text_utf8( PyObject * py_str )
	{
		char * str;
		Py_ssize_t len;
		if(!PyString_Check( py_str ))
		{
			return fail_reported();
		}
		if( PyString_AsStringAndSize( py_str, & str, & len ) )
		{
		}
		text_utf8( str, len );
		return true;
	}

	/*!
		Write a prepared string without ML or cset convertion
		(but with leading indent).

		This is how you write &NBSP; whe you need to
	*/
	bool text_ML( const char * str )
	{
		int len = strlen( str );
		return text_ML( str, len );
	}

	/*!
		Write a prepared py_str, without any convertion
	*/
	bool text_ML( PyObject * py_str )
	{
		char * str;
		Py_ssize_t len;
		if(!PyString_Check( py_str ))
		{
			return fail_reported();
		}
		if( PyString_AsStringAndSize( py_str, & str, & len ) )
		{
		}
		return text_ML( str, len );
	}

	//! -
	/*!
		-
	*/
	// write out text without any convertion, < & >
	bool text_ML( const char * str, int len )
	{
		pre_non_space();	// might be space, but dont look
		return buff.put_nn_bytes( len, (char *)str );
	}

	//! -
	/*!
		-
	*/
	// write out text without any convertion, < & >
	bool char_ML( const char chr )
	{
		pre_non_space();	// might be space, but dont look
		return buff.put_byte( chr );
	}

	//! -
	/*!
		-
	*/
	PyObject * print_tree(
		PyObject *arg1
	)
	{
		bool old_wrap = wrap;
		depth++;
		PyObject * retval = print_tree_1( arg1 );
		bool wrap = old_wrap;
		depth--;
		if( ok && retval )
			return retval;
		else {
			flush();
			ok = false; // should have already been set false
			return NULL;
		}
	}

	//! -
	/*!
		-
	*/
	PyObject * print_tree_1(
		PyObject *arg1
	); // not inline

	//! -
	/*!
		-
	*/
	bool print_atts(
		SPIN_elem_info * elem_info,
		PyObject * atts_
	)
	{
		P_List attrs( atts_ ); // converts Py_None to NULL
		int na = attrs.N();
		for( int a=0; a<na; a++ )
		{
			/*
				here is how the ATTR is decoded
			*/
			P_Tuple attr( attrs.get( a ) );
			if( attr.N() != 2 )
			{
				return PY_Static::TypeError( "ATTR should be tuple( name, val)" );
			}
	
			PyObject * attr_name = attr.get( 0 );
			PyObject * attr_value = attr.get( 1 );
	
			wrap_point();
			text_ML( " " );
			text_ML( attr_name );
			text_ML( "=\"" );
			text_ML( attr_value ); // no wrap_point() within
			text_ML( "\"" );
		}
		return true;
	}

	//! -
	/*!
		-
	*/
	bool print_start_element(
		SPIN_elem_info * elem_info,
		PyObject * atts_
	)
	{
		if(0) printf("print_start_element(%s)\n", PyString_AsString( elem_info->name) );
		element_might_be_gap( elem_info, 1 );
		text_ML( "<" );
		text_ML( elem_info->name );
		print_atts( elem_info, atts_ );
		text_ML( ">" );
		element_might_be_gap( elem_info, 2 );
		return true;
	}

	//! -
	/*!
		-
	*/
	bool print_end_element(
		SPIN_elem_info * elem_info
	)
	{
		element_might_be_gap( elem_info, 3 );
		text_ML( "</" );
		text_ML( elem_info->name );
		text_ML( ">" );
		element_might_be_gap( elem_info, 4 );
		return true;
	}

	//! -
	/*!
		-
	*/
	bool print_empty_element(
		SPIN_elem_info * elem_info,
		PyObject * atts_
	)
	{
		element_might_be_gap( elem_info, 1 );
		text_ML( "<" );
		text_ML( elem_info->name );
		print_atts( elem_info, atts_ );
		if(0) // OFF 2008-11-23 Sun Nov 02:51 should cope by now
		{
			if( 0 )
				text_ML( "><!-- EMPTY -->" );
			else
				text_ML( ">" );
		} else {
			text_ML( "/>" );
		}
		element_might_be_gap( elem_info, 4 );
		return true;
	}

	//! -
	/*!
		-
	*/
	PyObject * new_None()
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

	//! -
	/*!
		-
	*/
	bool print_list(
		PyObject * data_
	)
	{
		if( !data_ ) return true;
		P_List data( data_ );
		if( data.is_null() )
		{
			// allow NULL list here ??
			return true;
		}
		if( data.is_null_error() ) return fail_reported();

		int nd = data.N();
		for( int d=0; d<nd; d++ )
		{
			if( !print_tree( data.get( d ) ))
			{
				break;
			}
		}
		return ok;
	}

	//! -
	/*!
		-
	*/
	PyObject * print_elem_trip(
		SPIN_elem_info * elem_info,
		PyObject * atts_,
		PyObject * data_
	)
	{
		bool is_probably_block = false;
		if( boln )
			is_probably_block = true;
		// is_probably_block = false;

		if( elem_info->IS_empty() )
		{
			own_line();
			print_empty_element( elem_info, atts_ );
			own_line();
			return new_None();
		}

		if( elem_info->IS_pre() )
		{
			int old_depth = depth;
			bool old_wrap = wrap;
			depth = 0;
			wrap = false;
			own_line();
			if(!print_start_element( elem_info, atts_ ))return NULL;
			own_line();
			if( !print_list( data_ )) return NULL;
			own_line();
			print_end_element( elem_info );
			depth = old_depth;
			wrap = old_wrap;
			own_line();
			return new_None();
		}

		if( ( elem_info->element_breaks_word ) || is_probably_block )
		{
			bool extra1 = false;
			bool extra2 = false;
			if( elem_info->element_breaks_word ) 
				extra1 = true;
			own_line();
			print_start_element( elem_info, atts_ );
			if(extra1) own_line(); else if( extra2 ) text_ML(" ");
			// depth++;
			print_list( data_ );
			// depth--;
			if( extra1 ) own_line(); else if( extra2 ) text_ML(" ");
			print_end_element( elem_info );
			own_line();
			return new_None();
		}

		/*
			inline
		*/
		print_start_element( elem_info, atts_ );
		if( !print_list( data_ ) ) return NULL;
		print_end_element( elem_info );
		// own_line();

		return new_None();
	}

};


#if 0
/* --------------------------------------------------------------------- */
//		utf8_into_buff
/* --------------------------------------------------------------------- */

//! -
/*!
	-
*/
class utf8_into_buff : utf8_iterator_base
{
	SPIN_tree_print * print;
 public:
	utf8_into_buff( SPIN_tree_print * _print )
	{
		print = _print;
	}
	virtual void got_char( int wc )
	{
		if( wc > 0xFF )	
		{
			fprintf(stderr,"SPIN_tree_print received wide char 0x%x\n", wc );
			return;
		}
		if( wc > ' ' )
			print->buff.put_byte( (char) wc );
		else
			print->buff.put_byte( (char) wc ); // gap flag
	}
};
#endif

/* --------------------------------------------------------------------- */
//		C functions that Py calls
/* --------------------------------------------------------------------- */


/*
	SPIN_tree_print is a buffer to reduce the number of syscalls.

 Python usage:
	buff = Buffer( downstream, [lo], [hi] ) # create buffer
	buff.write( str ) # add str to buffer
	buff.flush()	  # calls downstream,write() and free() if..
	buff.flush2()	# buff.flush() downstream.flush2()

	Also need a derived class (type) which keeps line info
	such as indentation, line_is_empty, even wrapping text
	see (TBA) spin_line_buffer
*/

extern SPIN_tree_print *
as_SPIN_tree_print( PyObject * buff )
{
	/*
		Check that buff is really a write_buffer, else error
	*/
	if (buff && IS_SPIN_tree_print( buff )) {
		return (SPIN_tree_print *) buff;
	} else {
		PY_Static::TypeError("SPIN_tree_print expected");
		return (SPIN_tree_print *) NULL;
	}
}


//! -
/*!
	-
*/
static void
SPIN_tree_print_dealloc( SPIN_tree_print *self )
{
	self->flush2();
	delete self;
	fflush(0);
}

//! -
/*!
	-
*/
static PyObject *
SPIN_tree_print_getattr(
	SPIN_tree_print *self,
	char *name_ )
{
	str0 name( name_);
	if( name == "downstream" )
	{
		return self->downstream.new_ref() ; 
	}
	if( name == "lo" )
	{
		return PyInt_FromLong( self->lo );
	}
	if( name == "hi" )
	{
		return PyInt_FromLong( self->hi );
	}
	return self->get_attr_tail( SPIN_tree_print_methods, name );
}

static int
SPIN_tree_print_setattr(
	SPIN_tree_print *self,
	char *name,
	PyObject *v )
{
	if(0==strcmp( name, "downstream" ))
	{
		if( !v ) {
			PY_Static::AttributeError(
			        "cant delete SPIN_tree_print downstream attribute");
			return -1;
		}
		self->downstream.claim_this( v );
		return 0;
	}
	if(0==strcmp( name, "hi" ))
	{
		int val = PyInt_AsLong( v ); // no error detected
		self->hi = val;
		return 0;
	}
	if(0==strcmp( name, "lo" ))
	{
		int val = PyInt_AsLong( v ); // no error detected
		self->lo = val;
		return 0;
	}
	return self->set_attr_tail( name, v );
}


/* --------------------------------------------------------------------- */


//! -
/*!
	-
*/
/*!
	write simple passes data through (in correct sequence).

	The supplied string is a data block, no text convertion is done,
	no wrap is done, no counting is done! boln remains true!!

	It is used by PI_Scripted() to write php strings through,
	with LEGAL "<" inside, untouched by any functions here.
*/
static PyObject *
SPIN_tree_print_write(
	SPIN_tree_print * self,
	PyObject *args )
{
	char * str; // binary block though!
	int len;
	if (!PyArg_ParseTuple(args, (char *)"s#:SPIN_tree_print_write",
		&str, &len ))
	{
		// 	"Python should gen own error");
		return NULL;
	}
	/* 
		// buffer can still exceed hi, esp single request, but OK
	*/
	self->buff.put_nn_bytes( len, str );
	if(! self->possible_flush_point() ) return NULL;
	Py_INCREF(Py_None);
	return Py_None;
}

//! -
/*!
	-
*/
/*
	This is a bit vague
	converting utf8 to 8859
	or simply counting xpos char widths ??
*/
static PyObject *
SPIN_tree_print_text_utf8(
	SPIN_tree_print * self,
	PyObject *args )
{
	char * str; // binary block though!
	int len;
	if (!PyArg_ParseTuple(args, (char *)"s#:SPIN_tree_print_text_utf8",
		&str, &len ))
	{
		// 	"Python should gen own error");
		return NULL;
	}
	/* 
		// buffer can still exceed hi, esp single request, but OK
	*/
	self->text_utf8( str, len );
	Py_INCREF(Py_None);
	return Py_None;
}



//! -
/*!
	-
*/
// #include "elem_info.h"

static
PyObject *
SPIN_tree_print_print_tree(
	SPIN_tree_print * self,
	PyObject *args
)
{
	PyObject *arg1;
	if( !PyArg_ParseTuple( args, (char *)"O:SPIN_tree_print_print_tree", &arg1 ))
	{
		return PY_Static::TypeError( "WHY THIS? SPIN_tree_print_print_tree" );
		return NULL;
	}

	if(! self->print_tree( arg1 )) return NULL;
	return self->new_None();
}

//! -
/*!
	-
*/
PyObject *
SPIN_tree_print::
print_tree_1(
	PyObject * arg1
)
{
	// Plain string (is it a word list ?)
	if( PyString_Check( arg1 ) )
	{
		if(! text_utf8( arg1 ) ) return NULL;
	}

	// elem_obj
	else if( SPIN_elem_obj_Check( arg1 ) )
	{
		SPIN_elem_obj * elem = (SPIN_elem_obj *) arg1;
		if(! print_elem_trip(
			elem->eleminfo,
			elem->attslist,
			elem->datalist
		) ) return NULL;
	}

	// sdata
	else if( SPIN_sdata_Check( arg1 ) )
	{
		pre_non_space();
		SPIN_sdata * sdata = (SPIN_sdata *) arg1;
		if(! char_ML( '&' ) ) return NULL;
		if(! text_ML( sdata->name.get_str() ) ) return NULL;
		if(! char_ML( ';' ) ) return NULL;
	}

	// elem_as_tuple
	else if( PyTuple_Check( arg1 ) )
	{
		int n = PyTuple_GET_SIZE( arg1 );
		// info, atts, data
		if( n!=3 )
		{
			return PY_Static::TypeError( "tupule(info,atts,data) expected");
		}
		PyObject * ele_ = PyTuple_GET_ITEM( arg1, 0 );
		PyObject * atts = PyTuple_GET_ITEM( arg1, 1 );
		PyObject * data = PyTuple_GET_ITEM( arg1, 2 );
		SPIN_elem_info * elem_info = as_SPIN_elem_info( ele_ );                   
		if(! elem_info )
		{
			return PY_Static::TypeError("print_tree - elem_info ISNT");
		}
		if(! print_elem_trip( elem_info, atts, data ) ) return NULL;
	}

	// scripted instance - it has to handle the call itself
	else if( PyInstance_Check( arg1 ) )
	{
		return  PyObject_CallMethod(
			arg1,
			(char *) "tree_print",
			(char *) "(O)",
			(PyObject *) this
		);
	}

	else
	{
		PY_Static::Expect_elem_obj( arg1 );
		return NULL;
	}

	possible_flush_point(); // ignore error
	Py_INCREF(Py_None);
	return Py_None;
}

//! -
/*!
	-
*/
static PyObject *
SPIN_tree_print_flush(
	SPIN_tree_print * self,
	PyObject *args )
{
	if (!PyArg_ParseTuple(args, (char *)":SPIN_tree_print_flush"))
		return NULL;
	if( !self->flush() )
		return NULL;
	Py_INCREF(Py_None);
	return Py_None;
}

/*!
	flush2() does a flush() AND THEN a downstream flush[2]
*/
static PyObject *
SPIN_tree_print_flush2(
	SPIN_tree_print * self,
	PyObject *args )
{
	if (!PyArg_ParseTuple(args, (char *)":SPIN_tree_print_flush2"))
		return NULL;

	if(!self->flush2() ) return NULL;

	Py_INCREF(Py_None);
	return Py_None;
}

//! -
/*!
	-
*/
/* EXPORTED */
// SPIN_tree_print *
PyObject *
new_SPIN_tree_print(
        PyObject *, // the module
        PyObject * args )
{
        PyObject *downstream = 0;
	int lo, hi;
        if (!PyArg_ParseTuple(args, (char *)"Oii:SPIN_tree_print",
		&downstream,
		&lo,
		&hi
	))
                return NULL;
	return (PyObject *) new SPIN_tree_print( downstream, lo, hi );
}

//! -
/*!
	-
*/
PyTypeObject SPIN_tree_print_Type = {

	 PyObject_HEAD_INIT(&PyType_Type)
		0,				/*ob_size*/
	 (char *) "SPIN_tree_print",		/*tp_name*/
		sizeof(	SPIN_tree_print ),	/*tp_basicsize*/
		0,				/*tp_itemsize*/
	 /* methods */
	 (destructor)SPIN_tree_print_dealloc, /*tp_dealloc*/
	 (printfunc)	0,	/*tp_print*/
	 (getattrfunc)SPIN_tree_print_getattr, /*tp_getattr*/
	 (setattrfunc)SPIN_tree_print_setattr, /*tp_setattr*/
		0,				/*tp_compare*/
		0,				/*tp_repr*/
		0,				/*tp_as_number*/
		0,				/*tp_as_sequence*/
		0,				/*tp_as_mapping*/
		0,				/*tp_hash*/
};

//! -
/*!
	-
*/
// static
PyMethodDef SPIN_tree_print_methods[] = {
	{(char *)"write",	(PyCFunction)SPIN_tree_print_write,	1},
	{(char *)"text_utf8",	(PyCFunction)SPIN_tree_print_text_utf8,	1},
//	{(char *)"text_8859",	(PyCFunction)SPIN_tree_print_text_8859,	1},
//	{(char *)"text_ML",	(PyCFunction)SPIN_tree_print_text_ML,	1},
	{(char *)"flush",	(PyCFunction)SPIN_tree_print_flush,	1},
	{(char *)"flush2",	(PyCFunction)SPIN_tree_print_flush2,	1},
	{(char *)"print_tree",	(PyCFunction)SPIN_tree_print_print_tree, 1},
	{NULL,		NULL}		/* sentinel */
};

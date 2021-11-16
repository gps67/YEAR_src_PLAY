
#ifndef SPIN_APP_H
#define SPIN_APP_H 1

#ifdef __GNUG__
#pragma interface
#endif

#include "SGMLApplication.h"

#include "app_types.h"
#include "app_link.h"
#include "text_collector_utf.h"
#include "elem_info.h"
#include "elem_obj.h"
#include "str_obj.h"
#include "stack1.h"	// it controls us!
#include "P_util.h"

class SP_App : public SGMLApplication {
public:

	SP_App( SPIN_top * _top );
	~SP_App();

	void startDtd(const StartDtdEvent &); // ie start prolog
	void endDtd(const EndDtdEvent &); 
	void endProlog(const EndPrologEvent &);
	void startElement(const StartElementEvent &);
	void endElement(const EndElementEvent &);

	void pi(const PiEvent &);
	void commentDecl(const CommentDeclEvent &);
	void markedSectionStart(const MarkedSectionStartEvent &);
	void markedSectionEnd(const MarkedSectionEndEvent &);

	void subdocEntityRef(const SubdocEntityRefEvent &);
	void generalEntity(const GeneralEntityEvent &);
	void externalDataEntityRef(const ExternalDataEntityRefEvent &);

	void openEntityChange(const OpenEntityPtr &);
	void error(const ErrorEvent &);

	void data(const DataEvent &);
	void sdata(const SdataEvent &);
	void nonSgmlChar(const NonSgmlCharEvent &);
	void ignoredChars(const IgnoredCharsEvent &);

	SPIN_elem_info * find_elem_info(const StartElementEvent &);

	void appinfo(const AppinfoEvent &);


	Position pos_recent;
	OpenEntityPtr OEP;
	int OEP_change; // forces Location
	Location L;

	app_link app;

	int data_trim;
	stack1 stack;
	friend struct stack1; // allow stack access to buff.wrap or FN?
	text_collector_utf buff;

	int vret(AppObject ret) {
		return app.vret(ret);
	}

	void raise_error(const char * s ) {
		app.raise_error(s);
	}

	void set_SPIN_opt( const char * opt, AppObject val );

	void set_wrap( bool on ) {
		// printf("SP_App.set_wrap(%d)", on );
		// sp is always usable
		buff.wrap = on;
		stack.item[stack.sp].wrap = on;
	}

	void send_location(void); // to py set_location
	void keep_pos( Position pos );
	void keep_pos2( Position pos ); // send_location if OEP_change

	void data_not_fn(void);
	void data_not(void) {
		if(buff.buff.nbytes_used) data_not_fn();
	}
	void space_not(void) {
		buff.putc_prev = '!'; // anything but space
	}

	AppString mk_string( const CharString & s ) {
		return app.mk_string(s);
	}
	AppString mk_str( const char * s ) {
		return app.mk_str(s);
	}
	AppAttr mk_cdata_attr( const Attribute & a );
	AppAttr mk_tokenised_attr( const Attribute & a );
	AppEntity mk_entity(const Entity & ent);
	AppExternalId mk_externalId(const ExternalId & eid );
	AppNotation mk_notation(const Notation & note );
	AppLocation mk_location( void );

};

#endif /* SPIN_APP_H */

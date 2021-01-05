
#include "w_btn_plus.h"

class sar_btn_base : public w_btn_plus
{
 public:
	sar_top_app * top;

	sar_btn_base( sar_top_app * _top, const char * name )
	: w_btn_plus( name )
	, top( _top )
	{
	}
	virtual void event_clicked()
	{
		e_print("sar_btn_base clicked\n");
	}
};

class sar_set_time_span : public sar_btn_base
{
 public:
	int secs;
	sar_set_time_span( sar_top_app * _top, const char * name, int _secs )
	: sar_btn_base( _top, name )
	, secs( _secs )
	{
	}
	void event_clicked()
	{
		top->layout_selected_time_span_secs( secs );
	}
};


class sar_adj_time_pos : public sar_btn_base
{
 public:
	int secs;
	sar_adj_time_pos( sar_top_app * _top, const char * name, int _nsecs )
	: sar_btn_base( _top, name )
	, secs( _nsecs )
	{
	}
	void event_clicked()
	{
		top->layout_selected_time_adj( secs );
	}
};


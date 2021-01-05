#ifndef tv_radio_data_H
#define tv_radio_data_H

namespace TV_REM {

typedef enum 
{
	M01,
	M02,
	M03,
	M04,
	M05,
	M06,
	M07,
	M08,
	M09,
	M10,
	M11,
	M12,
	M13,
	M14,
	M15,
	M16,
	M17,
	M18,
	M19,
	M20,
	M21,
	M99_count
} M_menu_id ;

struct ir_menu // : public GRP_lib_media
{
	M_menu_id M_group;
	const char * name;
};
extern ir_menu ir_menu_table[];
extern int N_ir_menu_table;

struct ir_cable_channel // : public GRP_lib_media
{
	M_menu_id M_group;
	int channel3;
	gdouble vol_factor;
	const char * name;
};
extern ir_cable_channel ir_cable_channel_table[];
extern int N_ir_cable_channel_table;

struct ir_command_info // : public GRP_lib_media
{
	M_menu_id M_group;	// M10
	const char * name;	// 101 BBC One
	const char * remote;	// Cable || VCR || Radio || 
	const char * command;	// "1 0 1"
};

extern ir_command_info ir_command_table[];
extern int N_ir_command_table;

// one off button
extern ir_command_info vcr_play_pause;



// no MENU TAG

struct tv_chan_info  // : public GRP_lib_media
{
	const char * name;
	const char * cmd;
	gdouble vol_factor;
	int channel;
	int fine_tune;
};

struct radio_channel_info  // : public GRP_lib_media
{
	const char * name;
	float freq;
	gdouble vol_factor;
};

extern	radio_channel_info radio_channel_table[];
extern int N_radio_channel_table;

extern tv_chan_info tv_chan_table[];
extern int N_tv_chan_table;

};
#endif

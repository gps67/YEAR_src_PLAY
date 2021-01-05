README_graph.txt

	TODO:	linear scale only 
	WANT:	logarithmic or stretched axis

struct flt80_2048

	an array [n] of flt80

struct flt80_2048_pair : public dset_api

	flt80_2048 count
	flt80_2048 val

	this is a dset_api destination that receives and holds the data,
	but you have to initialise [t0[t2 [n]

class w_graph2 : public w_graph1

	flt80_2048_pair data1;

	w_graph2::plot_line( dset_src, linespec, t0, t2 )

		dset_src->select_segs_to_dset( data1, linespec, t0, t2 )

	data1_plot_green();
	data1_plot_black();

class w_graph3 : public w_graph2

	this doesnt do much more.
	It adds crosshairs and the UPLINK

class axis_draw_base : public GRP_sar_plot
class axis_draw_y : public axis_draw_base
class axis_draw_xtime : public axis_draw_base
class axis_scale_linear : public GRP_sar_plot
class stat_calc : public GRP_sar_plot

class w_segs_map : public w_drawing_area

class dset_api;
class mem_line_spec;
class sar_top_app;

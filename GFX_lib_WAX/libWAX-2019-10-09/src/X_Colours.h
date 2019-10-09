#ifndef X_Colors_H
#define X_Colors_H

namespace WAX {

/*!
	Each display has a specific depth and colourmap, a colour
	can ONLY be used on a compattible display/drawable.
*/
struct X_Colours
{
	Display * display;
	Colormap colormap;

	/*!
	*/
	X_Colours( Display * disp )
	: display( disp )
	{
		colormap = 0;
		if( disp )
		{
			late_init( display );
		} else {
			colormap = 0;
		}
	}

	/*!
	*/
	void late_init( Display * disp )
	{
		colormap = 0;
		display = disp;
		if( display ) colormap = DefaultColormap( display, 0);
	}

	/*!
	*/
	XColor Parse_Alloc( const char * spec )
	{
		XColor colour;
		XParseColor( display, colormap, spec, &colour );
		XAllocColor( display, colormap, &colour );
		return colour;
	}
};

}; // NAMESPACE
#endif

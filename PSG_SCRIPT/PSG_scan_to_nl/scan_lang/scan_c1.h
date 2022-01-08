#include "file_scan_base.h"

class scan_c1 : public file_scan_base
{
 public:

	scan_c1()
	: file_scan_base()
	{
	}

	~scan_c1()
	{
	}

	bool scan_file( const u8 * filename );

	int x_start;
	int y_start;

	void get_x_y_start();

};

#ifndef io_csv_accts_H
#define io_csv_accts_H

#include "io_csv_in.h"

namespace ACCTS {

class data_set;

/*!
	io_csv_in reads CSV lines and io_csv_accts::got_line_1 sends
	them to the dset
*/
class io_csv_accts : public io_csv_in
{
 public:
	data_set * dset;

	io_csv_accts( data_set * _dset );
	~io_csv_accts();

 virtual
	bool got_line_1();
};
}; // NAMESPACE

#endif

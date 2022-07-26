#ifndef TJ_blk_H
#define TJ_blk_H

struct blk1;

namespace TJ {
  struct TJ_blk_t {
	unsigned char *buff;
	unsigned long size;
	bool borrowed;

	TJ_blk_t();

	~TJ_blk_t();

	void release();

	bool tj_alloc( long _size );

	bool move_to_blk( TJ_blk_t & blk );
	void borrow_from_blk1( blk1 & blk );
	bool move_write_to_blk( blk1 & blk );
	bool read_entire_file( const char * filename,  long K_max );
	bool write_as_file( const char * filename );

  }; // struct
}; // namespace
#endif


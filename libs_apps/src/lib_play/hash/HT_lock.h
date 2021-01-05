#ifndef HT_lock_H
#define HT_lock_H


/*!
	I only use a single thread ANYWAY, so this is really
	debug code, to ensure that a callback (on a field)
	doesnt rentrant on this object.

	That code would blowup anyway (lock waiting for its caller
	to return)

	Could put this in obj_ref as part of 32 bit flags
*/

struct HT_lockable : public GRP_lib_play
{
	int flag;

	HT_lockable()
	{
		flag = 0;
	}
	void lock_start()
	{
		if( flag )
		{
			e_print("CODE ERROR - lock is %d locked\n", flag );
			throw "CODE ERROR - lock is locked";
		}
		flag++;
	}
	void lock_end()
	{
		flag--;
		if( flag )
		{
			e_print("CODE ERROR - lock is %d (un)locked\n", flag );
			throw "CODE ERROR - lock is non-zer";
		}
	}
	void lock_check_1()
	{
		if( flag != 1 )
			throw "CODE ERROR lock_check_1";
	}

};

struct HT_lock_user : public GRP_lib_play
{
	HT_lockable * lock;
	HT_lock_user( HT_lockable & lock_ )
	{
		lock = & lock_;
		lock -> lock_start();
	}
	~HT_lock_user()
	{
		lock -> lock_end();
	}

};

#endif



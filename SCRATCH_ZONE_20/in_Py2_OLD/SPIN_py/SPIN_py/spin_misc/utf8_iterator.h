#ifndef utf8_iterator_H
#define utf8_iterator_H

class utf8_iterator_base
{

	static const int T1 = 0x80;	// 1000 0000 // top bit mask/val
	static const int T2 = 0xC0;	// 1100 0000
	static const int T3 = 0xE0;	// 1110 0000
	static const int T4 = 0xF0;	// 1111 0000
	static const int T5 = 0xF8;	// 1111 1000
	static const int NOT_T2 = 0xFF & ~T2; 
	static const int NOT_T3 = 0xFF & ~T3;
	static const int NOT_T4 = 0xFF & ~T4;
	static const int NOT_T5 = 0xFF & ~T5;

 public:

	virtual void got_char( int wc ) = 0;

	// allow caller to jump over malformed bytes
	// but never over a NUL (unless caller has strlen)
	const unsigned char * bad_ptr_0;	// start of sequence
	const unsigned char * bad_ptr_1;	// problem byte
	const unsigned char * bad_ptr_2;	// possible restart place
	int bad_nbyte;	// Possible expected nbyte

	utf8_iterator_base() {}
	

	bool run( const char * utf8_nul )
	{
		const unsigned char * P = (const unsigned char *) utf8_nul;

		int nbyte = 0;
		while( 1 )
		{
			P += nbyte;
			int wc = 0;
			unsigned char c1 = P[0];

			if(!c1)			// 0000 0000
				return  true;

			if( c1 < 0x80 )		 // 0xxx xxxx
			{
				wc = c1;
				nbyte = 1;

			} else if( T2 ==  (c1 & T3) ) // 110x xxxx
			{
				wc = c1 & NOT_T3;
				nbyte = 2;

			} else if( T3 == (c1 & T4) ) // 1110 xxxx
			{
				wc = c1 & NOT_T4;
				nbyte = 3;

			} else if( T4 == (c1 & T5) ) // 1111 0xxx
			{
				wc = c1 & NOT_T5;
				nbyte = 4;

			} else {

				bad_ptr_0 = &P[0]; // start
				bad_ptr_1 = &P[0]; // problem
				bad_ptr_2 = &P[1]; // next
				bad_nbyte = nbyte;
				return false;
			}

			for( int b=1; b<nbyte; b++ )
			{
				unsigned char c2 = P[b];
				if( T1 == (c2 & T2 )) // 10xx xxxx
				{
					c2 = c2 & NOT_T2;
					wc <<= 6;
					wc |= c2;
				} else {
					bad_ptr_0 = &P[0];
					bad_ptr_1 = &P[b];
					bad_ptr_2 = &P[nbyte];
					bad_nbyte = nbyte;
					// prevent malformed jumping nul
					for( int b3=b; b3<nbyte; b3++ )
					{
						unsigned char c3 = P[b3];
						if(!c3) bad_ptr_2 = &P[b3];
					}
					return false;
				}
			}

			got_char( wc );
		}
	}
};

#endif

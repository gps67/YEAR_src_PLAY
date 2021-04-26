#ifndef is_diff_H
#define is_diff_H

/*!
	IS_DIFF_t is the basis for comparing two values.

	Alternative is where LESS/MORE has no meaning
*/
typedef enum {
	IS_SAME = 0,
	IS_MORE = 1,
	IS_LESS = -1
} IS_DIFF_t;

#endif

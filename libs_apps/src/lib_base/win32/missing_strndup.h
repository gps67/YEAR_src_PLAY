
#ifdef WIN32
// always - test it anyways
#endif

#define strndup(str,n) missing_strndup(str,n)
extern "C" char * missing_strndup(const char *s, size_t n);


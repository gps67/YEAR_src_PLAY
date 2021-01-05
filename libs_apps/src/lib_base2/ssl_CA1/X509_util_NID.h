#ifndef X509_util_NID_H
#define X509_util_NID_H

namespace CA1 {
/*!
	X509 NID's are enum values for things
*/
struct X509_util_NID
{
 public:
	static bool longName_to_NID( const char * longName, int & nid );
	static bool shortName_to_NID( const char * shortName, int & nid );
	static const char * shortName_of_NID( int nid );
	static const char * longName_of_NID( int nid );

	static bool test_one();
};

}; // namespace
#endif

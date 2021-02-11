#ifndef XML_DTD_H
#define XML_DTD_H

#include "XML_proto.h"
#include "obj_ref.h"
#include "HT_table.h"
#include "buffer2.h"

/*!
	the DTD and the data loaded by the parser and the document instance
*/
namespace XML_DTD 
{

 using namespace XML_DOC; // T_sdata_char T_entity -- T_text_frag

 /*!
	doxygen grouping
 */
 struct GRP_XML_DTD
 {
 };

 /*!
	doxygen grouping
 */
 struct GRP_XML_DTD_obj : public obj_ref
 {
 };

 /*!
	a pool of namespaces
 */
 struct T_NS_POOL : GRP_XML_DTD_obj
 {
	obj_hold<T_NS>		anon_name_space;
	HT_table<str2,T_NS>	ht_namespace;

	bool init_global_pool();

	T_NS * lookup_add_namespace( const p0p2 & name );
 };

 /*!
	a namespace - named child of global or anon
 */
 struct T_NS : GRP_XML_DTD_obj
 {
	T_NS_POOL *	ns_pool;	// WEAK uplink
	obj_hold<str2>	prefix;		// the abbreviation that is used
	obj_hold<str2>	name;		// the URI that is used once
	obj_hold<T_DTD>	dtd;		// this is that is this

	T_NS * lookup_add_namespace( const p0p2 & name );
 };

 /*!
	a specific XML_DTD
 */
 struct T_DTD : public GRP_XML_DTD_obj 
 {
	T_NS *			NS;		// WEAK uplink
	obj_hold<str2>		name;		// unused - debug
	obj_hold<str2>		dtd_url;	// used in doctype
	obj_hold<str2>		dtd_file;	// used if url is NULL

	HT_table<str2,T_elem_spec>	ht_elem_spec;
	HT_table<str2,T_sdata_char>	ht_sdata_char;
	HT_table<str2,T_entity>		ht_entity;

	buffer1		tmp_name;

// REWORK
#if 1
	// LURK - befuddled - Ns is not DTD - and occasionally
	// elems float without dtd
	obj_hold<str2>			name_space;
	HT_table<str2,T_DTD>		ht_namespace;
#endif

	obj_hold<T_elem_spec>	preferred_root;	// eg HTML is preferred root 

	T_elem_spec * create_elem( p0p2 & name );
	T_NS * 	lookup_add_namespace( p0p2 & name_space );

	T_elem_spec * lookup_elem( p0p2 & name );
	T_elem_spec * lookup_elem( p0p2 & space, p0p2 & name );
 };

 /*!
	a part of a RULE

		#PCDATA
		elem
		frag *
		one_of { frag | frag | frag }
		seq_of { frag ... }

		named_entity_that_expands_simply
		named_entity_that_expands_and_needs_merging
 */
 struct T_dtd_frag : public GRP_XML_DTD_obj 
 {
 	virtual ~T_dtd_frag() {}
 };

 /*!
	each element has an elem_spec = NAME, is_pre, ...
 */
 struct T_elem_spec : public T_dtd_frag
 {
	T_DTD	*		dtd;	// weak link
	obj_hold<str2>		name;	// 
	bool			is_pre;
	bool			has_text;
	obj_list<T_attr_spec>	attr_list;
	obj_hold<T_dtd_frag>	rule;

	T_elem_spec( T_DTD * _dtd )
	: dtd( _dtd )
	, is_pre( false )
	, has_text( false )
	{
	}
 };

 /*!
 */
 struct T_attr_spec : public GRP_XML_DTD_obj
 {
	T_elem_spec *		elem_spec; // weak link
	obj_hold<str2>		name;	// 
	bool			is_scalar; // int float simple name ...
	// special attr parser ...
 };

 struct T_dtd_frag_pcdata : public T_dtd_frag
 {
	//
 };

 struct T_dtd_frag_star : public T_dtd_frag
 {
	obj_hold<T_dtd_frag>	item;
 };

 struct T_dtd_frag_plus : public T_dtd_frag
 {
	obj_hold<T_dtd_frag>	item;
 };

 struct T_dtd_frag_opt : public T_dtd_frag
 {
	obj_hold<T_dtd_frag>	item;
 };

 struct T_dtd_frag_parenth : public T_dtd_frag
 {
	obj_hold<T_dtd_frag>	item;
 };

 struct T_dtd_frag_seq : public T_dtd_frag
 {
	obj_list<T_dtd_frag>	list;
 };

 struct T_dtd_frag_one : public T_dtd_frag
 {
	obj_list<T_dtd_frag>	list;
 };

}; // XML_DTD
#endif


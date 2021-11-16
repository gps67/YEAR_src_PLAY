#! /usr/bin/env python
from dtd_element import *
import sys

out = printer( sys.stdout )


dtd = dtd_builder( out )

pend	= dtd.entity_macro( "pend", "</P>" )
titleend = dtd.entity_macro( "titleend", '</TITLE>' )
psplit	= dtd.entity_macro( "psplit", '</P><P>' )

# pend.used()
# titleend.used()
# psplit.used()

# CHARS
dtd.entity_sdata_char_numb( "amp", 38, "Ampersand" )
dtd.entity_sdata_char_numb( "lt", 60, "Less Than" )
dtd.entity_sdata_char_numb( "gt", 62, "Greater Than" )

# a list of TEXT or <INLINE> or similar

ptext_any = dtd_rule_any()
ptext_any.add( dtd.rule_pcdata )
e_ptext = dtd.entity_rule(
	"ptext",
	dtd_rule_bracketed_star( ptext_any ),
	"P can contain these things"
)

# a list of <P> or similar

plist_any = dtd_rule_any()
e_plist = dtd.entity_rule( "plist", dtd_rule_bracketed_star( plist_any ))

# INLINE things

ptext_any.add( dtd.element( "B", e_ptext ))
ptext_any.add( dtd.element( "I", e_ptext ))
ptext_any.add( dtd.element( "U", e_ptext ))
ptext_any.add( dtd.element( "A", e_ptext ))
ptext_any.add( dtd.element( "TT", e_ptext ))
ptext_any.add( dtd.element( "CODE", e_ptext ))
ptext_any.add( dtd.element( "AHREF", e_ptext ))

# LIST things

ptext_any.add( dtd.element( "UL" ))
ptext_any.add( dtd.element( "OL" ))

# things close to the <P> level

P	= dtd.element( "P", e_ptext )
LI	= dtd.element( "LI", e_ptext )

# P things that are generic

PRE	= dtd.element( "PRE" )
PRE.rule = e_ptext

# OTHER declarations

LI.rule = e_ptext
r_li_star = dtd_rule_star( LI )
# specifically semantic contents driven

OL	= dtd.element( "OL" )
UL	= dtd.element( "UL" )

OL.rule = r_li_star
UL.rule = r_li_star

# empty

HR	= dtd.element( "HR" )
HR.rule = dtd.rule_empty
ptext_any.add( HR )
plist_any.add( HR )

# OTHER 

LIST	= dtd.element( "LIST" )
LIST.rule = r_li_star

#

plist_any.add( P )
plist_any.add( PRE )
plist_any.add( OL )
plist_any.add( UL )
plist_any.add( LIST )

# TOP # pend = "pend"

pmap = dtd.shortref_map( "pmap" )
pmap.add( '"&#RS;B&#RE;"', pend )
pmap.add( '"&#RS;&#RE;"', pend )

P.usemap( pmap )
P.add_attr( Attr( "date", "cdata", '"2000-01-01"', "When last edited" ))
P.add_attr( Attr( "bgcolor", "cdata", "#IMPLIED", "#RRGGBB in hex" ))
P.add_attr( Attr_enum( "wrap", [ "left", "right", "center", "none" ], "none", "justification" ))

# e_plist.print2( out )
# dtd.print_out()
# P.used()
uses( e_plist )
dtd.print_tree()
# dtd.print_tree(e_plist)


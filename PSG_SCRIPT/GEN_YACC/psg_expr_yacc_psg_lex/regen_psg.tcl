namespace eval regen {

# could create own db of PUNCT names
# or even N++

PUNCT "=="	PUNCT_CEQ
PUNCT "!="	PUNCT_CNE
PUNCT "<"	PUNCT_CLT
PUNCT "<="	PUNCT_CLE
PUNCT ">"	PUNCT_CGT
PUNCT ">="	PUNCT_CGE

PUNCT "="	PUNCT_EQUAL
PUNCT "("	PUNCT_LPAREN
PUNCT ")"	PUNCT_RPAREN
PUNCT "{"       PUNCT_LBRACE
PUNCT "}"	PUNCT_RBRACE

PUNCT "."       PUNCT_DOTOK_
PUNCT ","	PUNCT_COMMA

PUNCT "+"	PUNCT_PLUS
PUNCT "-"	PUNCT_MINUS
PUNCT "*"	PUNCT_ASTERISK
PUNCT "/"	PUNCT_SLASH
PUNCT "^"	PUNCT_CARET

PUNCT "&"	PUNCT_AMPERSAND
PUNCT "%"	PUNCT_PERCENT
PUNCT "|"	PUNCT_PIPE

# RW_extern auto named
# any ASCII word found ?

RWS if while for do
RW extern
RW return

}

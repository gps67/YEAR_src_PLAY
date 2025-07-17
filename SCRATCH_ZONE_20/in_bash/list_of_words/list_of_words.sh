LIST=(
	echo
#	echo is the command
	one
#	one
	two three
	four
)

PICK_LIST=" 4 1 2 0 3 @"
PICK_LIST=" 4 1 2 0 3 "

echo "LIST is  ${LIST[@]} <-- USES SYNTAX ARRAY[IDX] from LIST LIST"
echo "PICK_LIST is '${PICK_LIST}'"
echo "PICK is  \$i FROM for i in L I S T ; do { SCRIPT } # but BASH adjust EOLN"

for i in $PICK_LIST # no Q2 # expect IDX = i "is a good idea"
do
	echo "pick word $i  \${LIST[\$i]} -  ${LIST[$i]}"
done

sync

# echo "pick word '@' ${LIST[@]}"

echo '${LIST[@]}' "# print all LIST words" // A Lo
echo "${LIST[@]}"

exit

SUB_LEX STEP WORLD ARGV but OBJV

	MINI_MACHINE has a SCROLL of TEXT
	TEXT_SCROLL += TEXT_STREAM // PROVICE local EA_EXPR and tables
	TEXT_SCROLL += CSET_UTF8_Mostly_Latin_L2R_EU # BYTE_EFFICIENT
	Internally a C program might use ONE_OF ASCII BYTE_CSET BYTE_Latin1 UTF8
	Internally words are split into CSET of SUBLEX
		so sites that are mostly Latin1 can use that FONT
		so sites that are mostly UTF8 with PLENTY of FONT use plain_utf8
		"plain utf8" is simply mechanically encoding bytes into i6[N_bytes]
		i6 i6 was a guess RTFM 
			how many bits each BYTE carries (when not 8)
		can use that FONT not builtin

			builtin covers ASCII
			builtin covers 8859_Latin1
			builtin covers 8859_Cyrillic
			builtin covers utf8_DEMO_one // DEMO_FONT("_one")
			builtin covers utf8_DEMO_two // DEMO_FONT("_two")

		$0 = echo || TYPE_NAME || EXPR
		$1 = "SELF" || VAR_NAME || EXPR || EA_EXPR

		builtin can guarantee
		builtin can be expected to provide
		builtin FONT_SET_for_Latin1 
		builtin FONT_SET_for_BYTE_FONT_ATARI
		builtin FONT_SET_for_BYTE_FONT_SVGA
		builtin FONT_SET_for_BYTE_FONT_SVG
		builtin FONT_SET_for_BYTE_FONT_PDF_or_PS
		builtin FONT_SET_for_BYTE_FONT_FT2_FreeType_SCALABLE
		builtin FONT_SET_for_BYTE_UTF8_MULTI_ZONE_FONT_FT2 // TT_to_PS
		builtin BUILD_FONT_SET_for_Latin1_PLUS ATARI_FONTS 

		Latin1 font immediately usable for ASCII_GLYPHS Latin1_POOL
		FT knows it can find its own fall-backs in plain legible ASCII
		FT knows every Latin1 glyph can be made except CTRL non_glyph
		FT allows to not know if HIGH_CTRL_is_GLYPH # DECIDE LATER


ALSO_TREE as well as ARGV
ALSO SCOPE with local variables and LIBR facilities FUNC LOCAL_MODULE
MODULE_NAME SCOPE_LOCAL {
	MODULE = Module_SCOPE
	INSTANCE = SCOPE_LOCAL // has happened and THIS is STO for THAT
	SCOPE_LOCAL_t SCOPE_LOCAL { INIT_SCRIPT }
	}

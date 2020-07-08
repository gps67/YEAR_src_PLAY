# -*- coding: iso-8859-15 -*-

def plausible_list():
 lst = (
	( '\xA0', ' ', ' ' ), # NO-BREAK SPACE
	( '\xA1', '¡', '!' ), # INVERTED EXCLAMATION MARK
	( '\xA2', '¢', '_' ), # CENT SIGN
	( '\xA3', '£', '_' ), # POUND SIGN
	( '\xA4', '¤', '_' ), # CURRENCY SIGN
	( '\xA5', '¥', '_' ), # YEN SIGN
	( '\xA6', '¦', '_' ), # BROKEN BAR
	( '\xA7', '§', '_' ), # SECTION SIGN
	( '\xA8', '¨', '_' ), # DIAERESIS
	( '\xA9', '©', '_' ), # COPYRIGHT SIGN
	( '\xAA', 'ª', '_' ), # FEMININE ORDINAL INDICATOR
	( '\xAB', '«', '_' ), # LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
	( '\xAC', '¬', '_' ), # NOT SIGN
	 
	( '\xAD', '­', '-' ), # SOFT HYPHEN
	( '\xAE', '®', '_' ), # REGISTERED SIGN
	( '\xAF', '¯', '_' ), # MACRON
	( '\xB0', '°', '_' ), # DEGREE SIGN
	( '\xB1', '±', '_' ), # PLUS-MINUS SIGN
	( '\xB2', '²', '_' ), # SUPERSCRIPT TWO
	( '\xB3', '³', '_' ), # SUPERSCRIPT THREE
	( '\xB4', '´', '_' ), # ACUTE ACCENT
	( '\xB5', 'µ', '_' ), # MICRO SIGN
	( '\xB6', '¶', '_' ), # PILCROW SIGN
	( '\xB7', '·', '_' ), # MIDDLE DOT
	( '\xB8', '¸', '_' ), # CEDILLA
	( '\xB9', '¹', '_' ), # SUPERSCRIPT ONE
	( '\xBA', 'º', '_' ), # MASCULINE ORDINAL INDICATOR
	( '\xBB', '»', '_' ), # RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
	( '\xBC', '¼', '_' ), # VULGAR FRACTION ONE QUARTER
	( '\xBD', '½', '_' ), # VULGAR FRACTION ONE HALF
	( '\xBE', '¾', '_' ), # VULGAR FRACTION THREE QUARTERS
	( '\xBF', '¿', '_' ), # INVERTED QUESTION MARK
	( '\xC0', 'À', 'A' ), # LATIN CAPITAL LETTER A WITH GRAVE
	( '\xC1', 'Á', 'A' ), # LATIN CAPITAL LETTER A WITH ACUTE
	( '\xC2', 'Â', 'A' ), # LATIN CAPITAL LETTER A WITH CIRCUMFLEX
	( '\xC3', 'Ã', 'A' ), # LATIN CAPITAL LETTER A WITH TILDE
	( '\xC4', 'Ä', 'A' ), # LATIN CAPITAL LETTER A WITH DIAERESIS
	( '\xC5', 'Å', 'A' ), # LATIN CAPITAL LETTER A WITH RING ABOVE
	( '\xC6', 'Æ', '_' ), # LATIN CAPITAL LETTER AE
	( '\xC7', 'Ç', 'C' ), # LATIN CAPITAL LETTER C WITH CEDILLA
	( '\xC8', 'È', 'E' ), # LATIN CAPITAL LETTER E WITH GRAVE
	( '\xC9', 'É', 'E' ), # LATIN CAPITAL LETTER E WITH ACUTE
	( '\xCA', 'Ê', 'E' ), # LATIN CAPITAL LETTER E WITH CIRCUMFLEX
	( '\xCB', 'Ë', 'E' ), # LATIN CAPITAL LETTER E WITH DIAERESIS
	( '\xCC', 'Ì', 'I' ), # LATIN CAPITAL LETTER I WITH GRAVE
	( '\xCD', 'Í', 'I' ), # LATIN CAPITAL LETTER I WITH ACUTE
	( '\xCE', 'Î', 'I' ), # LATIN CAPITAL LETTER I WITH CIRCUMFLEX
	( '\xCF', 'Ï', 'I' ), # LATIN CAPITAL LETTER I WITH DIAERESIS
	( '\xD0', 'Ð', '_' ), # LATIN CAPITAL LETTER ETH
	( '\xD1', 'Ñ', 'N' ), # LATIN CAPITAL LETTER N WITH TILDE
	( '\xD2', 'Ò', 'O' ), # LATIN CAPITAL LETTER O WITH GRAVE
	( '\xD3', 'Ó', 'O' ), # LATIN CAPITAL LETTER O WITH ACUTE
	( '\xD4', 'Ô', 'O' ), # LATIN CAPITAL LETTER O WITH CIRCUMFLEX
	( '\xD5', 'Õ', 'O' ), # LATIN CAPITAL LETTER O WITH TILDE
	( '\xD6', 'Ö', 'O' ), # LATIN CAPITAL LETTER O WITH DIAERESIS
	( '\xD7', '×', '_' ), # MULTIPLICATION SIGN
	( '\xD8', 'Ø', 'O' ), # LATIN CAPITAL LETTER O WITH STROKE
	( '\xD9', 'Ù', 'U' ), # LATIN CAPITAL LETTER U WITH GRAVE
	( '\xDA', 'Ú', 'U' ), # LATIN CAPITAL LETTER U WITH ACUTE
	( '\xDB', 'Û', 'U' ), # LATIN CAPITAL LETTER U WITH CIRCUMFLEX
	( '\xDC', 'Ü', 'U' ), # LATIN CAPITAL LETTER U WITH DIAERESIS
	( '\xDD', 'Ý', 'Y' ), # LATIN CAPITAL LETTER Y WITH ACUTE
	( '\xDE', 'Þ', '_' ), # LATIN CAPITAL LETTER THORN
	( '\xDF', 'ß', '_' ), # LATIN SMALL LETTER SHARP S
	( '\xE0', 'à', 'a' ), # LATIN SMALL LETTER A WITH GRAVE
	( '\xE1', 'á', 'a' ), # LATIN SMALL LETTER A WITH ACUTE
	( '\xE2', 'â', 'a' ), # LATIN SMALL LETTER A WITH CIRCUMFLEX
	( '\xE3', 'ã', 'a' ), # LATIN SMALL LETTER A WITH TILDE
	( '\xE4', 'ä', 'a' ), # LATIN SMALL LETTER A WITH DIAERESIS
	( '\xE5', 'å', 'a' ), # LATIN SMALL LETTER A WITH RING ABOVE
	( '\xE6', 'æ', '_' ), # LATIN SMALL LETTER AE
	( '\xE7', 'ç', 'c' ), # LATIN SMALL LETTER C WITH CEDILLA
	( '\xE8', 'è', 'e' ), # LATIN SMALL LETTER E WITH GRAVE
	( '\xE9', 'é', 'e' ), # LATIN SMALL LETTER E WITH ACUTE
	( '\xEA', 'ê', 'e' ), # LATIN SMALL LETTER E WITH CIRCUMFLEX
	( '\xEB', 'ë', 'e' ), # LATIN SMALL LETTER E WITH DIAERESIS
	( '\xEC', 'ì', 'i' ), # LATIN SMALL LETTER I WITH GRAVE
	( '\xED', 'í', 'i' ), # LATIN SMALL LETTER I WITH ACUTE
	 
	( '\xEE', 'î', 'i' ), # LATIN SMALL LETTER I WITH CIRCUMFLEX
	( '\xEF', 'ï', 'i' ), # LATIN SMALL LETTER I WITH DIAERESIS
	( '\xF0', 'ð', '_' ), # LATIN SMALL LETTER ETH
	( '\xF1', 'ñ', 'n' ), # LATIN SMALL LETTER N WITH TILDE
	( '\xF2', 'ò', 'o' ), # LATIN SMALL LETTER O WITH GRAVE
	( '\xF3', 'ó', 'o' ), # LATIN SMALL LETTER O WITH ACUTE
	( '\xF4', 'ô', 'o' ), # LATIN SMALL LETTER O WITH CIRCUMFLEX
	( '\xF5', 'õ', 'o' ), # LATIN SMALL LETTER O WITH TILDE
	( '\xF6', 'ö', 'o' ), # LATIN SMALL LETTER O WITH DIAERESIS
	( '\xF7', '÷', '_' ), # DIVISION SIGN
	( '\xF8', 'ø', 'o' ), # LATIN SMALL LETTER O WITH STROKE
	( '\xF9', 'ù', 'u' ), # LATIN SMALL LETTER U WITH GRAVE
	( '\xFA', 'ú', 'u' ), # LATIN SMALL LETTER U WITH ACUTE
	( '\xFB', 'û', 'u' ), # LATIN SMALL LETTER U WITH CIRCUMFLEX
	( '\xFC', 'ü', 'u' ), # LATIN SMALL LETTER U WITH DIAERESIS
	( '\xFD', 'ý', 'y' ), # LATIN SMALL LETTER Y WITH ACUTE
	( '\xFE', 'þ', '_' ), # LATIN SMALL LETTER THORN
	( '\xFF', 'ÿ', 'y' ), # LATIN SMALL LETTER Y WITH DIAERESIS
 )
 return lst


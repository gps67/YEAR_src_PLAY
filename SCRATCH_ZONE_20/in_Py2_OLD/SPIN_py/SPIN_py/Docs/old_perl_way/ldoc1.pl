#!/usr/bin/perl
use SGMLS;			# Use the SGMLS package.
use SGMLS::Output;		# Use stack-based output.
use SGMLS::Refs;		# manually deleted items!
use Carp;
use Text::Wrap;

# if no SECT1, the SECT appears blank!

 $Text::Wrap::columns = 72; # Default includes init tab

$| = 1 ;# immediate flush stdout

# maybe lookup all this from the database
$dirsite = "/home/ftp/" ;
$dirname = "Raven/EyeView" ;
$docname = shift ; # command line arg not from sgml
$docsubdir  = $docname ; # SSR01 from ssr01 ?

$docpath = "/tmp/".$docsubdir."/" ;
mkdir($docpath,0755) ;
$docdb =  new SGMLS::Refs( $docname.db1 );

	$BACKGROUND= '"../misc/glassTTY.gif"' ;
	$FC_BGCOLOR='"#000000"' ;
	$FC_TEXT  = '"#FEFE90"' ;
	$FC_VLINK = '"#FF0000"' ;
	$FC_LINK  = '"#60F060"' ;
	$FC_SECT0 = '"#60F060"' ;
	$FC_SECT1 = '"#60F060"' ;
	$FC_SECT2 = '"#60F060"' ;
	$FC_VERB  = '"#FFFFCF"' ;
	$FC_TT    = '"#FFFFCF"' ;

	$FC_TITLE = '"#FFFFCF"' ;
	$FC_AUTHOR= '"#60F060"' ;
	$FC_EMAIL = '"#FFFFCF"' ;

# $sect_nn = 00 ;# decimal to str %2.2d
# $sect_file = $docpath . '/' . $docname . '-' . $NN . '.html' ;
# $TOC0_file = $docpath . '/' . $docname               '.html' ;

# bug in trim_wrap: fails if last line not \n terminated
sub trim_wrap {
	my($str)= @_ ;
	$str =~ s/\s+/ /g ;
	$str =~ s/\s*(.*)/$1\n/ ;
	return wrap( "\t", "\t", $str ) ;
}

sub trim_trailing {
	my($str) = @_;
	$str =~ s/^\s*(.*\S)\s*$/$1/;
	$str =~ s/^\s*$//;
	return $str
}
sub trim_leading {
	my($str) = @_;
	$str =~ s/^\s*(.*)$/$1/;
	return $str
}
sub trim {
	my($str) = @_;
	$str =~ s/^\s*(.*\S)\s*$/$1/;
#	$str =~ s/^\s*(.*)\s*$/$1/;
	$str =~ s/^\s*$//;
	return $str
}

######## HTML FORMATTERS ########

sub CODE { my $str=shift; return "<CODE>$str</CODE>"; }
sub PRE { my $str=shift; return "<PRE>$str</PRE>"; }
sub AHREF { my ($url,$text) = @_; return "<A \nHREF=\"$url\">$text</A>"; }
sub P { my $str=shift; return "<P>$str</P>"; }
sub B { my $str=shift; return "<B>$str</B>"; }
sub H1 { my $str=shift; return "<H1>$str</H1>"; }
sub H2 { my $str=shift; return "<H2>$str</H2>"; }
sub H3 { my $str=shift; return "<H3>$str</H3>"; }
sub EM { my $str=shift; return "<EM>$str</EM>"; }
sub CENTER { my $str=shift; return "<CENTER>$str</CENTER>"; }
sub ANAME {
	my($name,$str)=@_;
	return "<A NAME=\"$name\">$str</A>";
}
sub FCOLOR {
	my($colr,$str)=@_;
	if( $colr eq "" ) {
		carp "null str to FCOLOR";
		return $str;
	}
	return "<FONT COLOR=$colr>$str</FONT>";
}

######## SCANNER ROUTINES ############

# macro2 gathers all content as a string (includes output of sub-zones)
# trims it and put it in a sandwich
# BUT if it was empty it deletes it all!
# also avoid in core diverts for things larger than a paragraph or page

sub macro1 {
	my($name,$template) = @_;
	my($lhs,$rhs) = split(/ZONE/,$template);
	macro2( $name, $lhs, $rhs );
	return ;
}

sub macro2 {
	my($name,$lhs,$rhs) = @_;
	sgml( "<".$name.">", sub { push_output 'string'; } );
	sgml( "</".$name.">", sub {
		my $zone = trim(pop_output()); 
		if( $zone =~ /\S/ ) {
			output $lhs, $zone, $rhs ; 
		} else {
			# output "<!-- BLANK-$name-REMOVED $zone -->\n" ;
		}
	});
}

sub grab_var {
	my($name,$varname) = @_;
	$$varname = "<!-- UNSET -->" ;
	sgml( "<".$name.">", sub { push_output 'string'; } );
	sgml( "</".$name.">", sub {
		my $zone = trim(pop_output()); 
		$$varname = $zone ;
	});
}

#
# SDATA Handlers
#
sgml('|&lt;|', "&lt;");
sgml('|&nbsp;|', "&nbsp;");
sgml('|&amp;|', "&amp;");
sgml('|&nbsp|', "&nbsp;");
sgml('|&thinsp;|', "&nbsp;");
sgml('|#|', "#"); # bug in SPOUT search Dial-up IP SSR01
sgml('|{|', "{");
sgml('|^|', "^");
sgml('|||', "|");
sgml('|&gt;|', "&gt;");
sgml('|}|', "}");
sgml('|%|', "%");
sgml('|_|', "_");
sgml('|[|', "[");
sgml('|~|', "~");
macro1( 'F',   "[ZONE") ; # BAD BAD BAD

#
# Document Handlers.
#
sgml('start', sub {});
sgml('end', sub {});

#
# Element Handlers.
#

# most things on the title page will need to be kept as references
# but for now ...


sgml('<LINUXDOC>', "");	sgml('</LINUXDOC>', sub {
	end_list_SECT0();
});
sgml('<ARTICLE>', "");	sgml('</ARTICLE>', sub {
	toc_in_main();
});

# SHOULD be A, B, C, ....
# needs second list from SECT ?
sgml('<APPENDIX>', "");	sgml('</APPENDIX>', sub {
});

sgml('<TITLEPAG>', sub {
	macro1( 'TT',   "<TT>ZONE</TT>") ; # cancel effect here# grab_var?
	$name_tt = "UNSET";
	grab_var( "TT", name_tt );
});
sgml('</TITLEPAG>', sub {
	TT_normal();
	output "title:	$title \n" ;
	output "date:	$date \n" ;
	output "name:	$name \n" ;
	output "name_tt:	$name_tt \n" ;
	output "abst:	$abstract \n" ;
	toc_in_main();
#	exit 0
});
# grab_var( "AUTHOR", "author" ); # plain container ?
grab_var( "TITLE", "title" );
grab_var( "DATE", "date" );
grab_var( "NAME", "name" );
grab_var( "ABSTRACT", "abstract" );
sgml('<AUTHOR>', "");	sgml('</AUTHOR>', "");
sgml('<TOC>', "");	sgml('</TOC>', "\n<!-- TOC GOES HERE -->\n");

# -- section counters --

$sect0_n = 0 ;
$sect1_n = 0 ;
$sect2_n = 0 ; # unused

# ABOVE # $db = new SGMLS::Refs( $docname );
sub put_locn_fact_value {
	my($locn,$fact,$value) = @_;
	$docdb->put( $locn.".".$fact, $value );
	return $value;
}

sub get_locn_fact_default {
	my($locn,$fact,$value) = @_;
	return $docdb->get( $locn.".".$fact ) || $value;
}

sub get_locn_fact {
	my($locn,$fact) = @_;
	return get_locn_fact_default( $locn, $fact, "--default--");
}

sub s_locn_sect0 { return sprintf( "SECT0[%d]", $sect0_n) ;}
sub s_locn_sect1 { return sprintf( "SECT0[%d]SECT1[%d]", $sect0_n,$sect1_n) ;}
sub s_locn_sect2 { return sprintf( "SECT0[%d]SECT1[%d]SECT2[%d]",
	$sect0_n,$sect1_n,$sect2_n) ;}
sub end_list_SECT0 { put_locn_fact_value( "", "NSECT0", $sect0_n ); }
sub end_list_SECT1 { put_locn_fact_value( $locn_sect0, "NSECT1", $sect1_n ); }
sub end_list_SECT2 { put_locn_fact_value( $locn_sect1, "NSECT2", $sect2_n ); }

sub TD { return sprintf("<td>%s</td>", shift ) ;}
sub TR { return sprintf("\n<tr>%s</tr>", shift ) ;}
sub TABLE_bd_bg {
	my($border, $bgcol, $zone) = @_;
	if($bgcol eq "") {
	return sprintf("\n<TABLE BORDER=%s>%s\n</TABLE>", 
		$border, $zone );
	} else {
	return sprintf("\n<TABLE BORDER=%s BGCOLOR=%s>%s\n</TABLE>", 
		$border, $bgcol, $zone ) ;
	}
}

$html = "htm";
sub toc_in_main {
	my $i; my $j; my $h0; my $h1; $my_locn0; my $locn1; my $l1;
	my $lbl; my $file;
	my $col1 = '"#60F060"';
	my $col2 = '"#606010"';
	my $col2 = '' ;
	my $col2 = $FC_TEXT ;
	my $col2 = '"#101010"';
	push_output( 'file', sprintf( "%s/%s.%s",$docpath,$docname,$html ) );
	html_head( $title );
#	my $title = get_locn_fact( "", "title" );
	output H1( FCOLOR( $FC_TITLE, $title ));
	output "\n\n";
	output H2( FCOLOR( $FC_AUTHOR, $name )."<BR>".CODE(FCOLOR( $FC_EMAIL, $name_tt)) );
	output "\n$date\n";
	output P( "<HR>".EM( $abstract )."<HR>" );
	output "\n\n";

# the one liners that point to the full paragraph indexes

	my $nh0 = get_locn_fact( "", "NSECT0" );
	output "<p><H2><ol>\n";
	for( $i = 1; $i<= $nh0; $i++ ) {
		$locn0 = sprintf( "SECT0[%d]", $i) ;
		$n0 = sprintf( "%d. ", $i );
		$h0 = get_locn_fact( $locn0, "heading" );
		$lbl = sprintf( "#toc%d", $i );
		my $line = "$n0$h0";
		my $line = "$h0";
#		my $line = H2("$line");
		output "<li>".AHREF( $lbl, $line )."\n";
	}
	output "</ol></H2><HR></p>\n";

# each of the indexes for each file

#	my $nh0 = get_locn_fact( "", "NSECT0" );
	for( $i = 1; $i<= $nh0; $i++ ) {
		output "<p>\n";
		$locn0 = sprintf( "SECT0[%d]", $i) ;
		$file = get_locn_fact( $locn0, "filename" );
		$n0 = sprintf( "%d. ", $i );
		$h0 = get_locn_fact( $locn0, "heading" );
		$lbl = sprintf( "toc%d", $i );
		$n0 = ANAME( $lbl, $n0 );
		$h0 = AHREF( $file, $h0 );
		output H2( $n0.$h0 );
		output "\n<UL>";
		my $nh1 = get_locn_fact( $locn0, "NSECT1" );
		for( $j = 1; $j <= $nh1; $j++ ) {
			$locn1 = sprintf( "SECT0[%d]SECT1[%d]", $i,$j ) ;
			$n1 = sprintf( "%d.%d", $i,$j ) ;
			$l1 = sprintf( "ss%d.%d", $i,$j ) ;
			$h1 = get_locn_fact( $locn1, "heading" );
			my $ref = AHREF( $file."#".$l1, $n1." ".$h1 );
			# my $ref = AHREF( $l1, $h1 );
			output "<LI>$ref\n";
		}
		output "</UL>\n\n\n"
	}
	pop_output();
}

sub toc_in_page {
	my $i; my $j; my $h0; my $h1; $my_locn0; my $locn1; my $l1;
	my $col1 = '"#60F060"';
	my $col2 = '"#606010"';
	my $col2 = '' ;
	my $col2 = $FC_TEXT ;
	my $col2 = '"#101010"';
	$i = $sect0_n;
	$locn0 = sprintf( "SECT0[%d]", $i) ;
	$h0 = sprintf( "%d. ", $i );
	$h0 .= get_locn_fact( $locn_sect0, "heading" );
	my $nh1 = get_locn_fact( $locn0, "NSECT1" );
	push_output 'string'; 
#	output TR( TD("+").TD( H2( FCOLOR( $col1, $h0 ))));
	for( $j = 1; $j <= $nh1; $j++ ) {
		$locn1 = sprintf( "SECT0[%d]SECT1[%d]", $i,$j ) ;
		$n1 = sprintf( "%d.%d", $i,$j ) ;
		$l1 = sprintf( "#ss%d.%d", $i,$j ) ;
		$h1 = get_locn_fact( $locn1, "heading" );
		my $ref = AHREF( $l1, $n1." ".$h1 );
		# my $ref = AHREF( $l1, $h1 );
		output TR( TD($ref) );
		# output TR( TD($n1).TD($ref) );
	}
	output TABLE_bd_bg( 1, $col2, pop_output() ); 
#	output "\n<HR>\n"
}

@locn_here_stack=();
$locn_here = "UnDEFINED";
sgml('<SECT>', sub {
	# incr top counter
	# get filename
	# start new file
	$sect0_n ++ ;
	$sect1_n = 0;
	$sect2_n = 0;
	$locn_sect0 = s_locn_sect0();
	$locn_sect1 = "UNDEFinED";
	$locn_sect2 = "UNDEFineD";
	push(@locn_here_stack, $locn_here );
	$locn_here = $locn_sect0;

	$file_prev = $file_here ;
	$file_here = sprintf("%s-%2.2d.%s",$docname,$sect0_n,$html) ;
	put_locn_fact_value( $locn_here, "filename", $file_here );
	$file_next = "" ;
	output "\n<!-- STARTING NEW FILE $file_here -->\n";
	push_output( 'file', $docpath.$file_here );
	html_head( $title );

if($dangerous) {
	push_output 'string' ; 	# DANGEROUS! BAD STRUCTURE! 
	# keep until put_toc_in_page;
};
});

sub html_head {
	my($title) = @_;
	output "<HTML> <!-- $file_here -->\n" ;
	output "<HEAD><TITLE>$title</TITLE>\n" ;
	output "<BODY	BACKGROUND=$BACKGROUND\n" ;
	output "	BGCOLOR=$FC_BGCOLOR\n" ;
	output "	TEXT=$FC_TEXT\n" ;
	output "	VLINK=$FC_VLINK\n" ;
	output "	LINK=$FC_LINK>\n" ;
}

$dangerous = 1;

sgml('</SECT>', sub {
	if( $sect1_n == 0 ) { # not yet started 
		if( $dangerous ) {
			output pop_output();
		}
	}
	end_list_SECT2();
	end_list_SECT1();
	output "\n<!-- END FILE WITH NEXT PREV -->\n" ;
	output "</HTML>" ;
	pop_output();
	$locn_here = pop(@locn_here_stack);
}); # maybe stats for end of file ?

sgml('<SECT1>', sub {

	if( $sect1_n == 0 ) { # not yet started 
		if( $dangerous ) {
			my $intro = TD( CENTER( pop_output()));
			push_output 'string'; 	
			toc_in_page();
			my $toc = TD( pop_output());
			my $BGCOL='"#000000"';
			my $BGCOL='' ;
			output TABLE_bd_bg( 0, $BGCOL, TR($intro.$toc));
			#	TR($toc.$intro));
		} else {
			toc_in_page();
		}
	}

	# incr counter
	# create label
	$sect1_n ++;
	$sect2_n = 0;
	push(@locn_here_stack, $locn_here );
	$locn_here = $locn_sect1 = s_locn_sect1();
});
sgml('</SECT1>', sub {
	end_list_SECT2();
	$locn_sect1 = "UnDeFiNed"; 
	$locn_here = pop(@locn_here_stack);
});

##)
sgml('<SECT2>', sub {
	$sect2_n ++;
	push(@locn_here_stack, $locn_here );
	$locn_here = s_locn_sect2();
});
sgml('</SECT2>', sub {
	$locn_sect2 = "UnDefiNed"; 
	$locn_here = pop(@locn_here_stack);
});

# -- HEADING does half the work of SECTn --

sgml('<HEADING>', sub { push_output 'string'; });
sgml('</HEADING>', sub {
	my $heading = pop_output(); # HTML &encoded;
	$heading = trim( $heading );
	put_locn_fact_value( $locn_here, "heading", $heading ); # no nums
	my $parent = shift->parent->name;
	my $label = 'mylabel';
	output "\n";
	output "\n";
	my $s1 = "";
	my $s2 = "";
	if( $parent eq 'SECT') {
		my $nn = sprintf("%d", $sect0_n);
		my $ss = "s".$nn;
		my $s1 = $nn.". ".$heading ;
		my $s1 = ANAME( $ss, $s1 );
		my $s1 = FCOLOR( $FC_SECT0, $s1 );
		my $s1 = H2( $s1 );
		output "$s1";
	} elsif ($parent eq 'SECT1') {
		my $nn = sprintf("%d.%d", $sect0_n, $sect1_n);
		my $ss = "ss".$nn;
		$s1 = "<DIV ALIGN=RIGHT> $nn</DIV><HR>";
		$s1 .= "<DIV ALIGN=RIGHT>$heading</DIV>";
		$s1 = ANAME("$ss",$s1);
		$s1 = FCOLOR($FC_SECT1,$s1);
		output H2(B($s1));
	} elsif ($parent eq 'SECT2') {
		$s1 = $heading;
		$s1 = FCOLOR($FC_SECT2,$s1);
		output H2(B($s1));
	} else { croak 'illegal instructions'; }
	output "\n";
});

sub FACE1 {
	my $zone = shift;
	return "<FONT face=\"ariel\" size=+3>$zone</FONT>";
}

sub FACE2 {
	my $zone = shift;
	return "<FONT face=\"courier\" size=+4>$zone</FONT>";
}

sub Face_size_colour_zone {
	my($face,$size,$color,$zone) = @_;
	$face = " face=\"$face\"" if $face ne "" ;
	$size = " size=\"$size\"" if $size ne "" ;
	$color = " color=\"$color\"" if $color ne "" ;
	return "<FONT$face$size$color>$zone</FONT>";
}
sub FACE3 {
	return Face_size_colour_zone('ariel','+3',$FC_VERB,shift);
}
sub BIG4 {
	return Face_size_colour_zone('','+4','',shift);
}

$ZONE="ZONE" ;
# use of TT in the titlepag/author/name # is an error
# restore later # macro1( 'TT',   FCOLOR( $FC_TT,  CODE( $ZONE )) );
macro1( 'VERB',		PRE( B( FCOLOR( $FC_VERB."size=+4", $ZONE ))) );
macro1( "P",   		"\n<P>$ZONE</P>" );
macro1( "P",   		"\n".FACE1(P($ZONE)) );
macro2( "ITEMIZE",	"\n<UL>",	"</UL>" );
macro2( "ENUM",		"\n<OL>",	"</OL>" );
macro2( "ITEM",		"<LI>",		"</LI>\n" );
macro2( "TAG",  	"<DT><B>",	"</B><DD>\n" );
macro2( "DESCRIP",	"<DL>",		"</DL>" );
macro2( "BF",		"<B>",		"</B>" );
macro2( "EM",		"<EM>",		"</EM>" );
macro2( "IT",		"<I>",		"</I>" );
sub TT_normal {
macro1( 'TT',   CODE( B( FCOLOR( $FC_TT."size=+4", $ZONE ))) ); # restore proper handler
};

sgml('</HTMLURL>', "");
sgml('<HTMLURL>', sub {
	my $elem = shift;
	my $url = $elem->attribute("URL")->value;
	my $text = $elem->attribute("NAME")->value;
	# NOW FIXUP any CDATA in text, esp if text == url
	output AHREF( $url, $text);
});

if(1) {
 sgml('start_element',sub { die "Unknown element: " . $_[0]->name; });
 sgml('end_element','');
 sgml('cdata',sub { output $_[0]; });
 sgml('sdata',sub { die "Unknown SDATA: " . $_[0]; });
 sgml('re',"\n");
 sgml('pi',sub { die "Unknown processing instruction: " . $_[0]; });
 sgml('entity',sub { die "Unknown external entity: " . $_[0]->name; });
 sgml('start_subdoc',sub { die "Unknown subdoc entity: " . $_[0]->name; });
 sgml('end_subdoc','');
 sgml('conforming','');
}
1;

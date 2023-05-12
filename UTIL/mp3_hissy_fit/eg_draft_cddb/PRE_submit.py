#!/usr/bin/env python3

import http.client

# POST /~cddb/submit.cgi HTTP/1.0
# Category: newage
# Discid: 4306eb06
# User-Email: joe@myhost.home.com
# Submit-Mode: submit
# Charset: ISO-8859-1
# X-Cddbd-Note: Sent by free CD player - Questions: support@freecdplayer.org.
# Content-Length: 960

def check_category( category ):
	ok_list = [
	 "blues",
	 "classical",
	 "country",
	 "data",
	 "folk",
	 "jazz",
	 "misc",
	 "newage",
	 "reggae",
	 "rock",
	 "soundtrack"
	]
	for ok in ok_list:
	 if category == ok:
	 	return True;
	raise ValueError( "category not found: " + category )

uri_cgi = "/~cddb/submit.cgi"
category = "rock"
discid = "4306eb06"
user_email = "lists@information-cascade.co.uk"
submit_mode = "submit"
submit_mode = "test"
charset = "ISO-8859-1"
x_cddbd_note = "Sent by hissy_fit --submit - see github gps67 YEAR_src_PLAY UTIL/mp3_hissy_fit " + user_email

# category = category + "_X" # trigger error
check_category( category )


host = "127.0.0.1"
port = "8080"


io = http.client.HTTPConnection( host, port )
# POST /~cddb/submit.cgi HTTP/1.0
io.putrequest( "POST", uri_cgi )
io.putheader( "Category", category )
io.putheader( "Discid", discid )
io.putheader( "User-Email", user_email )
io.putheader( "Submit-Mode", submit_mode )
io.putheader( "Charset", charset )
io.putheader( "X-Cddbd-Note", x_cddbd_note )
# Content-Length: 960

io.endheaders()

resp = io.getresponse() # can throw connection error
resp.read()

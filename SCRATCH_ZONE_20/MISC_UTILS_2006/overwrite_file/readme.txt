This program was used to overwrite a file 
which has previously contained a bad sector on the device.

The write is a multimple of 512 so that no read is involved,
and so that the device can do its own bad-block mapping.

That worked (I hope) and the IDE drive substituted a good block
for the bad block (where did that come from??)

NB If the LAST block is the bad one, there is a line
that writes to the sector boundry

-

Another use is to fill files with NUL's, prior to delete.
NB 

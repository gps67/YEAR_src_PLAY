PROG=adjtime
EXEC=$(PROG).exec

usr_bin_EXEC=/usr/bin/$(EXEC)

$(EXEC): $(PROG).c
	gcc -O2 -o $(EXEC) $(PROG).c
	strip $(EXEC)
	chown root $(EXEC)
	chmod 4755 $(EXEC)
	# DONT # cp -ip $(EXEC) $(usr_bin_EXEC)

clean:
	rm -f $(EXEC)

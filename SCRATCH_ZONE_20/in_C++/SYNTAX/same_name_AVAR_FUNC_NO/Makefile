NAME=same_name_AVAR_FUNC

test: $(NAME).exe
	./$<

$(NAME).exe: $(NAME).o
	g++ -O3 -o $@ $<
	size $@
	ldd $@

$(NAME).o: $(NAME).cxx
	g++ -O3 -c $(NAME).cxx

clean:
	rm -f $(NAME).exe $(NAME).o

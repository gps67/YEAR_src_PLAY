NAME=defined_name

test: $(NAME).exe
	./$<

$(NAME).exe: $(NAME).o
	gcc -o $@ $<

$(NAME).o: $(NAME).cxx
	gcc -c $(NAME).cxx

clean:
	rm -f $(NAME).exe $(NAME).o

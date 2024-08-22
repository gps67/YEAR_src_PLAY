
extern int write(int fd, char * MSG, int n );

int main( int argc, char ** argv )
{
	char MSG[] = "# HELLO from write\n";
	int n = sizeof(MSG);
	int fd = 1;
	write(fd, MSG, n );
	return 0;
}

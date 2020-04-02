
   int test (int n)
   {
     int i;
     int total = 0;

     for (i = 0; i < n; i++)
       total += i * i;

     return total;
   }

   int main( int argc, char ** argv )
   {
   	test( 4 );
   }

//          compiling to (x86_64) assembly via -S and emitting the result
//           direct to stdout via -o -

//                   gcc -S test.c -fverbose-asm -Os -o -



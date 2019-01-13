#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

void floatToInt( double *input, int16_t *output, int length )
{
  int i;
 
  for ( i = 0; i < length; i++ ) {
    if ( input[i] > 32767.0 ) {
      input[i] = 32767.0;
    } else if ( input[i] < -32768.0 ) {
      input[i] = -32768.0;
    }
    // convert
    output[i] = (int16_t)input[i];
  }
}

#define LENGTH 10000

double array[LENGTH];
int16_t output[LENGTH];

int main()
{
  FILE *out = fopen("input.pcm","wb");
  int i;

  int j;
  int offset=5000;
  int bias = 10000;

  for(i=0,j=0; i<LENGTH; i++)
    {
      if (i%8==0)
	{
	  offset=-offset;
	}
      
      array[i] = offset + rand()%200;
    }

  floatToInt(array,output,LENGTH);

  fwrite(output,sizeof(int16_t),LENGTH*sizeof(int16_t),out);

  fclose(out);
}

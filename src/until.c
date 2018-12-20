#include <stdlib.h>
#include <time.h>
#include <util.h>

void _xy(int x, int y, int *_x, int *_y)
{
	*_x = x;
	*_y = y;
}

long rrandom(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}

double frandom()
{
	return ( ((double)rrandom(100000)) / 100000.00);
}

int chance(float chance)
{
	if(chance==1.f) return 1;
	return frandom() < chance;
}

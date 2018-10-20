#include <stdio.h>

int round_half_away_from_zero(double x)
{
  double y;
  
  if (0 < x) {
    y = x + 0.5;
  } else if (x < 0) {
    y = x - 0.5;
  } else {
    y = x; /* x = 0 */
  }
  
  return (int) y; /* round towards zero; truncate */
}

int main(int argc, char* argv[])
{
  printf("round_half_away_from_zero( 1.0) = %+d\n", round_half_away_from_zero( 1.0));
  printf("round_half_away_from_zero( 0.9) = %+d\n", round_half_away_from_zero( 0.9));
  printf("round_half_away_from_zero( 0.5) = %+d\n", round_half_away_from_zero( 0.5));
  printf("round_half_away_from_zero( 0.1) = %+d\n", round_half_away_from_zero( 0.1));
  printf("round_half_away_from_zero( 0.0) = %+d\n", round_half_away_from_zero( 0.0));
  printf("round_half_away_from_zero(-0.1) = %+d\n", round_half_away_from_zero(-0.1));
  printf("round_half_away_from_zero(-0.5) = %+d\n", round_half_away_from_zero(-0.5));
  printf("round_half_away_from_zero(-0.9) = %+d\n", round_half_away_from_zero(-0.9));
  printf("round_half_away_from_zero(-1.0) = %+d\n", round_half_away_from_zero(-1.0));
  
  return 0;
}


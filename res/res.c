#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

char getch() {
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0) {
    perror("tcsetattr()");
  }
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0) {
    perror("tcsetattr ICANON");
  }
  if (read(0, &buf, 1) < 0) {
    perror("read()");
  }
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0) {
    perror("tcsetattr ~ICANON");
  }
  return (buf);
}

int gcf(unsigned int a, unsigned int b) {
  if (b == 0) {
    return a;
  }
  return gcf(b, a%b);
}

int main(int argc, char** argv) {
  // parse arguments
  if (argc < 2) {
    printf("%s", "no arguments\n");
    return -1;
  } else if (argc > 3) {
    printf("%s", "too many arguments\n");
    return -1;
  }
  int x = atoi(argv[1]);
  int y = atoi(argv[2]);

  // find greatest common factor
  int cf;
  if (x > y) {
    cf = gcf(x, y);
  } else {
    cf = gcf(y, x);
  }
  // calculate aspect ratio
  int rl = x/cf;
  int rr = y/cf;
  // 16:10 instead of 8:5
  if (rl == 8 && rr == 5) {
    rl = 16;
    rr = 10;
  }
  printf("ASPECT RATIO %d:%d\n", rl, rr);

  // calculate singular increment values
  double inc_x = 1;
  double inc_y = ((double)y/cf)/(x/cf);

  // cast dimensions to float so we can increment them
  double xf = (double)x;
  double yf = (double)y;

  while (1) {
    printf("                                 \r"); // TODO: make this less stupid
    printf("(%f, %f)", xf, yf);
    fflush(stdout);

    char ch = getch();
    if (ch == '\033') {
      getch(); // all arrow keys follow esc with '['
      ch = getch();
      switch (ch) {

      // up
      case 'A':
	xf += inc_x;
	yf += inc_y;
	break;

      // down
      case 'B':
	xf -= inc_x;
	yf -= inc_y;
	break;

      // left
      case 'D':
	break;

      // right
      case 'C':
	break;
      }
    }
  }
  return 0;
}

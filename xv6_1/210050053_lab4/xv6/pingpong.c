#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


#define BUFSIZE 512

char buf[BUFSIZE];
char tmp[BUFSIZE];
char pattern[5] = "ping";

int cmp(int sz){
  if(sz<4) return 0;
  for(int i=0; i<4; i++) {
    if(tmp[i]!=pattern[i]) return 0;
  }
  return 1;
}
void ping_pong(char *pattern, int fd) {
  int i, n;
  int t=0;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      if(buf[i]== ' '||buf[i]=='\n') {
        if(cmp(t)) {
          printf(1,"pong\n");
        }
        t=0;
      }
      else {
      tmp[t] = buf[i];
      t++;
      }
    }
  }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf(1, "Usage: %s <input_file>\n", argv[0]);
        exit();
    }
  
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf(1, "Error opening file %s\n", argv[1]);
        exit();
    }

    ping_pong(pattern ,fd);
    close(fd);

    exit();
}

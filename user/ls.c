#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;/*p will start at end of path, move backwards every loop*/
  p++;/*move forward 2 position, so file name is found(a/b/c \0 -> c is found)*/

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));/*copy p content to empty buf*/
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));/*buf is 15 char long, fill places not used by p with ' '*/
  buf[sizeof(buf)-1] = '\0';/*make last slot of buf \0 so it is a proper string*/
  return buf;/*so basically this fmtname is a file name parser, located file name, make it into a string and return*/
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    printf("%s %d %d %d\n", fmtname(path), st.type, st.ino, (int) st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);/*copy path to buf*/
    p = buf+strlen(buf);/*move p to end of buf(where there is not empty)*/
    *p++ = '/';/*put a / there*/
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);/*put de.name after buf(remember p is at the end with newly attached /)*/
      p[DIRSIZ] = 0;/*end of string, same as p[DIRSIZ] = '\0'*/
      if(stat(buf, &st) < 0){ /*find the file that buf describes, fill its stats in &st*/
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, (int) st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}

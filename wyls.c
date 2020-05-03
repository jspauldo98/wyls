/*
 * wyls.c
 * Author: Jared Spaulding
 * Date: Mar 3, 2020
 *
 * COSC 3750, Homework 5
 *
 * This is a simple version of the ls utility. It is designed to list
 * directory contents
 *
 */

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<pwd.h>
#include <grp.h>
#include<time.h>

/*
 * Lists contents of a pathname
 * @param path - path(s) whose contents to list
 * @param options - options to apply
 */
void listContents(char *path, char *options) {
  DIR *dp;
  struct dirent *ep;

  dp = opendir (path);
  if (dp != NULL) {
    while ((ep = readdir (dp))) {
      struct stat sb;

      char *type;

      stat(ep->d_name, &sb);
      struct passwd *pas = getpwuid(sb.st_uid);
      struct group *gr = getgrgid(sb.st_gid);

      if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0) {
        switch (sb.st_mode & S_IFMT) {
          case S_IFDIR:
            type = "d";
            break;
          case S_IFLNK:
            type = "l";
            break;
          default:
            type = "-";
        }

        char perm[10] = {'-','-','-','-','-','-','-','-','-','\0'};

        // Owner permissions
        if (sb.st_mode & S_IRUSR) 
          perm[0] = 'r';
        if (sb.st_mode & S_IWUSR)
          perm[1] = 'w';
        if (sb.st_mode & S_IXUSR)
          perm[2] = 'x';

        // Group permissions
        if (sb.st_mode & S_IRGRP) 
          perm[3] = 'r';
        if (sb.st_mode & S_IWGRP)
          perm[4] = 'w';
        if (sb.st_mode & S_IXGRP)
          perm[5] = 'x';

        // Others permissions
        if (sb.st_mode & S_IROTH) 
          perm[6] = 'r';
        if (sb.st_mode & S_IWOTH)
          perm[7] = 'w';
        if (sb.st_mode & S_IXOTH)
          perm[8] = 'x';


        // Caculate time stuff
        time_t rawtime;
        time(&rawtime);
        char timeBuf[64];
        struct tm *time = localtime(&sb.st_mtime);

        if ((rawtime - sb.st_mtime) < 15552000)
          strftime(timeBuf, sizeof timeBuf, "%b %d %H:%M", time);
        else
          strftime(timeBuf, sizeof timeBuf, "%b %d %Y", time);

        if (options != NULL) {

          // Calculate human view for h flag
          char letter;
          float size;
          if (sb.st_size < 1024) {
            letter = ' ';
            size = sb.st_size;
          } else if (sb.st_size < 8192 && sb.st_size > 1024) {
            letter = 'k';
            size = sb.st_size / 1024;
          } else if (sb.st_size < 1048576 && sb.st_size > 8192) {
            letter = 'M';
            size = sb.st_size / 1024 / 1024;
          } else {
            letter = 'G';
            size = sb.st_size / 1024 / 1024 / 1024;
          }

          if (strcmp(options, "n") == 0) {
            printf("%1s%9s %10d %10d %5lld %10s %-10s\n",
              type,
              perm,
              sb.st_uid,
              sb.st_gid,
              (long long) sb.st_size,
              timeBuf,
              ep->d_name);
          } else if (strcmp(options, "h") == 0) {
            printf("%1s%9s %10s %10s %5.1g%c %10s %-10s\n",
              type,
              perm,
              pas->pw_name,
              gr->gr_name,
              size, letter,
              timeBuf,
              ep->d_name);
          } else if (strcmp(options, "hn") == 0){
            printf("%1s%9s %10d %10d %5.1g%c %10s %-10s\n",
              type,
              perm,
              sb.st_uid,
              sb.st_gid,
              size, letter,
              timeBuf,
              ep->d_name);
          }
        } else {
          printf("%1s%9s %10s %10s %5lld %10s %-10s\n",
            type,
            perm,
            pas->pw_name,
            gr->gr_name,
            (long long) sb.st_size,
            timeBuf,
            ep->d_name);
        }
      }
    }
  closedir (dp);
  }
  else
    perror ("Couldn't open the directory");
}

char * getCurPath() {
  size_t size;
  char *buf;
  char *curPath;

  size = pathconf(".", _PC_PATH_MAX);

  if ((buf = (char *)malloc(size)) != NULL)
    curPath = getcwd(buf, size);

  return curPath;
}

int main(int argc, char **argv)
{

  // Check number of args
  if (argc < 2) {
    listContents(getCurPath(), NULL);
  } else {
    short hFlag = 0;
    short nFlag = 0;
    short option = 1;

    char *opt = NULL;

    int i;
    // Loop through args
    for (i=1;i<argc;i++) {
      if (argv[i][0] == '-') {
        if (i == 1)
          option = 0;
        if (option == 0){
          if (strchr(argv[i], 'h') != NULL)
            hFlag = 1;
          if (strchr(argv[i], 'n') != NULL) 
            nFlag = 2;
        }
        // assign options
        switch(nFlag + hFlag) {
          case 1:
            opt = "h";
            break;
          case 2:
            opt = "n";
            break;
          case 3:
            opt = "hn";
            break;
        }
      } else {
        option = 1;

        printf("%s\n", argv[i]);
        listContents(argv[i], opt);
      }
      if (i+1 == argc && option == 0){

        listContents(getCurPath(), opt);
      }
    }
  }

  return 0;
}
#include "util/base.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

const int filesize = 1024 * 1024 * 1024;
const char* srcfile = "./testdata_srcfile";
const char* rwfile = "./testdata_rw";
const char* mmapfile = "./testdata_mmap";

void testClear() {
  remove(srcfile);
  remove(rwfile);
  remove(mmapfile);
}

int openr(const char* file) {
  int fd = open(file, O_RDONLY);
  if (fd < 0) {
    println("open error: ", file);
    exit(1);
  }
  return fd;
}

int openw(const char* file) {
  int fd = open(file, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    println("open error: ", file);
    exit(1);
  }
  return fd;
}

void testPrepare() {
  testClear();
  constexpr int bufsize = 4 * 1024;
  char buf[bufsize];
  for (int i = 0; i < bufsize; i++) {
    buf[i] = i;
  }
  const char* files[3] = {srcfile, rwfile, mmapfile};
  auto cost = nowMs();
  for (size_t file_i = 0; file_i < sizeof(files)/sizeof(char*); file_i++) {
    int fd = openw(files[file_i]);
    for (int i = 0; i < filesize; i += bufsize) {
      if (write(fd, buf, bufsize) != bufsize) {
        println("write error: ", files[file_i]);
        exit(1);
      }
    }
    close(fd);
    // chmod(files[i], S_IRUSR|S_IWUSR);
    memset(buf, 0, bufsize);
  }
  cost = nowMs() - cost;
  println("testPrepare cost ", cost);
}

void testRWcopy() {
  const char* dstfile = rwfile;
  int rfd = openr(srcfile);
  int wfd = openw(dstfile);
  constexpr int bufsize = 4 * 1024;
  char buf[bufsize];

  auto cost = nowMs();
  for (int n = 0; n < filesize; n += bufsize) {
    if (read(rfd, buf, bufsize) != bufsize) {
      println("read error: ", srcfile);
      exit(1);
    }
    if (write(wfd, buf, bufsize) != bufsize) {
      println("write error: ", dstfile);
      exit(1);
    }
  }
  cost = nowMs() - cost;

  close(rfd);
  close(wfd);
  println("testRWcopy cost ", cost);
}

void testMmapCopy() {
  const char* dstfile = mmapfile;
  int rfd = openr(srcfile);
  int wfd = openw(dstfile);

  void* rbuf = mmap(NULL, filesize, PROT_READ, MAP_SHARED, rfd, 0);
  close(rfd);

  auto cost = nowMs();
  if (write(wfd, rbuf, filesize) != filesize) {
    println("write error: ", dstfile);
    exit(1);
  }
  cost = nowMs() - cost;

  munmap(rbuf, filesize);
  close(wfd);
  println("testMmapCopy cost ", cost);
}

run() {
  testPrepare();   // 9000
  testRWcopy();    // 3200
  testMmapCopy();  // 800
  testClear();
}

main_run;

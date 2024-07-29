#include "utils/utils.h"
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

int mssleep(int msec) {
  struct timespec ts;
  int res;

  if (msec < 0) {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}

void wait_sigint(void *sig_handler) {
  sigset_t set;
  int sig = -1;
  struct sigaction sa;
  sa.sa_handler = sig_handler;
  sa.sa_handler = sig_handler;
  sa.sa_handler = sig_handler;
  sa.sa_handler = sig_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);
  sigemptyset(&set);
  sigaddset(&set, SIGINT);

  while (sig != SIGINT) {
    sigwait(&set, &sig);

    if (sig != SIGINT)
      printf("press ^C to exit\n");
  }

  printf("\n");
}

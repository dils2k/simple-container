#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sched.h>
#include <cerrno>
#include <unistd.h>

using namespace std;

int main() {
  if (unshare(CLONE_NEWPID) == -1) {
    perror("unshare");
  }

  cout << "preprocessing..." << endl;

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    return 1;
  }

  if (pid == 0) {
    if (unshare(CLONE_NEWNS) == -1) {
      perror("unshare mount namespace");
      return 1;
    }

    if (mount("/itachi", "/sasuke", nullptr, MS_BIND, nullptr) == -1) {
      perror("bind mount");
      return 1;
    }

    cout << "child pid: " << getpid() << endl;
    cout << "**** reading sasuke quotes ****" << endl;

    ifstream file("/sasuke/quotes.txt");
    if (!file) {
      cerr << "can't read /sasuke/quotes.txt" << endl;
      return 1;
    }

    cout << file.rdbuf();

    sleep(2);
    return 0;
  } 

  cout << "waiting for child to complete, pid: " << getpid() << endl;
  waitpid(pid, NULL, 0);
  cout << "child has completed" << endl;

  return 0;
}


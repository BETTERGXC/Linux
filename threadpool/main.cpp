#include "threadpool.hpp"

bool handler(int data) {
  srand(time(NULL));
  int n = rand() % 5;
  printf("Thread: %p Run Tast:%d--sleep %d sec\n",pthread_self(), data, n);
  sleep(n);
  return true;
}

int main() {
  int i;
  ThreadPool pool;
  pool.PoolInit();
  for (i = 0; i < 10; i++) {
    ThreadTask *tt = new ThreadTask(i, handler);
    pool.PushTask(tt);

  }
  pool.PoolQuit();
  return 0;

}



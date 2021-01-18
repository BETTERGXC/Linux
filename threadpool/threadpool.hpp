#pragma once 
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREAD  5
typedef bool (*handler_t)(int);
class ThreadTask {
  
  private:
    int _data;   //任务中要处理的数据
    handler_t _handler;   //任务中处理数据的方法

  public:
    ThreadTask()
      :_data(-1)
      ,_handler(NULL)
    {}

    ThreadTask(int data, handler_t handler) {
      _data = data;
      _handler = handler;
    }

    void SetTask(int data, handler_t handler) {
      _data = data;
      _handler = handler;
    }

    void Run() {
      _handler(_data);
    }
};

class ThreadPool {
  
  private:
    int _thread_max;     //线程池中线程的最大数量 -- 根据这个初始化创建指定数量的线程
    int _thread_cur;     //当前有多少个线程
    bool _tp_quit;       //表示线程池是否结束
    std::queue<ThreadTask *> _task_queue;   //任务等待队列
    pthread_mutex_t _lock;  //保护队列操作的互斥锁
    pthread_cond_t _cond;   //实现从队列中获取节点的同步条件变量

  private:
	//加锁
    void LockQueue() {
      pthread_mutex_lock(&_lock);
    }
	
	//解锁
    void UnLockQueue() {
      pthread_mutex_unlock(&_lock);
    }
	
	//唤醒一个线程
    void WakeUpOne() {
      pthread_cond_signal(&_cond);
    }
	
	//唤醒所有线程
    void WakeUpAll() {
      pthread_cond_broadcast(&_cond);
    }
	
	//线程退出
    void ThreadQuit() {
      _thread_cur--;
      UnLockQueue();
      pthread_exit(NULL);
    }
    void ThreadWait() {
      if(_tp_quit) {
        ThreadQuit();
      }
      pthread_cond_wait(&_cond, &_lock);
    }
    bool IsTmpty() {
      return _task_queue.empty();
    }
	
	//加static去掉参数里边隐含的this指针
	//不断的从任务队列中取出任务，执行任务的Run接口就可以
    static void *thr_start(void *arg) {
      ThreadPool *tp = (ThreadPool *)arg;
      while(1) {
       tp->LockQueue();
       while(tp->IsTmpty()) {
		  //如果当前任务队列为空并且退出条件为真，一步一步减少线程数
		  if(tp->_tp_quit) {
		    std::cout << "thread exit" << pthread_self() << std::endl;
			tp->_thread_cur--;
			tp->UnLockQueue();
			tp->WakeUpOne();
			pthread_exit(NULL);
		  }
		  tp->ThreadWait();
       }
       ThreadTask *tt;
       tp->PopTask(&tt);
       tp->UnLockQueue();
       tt->Run();
       delete tt;
      }
      return NULL;
    }

  public:
    ThreadPool(int max = MAX_THREAD)
      :_thread_max(max)
      , _thread_cur(0)
       ,_tp_quit(false) {
      pthread_mutex_init(&_lock, NULL);
      pthread_cond_init(&_cond, NULL);
    }

    ~ThreadPool() {
      pthread_mutex_destroy(&_lock);
      pthread_cond_destroy(&_cond);
    }

    bool PoolInit() {
      pthread_t tid;
      for (int i = 0; i < _thread_max; i++) {
        int ret = pthread_create(&tid, NULL, thr_start, this);
        if (ret != 0) {
          std::cout<<"create pool thread error\n";
          return false;
        }
		_thread_cur++;
      }
      return true;
    }

    bool PushTask(ThreadTask *tt) {
      LockQueue();
      if (_tp_quit) {
        UnLockQueue();
        return false;

      }
      _task_queue.push(tt);
      WakeUpOne();
      UnLockQueue();
      return true;
    }
	
    bool PopTask(ThreadTask **tt) {
      *tt = _task_queue.front();
      _task_queue.pop();
      return true;

    }

    bool PoolQuit() {
	  if(!_tp_quit) {  
        LockQueue();
		_tp_quit = true;
		UnLockQueue();
		WakeUpAll();
		while(_thread_cur > 0) {
		  pthread_cond_wait(&_cond, &_lock);
		}
	  }
      return true;

    }
    

};
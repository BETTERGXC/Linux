#pragma once 

#include <ucontext.h>
#define CORSZ   (1024)
#define STACKSZ (1024*64)

enum State {
  DEAD,
  READY,
  RUNNING,
  SUSPEND
};

struct schedule;
// 协程结构体
typedef struct {
  // 回调函数
  void *(*call_back)(struct schedule *s, void* args); 
  // 回调函数参数
  void *args;
  // 协程上下文
  ucontext_t ctx;
  // 协程栈
  char stack[STACKSZ];
  // 协程状态
  enum State state;
}coroutine_t;

// 协程调度器
typedef struct schedule {
  // 所有协程
  coroutine_t **coroutines;
  // 当前正在运行的协程，如果没有正在运行的协程为 -1
  int current_id;
  // 最大下标
  int max_id;
  // 主流程上下文
  ucontext_t ctx_main;
}schedule_t;


// 创建协程调度器                                                                                                 
schedule_t *schedule_creat();

// 协程执行函数                       
static void* main_fun(schedule_t *s);

// 创建协程，返回当前协程在调度器的下标
int coroutine_creat(schedule_t *s, void *(*call_back)(schedule_t *,void *args),void *args);

// 让出 CPU                                                                                                       
void coroutine_yield(schedule_t *s);

// 恢复 CPU
void coroutine_resume(schedule_t *s, int id);

// 删除协程
static void delete_coroutine(schedule_t *s, int id);

// 释放调度器
void schedule_destroy(schedule_t *s);

// 判断所有协程都运行完了
int schedule_finish(schedule_t *s);

// 启动协程
void coroutine_running(schedule_t *s, int id);


source videos
https://www.bilibili.com/video/BV1kt411z7ND?p=4&spm_id_from=333.851.header_right.history_list.click


//thread -> point to a structure of type pthread_t; pass it to pthread_create and in order to initialize it;
//attr -> specify any attribute this thread may have, eg. incl. setting stack size or info. about the scheduling priority of the thread. \
           or it can be simply pass with NULL in;
//3rd arg. -> most complex, is function pointer, function name (start_routine) which is passed a single argument of type void* ,returns a value of type void *
//arg -> is the argument to be passed to the function where thread begins execution

int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void*),
                   void *arg);


Userful link for thread introduction: 
https://subingwen.cn/linux/thread/#1-%E7%BA%BF%E7%A8%8B%E6%A6%82%E8%BF%B0
/*
*class内部不能又其他的数据类型，只能默认空类
*/
#include<iostream>
#include<Windows.h>
#include<list>  //双链表
using namespace std;

class myclass;  //声明

struct info
{
          myclass* p;//内存的首地址
          size_t n;    //代表多少个块
};

list<info> myclasslist;   //创建双链表的数据结构，每个节点是一个结构体

void mem_status()
{
          //显示内存
          for (auto i : myclasslist)
          {
                    cout << "内存地址：" << i.p << "       " << "个数：" << i.n << endl;
          }
}

class myclass
{
public:
          ~myclass()
          {
                    cout << "~myclass()" << endl;
          }
          myclass()
          {
                    cout << "myclass()" << endl;
          }

          /*重载new*/
          void* operator new(size_t size)
          {
                    cout << "new " << size << endl;
                    void* p = (void*)malloc(size);
                    if (p != nullptr)
                    {
                              info Current_memblock;
                              Current_memblock.p = (myclass*)p;   //分配就记录

                              if (size / sizeof(myclass) == 1)
                              {
                                        Current_memblock.n = 1;
                              }
                              else
                              {
                                        Current_memblock.n = (size - 4) / sizeof(myclass);   //减去一个指针的大小
                              }
                              myclasslist.push_back(Current_memblock);
                              return p;
                    }
                    else
                    {
                              return nullptr;
                    }
                   
          }

          void* operator new[](size_t size)
          {
                    cout << "new []   " << size << endl;
                    return operator new(size);  //回调new
          }

                    /*重载delete*/
          void operator delete(void* p)
          {
                    //双链表中表中检索内存地址是否匹配，若匹配则释放
                    for (auto ib = myclasslist.begin(), ie = myclasslist.end(); ib != ie; ++ib)
                    {
                              if (p == (*ib).p)
                              {
                                        myclasslist.erase(ib);
                                        free(p);
                                        break;
                              }
                    }
          }
          void operator delete [](void* p)
          {
                    return operator delete(p);  //回调delete
          }
};

int main()
{
          myclass* p1 = new myclass;
          myclass* p2 = new myclass;
          myclass* p3 = new myclass;
          myclass* p4 = new myclass;
          myclass* pa1 = new myclass[5];
          myclass* pa2 = new myclass[10];
          myclass* pa3 = new myclass[15];


          delete p1;
          delete[] pa1;
          delete[] pa2;
          //delete p1;    //哪怕是delete两次也不会报错。被重载过滤了
          mem_status();
          return 0;
}
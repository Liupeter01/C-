/*
*class�ڲ��������������������ͣ�ֻ��Ĭ�Ͽ���
*/
#include<iostream>
#include<Windows.h>
#include<list>  //˫����
using namespace std;

class myclass;  //����

struct info
{
          myclass* p;//�ڴ���׵�ַ
          size_t n;    //������ٸ���
};

list<info> myclasslist;   //����˫��������ݽṹ��ÿ���ڵ���һ���ṹ��

void mem_status()
{
          //��ʾ�ڴ�
          for (auto i : myclasslist)
          {
                    cout << "�ڴ��ַ��" << i.p << "       " << "������" << i.n << endl;
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

          /*����new*/
          void* operator new(size_t size)
          {
                    cout << "new " << size << endl;
                    void* p = (void*)malloc(size);
                    if (p != nullptr)
                    {
                              info Current_memblock;
                              Current_memblock.p = (myclass*)p;   //����ͼ�¼

                              if (size / sizeof(myclass) == 1)
                              {
                                        Current_memblock.n = 1;
                              }
                              else
                              {
                                        Current_memblock.n = (size - 4) / sizeof(myclass);   //��ȥһ��ָ��Ĵ�С
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
                    return operator new(size);  //�ص�new
          }

                    /*����delete*/
          void operator delete(void* p)
          {
                    //˫�����б��м����ڴ��ַ�Ƿ�ƥ�䣬��ƥ�����ͷ�
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
                    return operator delete(p);  //�ص�delete
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
          //delete p1;    //������delete����Ҳ���ᱨ�������ع�����
          mem_status();
          return 0;
}
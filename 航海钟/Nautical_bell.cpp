#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

class bells
{
private:
    SYSTEMTIME st;
    string watch[7], count[8];
    static bells *_inst;

    static DWORD WINAPI bell(LPVOID p)
    {
        DWORD wait = _inst->waitTime();
        while (true)
        {
            Sleep(wait);
            _inst->playBell();
            wait = _inst->waitTime();
        }
    }

    DWORD waitTime()
    {
        GetLocalTime(&st);
        int m = st.wMinute >= 30 ? st.wMinute - 30 : st.wMinute;
        return (1800000 - ((m * 60 + st.wSecond) * 1000 + st.wMilliseconds));
    }

    void playBell()
    {
        GetLocalTime(&st);
        int b = (2 * st.wHour + st.wMinute / 30) % 8;
        b = b == 0 ? 8 : b;
        int w = (60 * st.wHour + st.wMinute);
        w = w < 1 ? 5 : (w - 1) / 50;
        char hr[32];
        sprintf(hr, "%.2d:%.2d", st.wHour, st.wMinute);
        cout << hr << " - " << watch[w] << " watch - " << count[b - 1] << " Bell";
        //判断是否是多次响声
        if (b > 1)
            cout << "s";
        else
            cout << " ";
        //结束语gone
        cout << " Gone." << endl;
        //输出响多少次声音
        for (int x = 0, c = 1; x < b; x++, c++)
        {
            //\7表示响声
            cout << "\7";
            //睡眠0.5秒
            Sleep(500);
            //判断c的奇偶，在进行睡眠
            if (!(c % 2))
                Sleep(300);
        }
    }

public:
    void start()
    {
        //表示是什么时候轮班
        watch[0] = "Middle";
        watch[1] = "Morning";
        watch[2] = "Forenoon";
        watch[3] = "Afternoon";
        watch[4] = "Dog";
        watch[5] = "First";
        //count数组表示钟响的声音次数
        count[0] = "One";
        count[1] = "Two";
        count[2] = "Three";
        count[3] = "Four";
        count[4] = "Five";
        count[5] = "Six";
        count[6] = "Seven";
        count[7] = "Eight";
        //this指针
        _inst = this;
        //创建一个线程，创建的线程在调用进程的虚拟地址空间中运行
        CreateThread(NULL, 0, bell, NULL, 0, NULL);
    }
};

bells* bells::_inst = 0;
//--------------------------------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
    //定义航海钟实例变量
    bells b;
    //函数开始
    b.start();
    // <- runs forever!，即线程一直运行
    while( 1 );
    return 0;
}
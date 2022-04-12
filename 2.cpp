#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<algorithm>
using namespace std;
int row_1_now;
int row_2_now;

void read_data(string file_name,int a[][200]);
void dec_to_bin(int a[][200],unsigned int bin_a[][5],int row_num);
//确定矩阵的一行对应的首个非零元素
void find_first_1(unsigned int *bin_t, int& pos_n ,int& status);
//void handle_1(one_row& r1, one_row& r2, one_row* a);
//bool judge_equal_first1(one_row a, one_row b);


class one_row
{
public:
    one_row(){}
    //one_row(unsigned int* bin_tt);
    void init_row(unsigned int* bin_tt,int sta);
    int get_pos() { return pos_n; }
    bool exist = true;//若该行全为0，则exit=false
    int status = -1;//该行全为0->0 属于消元子->1 属于被消元行->2
    int pos_n = -1;//首个非零元素所在位置
    unsigned int bin_t[5] = { 0 };
    int rank_num = -1;//该行在当前矩阵中的排序
};

void read_data(string file_name, int a[][200])
{
    ifstream ifs(file_name, ifstream::in);
    string str;
    int row_num_now = 0;
    while (getline(ifs, str))
    {
        int intS[200];
        //int i = 0;//某个数的位数
        int num = 0;//一行中的数据数目
        int len = str.length();
        int len_temp = 0;
        string temp[200];
        for (; len_temp < len; )
        {
            while (str[len_temp] != ' ' && len_temp < len)
            {
                temp[num] += str[len_temp];
                len_temp++;
            }
            //将string类型的temp转化为int类型
            intS[num] = atoi(temp[num].c_str());
            num++;
            len_temp++;
        }
        for (int i = 0; i < num; i++)
            a[row_num_now][i] = intS[i];
        a[row_num_now][num] = -1;//结束符
        row_num_now++;
    }
    ifs.close();
}
void dec_to_bin(int a[][200], unsigned int bin_a[][5], int row_num)
{

    for (int i = 0; i < row_num; i++)
    {
        int temp1;
        int temp2;
        int j = 0;
        while (a[i][j] != -1)
        {
            //bin_a[i][temp1]+=1(后面有temp2个0)->设置临时变量为1，并将其左移temp位
            temp1 = 4 - a[i][j] / 32;
            temp2 = a[i][j] % 32;
            unsigned int temp3 = 1;
            temp3 <<= temp2;
            bin_a[i][temp1] += temp3;
            j++;
        }
    }
}
void one_row::init_row(unsigned int* bin_tt, int sta)
{
    for (int i = 0; i < 5; i++)
        bin_t[i] = bin_tt[i];
    //exist = true;
    status = sta;
    find_first_1(bin_t, pos_n, status);
}
void find_first_1(unsigned int* bin_t, int& pos_n, int& status)
{

    for (int i = 0; i < 5; i++)
    {
        if (bin_t[i] != 0)
        {
            unsigned int temp = bin_t[i];
            int n = 0;
            //将该行右移，直到为0，则能确定第一个1所在位置
            while (temp != 0)
            {
                temp >>= 1;
                n++;
            }
            pos_n = 32 * (i + 1) - n - 30;
            break;
        }
    }
    if (pos_n == -1)
    {
        if (status == 1)
            row_1_now -= 1;
        if (status == 2)
            row_2_now -= 1;
        status = 0;
    }
}
void handle_1(one_row& r1, one_row& r2, one_row* a)
{
    //通过异或运算进行消元
    for (int i = 0; i < 5; i++)
        r2.bin_t[i] = r2.bin_t[i] ^ r1.bin_t[i];
    //消元后的处理
    //消元后：1）被消元行变为0->丢弃->通过将其状态（status）设置为0
    //        2）被消元行首项没有对应的消元子->升级为消元子（不再为被消元行）->将其状态设置为1，并将其添加到消元子中(XXX.init_row( ))
    int pos;//消元后的被消元行首个非零元素位置
    find_first_1(r2.bin_t, pos, r2.status);
    if (r2.status != 0)//消元行不为0
    {
        bool h = false;
        //遍历当前所有a_row
        for (int i = 0; i < row_1_now; i++)
        {
            int temp_pos;
            find_first_1(a[i].bin_t, temp_pos, a[i].status);
            if (pos == temp_pos)
            {
                h = true;
                break;
            }
        }
        //将该被消元行填入到消元子中
        if (h == false)
        {
            r2.status = 1;
            a[row_1_now].init_row(r2.bin_t, r2.status);
            row_1_now++;
        }
    }
    //遍历的时候是否将其视为被消元行是由status来判断、控制的
    //else
    //{
    //    row_2_now--;
    //}
}
bool judge_equal_first1(one_row a, one_row b)
{
    int pos1, pos2;
    find_first_1(a.bin_t, pos1, a.status);
    find_first_1(b.bin_t, pos2, b.status);
    if (pos1 == pos2)
        return true;
    else
        return false;
}
bool cmp(one_row a, one_row b)
{
    return a.pos_n < b.pos_n;
}

int main()
{
    int row_num_1 = 22;
    int row_num_2 = 8;
    row_1_now = row_num_1;
    row_2_now = row_num_2;
    int col_num = 130;
    //读取文件中的数据
    string str1 = "消元子.txt";
    string str2 = "被消元行.txt";
    int a[50][200];
    int b[50][200];
    read_data(str1,a);
    read_data(str2,b);
    //将数据转化为二进制形式存储
    unsigned int bin_a[50][5] = { 0 };//将一行的130个数分为32+32+32+32+2的5个4Byte的形式来存储
    unsigned int bin_b[50][5] = { 0 };
    dec_to_bin(a,bin_a,row_num_1);
    dec_to_bin(b,bin_b,row_num_2);
    //保存a,b数组一行的信息
    one_row a_row[50];
    one_row b_row[50];
    for (int i = 0; i < row_num_1; i++)
        a_row[i].init_row(bin_a[i],1);
    for (int i = 0; i < row_num_2; i++)
        b_row[i].init_row(bin_b[i],2);
    //对于每个被消元行，遍历消元子
    for (int i = 0; i < row_2_now && b_row[i].status == 2; i++)
    {
        for (int j = 0; j < row_1_now; j++)
        {
            //判断此时消元子与被消元行的首元素是否位于同一位置
            if (judge_equal_first1(a_row[j], b_row[i]))//位于同一位置
            {
                handle_1(a_row[j], b_row[i], a_row);
            }
        }
    }
    one_row c[50];
    int k = 0;
    for (int i = 0; i < row_2_now && b_row[i].status == 2; i++)
    {
        //得到b_row[i]首个非零元素位置
        find_first_1(b_row[i].bin_t, b_row[i].pos_n, b_row[i].status);
        //将消元后的结果保存到一个新的数组中
        c[k].init_row(b_row[i].bin_t, b_row[i].status);
        c[k].pos_n = b_row[i].pos_n;
        k++;
    }
    //将消元得到的结果按照首个非零元素所在位置排序(从小到大)
    sort(c, c + k, cmp);
}

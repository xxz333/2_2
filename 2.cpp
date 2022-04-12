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
//ȷ�������һ�ж�Ӧ���׸�����Ԫ��
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
    bool exist = true;//������ȫΪ0����exit=false
    int status = -1;//����ȫΪ0->0 ������Ԫ��->1 ���ڱ���Ԫ��->2
    int pos_n = -1;//�׸�����Ԫ������λ��
    unsigned int bin_t[5] = { 0 };
    int rank_num = -1;//�����ڵ�ǰ�����е�����
};

void read_data(string file_name, int a[][200])
{
    ifstream ifs(file_name, ifstream::in);
    string str;
    int row_num_now = 0;
    while (getline(ifs, str))
    {
        int intS[200];
        //int i = 0;//ĳ������λ��
        int num = 0;//һ���е�������Ŀ
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
            //��string���͵�tempת��Ϊint����
            intS[num] = atoi(temp[num].c_str());
            num++;
            len_temp++;
        }
        for (int i = 0; i < num; i++)
            a[row_num_now][i] = intS[i];
        a[row_num_now][num] = -1;//������
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
            //bin_a[i][temp1]+=1(������temp2��0)->������ʱ����Ϊ1������������tempλ
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
            //���������ƣ�ֱ��Ϊ0������ȷ����һ��1����λ��
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
    //ͨ��������������Ԫ
    for (int i = 0; i < 5; i++)
        r2.bin_t[i] = r2.bin_t[i] ^ r1.bin_t[i];
    //��Ԫ��Ĵ���
    //��Ԫ��1������Ԫ�б�Ϊ0->����->ͨ������״̬��status������Ϊ0
    //        2������Ԫ������û�ж�Ӧ����Ԫ��->����Ϊ��Ԫ�ӣ�����Ϊ����Ԫ�У�->����״̬����Ϊ1����������ӵ���Ԫ����(XXX.init_row( ))
    int pos;//��Ԫ��ı���Ԫ���׸�����Ԫ��λ��
    find_first_1(r2.bin_t, pos, r2.status);
    if (r2.status != 0)//��Ԫ�в�Ϊ0
    {
        bool h = false;
        //������ǰ����a_row
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
        //���ñ���Ԫ�����뵽��Ԫ����
        if (h == false)
        {
            r2.status = 1;
            a[row_1_now].init_row(r2.bin_t, r2.status);
            row_1_now++;
        }
    }
    //������ʱ���Ƿ�����Ϊ����Ԫ������status���жϡ����Ƶ�
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
    //��ȡ�ļ��е�����
    string str1 = "��Ԫ��.txt";
    string str2 = "����Ԫ��.txt";
    int a[50][200];
    int b[50][200];
    read_data(str1,a);
    read_data(str2,b);
    //������ת��Ϊ��������ʽ�洢
    unsigned int bin_a[50][5] = { 0 };//��һ�е�130������Ϊ32+32+32+32+2��5��4Byte����ʽ���洢
    unsigned int bin_b[50][5] = { 0 };
    dec_to_bin(a,bin_a,row_num_1);
    dec_to_bin(b,bin_b,row_num_2);
    //����a,b����һ�е���Ϣ
    one_row a_row[50];
    one_row b_row[50];
    for (int i = 0; i < row_num_1; i++)
        a_row[i].init_row(bin_a[i],1);
    for (int i = 0; i < row_num_2; i++)
        b_row[i].init_row(bin_b[i],2);
    //����ÿ������Ԫ�У�������Ԫ��
    for (int i = 0; i < row_2_now && b_row[i].status == 2; i++)
    {
        for (int j = 0; j < row_1_now; j++)
        {
            //�жϴ�ʱ��Ԫ���뱻��Ԫ�е���Ԫ���Ƿ�λ��ͬһλ��
            if (judge_equal_first1(a_row[j], b_row[i]))//λ��ͬһλ��
            {
                handle_1(a_row[j], b_row[i], a_row);
            }
        }
    }
    one_row c[50];
    int k = 0;
    for (int i = 0; i < row_2_now && b_row[i].status == 2; i++)
    {
        //�õ�b_row[i]�׸�����Ԫ��λ��
        find_first_1(b_row[i].bin_t, b_row[i].pos_n, b_row[i].status);
        //����Ԫ��Ľ�����浽һ���µ�������
        c[k].init_row(b_row[i].bin_t, b_row[i].status);
        c[k].pos_n = b_row[i].pos_n;
        k++;
    }
    //����Ԫ�õ��Ľ�������׸�����Ԫ������λ������(��С����)
    sort(c, c + k, cmp);
}

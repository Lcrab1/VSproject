#include"lc3.h"
#include"opcode.h"
#include"test.h"


uint16_t memory[UINT16_MAX];

uint16_t reg[R_COUNT];

//�����ʼ����α����� 
HANDLE hStdin = INVALID_HANDLE_VALUE;

//�洢����̨��ģʽ�;�ģʽ
DWORD fdwMode, fdwOldMode;

uint16_t sign_extend(uint16_t x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

uint16_t swap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}

void update_flags(uint16_t r)
{
    if (reg[r] == 0)
    {
        reg[R_COND] = FL_ZRO;
    }
    else if (reg[r] >> 15) 
    {
        reg[R_COND] = FL_NEG;
    }
    else
    {
        reg[R_COND] = FL_POS;
    }
}

void read_image_file(FILE* file)
{
   
    uint16_t origin;


    //file�ļ��ĵ�һ�����ݴ浽origin�� ��С�������0x00 30 ����Ӧ����0x30 00 ˵����ʱ��λ���ŵ��˵�λ����λ���ŵ��˸�λ��ԭ����Ӧ���Ǵ����
    fread(&origin, sizeof(origin), 1, file);

    //��˻�С��
    origin = swap16(origin);

    //UINT16_MAX 65536
    uint16_t max_read = UINT16_MAX - origin;
    //�����￪ʼ��ȡ
    uint16_t* p = memory + origin;
    size_t read = fread(p, sizeof(uint16_t), max_read, file);


    //����ȡ�Ĵ��������ȫ���任ΪС����
    while (read-- > 0)
    {
        *p = swap16(*p);
        ++p;
    }
}

int read_image(const char* image_path)
{
    //rb �����ƴ��ļ�
    FILE* file = fopen(image_path, "rb");       
    if (!file) {
        return 0;
    };
    //����read_image_file
    read_image_file(file);
    //�ر��ļ�
    fclose(file);
    return 1;
}

uint16_t check_key()
{
    return WaitForSingleObject(hStdin, 1000) == WAIT_OBJECT_0 && _kbhit();
}

void mem_write(uint16_t address, uint16_t val)
{
    memory[address] = val;
}

uint16_t mem_read(uint16_t address)
{
    if (address == MR_KBSR)
    {
        //�������Ƿ������룬������Ϊ�ⲿʱ������������,ʵ�ֶԼ�������ļ�ʱ��Ӧ
        if (check_key())
        {
            memory[MR_KBSR] = (1 << 15);
            memory[MR_KBDR] = getchar();
        }
        else
        {
            memory[MR_KBSR] = 0;
        }
    }
    return memory[address];
}

//�������뻺��
void disable_input_buffering()
{
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &fdwOldMode);
    fdwMode = fdwOldMode ^ ENABLE_ECHO_INPUT ^ ENABLE_LINE_INPUT;  //����������Ժ�������
    //�����µĿ���̨ģʽ
    SetConsoleMode(hStdin, fdwMode);    
    //������뻺����,��ֹδ������������ڻ�����
    FlushConsoleInputBuffer(hStdin);  
}

//�ָ�����̨��ʼ״̬
void restore_input_buffering()
{
    //printf("hello!\r\\n");
    SetConsoleMode(hStdin, fdwOldMode);
    //hStdin�ǿ���̨��������
    //fdwOldMode���ڸ��Ŀ���̨ģʽ֮ǰ�����ԭʼģʽ��
}

//�жϴ���
void handle_interrupt(int signal)
{
    restore_input_buffering();


    printf("\n");
    //-2 ��ʾ���յ��ж��ź�
    exit(-2);
}





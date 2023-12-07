#include"cashier.h"


uint32_t __mark = 0;

uint32_t getShortest(vector<queue<Customer>>* cashier)
{
    uint32_t shortestSign = 0;
    uint32_t shortestQ = UINT32_MAX;
    for (int i = 0; i < (*cashier).size(); i++)
    {
        if ((*cashier)[i].size() < shortestQ)
        {
            shortestQ = (*cashier)[i].size();
            shortestSign = i;
        }
    }

    //��¼��С���еı��
    int count = 0;
    int index = 0;
    int arr[10] = { 0 };
    for (int i = 0; i < (*cashier).size(); i++)
    {
        if ((*cashier)[i].size() == shortestQ)
        {
            arr[index++] = i;
            count++;
        }
    }


    //��С���в�ֹһ�� �Ƚ϶�ͷ��payingʱ�䣬����paying��Ķ���
    if (count > 1)
    {
        int earlist = arr[0];
        for (int i = 0; i < count; i++)
        {
            //��ȡ����ͷ��������������±�
            if ((*cashier)[arr[i]].empty())
            {
                earlist = arr[i];
                break;
            }

            Time time1 = (*cashier)[earlist].front()->m_paying;
            Time time2 = (*cashier)[arr[i]].front()->m_paying;
            if (time1 > time2)
            {
                earlist = arr[i];
            }
        }
        return earlist;
    }


    return shortestSign;
}

void chooseCashier(vector<queue<Customer>>* cashier, queue<Customer>* q, Customer cus)
{
    //����
    //
    // 1��ѡ�������̵�����̨
    uint32_t shortestQ = getShortest(cashier);
    // 2������̨Ϊ��  ֱ�ӽ��� ֱ�Ӹ���
    if ((*cashier)[shortestQ].empty())
    {

        cus->m_paying = cus->m_enter;
        cus->m_leave.m_hour = cus->m_paying.m_hour;
        cus->m_leave.m_minu = cus->m_paying.m_minu + 3;
        cus->m_leave.m_secn = cus->m_paying.m_secn;
        cus->m_NoC = shortestQ;
        (*cashier)[shortestQ].push(cus);
        q->push(cus);
    }
    else
    { // 3������̨��Ϊ�գ�ֱ�ӽ���
        cus->m_NoC = shortestQ;
        (*cashier)[shortestQ].push(cus);
    }
}

void updateQueueQ(vector<queue<Customer>>* cashier, queue<Customer>* q) {
    //����ʱ����ͷԪ���Ѿ��뿪
    Customer temp = q->front();
    q->pop();
    (*cashier)[temp->m_NoC].pop();
    if (!(*cashier)[temp->m_NoC].empty())
    {

        //����ʱ����ͷԪ���Ѿ��뿪
        if ((*cashier)[temp->m_NoC].front()->m_enter > temp->m_leave) {
            (*cashier)[temp->m_NoC].front()->m_paying = (*cashier)[temp->m_NoC].front()->m_enter;
            (*cashier)[temp->m_NoC].front()->m_leave = ((*cashier)[temp->m_NoC].front()->m_paying + 3);
            q->push((*cashier)[temp->m_NoC].front());
        }
        else {
            (*cashier)[temp->m_NoC].front()->m_paying = temp->m_leave;
            (*cashier)[temp->m_NoC].front()->m_leave = ((*cashier)[temp->m_NoC].front()->m_paying + 3);
            q->push((*cashier)[temp->m_NoC].front());
        }
    }
}

void deleteTemp(Customer temp) {
    if (temp != nullptr) {
        delete temp;
        temp = nullptr;
    }
}
//�˿ͽ�������̨
void enterCashier(vector<queue<Customer>>* cashier, queue<Customer>* q, Time& time)
{

    Customer arr[2];

    for (int i = 0; i < 2; i++)
    {
        Customer cus = new _CUSTOMER_;
        cus->m_num = ++__mark;
        cus->m_enter.m_hour = time.m_hour;
        cus->m_enter.m_minu = time.m_minu;
        cus->m_enter.m_secn = time.m_secn + rand() % 59 + 1;
        arr[i] = cus;
        //���߲���������̨
        chooseCashier(cashier, q, cus);
    }
    //��������Ϊ����ͬʱ�޸��뿪����
    if (arr[0]->m_enter > arr[1]->m_enter)
    {
        swapTime(arr[0]->m_enter, arr[1]->m_enter);
        swapTime(arr[0]->m_leave, arr[1]->m_leave);
    }
    bool flag1 = false;
    bool flag2 = false;
    int count = 0;
    //���в�Ϊ�� ����ӷ����뵱ǰ������ͬ
    while ((!q->empty()) && (q->front()->m_leave.m_minu == arr[0]->m_enter.m_minu))
    {
        // L��ͷ
        if (flag1 == false && q->front()->m_leave <= arr[0]->m_enter)
        {
            flag1 = true;
            if (count == 2) {
                break;
            }
            count++;
            Customer temp = q->front();
            updateQueueQ(cashier, q);
            printLeaveCus(temp, cashier);
            deleteTemp(temp);
            printf("\r\n");
        }

        // LLEE
        if (flag1 == true && q->front()->m_leave <= arr[0]->m_enter)
        {
            if (count == 2) {
                break;
            }
            count++;
            Customer temp = q->front();
            updateQueueQ(cashier, q);
            printLeaveCus(temp, cashier);
            printf("\r\n");

            for (int i = 0; i < 2; i++)
            {
                printEnterCus(arr[i], cashier);
                printf("\r\n");
            }
            time.m_minu++;
            deleteTemp(temp);
            return;
        }
        // LELE
        else if (flag1 == true &&
            q->front()->m_leave >= arr[0]->m_enter &&
            q->front()->m_leave <= arr[1]->m_enter)
        {
            if (count == 2) {
                break;
            }
            count++;
            Customer temp = q->front();
            updateQueueQ(cashier, q);

            printEnterCus(arr[0], cashier); //���˴˴�
            printf("\r\n");

            printLeaveCus(temp, cashier);
            printf("\r\n");

            printEnterCus(arr[1], cashier); //���˴˴�
            printf("\r\n");

            deleteTemp(temp);
            time.m_minu++;
            return;
        }
        // LEEL
        if (flag1 == true && q->front()->m_leave >= arr[1]->m_enter)
        {
            if (count == 2) {
                break;
            }
            count++;
            Customer temp = q->front();
            updateQueueQ(cashier, q);

            printEnterCus(arr[0], cashier); //���˴˴�
            printf("\r\n");

            printEnterCus(arr[1], cashier); //���˴˴�
            printf("\r\n");

            printLeaveCus(temp, cashier);
            printf("\r\n");

            deleteTemp(temp);
            time.m_minu++;
            return;
        }

        // E��ͷ EL
        if (flag1 == false && flag2 == false &&
            q->front()->m_leave >= arr[0]->m_enter &&
            q->front()->m_leave <= arr[1]->m_enter)
        {

            flag2 = true;
            if (count == 2) {
                break;
            }
            count++;
            Customer temp = q->front();
            updateQueueQ(cashier, q);

            printEnterCus(arr[0], cashier); //���˴˴�
            printf("\r\n");
            printLeaveCus(temp, cashier);
            printf("\r\n");
            deleteTemp(temp);
        }
        // ELLE
        if (flag2 == true &&
            q->front()->m_leave >= arr[0]->m_enter &&
            q->front()->m_leave <= arr[1]->m_enter)
        {
            if (count == 2) {
                break;
            }
            count++;
            Customer temp = q->front();
            updateQueueQ(cashier, q);


            printLeaveCus(temp, cashier);
            printf("\r\n");

            printEnterCus(arr[1], cashier); //���˴˴�
            printf("\r\n");
            deleteTemp(temp);
            time.m_minu++;
            return;
        }
        // ELEL
        else if (flag2 == true && q->front()->m_leave >= arr[1]->m_enter)
        {
            if (count == 2) {
                break;
            }
            count++;
            Customer temp = q->front();
            updateQueueQ(cashier, q);

            printEnterCus(arr[1], cashier); //���˴˴�
            printf("\r\n");

            printLeaveCus(temp, cashier);
            printf("\r\n");
            deleteTemp(temp);
            time.m_minu++;
            return;
        }
        // EELL
        if (flag1 == false && flag2 == false && q->front()->m_leave >= arr[1]->m_enter)
        {
            if (count == 2) {
                break;
            }
            count++;
            for (int i = 0; i < 2; i++)
            {
                printEnterCus(arr[i], cashier);
                printf("\r\n");
            }

            Customer temp = q->front();
            updateQueueQ(cashier, q);

            printLeaveCus(temp, cashier);
            printf("\r\n");
            deleteTemp(temp);

            if (!q->empty())
            {
                temp = q->front();
                updateQueueQ(cashier, q);

                printLeaveCus(temp, cashier);
                printf("\r\n");
            }
            deleteTemp(temp);
            time.m_minu++;
            return;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        printEnterCus(arr[i], cashier);
        printf("\r\n");
    }

    time.m_minu++;
}

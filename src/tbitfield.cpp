// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) // конструктор класса
{                                       // len - кол-во битов, которые надо хранить в битовом поле
    if (len <= 0) throw -1;             // если длина битового поля некорректна, то исключение
    BitLen = len;                       // длина битового поля
    //MemLen = len / sizeof(TELEM) + 1;   // расчет кол-ва элементов массива, необходимых для хранения битов. 1 эл-т массива - 32 бита
    MemLen = (len + 31) / 32;
    pMem = new TELEM[MemLen];           // динамический массив памяти для хранения битов (MemLen - кол-во элементов в массиве)
    for (int i = 0; i < MemLen; i++)
    {   
        pMem[i] = 0;                    // каждый эл-т массива (каждый бит) = 0
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{                                             // bf - копия объекта
    MemLen = bf.MemLen;                       // длина массива
    BitLen = bf.BitLen;                       // кол-во бит
    pMem = new TELEM[MemLen];                 // выделяем память
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];                 // копируем элементы из массива
    }
}

TBitField::~TBitField() // деструктор, освобождение памяти
{
    //delete[] pMem;                            // удаление памяти выделенной под массив
    //std::cout << "Destructor called. Freeing memory..." << std::endl;


    if (pMem != nullptr) 
    {
        delete[] pMem;
        pMem = nullptr; // предотвращаем двойное освобождение
    }

}

int TBitField::GetMemIndex(const int n) const       // индекс Мем для бита n
{                                                   // вычисление индекста элемента массива pMem, в котором хранится бит с индексом n
    if (n > (BitLen - 1) || (n < 0)) throw - 1;     // проверка корректности индекса и деление на кол-во битов в одном эл-те массива (на 32, тк int)
    int res = n / 32;                               // рассчет индекса элемента МАССИВА,тк в каждом эл-те может храниться до 32 бит
    return res;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % 32);                      // создание маски для конкретного бита. маска - число, у которого только один бит установлен в 1 на нужной позиции
                                               // сдвигает единицу на позицию, соответствующую индексу бита
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{           // устанавливает бит с индексом n в 1
    if (n < 0 || n >= BitLen) throw - 1;        // проверка на выход индекса за пределы диапазона. вызов исключения

    int i = GetMemIndex(n);                    // получаем индекс элем-та МАССИВа, в котором находится бит
    TELEM t = GetMemMask(n);                   // создаем маску чтоб установить конкретный бит
    pMem[i] = pMem[i] | t;                     // устанавливаем нужный бит в 1 используя логическое "или"
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw - 1;

    int i = GetMemIndex(n);         
    TELEM t = ~GetMemMask(n);       // создаем маску где нужный бит = 0, остальные = 1 (маска инвертируется)
    pMem[i] = pMem[i] & t;          // сбрасываем нужный бит используя операцию "и"
    //return pMem[i] & t;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw - 1;

    int i = GetMemIndex(n);
    TELEM t = GetMemMask(n);            // создаем маску, где стоит 1 в том бите который соответствует индексу n
    return (pMem[i] & t);// != 0;          // извлечение значения конкретного бита

}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // Оператор присваивания
{
    if (this != &bf) 
    {                           // Проверка на самоприсваивание
        if (MemLen != bf.MemLen) 
        {                    // Если размеры массивов различаются
            delete[] pMem; // Освобождаем старую память
            MemLen = bf.MemLen; // Обновляем длину массива
            pMem = new TELEM[MemLen]; // Выделяем новую память
        }

        BitLen = bf.BitLen; // Копируем количество битов

        for (int i = 0; i < MemLen; i++) 
        {
            pMem[i] = bf.pMem[i]; // Копируем элементы массива
        }
    }
    return *this; // Возвращаем текущий объект
}


int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this -> BitLen != bf.BitLen)                // если кол-во битов у текущ.объекта и у копии (?) разное...
    {                                       // ...то они никак не могут быть равными
        return 0;
    }
    for (int i = 0; i < BitLen; i++)
    {
        if (this->GetBit(i) != bf.GetBit(i)) 
        {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxBitLen; // Определяем минимальную длину для нового результата
    if (BitLen > bf.BitLen)
    {
        maxBitLen = BitLen;     // Используем длину текущего объекта
    }
    else
    {
        maxBitLen = bf.BitLen;  // Используем длину другого битового поля
    }

    TBitField res(maxBitLen);
    
    for (int i = 0; i < (res.MemLen); i++) {
        // Для каждого элемента используем pMem или 0, если индекс выходит за пределы  (см. operator&)
        res.pMem[i] = (i < this->MemLen ? this->pMem[i] : 0) | (i < bf.MemLen ? bf.pMem[i] : 0);
    }
    return res;

}


TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxBitLen; // Определяем минимальную длину для нового результата
    if (BitLen > bf.BitLen)
    {
        maxBitLen = BitLen;     // Используем длину текущего объекта
    }
    else
    {
        maxBitLen = bf.BitLen;  // Используем длину другого битового поля
    }

    TBitField res(maxBitLen);

    for (int i = 0; i < res.MemLen; i++)
    {                  
        res.pMem[i] = (i < this->MemLen ? this->pMem[i] : 0) & (i < bf.MemLen ? bf.pMem[i] : 0);

        // (i < MemLen ? pMem[i] : 0) - проверка меньшели i чем MemLen (длина массива текущего битвилда)
                // если i меньше МемЛен, то выбирается значение из pMem[i]
                // если >= то возврат 0
        //  (i < bf.MemLen ? bf.pMem[i] : 0) - проверка меньше ли i чем bf.MemLen (длина массива переданного битфилда)
                // аналогично выше
        // после проивзодится побитовая операция "И".
        // пример: 5 в двоичном - 0101, 3 - 0011, ответ будет 1 - 0001
    }
    return res;
}


TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(this->BitLen);
    for (int i = 0; i < this->MemLen; i++)
    {
        result.pMem[i] = ~this->pMem[i];
    }
    
    int bitsInLastElem = BitLen % 32;       // определяем кол-во бит, используемых в последнем эл-те массива..
                                            // ..pMem для хранения битов в битовом поле. пример: тк каждый эл-т pMem обычно инт,..
                                            // ..то может хранить 32 бита. с помощбю операции % мы узнаем сколько битов реально в последнем эл-те...
                                            // ..если БитЛен = 34, то эта переменная будет = 2, что означает, что в последнем эл-те используется ток 2 бита
    
    // Проверяем, есть ли значащие биты в последнем элементе
    if (bitsInLastElem != 0) 
    {
        TELEM mask = (1 << bitsInLastElem) - 1; // Создаем битовую маску для обрезки лишних битов
                                                // Сдвигаем единицу влево на количество значащих битов и вычитаем 1
                                                // Например, если bitsInLastElem = 2, то mask будет равно 3 (или 11 в двоичном)
       
       
        result.pMem[result.MemLen - 1] &= mask; // Применяем побитовую операцию "И" к последнему элементу pMem,
                                                // чтобы сохранить только значащие биты и обнулить остальные
    }
    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        int b;
        istr >> b;          // читается очерендное число из потока (0 или 1) которые являются значением бита
        if (b)              // если б != 0 то
        {
            bf.SetBit(i);   // вызываем метод для установки бита на позицию i в 1
        }
        else
        {
            bf.ClrBit(i);   // если б = 0, то очищаем бит на позиции i
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i) == 0)      // если бит "0", то выводим 0
        {
            ostr << 0;
        }
        else
        {
            ostr << 1;
        }
    }
    return ostr;
}


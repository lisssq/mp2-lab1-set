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
    MemLen = len / sizeof(TELEM) + 1;   // расчет кол-ва элементов массива, необходимых для хранения битов. 1 эл-т массива - 32 бита
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
        pMem[i] = bf.MemLen;                  // копируем элементы из массива
    }
}

TBitField::~TBitField() // деструктор, освобождение памяти
{
    delete[] pMem;                            // удаление памяти выделенной под массив
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{                                             // вычисление индекста элемента массива pMem, в котором хранится бит с индексом n
    if (n > (BitLen - 1) || (n < 0)) throw - 1;// проверка корректности индекса и деление на кол-во битов в одном эл-те массива (на 32, тк int)
    int res = n / 32;                         // рассчет индекса элемента МАССИВА,тк в каждом эл-те может храниться до 32 бит
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
  return FAKE_INT;
}

void TBitField::SetBit(const int n) // установить бит
{                                              // устанавливает бит с индексом n в 1
    int i = GetMemIndex(n);                    // получаем индекс элем-та МАССИВа, в котором находится бит
    TELEM t = GetMemMask(n);                   // создаем маску чтоб установить конкретный бит
    pMem[i] = pMem[i] | t;                     // устанавливаем нужный бит в 1 используя логическое "или"
}

void TBitField::ClrBit(const int n) // очистить бит
{
    int i = GetMemIndex(n);         
    TELEM t = ~GetMemMask(n);       // создаем маску где нужный бит = 0, остальные = 1 (маска инвертируется)
    pMem[i] = pMem[i] & t;          // сбрасываем нужный бит используя операцию "и"

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int i = GetMemIndex(n);
    TELEM t = GetMemMask(n);            // создаем маску, где стоит 1 в том бите который соответствует индексу n
    return (pMem[i] & t) != 0;          // извлечение значения конкретного бита

}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    if (MemLen != bf.MemLen)                // проверка совпадения длины массива памяти текущего объекта и копии (?)
    {
        delete[]pMem;                       // если нет, то удаляем память
        MemLen = bf.MemLen;                 // устанавливаем новую длину для памяти
        pMem = new TELEM[bf.MemLen];        // выделяем новый массив для хранения
     }
    BitLen = bf.BitLen;                     // копируем кол-во битов
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];               // копируем элементы массива
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)                // если кол-во битов у текущ.объекта и у копии (?) разное...
    {                                       // ...то они никак не могут быть равными
        return 0;
    }
    for (int i = 0; i < MemLen; ++i)
    {
        if (pMem[i] != bf.pMem[i]) 
        {
            return 0;
        }
    }
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator~(void) // отрицание
{
    return FAKE_BITFIELD;
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
        ostr << bf.GetBit(i);
    }
    return ostr;
}

// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

//помогите пожалуйста 
// 
// 
// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)       //создается битовое поле
{
    MaxPower = mp;      //макс.кол-во элементов в множестве
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;      
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(-1)
{
    MaxPower = bf.GetLength();      //оздаёт множество из битового поля bf. 
                                    //MaxPower равен длине битового поля.
}

TSet::operator TBitField()
{
    return BitField;        // возвращает битовое поле для объекта множества
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;        // возвращает максимальное количество элементов, 
                            // которое может содержаться в множестве.
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);    //проверяет, входит ли элемент Elem в множество. 
                                     // Используется метод GetBit, который проверяет бит в битовом поле.
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    //BitField.SetBit(Elem);      //добавляет элемент в множество, 
                                //устанавливая соответствующий бит в битовом поле.

    if (Elem >= 0 && Elem < MaxPower)  // Проверка границ
    {
        BitField.SetBit(Elem);
    }
    else
    {
        throw -1;
    }

}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);      //удаляет элемент из множества, 
                                //очищая соответствующий бит в битовом поле.
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
   // return FAKE_SET;
    if (this != &s)     
    {
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    //сравнивает два множества
    if ((MaxPower == s.MaxPower) && (BitField == s.BitField))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    //сравнение на неравенство. Возвращает 1, если множества отличаются.
    return ((MaxPower != s.MaxPower) || (BitField != s.BitField));
}

TSet TSet::operator+(const TSet &s) // объединение
{

    // Определяем максимальный размер для нового множества
    int newMaxPower = MaxPower > s.MaxPower ? MaxPower : s.MaxPower;
    // Создаем новое множество с максимальной мощностью
    TSet res(newMaxPower);
    // Объединяем битовые поля
    res.BitField = BitField | s.BitField;
    return res;

}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    //добавляет один элемент в множество
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    //удаляет элемент из множества
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    // Определяем максимальный размер для результирующего множества
    int newMaxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    // Создаем новое множество с максимальной мощностью
    TSet res(newMaxPower);
    // Пересекаем битовые поля (по минимальной мощности)
    res.BitField = BitField & s.BitField;
    
    return res;
}

TSet TSet::operator~(void) // дополнение
{
    //дополнение множества, инвертирует биты в битовом поле
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}

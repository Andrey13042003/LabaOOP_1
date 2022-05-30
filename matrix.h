#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <ostream>

using namespace std;

template<typename T>
class Matrix{
public:
    //выделяем память под каждый элемент матрицы
    Matrix(unsigned int n, unsigned int m){
        Mx = new T*[n * m];
        copy_for_matrix = new T*[n * m];
        _n = n;
        _m = m;
    }

    //конструктор копирования
    Matrix(const Matrix<T>& Mx)
    {
        _m = Mx.m;
        _n = Mx.n;
        copy_for_matrix = new T[size];
        for (unsigned int i = 0; i < size; i++)
        {
            for (unsigned int j = 0; j < size; j++){
                int count = 0;
                copy_for_matrix[count] = Mx[i][j];//записываем в массив
                count += 1;
            }
        }
    }
    //конструктор перемещения
    Matrix(const Matrix<T>&& Mx)
    {
        _m = Mx._m;
        _n = Mx._n;
        copy_for_matrix = new T[size];
        for (unsigned int i = 0; i < size; i++)
        {
            copy_for_matrix[i] = Mx.copy_for_matrix[i];//Mx.copy_for_matrix[i] ?
        }
        delete[] copy_for_matrix;
        /*swap(_m, Mx.m_rows );
        swap( m_colls, Mx.m_colls );
        swap( m_matrix, Mx.m_matrix );*/
    }
    //конструктор со списком инициализации
    Matrix(initializer_list<initializer_list<T>> lst)
    {
        _m = lst.size();
        _n = 0;
        for (const auto& x : lst)//работа с столбцами
        {
            if (x.size() > _n)
                _n = x.size();
        }
        T* res = new T[size + 1];//выделяем память под массив(как под матрицу)
        T* m = new T[_n];//выделяем память под массив(кол-во столбцов)
        auto jump = lst.begin();//jump - указатель на начало lst
        for (unsigned int i = 0; i < _m; i++, jump++)
        {
            copy(jump->begin(), jump->end(), m);
            for (unsigned int j = 0; j < _n; j++)
            {
                res[i *_n + j] = m[j];
            }
        }
        copy_for_matrix = res;

    }
    //деструктор
    ~Matrix(){
        for(int i = 0; i < _n; i++){
            if(Mx[i] != nullptr)
                delete[] Mx[i];
        }
    }
    //перегрузка оператора присваивания для двух матриц типа T
    Matrix<T>& operator =(Matrix<T>& Mx){
        if(&Mx != this){
            _m = Mx._m;
            _n = Mx._n;
            if(size != Mx.size){
                delete[] copy_for_matrix;
                size = Mx.size;
                copy_for_matrix = new T[size];
                for (unsigned int i = 0; i < Mx.size; i++)
                {
                    copy_for_matrix[i] = Mx.copy_for_matrix[i];//количество столбцов
                }
            }
            for(unsigned int i = 0; i < size; i++){
                for(unsigned int j = 0; j < size(copy_for_matrix); j++){
                    copy_for_matrix[i][j] = Mx.copy_for_matrix[i][j];
                }
            }
        }
        return *this;//разрешает множественное присваивание, например x = t = e
    }

    //перегрузка оператора += для двух матриц типа T
    Matrix<T>& operator +=(Matrix<T>& Mx){
        if(size != Mx.size){
            cout << "Массивы разного размера!" << endl;
            return 0;
        }
        for(int i = 0; i < size; i++){
            for(unsigned int j = 0; j < size(copy_for_matrix); j++){
                copy_for_matrix[i][j] = copy_for_matrix[i][j] + Mx.copy_for_matrix[i][j];
            }
        }
        return *this; //вернуть сумму
    }

    //перегрузка оператора -= для двух матриц типа T
    Matrix<T>& operator -=(Matrix<T>& Mx){
        if(size != Mx.size){
            cout << "Массивы разного размера!" << endl;
            return 0;
        }
        for(int i = 0; i < size; i++){
            for(unsigned int j = 0; j < size(copy_for_matrix); j++){
                copy_for_matrix[i][j] = copy_for_matrix[i][j] - Mx.copy_for_matrix[i][j];
            }
        }
        return *this; //вернуть разность
    }

    //перегрузка оператора - для вычисления разницы матрицы с числом
    template<typename _T>
    friend Matrix<_T> operator -(Matrix<_T> &Mx, double num){
        Matrix<_T> Total;//можно ли эти 3 строчки вынести в отдельную функцию?
        Total._m, Total._n = Mx._m, Mx._n;
        Total.copy_for_matrix = new T[Mx.size];
        for(unsigned int i = 0; i < Mx._m; i++){
            for(unsigned int j = 0; j < Mx._n; j++){
                Total.copy_for_matrix[i][j] = Mx.copy_for_matrix[i][j] - num;
            }
        }
        return Total;
    }

    //перегрузка оператора + для сложения матрицы с числом
    template<typename _T>
    friend Matrix<_T> operator +(Matrix<_T> &Mx, double num){
        Matrix<_T> Total;
        Total._m, Total._n = Mx._m, Mx._n;
        Total.copy_for_matrix = new T[Mx.size];
        for(unsigned int i = 0; i < Mx._m; i++){
            for(unsigned int j = 0; j < Mx._n; j++){
                Total.copy_for_matrix[i][j] = Mx.copy_for_matrix[i][j] + num;
            }
        }
        return Total;
    }


    //перегрузка оператора / для вычесления частного матрицы с числом
    template<typename _T>
    friend Matrix<_T> operator/(Matrix<_T> &Mx, double num){
        Matrix<T> Total;
        Total._m, Total._n = Mx.size, Mx._n;
        Total.copy_for_matrix = new T[Mx.size];
        for(unsigned int i = 0; i < Mx._m; i++){
            for(unsigned int j = 0; j < Mx._n; j++){
                Total.copy_for_matrix[i][j] = Mx.copy_for_matrix[i][j] / num;
            }
        }
        return Total;
    }

    //перегрузка оператора * для умножения матрицы с числом
    template<typename _T>
    friend Matrix<_T> operator*(Matrix<_T> &Mx, double num){
        Matrix<T> Total;
        Total._m, Total._n = Mx._m, Mx._n;
        Total.copy_for_matrix = new T[Mx.size];
        for(unsigned int i = 0; i < Mx._m; i++){
            for(unsigned int j = 0; j < Mx._n; j++){
                Total.copy_for_matrix[i][j] = Mx.copy_for_matrix[i][j] * num;
            }
        }
        return Total;
    }

    //перегрузка оператора + для двух матриц
    template<typename _T>
    friend Matrix<_T> operator +(Matrix <_T>& Mx, Matrix<_T>& Mx2){
        Matrix<_T> Total;
        if(Mx.size == Mx2.size){
            Total._m, Total._n = Mx._m, Mx._n;
            Total.copy_for_matrix = new T[Mx.size];
            for(unsigned int i = 0; i < Mx.size; i++){
                for(unsigned j = 0; j < Mx.size; j++){
                    Total.copy_for_matrix[i][j] = Mx.copy_for_matrix[i][j] + Mx2.copy_for_matrix[i][j];
                }
            }
            return Total;
        }
        cout << "Разный размер матриц" << endl;
        return 0;
    }

    //перегрузка оператора - для двух матриц
    template<typename _T>
    friend Matrix<_T> operator -(Matrix <_T>& Mx, Matrix<_T>& Mx2){
        Matrix<_T> Total;
        if(Mx.size == Mx2.size){
            Total._m, Total._n = Mx._m, Mx._n;
            Total.copy_for_matrix = new T[Mx.size];
            for(unsigned int i = 0; i < Mx.size; i++){
                for(unsigned j = 0; j < Mx.size; j++){
                    Total.copy_for_matrix[i][j] = Mx.copy_for_matrix[i][j] + Mx2.copy_for_matrix[i][j];
                }
            }
            return Total;
        }
        cout << "Разный размер матриц" << endl;
        return 0;
    }

    //перегрузка оператора * для двух матриц
    template<typename _T>
    friend Matrix<_T> operator *(Matrix<_T>& Mx, Matrix<_T>& Mx2){
        if(Mx.size == Mx2.size){
            Matrix<T> Total;
            Total._m, Total._n = Mx._m, Mx._n;
            Total.copy_for_matrix = new T[Mx.size];
            for(unsigned int i = 0; i < Mx._m; i++){
                for(unsigned int j = 0; j < Mx._n; j++){
                    Total.copy_for_matrix[i][j] = 0;
                    for(unsigned int k = 0; k < Mx._m; k++){
                        Total.copy_for_matrix[i][j] += Mx.copy_for_matrix[i][k] * Mx2.copy_for_matrix[k][j];
                    }
                }
            }
            return Total;
        }
        cout << "Разный размер матриц" << endl;
        return 0;
    }

    //перегрузка оператора << для вывода класса в поток (cout к примеру)
    template<typename _T>
    friend ostream& operator <<(ostream& os, Matrix<_T>& Mx){
        for(unsigned int i = 0; i < Mx.size; i++){
            if(i % Mx._n == 0){
                os << endl;
            }
            else
                os << Mx.copy_for_matrix[i];
        }
        return os;
    }

    //Метод получения итератора на начало матрицы(первый элемент)
    template<typename _T>
    Iterator<T> iterator_begin(){

    }

    //метод проверки матрицы на квадратную
    bool is_square(){
        if(_n == _m)
            return true;
        return false;
    }
    int is_Empty(){
        if(_n < 1 && _m < 1){
            cout << "Не верно введены размеры матрицы" << endl;
            return 0;
        }
    }
    //метод получения числа столбцов матрицы
    unsigned int get_n(){
       return _n;
    }

    //метод получения числа строк матрицы
    unsigned int get_m(){
        return _m;
    }

    //метод получения элемента матрицы по индексу
    T get_elem(unsigned int i, unsigned int j){
        return Mx[i][j];
    }

    //метод изменения элемента матрицы по индексу
    void set_elem(unsigned int i, unsigned int j, T elem){//устанавливает элемент
       Mx[i][j] = elem;
    }
private:
    T** copy_for_matrix;//копия для матрицы
    T** Mx;//матрица
    int _n = 0;
    int _m = 0;
    int size = _n * _m;
};

#endif // MATRIX_H

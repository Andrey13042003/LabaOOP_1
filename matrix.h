#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <ostream>
#include <iterator>

using namespace std;

template<typename T>
class Matrix{
public:
    Matrix(){
        _n = 0;
        _m = 0;
        Mx = nullptr;
    }

    //выделяем память под каждый элемент матрицы
    Matrix(size_t n, size_t m){
        _n = n;
        _m = m;
        Mx = new T[n * m];
        copy_for_matrix = new T[n * m];
    }

    //конструктор копирования
    Matrix(const Matrix<T>& Mx)
    {
        _m = Mx.m;
        _n = Mx.n;
        copy_for_matrix = new T[_size];
        for (size_t i = 0; i < _size; i++)
        {
            for (size_t j = 0; j < _size; j++){
                int count = 0;
                copy_for_matrix[count] = Mx[i * _m + j];
                count += 1;
            }
        }
    }

    //конструктор перемещения
    Matrix(const Matrix<T>&& Mx)
    {
        _m = Mx._m;
        _n = Mx._n;
        _size = _n * _m;
        copy_for_matrix = new T[_size];
        for (int i = 0; i < _size; i++)
        {
            copy_for_matrix[i] = Mx.copy_for_matrix[i];
        }
        delete[] copy_for_matrix;
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
        T* res = new T[_n * _m + 1];//выделяем память под массив(как под матрицу)
        T* m = new T[_n];//выделяем память под массив(кол-во столбцов)
        auto jump = lst.begin();//jump - указатель на начало lst
        for (int i = 0; i < _m; i++, jump++)
        {
            copy(jump->begin(), jump->end(), m);
            for (int j = 0; j < _n; j++)
            {
                res[i *_n + j] = m[j];
            }
        }
        copy_for_matrix = res;
    }

    //деструктор
    ~Matrix(){
        delete Mx;
    }

    //перегрузка оператора присваивания для двух матриц типа T
    Matrix<T>& operator =(Matrix<T>& Mx){
        if(&Mx != this){
            _m = Mx._m;
            _n = Mx._n;
            _size = _n * _m;
            if(_size != Mx._n * Mx._m){
                delete[] copy_for_matrix;
                _size = Mx._n * Mx._m;
                copy_for_matrix = new T[_size];
                for (size_t i = 0; i < Mx._n * Mx._m; i++)
                {
                    copy_for_matrix[i] = Mx.copy_for_matrix[i];
                }
            }
            for(size_t i = 0; i < _size; i++){
                for(size_t j = 0; j < size(copy_for_matrix); j++){
                    copy_for_matrix[i * _m + j] = Mx.copy_for_matrix[i * _m + j];
                }
            }
        }
        return *this;//разрешает множественное присваивание, например x = t = e
    }

    //перегрузка оператора += для двух матриц типа T
    Matrix<T>& operator +=(Matrix<T>& Mx){
        _size = _n * _m;
        for(int i = 0; i < _size; i++){
            for(size_t j = 0; j < size(copy_for_matrix); j++){
                copy_for_matrix[i * _m + j] += Mx.copy_for_matrix[i * _m + j];
            }
        }
        return *this; //вернуть сумму
    }

    //перегрузка оператора -= для двух матриц типа T
    Matrix<T>& operator -=(Matrix<T>& Mx){
        _size = _n * _m;
        for(int i = 0; i < _size; i++){
            for(size_t j = 0; j < size(copy_for_matrix); j++){
                copy_for_matrix[i * _m + j] -=  Mx.copy_for_matrix[i * _m + j];
            }
        }
        return *this; //вернуть разность
    }

    //перегрузка оператора - для вычисления разницы матрицы с числом
    template<typename _T>
    friend Matrix<_T> operator -(Matrix<_T> &Mx, double num){
        Matrix<_T> Total;
        Total._m = Mx._m;
        Total._n = Mx._n;
        Total.copy_for_matrix = new T[Total._n * Total._m];
        for(size_t i = 0; i < Mx._m; i++){
            for(size_t j = 0; j < Mx._n; j++){
                Total.copy_for_matrix[i * Mx._m + j] = Mx.copy_for_matrix[i * Mx._m + j] - num;
            }
        }
        return Total;
    }

    //перегрузка оператора + для сложения матрицы с числом
    template<typename _T>
    friend Matrix<_T> operator +(Matrix<_T> &Mx, double num){
        Matrix<_T> Total;
        Total._m = Mx._m;
        Total._n = Mx._n;
        Total.copy_for_matrix = new T[Total._m * Total._n];
        for(size_t i = 0; i < Mx._m; i++){
            for(size_t j = 0; j < Mx._n; j++){
                Total.copy_for_matrix[i * Mx._m + j] = Mx.copy_for_matrix[i * Mx._m + j] + num;
            }
        }
        return Total;
    }

    //перегрузка оператора / для вычесления частного матрицы с числом
    template<typename _T>
    friend Matrix<_T> operator/(Matrix<_T> &Mx, double num){
        Matrix<T> Total;
        Total._m  = Mx._m;
        Total._n = Mx._n;
        Total.copy_for_matrix = new T[Total._m * Total._n];
        for(size_t i = 0; i < Mx._m; i++){
            for(size_t j = 0; j < Mx._n; j++){
                Total.copy_for_matrix[i * Mx._m + j] = Mx.copy_for_matrix[i * Mx._m + j] / num;
            }
        }
        return Total;
    }

    //перегрузка оператора * для умножения матрицы с числом
    template<typename _T>
    friend Matrix<_T> operator*(Matrix<_T> &Mx, double num){
        Matrix<T> Total;
        Total._m = Mx._m;
        Total._n = Mx._n;
        Total.copy_for_matrix = new T[Total._m * Total._n];
        for(size_t i = 0; i < Mx._m; i++){
            for(size_t j = 0; j < Mx._n; j++){
                Total.copy_for_matrix[i * Mx._m + j] = Mx.copy_for_matrix[i * Mx._m + j] * num;
            }
        }
        return Total;
    }

    //перегрузка оператора + для двух матриц
    template<typename _T>
    friend Matrix<_T> operator +(Matrix <_T>& Mx, Matrix<_T>& Mx2){
        Matrix<_T> Total;
        Total._m = Mx._m;
        Total._n = Mx._n;
        Total.copy_for_matrix = new T[Total._m * Total._n];
        for(int i = 0; i < Mx._n * Mx._m; i++){
            for(int j = 0; j < Mx._n * Mx._m; j++){
                Total.copy_for_matrix[i * Mx._m + j] = Mx.copy_for_matrix[i * Mx._m + j] + Mx2.copy_for_matrix[i * Mx._m + j];
            }
        }
        return Total;
    }

    //перегрузка оператора - для двух матриц
    template<typename _T>
    friend Matrix<_T> operator -(Matrix <_T>& Mx, Matrix<_T>& Mx2){
        Matrix<_T> Total;
        Total._m, Total._n = Mx._m, Mx._n;
        Total.copy_for_matrix = new T[Total._m * Total._n];
        for(size_t i = 0; i < Mx._n * Mx._m; i++){
            for(unsigned j = 0; j < Mx._n * Mx._m; j++){
                Total.copy_for_matrix[i * Mx._m + j] = Mx.copy_for_matrix[i * Mx._m + j] + Mx2.copy_for_matrix[i * Mx._m + j];
            }
        }
        return Total;
    }

    //перегрузка оператора * для двух матриц
    template<typename _T>
    friend Matrix<_T> operator *(Matrix<_T>& Mx, Matrix<_T>& Mx2){
            Matrix<T> Total;
            Total._m = Mx._m;
            Total._n = Mx._n;
            Total.copy_for_matrix = new T[Total._m * Total._n];
            for(size_t i = 0; i < Mx._m; i++){
                for(size_t j = 0; j < Mx._n; j++){
                    Total.copy_for_matrix[i * Mx._m + j ] = 0;
                    for(size_t k = 0; k < Mx._m; k++){
                        Total.copy_for_matrix[i * Mx._m + j] += Mx.copy_for_matrix[i * Mx._m + k] * Mx2.copy_for_matrix[k * Mx._m + j];
                    }
                }
            }
        return Total;
    }

    //перегрузка оператора << для вывода класса в поток (cout к примеру)
    template<typename _T>
    friend ostream& operator <<(ostream& os, Matrix<_T>& Mx){
        for(size_t i = 0; i < Mx._n * Mx._m; i++){
            if(i % Mx._n == 0){
                os << endl;
            }
            else
                os << Mx.copy_for_matrix[i];
        }
        return os;
    }

    //Метод получения итератора на начало матрицы(первый элемент)
    T* iterator_begin(){
        return &Mx[0];
    }

    T* iterator_end(){
        return &Mx[_n * _m - 1];
    }

    const T* iterator_begin()const{
        return &Mx[0];
    }

    const T* iterator_end()const{
        return &Mx[_n * _m - 1];
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
    size_t get_n(){
       return _n;
    }

    //метод получения числа строк матрицы
    size_t get_m(){
        return _m;
    }

    //метод получения элемента матрицы по индексу
    T get_elem(size_t i, size_t j){
        return Mx[i * j];
    }

    //метод изменения элемента матрицы по индексу
    void set_elem(size_t i, size_t j, T elem){
       Mx[i * j] = elem;
    }
private:
    T* copy_for_matrix;//копия для матрицы
    T* Mx;//матрица
    int _n = 0;
    int _m = 0;
    int _size = _n * _m;
};

#endif // MATRIX_H

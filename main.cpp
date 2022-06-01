#include <QCoreApplication>
#include <iostream>
#include <string>
#include <matrix.h>
#include <ostream>
#include <test.h>

using namespace std;

template<typename T>
bool Test_sum(Matrix<T> A, Matrix<T> B, Matrix<T> C){
    return A + B == C;
}

void Save_tests(){
    Assert(Test_sum({{1, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}, {{13, 14, 15}, {16, 17, 18}}), "mistake");
    Assert(!Test_sum({{1, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}, {{13, 14, 15}, {16, 17, 18}}), "mistake");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Matrix<int> satisfy_for_matrix(2, 2);//задаем количество строк и столбцов матрицы
    satisfy_for_matrix.set_elem(0, 0, 1);//каждому элементу задаем свое значение
    satisfy_for_matrix.set_elem(0, 1, 2);
    satisfy_for_matrix.set_elem(1, 0, 3);
    satisfy_for_matrix.set_elem(1, 1, 4);
    cout << satisfy_for_matrix.get_elem(0, 0);
    TestRunner obj;
    obj.RunTest(Save_tests, "the first set");
    Matrix<int> lhoys({{1, 2, 3}, {4, 5, 6}});
    Matrix<int> rhoys({{1, 2, 3}, {4, 5, 6}});
    Matrix<int> result = lhoys + rhoys;

    return a.exec();
}

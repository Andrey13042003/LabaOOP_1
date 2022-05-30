#include <QCoreApplication>
#include <iostream>
#include <string>
#include <matrix.h>
#include <ostream>

using namespace std;

template <class T>

//оператор записи в поток
ostream& operator << (ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
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

    return a.exec();
}

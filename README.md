# matrix_library

В данном проекте реализована библиотека на языке Си для работы с матрицами. Библиотека предоставляет следующие функции:

- `create_matrix` — создание матрицы  
- `remove_matrix` — очистка и уничтожение матрицы  
- `eq_matrix` — сравнение матриц  
- `sum_matrix` — сложение матриц  
- `sub_matrix` — вычитание матриц  
- `mult_matrix` — умножение матриц  
- `mult_number` — умножение матрицы на число  
- `transpose` — транспонирование матрицы  
- `determinant` — вычисление определителя  
- `calc_complements` — вычисление матрицы алгебраических дополнений  
- `inverse_matrix` — поиск обратной матрицы  

Решение оформляется в виде статической библиотеки `s21_matrix.a`.

Тесты написаны с использованием библиотеки `check.h`.

Для сборки библиотеки и тестов используется Makefile со следующими целями:  
`all`, `clean`, `test`, `s21_matrix.a`, `gcov_report`.

Цель `gcov_report` генерирует отчёт о проценте покрытия unit-тестами функций библиотеки.  
После генерации отчёта HTML-файл можно открыть следующим образом:

- **Linux / Ubuntu**:  
  ```bash
  xdg-open gcov_report/report.html

- **macOS**:
  ```bash
  open gcov_report/report.html

- **Windows**:
  ```cmd
  start gcov_report/report.html

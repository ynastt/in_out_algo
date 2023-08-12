# Тема работы ?
О схеме Горнера в контексте структур реализации интерпретатора функционального языка программирования Рефал-5/ Фокусы с арифметикой натуральных чисел  

экспериментальная работа по развитии и оптимизации арифметики в системе Рефал-5.  

# цель
реализация стандартного и нвоого алгоритмов перевода из 10й системы в 2^64-и ричную и их сравнение по оптимальности (скорости).  

# задачи
1. Реализация нового алгоритма перевода из 10-й системы в 2^64-и ричную без деления.    
  1.1. Реализация перевода числа N в восьмеричную систему счисления, используя алгоритм без делений.  
  1.2. Реализация перевода числа из восьмеричной системы счисления в систему счисления по основанию 2^64, где результат должен быть представлен в структурах, близких к реализации Рефала-5.  
  1.3. Проведение большой серии с длинными входными числами (100-1000 десятичных знаков) и фиксирование среднего (по всем экспериментам) времени перевода из 10-ричной системы в систему счисления по основанию 2^64.  
2. Реализация стандартного алгоритма перевода из 10-й системы в 2^64-и ричную.  
  2.1. Реализация перевода числа N в восьмеричную систему счисления, используя стандартный алгоритм взятия остатков от деления на основание системы счисления.  
  2.2. Реализация перевода числа из восьмеричной системы счисления в систему счисления по основанию 2^64, где результат должен быть представлен в структурах, близких к реализации Рефала-5.  
  2.3. Проведение большой серии с длинными входными числами (100-1000 десятичных знаков) и фиксирование среднего (по всем экспериментам) времени перевода из 10-ричной системы в систему счисления по основанию 2^64.    
3. Систематизация результатов тестирования в сравнительные таблицы времен двух разных алгоритмов перевода из 10-й системы в систему счисления по основанию 2^64.  
4. Реализация нового алгоритма перевода из системы счисления по основанию 2^64 в 10-ю без деления.    
  4.1. Реализация перевода числа N в восьмеричную систему счисления, используя алгоритм без делений.  
  4.2. Реализация перевода числа из восьмеричной системы счисления в 10-ю систему счисления.  
  4.3. Проведение большой серии с длинными входными числами (100-1000 десятичных знаков) и фиксирование среднего (по всем экспериментам) времени перевода из системы счисления по основанию 2^64 в 10-й систему счисления.  
5. Реализация стандартного алгоритма перевода из системы счисления по основанию 2^64 в 10-ю.  
  5.1. Реализация перевода числа N в восьмеричную систему счисления, используя стандартный алгоритм взятия остатков от деления на основание системы счисления.  
  5.2. Реализация перевода числа из восьмеричной системы счисления в 10-ю систему счисления.   
  5.3. Проведение большой серии экспериментов и фиксирование среднего (по всем экспериментам) времени перевода из системы счисления по основанию 2^64 в 10-ю систему счисления.  
6. Систематизация результатов тестирования в сравнительные таблицы времен двух разных алгоритмов перевода из системы счисления по основанию 2^64 в 10-ю.  

## Ссылка на новый алгоритм
Ссылка на алгоритмы перевода из 8-ричной системы в 10-ричную и обратно.  
https://www.mccme.ru/free-books/mmmf-lectures/book.29v2.pdf  
См. стр. 47, параграф "СХЕМА ГОРНЕРА И ПЕРЕВОД ИЗ ОДНОЙ ПОЗИЦИОННОЙ СИСТЕМЫ В ДРУГУЮ"  


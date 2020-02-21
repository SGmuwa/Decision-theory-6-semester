using System;
using System.Linq;
using System.Collections.Generic;

public static class PatternSearch
{
    public static void Main(string[] args)
    {
        int n = 2; // Размерность
        int d = 2; // Коэффициент уменьшения шага
        double E = 0.1; // Точность поиска
        double m = 0.5; // Ускоряющий множитель
        double h = 0.2; // Шаг

        double[] base_point = new double[n]; // Координаты центральной точки
        double[] xP = new double[n]; // Координаты точки для поиска по образцу
        double[] current_point = new double[n];
        double[] test_point = new double[n]; // Координаты тестовых точек временно хранятся в этом массиве

        for (int i = 0; i < n; i++)
            base_point[i] = 0;

        // Считаем тестовую, если она больше текущей, то производится поиск по следующим направлениям и координатам
        do
        {
            Console.WriteLine("Шаг: " + h.ToString("f3"));
            for (int i = 0; i < n; i++)
                test_point[i] = current_point[i] = base_point[i];
            Console.Write($"Базисная\t[0]+h\t[0]-h\t[1]+h\t[1]-h\nf{base_point.PointToString()} = {TargetFunction(base_point).ToString("f3")}");
            for (int i = 0; i < n; i++)
            {
                test_point[i] = base_point[i] + h;
                Console.Write($"\tf{test_point.PointToString()} = {TargetFunction(test_point).ToString("f3")}");
                if (TargetFunction(test_point) < TargetFunction(current_point))
                    current_point[i] = test_point[i];
                test_point[i] = base_point[i] - h;
                Console.Write($"\tf{test_point.PointToString()} = {TargetFunction(test_point).ToString("f3")}");
                if (current_point[i] != base_point[i] && TargetFunction(test_point) < TargetFunction(current_point))
                    current_point[i] = test_point[i];
            }
            Console.WriteLine($"\nМинимальная точка: f{current_point.PointToString()} = {TargetFunction(current_point).ToString("f3")}"); // debug
            // Сравнение с базисной точкой x0
            if (base_point.SequenceEqual(current_point))
            { // Если х1 = х0, то уменьшаем шаг
                Console.WriteLine($"Уменьшение шага: {h.ToString("f3")} / {d.ToString("f3")} = {(h / d).ToString("f3")}");
                h /= d;
            }
            else
            { // Если х1!=х0, то поиск по образцу
                for (int j = 0; j < n; j++)
                    xP[j] = current_point[j] + m * (current_point[j] - base_point[j]);
                Console.WriteLine($"Поиск по образцу: f({current_point.PointToString()} + {m.ToString("f3")} * ({current_point.PointToString()} - {base_point.PointToString()})) = f{xP.PointToString()} = {TargetFunction(xP).ToString("f3")}");
                if (TargetFunction(xP) < TargetFunction(current_point))
                    for (int j = 0; j < n; j++)
                        base_point[j] = xP[j];
                else
                    for (int j = 0; j < n; j++)
                        base_point[j] = current_point[j];
                Console.WriteLine($"Новая базисная точка: f{base_point.PointToString()} = {TargetFunction(base_point).ToString("f3")}");
            }
            Console.WriteLine();
        } while (h >= E);
        Console.WriteLine($"Шаг < E ({h.ToString("f3")} < {E.ToString("f3")})");
        Console.WriteLine($"Минимальная точка: f{base_point.PointToString()} = {TargetFunction(base_point).ToString("f3")}");
    }

    internal static IEnumerable<O> EveryConverter<T, O>(this IEnumerable<T> that, Func<T, O> converter)
    {
        foreach (var e in that)
            yield return converter(e);
    }

    public static string PointToString(this double[] that)
        => $"({string.Join("; ", that.EveryConverter(e => e.ToString("f3")))})";

    public static double TargetFunction(double[] mas)
        => -3.3 * mas[0] +
        5.2 * Math.Pow(mas[0], 2) -
        4.2 * mas[1] +
        2.8 * Math.Pow(mas[1], 2);
}

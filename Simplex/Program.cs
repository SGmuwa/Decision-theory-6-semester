using System;
using System.Linq;
using System.Text;
using System.Collections.Generic;

public static class Simplex
{
    public static void Main(string[] args)
    {
        int iteration = 0; // Счётчик итераций
        int n = 2;
        double E = 0.75;
        double m = 0.25;
        double[] start = { 1, 1 };
        double[,] tableSimplex = new double[n + 1, n + 1];
        for (int i = 0; i < n + 1; i++)
            for (int j = 0; j < n; j++)
                if (i == 0)
                    tableSimplex[i, j] = start[j];
                else
                    tableSimplex[i, j] = -0;
        tableSimplex[0, n] = TargetFunction(start);

        double increment1 =
            (double)(Math.Sqrt(n + 1) - 1) / (n * Math.Sqrt(2)) * m;
        double increment2 =
            (double)(Math.Sqrt(n + 1) + n - 1) / (n * Math.Sqrt(2)) * m;

        //Вычисление координат остальных вершин симплекса
        for (int tempN = 0; tempN < n; tempN++)
        {
            double[] x = new double[n];
            for (int i = 0; i < n; i++)
                if (tempN == i)
                    x[i] = increment1 + start[i];
                else
                    x[i] = increment2 + start[i];
            for (int i = 0; i < n + 1; i++)
                for (int j = 0; j < n; j++)
                    if (i == tempN + 1) tableSimplex[i, j] = x[j];
            tableSimplex[tempN + 1, n] = TargetFunction(x);
        }
        Console.WriteLine(tableSimplex.TableToString("f3"));
        while (true)
        {
            Console.WriteLine("Итерация " + iteration++);
            double[] arrayFuncValue = new double[n + 1];
            double[] centerOfGravityXc = new double[n]; // Координаты центра тяжести
            double[] reflectedVertex = new double[n]; // Координаты отраженной вершины
            for (int i = 0; i < arrayFuncValue.Length; i++)
                arrayFuncValue[i] = tableSimplex[i, n];
            int maxVertex = Array.IndexOf(arrayFuncValue, arrayFuncValue.Max());
            Console.WriteLine($"Максимум: [{maxVertex}] = {arrayFuncValue[maxVertex].ToString("f3")}");
            for (int t = 0; t < n; t++)
                for (int i = 0; i < n + 1; i++)
                    if (i == maxVertex)
                        start[t] = tableSimplex[i, t];
                    else
                    {
                        centerOfGravityXc[t] +=
                            tableSimplex[i, t] / (tableSimplex.GetLength(0) - 1);
                    }
            Console.WriteLine("Центр аргументов кроме максимума: " + string.Join("; ", centerOfGravityXc.EveryConverter(e => e.ToString("f3"))));
            // Координаты отраженной вершины
            Console.Write("Отражённая вершина = f(");
            for (int i = 0; i < n; i++)
            {
                reflectedVertex[i] = 2 * centerOfGravityXc[i] - start[i];
                Console.Write($"2 * {centerOfGravityXc[i].ToString("f3")} - {start[i].ToString("f3")};");
            }
            Console.WriteLine($") = f({string.Join("; ", reflectedVertex.EveryConverter(e => e.ToString("f3")))}) = {TargetFunction(reflectedVertex).ToString("f3")}");
            if (TargetFunction(reflectedVertex) < TargetFunction(start))
            {
                for (int i = 0; i < reflectedVertex.Length; i++)
                    tableSimplex[maxVertex, i] = reflectedVertex[i];
                tableSimplex[maxVertex, n] = TargetFunction(reflectedVertex);
            }
            else
            {
                for (int i = 0; i < tableSimplex.GetLength(0); i++)
                    arrayFuncValue[i] = tableSimplex[i, n];
                int minVertex = Array.IndexOf(arrayFuncValue, arrayFuncValue.Min());
                Console.WriteLine($"Редукция: [{minVertex}] = {arrayFuncValue[minVertex]}");
                for (int i = 0; i < n + 1; i++)
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (i != minVertex)
                        {
                            if (t != n)
                            {
                                start[t] = tableSimplex[i, t] =
                                    tableSimplex[minVertex, t] +
                                    ((double)1 / 2) *
                                    (tableSimplex[i, t] - tableSimplex[minVertex, t]);
                            }
                            else // if (t == n)
                                tableSimplex[i, t] = TargetFunction(start);
                        }
                        for (int k = 0; k < start.Length; k++)
                            start[k] = 0;
                    }
            }
            for (int i = 0; i < centerOfGravityXc.Length; i++)
                centerOfGravityXc[i] = 0;
            for (int i = 0; i < tableSimplex.GetLength(0); i++)
                for (int t = 0; t < centerOfGravityXc.Length; t++)
                    centerOfGravityXc[t] += tableSimplex[i, t] / tableSimplex.GetLength(0);
            Console.WriteLine(tableSimplex.TableToString("f3"));
            double centerY = TargetFunction(centerOfGravityXc);
            Console.WriteLine($"Центр тяжести симплекса: f({string.Join(", ", centerOfGravityXc.EveryConverter(e => e.ToString("f3")))}) = {centerY.ToString("f3")}");

            // Проверка условий окончания поиска
            int checkEnd = 0;
            for (int i = 0; i < tableSimplex.GetLength(0); i++)
                if (Math.Abs(tableSimplex[i, n] - centerY) < E)
                    checkEnd++;
                else break;
            if (checkEnd == tableSimplex.GetLength(0))
            {
                for (int i = 0; i < tableSimplex.GetLength(0); i++)
                    arrayFuncValue[i] = tableSimplex[i, n];
                int minVertex = Array.IndexOf(arrayFuncValue, arrayFuncValue.Min());
                double resultMin = tableSimplex[minVertex, n];
                Console.WriteLine("Минимум: " + resultMin.ToString("f3"));
                return;
            }
        }
    }

    public static double TargetFunction(double[] mas)
        => -3.3 * mas[0] +
        5.2 * Math.Pow(mas[0], 2) -
        4.2 * mas[1] +
        2.8 * Math.Pow(mas[1], 2);

    /// <summary>
    /// Преобразует объект в строку заданного формата, вставляя дополнительные пробелы,
    /// чтобы подогнать под размер <code>len</code>.
    /// </summary>
    /// <param name="toInsert">Объект, который надо преобразовать в строку.</param>
    /// <param name="len">Минимальная длинна выходной строки.</param>
    /// <param name="format">Формат вывода.</param>
    /// <returns>Строка, представляющая объект <code>toInstert</code>.</returns>
    internal static string ToString(this object toInsert, int len, string format)
                => string.Format(string.Format("{{0, {0} :{1}}}", len, format), toInsert);
    
    internal static IEnumerable<O> EveryConverter<T, O>(this IEnumerable<T> that, Func<T, O> converter)
    {
        foreach(var e in that)
            yield return converter(e);
    }

    /// <summary>
    /// Превращает двухмерную таблицу в строку.
    /// </summary>
    /// <param name="input">Таблица.</param>
    /// <param name="format">Формат вывода каждого элемента.</param>
    /// <param name="renderForeach">Функция преобразования </param>
    /// <returns>Строковое представление каждого элемента массива в виде таблицы.</returns>
    internal static string TableToString(this Array input, string format = null, Func<dynamic, object> renderForeach = null)
    {
        if (input.Rank != 2)
            throw new NotSupportedException();
        Array array;
        if (renderForeach == null)
            array = input;
        else
        {
            array = new object[input.GetLength(0), input.GetLength(1)];
            for (int y = 0; y < array.GetLength(0); y++)
                for (int x = 0; x < array.GetLength(1); x++)
                    array.SetValue(renderForeach(input.GetValue(y, x)), y, x);
        }
        int max = -1;
        foreach (var a in array)
            if (a.ToString(0, format).Length > max)
                max = a.ToString(0, format).Length;
        max++;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < array.GetLength(0); i++)
        {
            for (int j = 0; j < array.GetLength(1); j++)
            {
                sb.Append(array.GetValue(i, j).ToString(max, format));
            }
            sb.Append('\n');
        }
        return sb.ToString();
    }
}

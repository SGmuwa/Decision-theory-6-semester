using System;
using System.Text;
using System.Collections.Generic;

public static class NelderMead
{
    public static void Main(string[] args)
    {
        Task1(out int n, out double E, out double m, out double B, out double Y, out double[] mas, out double[,] tableSimplex);
        Task2(tableSimplex, n, mas, m);
        bool EBad;
        do
        {
            ((int I, double Value) max, (int I, double Value) min, (int I, double Value) followMax) = Task3(tableSimplex, n, mas, B, Y, E);
            int currentId = max.I;
            double[] centerOfGravityXc = Task4(tableSimplex, n, max.I, mas);
            (double[] reflectedVertex, double fReflected) = Task5(centerOfGravityXc, tableSimplex, max.I);
            double currentF = fReflected;
            EBad = Task6(ref currentF, fReflected, max.Value, n, tableSimplex, max.I, reflectedVertex, min.Value, mas, currentId, centerOfGravityXc, B, Y, E, followMax.Value);
        } while (!EBad);
    }

    /// <summary>
    /// Задать начальные денные.
    /// </summary>
    public static void Task1(out int n, out double E, out double m, out double B, out double Y, out double[] mas, out double[,] tableSimplex)
    {
        Console.WriteLine("Шаг 1.");
        n = 2;
        E = 0.1;
        m = 1.0; // Длина ребра многогранника.
        B = 2.8; // Коэффициент растяжения.
        Y = 0.4; // Коэффициент сжатия.
        mas = new double[n];
        mas[0] = 0.5;
        mas[1] = 0.5;
        tableSimplex = new double[n + 1, n + 1];
        Console.WriteLine($"n = {n}\n"
            + $"E = {E:f3}\n"
            + $"m = {m:f3}\n"
            + $"B = {B:f3}\n"
            + $"Y = {Y:f3}\n"
            + $"Начальная точка = ({string.Join("; ", mas.EveryConverter(e => e.ToString("f3")))})"
            );
    }

    /// <summary>
    /// Построить начальный многогранник.
    /// </summary>
    public static void Task2(in double[,] tableSimplex, int n, in double[] mas, in double m)
    {
        Console.WriteLine("Шаг 2.");
        if (n != 2)
            throw new NotSupportedException(); // line current+15
        for (int j = 0; j < n; j++)
            tableSimplex[0, j] = mas[j];
        tableSimplex[0, n] = TargetFunction(mas);
        // Приращения.
        double increment1 = (Math.Sqrt(n + 1) - 1) / (n * Math.Sqrt(2)) * m;
        double increment2 = (Math.Sqrt(n + 1) + n - 1) / (n * Math.Sqrt(2)) * m;
        Console.WriteLine($"Инкрименты: ({increment1:f3}; {increment2:f3})");
        // Находим координаты остальных вершин.

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                tableSimplex[i + 1, j] = (i == j ? increment1 : increment2) + mas[j];
                Console.WriteLine($"x[{i + 1}, {j}] = {(i == j ? increment1 : increment2):f3} + {mas[j]:f3} = {tableSimplex[i + 1, j]:f3}");
            }
            tableSimplex[i + 1, n] = TargetFunction(tableSimplex[i + 1, 0], tableSimplex[i + 1, 1]); // !
        }
        // Вывод таблицы.
        Console.WriteLine(tableSimplex.TableToString("f3"));
    }

    /// <summary>
    /// Определить номера k вершин.
    /// </summary>
    public static ((int I, double Value) maxVertex, (int I, double Value) minValObjFunction, (int I, double Value) followMaxIndex) Task3(in double[,] tableSimplex, in int n, in double[] mas, in double B, in double Y, in double E)
    {
        Console.WriteLine("Шаг 3.");
        double[] arrayFuncValue = new double[n + 1];

        for (int i = 0; i < n + 1; i++)
            arrayFuncValue[i] = tableSimplex[i, n];

        (int I, double Value) max, min, followMax;
        min.I = SearchMin(arrayFuncValue);
        min.Value = TargetFunction(tableSimplex[min.I, 0], tableSimplex[min.I, 1]);
        max.I = SearchMax(arrayFuncValue);
        max.Value = TargetFunction(tableSimplex[max.I, 0], tableSimplex[max.I, 1]);
        followMax.I = SearchFollowMax(arrayFuncValue);
        followMax.Value = TargetFunction(tableSimplex[followMax.I, 0], tableSimplex[followMax.I, 1]);
        Console.WriteLine($"Минимальная точка: [{min.I}], Максимальная: [{max.I}], Следующая за максимальной: [{followMax.I}]");

        // Определяем максимальную, минимальную и следующую за максимальной вершиной.
        return (max, min, followMax);
    }

    public static double[] Task4(in double[,] tableSimplex, int n, int maxVertex, double[] mas)
    {
        Console.WriteLine("Шаг 4.");
        double[] centerOfGravityXc = new double[n];
        // Считаем центр тяжести вершин симплекса (кроме максимальной).
        for (int t = 0; t < n; t++)
        {
            Console.Write($"Центр[{t}] = ");
            for (int i = 0; i < n + 1; i++)
            {
                if (i != maxVertex)
                {
                    Console.Write($"{tableSimplex[i, t]:f3} / {n}{(i + 1 < n + 1 && i + 1 != maxVertex || i + 2 < n + 1 ? " + " : "")}");
                    centerOfGravityXc[t] += tableSimplex[i, t] / n;
                }
            }
            Console.WriteLine($" = {centerOfGravityXc[t]:f3}");
        }
        return centerOfGravityXc;
    }

    public static (double[] reflectedVertex, double fReflected) Task5(double[] centerOfGravityXc, double[,] tableSimplex, int maxVertex)
    {
        Console.WriteLine("Шаг 5.");
        double[] reflectedVertex = new double[centerOfGravityXc.Length];
        // Находим координаты отраженной вершины.
        Console.Write("Отражение = f(");
        for (int i = 0; i < centerOfGravityXc.Length; i++)
        {
            reflectedVertex[i] = 2 * centerOfGravityXc[i] - tableSimplex[maxVertex, i];
            Console.Write($"2 * {centerOfGravityXc[i]:f3} - {tableSimplex[maxVertex, i]:f3}{(i + 1 < centerOfGravityXc.Length ? "; " : "")}");
        }
        double fReflected = TargetFunction(reflectedVertex);
        Console.WriteLine($") = f({string.Join("; ", reflectedVertex.EveryConverter(e => e.ToString("f3")))}) = {fReflected.ToString("f3")}");
        return (reflectedVertex, fReflected);
    }

    public static bool Task6(ref double currentF, in double fReflected, in double maxValue, in int n, in double[,] tableSimplex, in int maxVertex, in double[] reflectedVertex, in double minValue, in double[] mas, in int currentId, in double[] centerOfGravityXc, in double B, in double Y, in double E, in double followMaxValue)
    {
        Console.WriteLine("Шаг 6.");
        if (fReflected < maxValue)
        {
            Console.WriteLine($"Отражение меньше максимума. ({fReflected:f3} < {maxValue:f3})");
            // Заменяем вершину в таблице.
            for (int j = 0; j < n; j++)
                tableSimplex[maxVertex, j] = reflectedVertex[j];
            tableSimplex[maxVertex, n] = fReflected;
            Console.WriteLine($"После замены вершины [{maxVertex}] на отражённую:\n{tableSimplex.TableToString("f3")}");
            return Task7(ref currentF, minValue, n, currentId, mas, tableSimplex, centerOfGravityXc, maxValue, followMaxValue, B, E, Y);
        }
        else
        {
            Console.WriteLine($"Отражение больше или равно максимума. ({fReflected:f3} ≥ {maxValue:f3})");
            return Task9(tableSimplex, Y, E, currentF, currentId, maxValue, followMaxValue, n, centerOfGravityXc);
        }
    }

    public static bool Task7(ref double currentF, in double minValue, in int n, in int currentId, in double[] mas, in double[,] tableSimplex, in double[] centerOfGravityXc, in double maxValue, in double followMaxValue, in double B, in double E, in double Y)
    {
        Console.WriteLine("Шаг 7.");
        if (currentF < minValue)
        {
            Console.WriteLine($"Текущая точка меньше минимума ({currentF:f3} < {minValue:f3}).");
            // Операция растяжения.
            double[] newVertexsStretch = new double[n];
            Console.Write("Растяжение = f(");
            for (int r = 0; r < n; r++)
            {
                newVertexsStretch[r] = centerOfGravityXc[r] + B * (tableSimplex[currentId, r] - centerOfGravityXc[r]);
                Console.Write($"{centerOfGravityXc[r]:f3} + {B:f3} * ({tableSimplex[currentId, r]:f3} - {centerOfGravityXc[r]:f3}{(r + 1 < n ? "; " : "")}");
            }
            double newVertexsStretchValue = TargetFunction(newVertexsStretch);
            Console.WriteLine($") = f({string.Join("; ", newVertexsStretch.EveryConverter(e => e.ToString("f3")))}) = {newVertexsStretchValue:f3}");
            return Task8(newVertexsStretch, newVertexsStretchValue, ref currentF, tableSimplex, currentId, n, E, Y, maxValue, followMaxValue, centerOfGravityXc);
        }
        else
        {
            Console.WriteLine($"Текущая точка больше или равна минимому ({currentF:f3} ≥ {minValue:f3}).");
            return Task9(tableSimplex, Y, E, currentF, currentId, maxValue, followMaxValue, n, centerOfGravityXc);
        }
    }

    public static bool Task8(in double[] newVertexsStretch, in double newVertexsStretchValue, ref double currentF, in double[,] tableSimplex, in int currentId, in int n, in double E, in double Y, in double maxValue, in double followMaxValue, in double[] centerOfGravityXc)
    {
        Console.WriteLine("Шаг 8.");
        if (newVertexsStretchValue < currentF)
        {
            Console.WriteLine($"Точка растяжения меньше текущей ({newVertexsStretchValue:f3} < {currentF:f3}).");
            // Заменяем вершину в таблице.
            for (int j = 0; j < n; j++)
                tableSimplex[currentId, j] = newVertexsStretch[j];
            tableSimplex[currentId, n] = newVertexsStretchValue;
            Console.WriteLine($"После замены вершины [{currentId}] на точку растяжения:\n{tableSimplex.TableToString("f3")}");
            return Task12(tableSimplex, n, E);
        }
        else
        {
            currentF = newVertexsStretchValue;
            return Task9(tableSimplex, Y, E, currentF, currentId, maxValue, followMaxValue, n, centerOfGravityXc);
        }
    }

    public static bool Task9(double[,] tableSimplex, double Y, double E, double currentF, int currentId, double maxValue, double followMaxValue, int n, double[] centerOfGravityXc)
    {
        Console.WriteLine("Шаг 9.");
        double[] mas = new double[n];
        double[] arrayFuncValue = new double[n + 1];
        if (currentF < maxValue && currentF > followMaxValue)
        {
            // Сжатие симплекса.
            double[] newVerticesCompress = new double[n];
            for (int r = 0; r < n; r++)
                newVerticesCompress[r] =
                    centerOfGravityXc[r] + Y * (tableSimplex[currentId, r] - centerOfGravityXc[r]);
            Console.WriteLine("Сжатая вершина: f("
                + string.Join("; ", newVerticesCompress.EveryConverter(e => e.ToString("f3")))
                + ") = "
                + TargetFunction(newVerticesCompress).ToString("f3"));
            return Task10(newVerticesCompress, tableSimplex, n, currentId, E, arrayFuncValue, mas);
        }
        else
            return Task11(tableSimplex, arrayFuncValue, n, mas, E);
    }

    public static bool Task10(in double[] newVerticesCompress, in double[,] tableSimplex, in int n, in int currentId, in double E, in double[] arrayFuncValue, in double[] mas)
    {
        if (TargetFunction(newVerticesCompress) < tableSimplex[currentId, n])
        {
            // Заменяем вершину в таблице
            for (int j = 0; j < n; j++)
                tableSimplex[currentId, j] = newVerticesCompress[j];
            tableSimplex[currentId, n] = TargetFunction(newVerticesCompress);
            return Task12(tableSimplex, n, E);
        }
        else
            return Task11(tableSimplex, arrayFuncValue, n, mas, E);
    }

    public static bool Task11(in double[,] tableSimplex, in double[] arrayFuncValue, in int n, in double[] mas, in double E)
    {
        for (int i = 0; i < n + 1; i++)
            arrayFuncValue[i] = tableSimplex[i, n];
        int minVertex = SearchMin(arrayFuncValue);
        Console.WriteLine("Редукция. Индекс минимальной вершины в таблице: " + minVertex);
        for (int i = 0; i < n + 1; i++)
            if (i != minVertex)
            {
                for (int t = 0; t < n; t++)
                    mas[t] = tableSimplex[i, t] =
                        tableSimplex[minVertex, t] + (tableSimplex[i, t] - tableSimplex[minVertex, t]) / 2;
                tableSimplex[i, n] = TargetFunction(mas);
            }
        for (int k = 0; k < mas.Length; k++)
            mas[k] = 0;
        Console.WriteLine($"Таблица после редукции:\n{tableSimplex.TableToString("f3")}");
        return Task12(tableSimplex, n, E);
    }

    public static bool Task12(double[,] tableSimplex, int n, double E)
    {
        // Найдём центр тяжести всего симплекса.
        double[] centerOfGravityXc = new double[n];
        double[] arrayFuncValue = new double[n + 1];
        for (int i = 0; i < centerOfGravityXc.Length; i++)
            centerOfGravityXc[i] = 0;
        for (int i = 0; i < n + 1; i++)
            for (int t = 0; t < n; t++)
                centerOfGravityXc[t] += tableSimplex[i, t] / (n + 1);
        double fxc = TargetFunction(centerOfGravityXc);
        Console.WriteLine($"Центр тяжести симплекса: f({string.Join("; ", centerOfGravityXc.EveryConverter(e => e.ToString("f3")))}) = {fxc.ToString("f3")}");
        double sigma = 0;
        for (int i = 0; i < n + 1; i++)
            sigma += Math.Pow((tableSimplex[i, n] - fxc), 2) / (n + 1);
        sigma = Math.Sqrt(sigma);

        return Task13(sigma, E, arrayFuncValue, tableSimplex, n);
    }

    public static bool Task13(double sigma, double E, double[] arrayFuncValue, double[,] tableSimplex, int n)
    {
        if (sigma < E)
        {
            Console.Write($"Поиск окончен так как: σ < ε ({sigma:f3} < {E:f3})");
            Console.WriteLine();
            for (int i = 0; i < n + 1; i++)
                arrayFuncValue[i] = tableSimplex[i, n];

            int min = SearchMin(arrayFuncValue);
            double resultMin = tableSimplex[min, n];
            Console.WriteLine($"Минимальная вершина: [{min}] f([{tableSimplex[min, 0]:f3}; {tableSimplex[min, 1]:f3}]) = {resultMin:f3}");
            return true;
        }
        else
        {
            Console.WriteLine($"Поиск продолжается так как: sigma ≥ E ({sigma:f3} ≥ {E:f3})");
            return false;
        }
    }

    public static double TargetFunction(double x, double y)
        => -3.3 * x +
        5.2 * Math.Pow(x, 2) -
        4.2 * y +
        2.8 * Math.Pow(y, 2);

    public static double TargetFunction(double[] args)
        => TargetFunction(args[0], args[1]);

    public static double TargetFunction(IEnumerable<double> args)
    {
        using IEnumerator<double> e = args.GetEnumerator();
        e.MoveNext();
        double x = e.Current;
        e.MoveNext();
        double y = e.Current;
        return TargetFunction(x, y);
    }

    public static int SearchMax(double[] arr)
    {
        double max = arr[0];
        int ind = 0;
        for (int i = 1; i < arr.Length; i++)
            if (max < arr[i])
                max = arr[ind = i];
        return ind;
    }
    public static int SearchFollowMax(double[] mas)
    {
        double max = mas[0];
        double follow = max;
        int indMax = 0;
        int indFollow = 0;
        if (max < mas[1])
        {
            follow = max;
            max = mas[1];
            indMax = 1;
            indFollow = 0;
        }
        for (int i = 1; i < mas.Length; i++)
        {
            if (mas[i] > max)
            {
                follow = max;
                indFollow = indMax;
                max = mas[i];
                indMax = i;
            }
            if (mas[i] < max && mas[i] > follow)
                follow = mas[indFollow = i];
        }
        return indFollow;
    }
    public static int SearchMin(double[] arr)
    {
        double min = arr[0];
        int ind = 0;
        for (int i = 1; i < arr.Length; i++)
            if (min > arr[i])
                min = arr[ind = i];
        return ind;
    }

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
        foreach (var e in that)
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
                sb.Append(array.GetValue(i, j).ToString(max, format));
            sb.Append('\n');
        }
        return sb.ToString();
    }
}

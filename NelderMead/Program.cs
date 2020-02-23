using System;
using System.Text;
using System.Collections.Generic;
public static class NelderMead
{
    public static void Main(string[] args)
    {
        Task1(out int n, out double E, out double m, out double B, out double Y, out double[] mas, out double[,] tableSimplex);
        Task2(tableSimplex, n, mas, m);
        Task3(tableSimplex, n, mas, B, Y, E);
    }

    /// <summary>
    /// Задать начальные денные.
    /// </summary>
    public static void Task1(out int n, out double E, out double m, out double B, out double Y, out double[] mas, out double[,] tableSimplex)
    {
        n = 2;
        E = 0.1;
        m = 1.0; // Длина ребра многогранника.
        B = 2.8; // Коэффициент растяжения.
        Y = 0.4; // Коэффициент сжатия.
        mas = new double[n];
        mas[0] = 0.275;
        mas[1] = 0.8058;
        tableSimplex = new double[n + 1, n + 1];
    }

    /// <summary>
    /// Построить начальный многогранник.
    /// </summary>
    public static void Task2(in double[,] tableSimplex, int n, in double[] mas, in double m)
    {
        for (int j = 0; j < n; j++)
            tableSimplex[0, j] = mas[j];
        tableSimplex[0, n] = TargetFunction(mas);
        // Приращения.
        double increment1 = (Math.Sqrt(n + 1) - 1) / (n * Math.Sqrt(2)) * m;
        double increment2 = (Math.Sqrt(n + 1) + n - 1) / (n * Math.Sqrt(2)) * m;
        // Находим координаты остальных вершин.

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                tableSimplex[i + 1, j] = (i == j ? increment1 : increment2) + mas[j];
            tableSimplex[i + 1, n] = TargetFunction(tableSimplex[i + 1, 0], tableSimplex[i + 1, 1]);
        }
        // Вывод таблицы.
        Console.WriteLine(tableSimplex.TableToString("f3"));
    }

    /// <summary>
    /// Определить номера k вершин.
    /// </summary>
    public static void Task3(in double[,] tableSimplex, in int n, in double[] mas, in double B, in double Y, in double E)
    {
        while (true)
        {
            double[] arrayFuncValue = new double[n + 1];
            double[] centerOfGravityXc = new double[n]; for (int k = 0; k < n; k++) centerOfGravityXc[k] = 0;
            double[] reflectedVertex = new double[n]; // Координаты отраженной вершины.

            for (int i = 0; i < n + 1; i++)
                arrayFuncValue[i] = tableSimplex[i, n];
            // Определяем максимальную, минимальную и следующую за максимальной вершиной.
            int maxVertex = SearchMax(arrayFuncValue);
            int minValObjFunction = SearchMin(arrayFuncValue);
            int followMaxIndex = followMaxValueSearch(arrayFuncValue);

            double maxValue = tableSimplex[maxVertex, n];
            double followMaxValue = tableSimplex[followMaxIndex, n];
            double minValue = tableSimplex[minValObjFunction, n];
            Console.WriteLine("\n\nМаксимальное значение Fh = " + maxValue);
            Console.WriteLine("Следующее за максимальным значением Fs = " + followMaxValue);
            Console.WriteLine("Минимальное значение Fl = " + minValue);

            int currentId = maxVertex;

            // Считаем центр тяжести вершин симплекса (кроме максимальной).
            for (int i = 0; i < n + 1; i++)
            {
                for (int t = 0; t < n; t++)
                {
                    if (i == maxVertex)
                        mas[t] = tableSimplex[i, t];
                    else
                        centerOfGravityXc[t] = centerOfGravityXc[t] + tableSimplex[i, t] / n;
                }
            }
            //Находим координаты отраженной вершины.
            for (int i = 0; i < n; i++)
            {
                reflectedVertex[i] = 2 * centerOfGravityXc[i] - mas[i];
            }
            double fReflected = TargetFunction(reflectedVertex);
            Console.WriteLine("Значение функции в отраженной вершине: " + fReflected);
            double currentF = fReflected;

            Task6(ref currentF, fReflected, maxValue, n, tableSimplex, maxVertex, reflectedVertex, minValue, mas, currentId, centerOfGravityXc, B, Y, E, followMaxValue);
        }
    }

    public static void Task6(ref double currentF, in double fReflected, in double maxValue, in int n, in double[,] tableSimplex, in int maxVertex, in double[] reflectedVertex, in double minValue, in double[] mas, in int currentId, in double[] centerOfGravityXc, in double B, in double Y, in double E, in double followMaxValue)
    {
        if (fReflected < maxValue)
        {
            // Пункт 6:
            // Заменяем вершину в таблице.
            for (int i = 0; i < n + 1; i++)
                for (int j = 0; j < n; j++)
                    if (i == maxVertex) { tableSimplex[i, j] = reflectedVertex[j]; }
            tableSimplex[maxVertex, n] = fReflected;
            Console.WriteLine($"После замены вершины на отраженную:\n{tableSimplex.TableToString("f3")}");
            Task7(ref currentF, minValue, n, currentId, mas, tableSimplex, centerOfGravityXc, maxValue, followMaxValue, B, E, Y);
        }
        else
        {
            bool proof = Task9(tableSimplex, Y, E, currentF, currentId, maxValue, followMaxValue, n, centerOfGravityXc);
            if (proof == true) return;
        }
    }

    public static void Task7(ref double currentF, in double minValue, in int n, in int currentId, in double[] mas, in double[,] tableSimplex, in double[] centerOfGravityXc, in double maxValue, in double followMaxValue, in double B, in double E, in double Y)
    {
        // Пункт 7:
        if (currentF < minValue)
        {
            // Операция растяжения.
            double[] newVertexsStretch = new double[n];
            for (int i = 0; i < n + 1; i++)
                for (int j = 0; j < n; j++)
                    if (i == currentId) mas[j] = tableSimplex[i, j];
            for (int r = 0; r < n; r++)
            {
                newVertexsStretch[r] = centerOfGravityXc[r] + B * (mas[r] - centerOfGravityXc[r]);
            }
            Console.WriteLine("Значение функции после растяжения: " + TargetFunction(newVertexsStretch));
            // Пункт 8:
            if (TargetFunction(newVertexsStretch) < currentF)
            {
                Console.WriteLine("Пункт 8");
                // Заменяем вершину в таблице.
                for (int i = 0; i < n + 1; i++)
                    for (int j = 0; j < n; j++)
                        if (i == currentId) { tableSimplex[i, j] = newVertexsStretch[j]; }
                tableSimplex[currentId, n] = TargetFunction(newVertexsStretch);
                Console.WriteLine($"После замены вершины на отраженную:{tableSimplex.TableToString("f3")}");
                bool proof = Task12(tableSimplex, n, E);
                if (proof == true) return;
            }
            else
            {
                currentF = TargetFunction(newVertexsStretch);//
                bool proof = Task9(tableSimplex, Y, E, currentF, currentId, maxValue, followMaxValue, n, centerOfGravityXc);
                if (proof == true) return;
            }
        }
        else
        {
            bool proof = Task9(tableSimplex, Y, E, currentF, currentId, maxValue, followMaxValue, n, centerOfGravityXc);
            if (proof == true) return;
        }
    }

    public static bool Task9(double[,] tableSimplex, double Y, double E, double currentF, int currentId, double maxValue, double followMaxValue, int n, double[] centerOfGravityXc)
    {
        Console.WriteLine("Следующее за максимальным = " + followMaxValue + " \t f(x) = " + currentF + "\t максимальное = " + maxValue);
        double[] mas = new double[n];
        double[] arrayFuncValue = new double[n + 1];
        if (currentF < maxValue & currentF > followMaxValue)
        {
            // Сжатие симплекса.
            double[] newVerticesCompress = new double[n];
            for (int i = 0; i < n + 1; i++)
                for (int j = 0; j < n; j++)
                    if (i == currentId) mas[j] = tableSimplex[i, j];
            for (int r = 0; r < n; r++)
            {
                newVerticesCompress[r] = centerOfGravityXc[r] + Y * (mas[r] - centerOfGravityXc[r]);
            }
            Console.WriteLine("Сжатая вершина: " + newVerticesCompress[0] + "   " + newVerticesCompress[1] + "	" + TargetFunction(newVerticesCompress));
            Console.WriteLine("ТУТ" + tableSimplex[currentId, n] + "   " + currentId);
            // Пункт 10:
            if (TargetFunction(newVerticesCompress) < tableSimplex[currentId, n])
            {
                // Заменяем вершину в таблице
                int tempInd = -1;
                for (int i = 0; i < n + 1; i++)
                    for (int j = 0; j < n; j++)
                        if (i == currentId)
                        {
                            tableSimplex[i, j] = newVerticesCompress[j];
                            tempInd = i;
                        }
                tableSimplex[tempInd, n] = TargetFunction(newVerticesCompress);
                // Пункт 12:
                bool proof = Task12(tableSimplex, n, E);
                if (proof == true) return true;
            }
            // Пункт 11:
            else
            { // Редукция.
                for (int i = 0; i < n + 1; i++)
                {
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (t == n) arrayFuncValue[i] = tableSimplex[i, t];
                    }
                }
                int minVertex = SearchMin(arrayFuncValue);
                Console.WriteLine("Редукция. Индекс минимальной вершины в таблице: " + minVertex);
                for (int i = 0; i < n + 1; i++)
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (t != n & i != minVertex)
                        {
                            double temp = tableSimplex[i, t];
                            tableSimplex[i, t] = tableSimplex[minVertex, t] + ((double)1 / 2) * (temp - tableSimplex[minVertex, t]);
                            mas[t] = tableSimplex[i, t];
                        }
                        else
                        {
                            if (t == n & i != minVertex) tableSimplex[i, t] = TargetFunction(mas);
                            for (int k = 0; k < mas.Length; k++) mas[k] = 0;
                        }
                    }
                // Вывод таблицы.
                Console.WriteLine($"Таблица после редукции:\n{tableSimplex.TableToString("f3")}");
                bool proof = Task12(tableSimplex, n, E);
                if (proof == true) return true;
            }
        }
        else
        { // Редукция.
          // Пункт 11:
            for (int i = 0; i < n + 1; i++)
            {
                for (int t = 0; t < n + 1; t++)
                {
                    if (t == n) arrayFuncValue[i] = tableSimplex[i, t];
                }
            }
            int minVertex = SearchMin(arrayFuncValue);
            Console.WriteLine("Редукция. Индекс минимальной вершины в таблице: " + minVertex);
            for (int i = 0; i < n + 1; i++)
                for (int t = 0; t < n + 1; t++)
                {
                    if (t != n & i != minVertex)
                    {
                        double temp = tableSimplex[i, t];
                        tableSimplex[i, t] = tableSimplex[minVertex, t] + ((double)1 / 2) * (temp - tableSimplex[minVertex, t]);
                        mas[t] = tableSimplex[i, t];
                    }
                    else
                    {
                        if (t == n & i != minVertex) tableSimplex[i, t] = TargetFunction(mas);
                        for (int k = 0; k < mas.Length; k++) mas[k] = 0;
                    }
                }
            // Вывод таблицы.
            Console.WriteLine($"Таблица после редукции:\n{tableSimplex.TableToString("f3")}");
            bool proof = Task12(tableSimplex, n, E);
            if (proof == true) return true;
        }
        return false;
    }

    public static bool Task12(double[,] tableSimplex, int n, double E)
    {
        // Найдем центр тяжести всего симплекса.
        Console.WriteLine("\n\nПроверка окончания поиска");
        double[] centerOfGravityXc = new double[n];
        double[] arrayFuncValue = new double[n + 1];
        for (int i = 0; i < centerOfGravityXc.Length; i++)
            centerOfGravityXc[i] = 0;
        for (int i = 0; i < n + 1; i++)
            for (int t = 0; t < n; t++)
                centerOfGravityXc[t] = centerOfGravityXc[t] + ((double)1 / (n + 1)) * tableSimplex[i, t];
        double fxc = TargetFunction(centerOfGravityXc);
        Console.WriteLine("Центр тяжести симплекса:  " + fxc);
        double sigma = 0;
        for (int i = 0; i < n + 1; i++)
            for (int t = 0; t < n + 1; t++)
                if (t == n) sigma = sigma + ((double)1 / (n + 1)) * Math.Pow((tableSimplex[i, t] - fxc), 2);
        sigma = Math.Sqrt(sigma);

        // Пункт 13:
        if (sigma < E)
        {
            Console.Write("Поиск окончен так как: ");
            Console.WriteLine("sigma  = " + sigma + " < E=" + E);
            for (int i = 0; i < n + 1; i++)
                arrayFuncValue[i] = tableSimplex[i, n];

            int min = SearchMin(arrayFuncValue);
            double resultMin = tableSimplex[min, n];
            Console.WriteLine("Минимальная вершина: " + resultMin);
            return true;
        }
        else
        {
            Console.WriteLine("Поиск продолжается так как: sigma  = " + sigma + " > E=" + E);
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

    public static int SearchMax(double[] arr)
    {
        double max = arr[0];
        int ind = 0;
        for (int i = 1; i < arr.Length; i++)
            if (max < arr[i])
                max = arr[ind = i];
        return ind;
    }
    public static int followMaxValueSearch(double[] mas)
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

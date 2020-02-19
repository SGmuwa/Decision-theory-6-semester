using System;

public class Simplex
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
        for (int i = 0; i < n + 1; i++)
        {
            for (int t = 0; t < n + 1; t++)
            {
                Console.Write(tableSimplex[i, t] + "\t");
            }
            Console.WriteLine();
        }
        while (true)
        {
            Console.WriteLine("Итерация " + iteration++);
            double[] arrayFuncValue = new double[n + 1];
            double[] centerOfGravityXc = new double[n]; // Координаты центра тяжести
            for (int k = 0; k < n; k++)
                centerOfGravityXc[k] = 0;
            double[] reflectedVertex = new double[n]; // Координаты отраженной вершины
            for (int i = 0; i < n + 1; i++)
                for (int t = 0; t < n + 1; t++)
                    if (t == n)
                        arrayFuncValue[i] = tableSimplex[i, t];
            int maxVertex = SearchMax(arrayFuncValue);
            for (int i = 0; i < n + 1; i++)
                for (int t = 0; t < n; t++)
                    if (i == maxVertex)
                        start[t] = tableSimplex[i, t];
                    else
                        centerOfGravityXc[t] =
                            centerOfGravityXc[t] +
                            ((double)1 / n) * tableSimplex[i, t];

            // Координаты отраженной вершины
            for (int i = 0; i < n; i++)
                reflectedVertex[i] = 2 * centerOfGravityXc[i] - start[i];
            Console
                .WriteLine("Значение функции максимальной точки Симлекса: " +
                TargetFunction(start) +
                "; значение в отражённой вершине: " +
                TargetFunction(reflectedVertex));
            if (TargetFunction(reflectedVertex) < TargetFunction(start))
            {
                for (int i = 0; i < n + 1; i++)
                    for (int j = 0; j < n; j++)
                        if (i == maxVertex) tableSimplex[i, j] = reflectedVertex[j];
                tableSimplex[maxVertex, n] =
                    TargetFunction(reflectedVertex);
            }
            else
            {
                Console.WriteLine("редукция");
                for (int i = 0; i < n + 1; i++)
                    for (int t = 0; t < n + 1; t++)
                        if (t == n)
                            arrayFuncValue[i] = tableSimplex[i, t];
                int minVertex = SearchMin(arrayFuncValue);
                for (int i = 0; i < n + 1; i++)
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (t != n & i != minVertex)
                        {
                            start[t] = tableSimplex[i, t] =
                                tableSimplex[minVertex, t] +
                                ((double)1 / 2) *
                                (tableSimplex[i, t] - tableSimplex[minVertex, t]);
                        }
                        else
                            if (t == n & i != minVertex)
                                tableSimplex[i, t] = TargetFunction(start);
                        for (int k = 0; k < start.Length; k++)
                            start[k] = 0;
                    }
            }
            for (int i = 0; i < centerOfGravityXc.Length; i++)
                centerOfGravityXc[i] = 0;
            for (int i = 0; i < n + 1; i++)
            {
                for (int t = 0; t < n + 1; t++)
                {
                    if (t < n)
                        centerOfGravityXc[t] =
                            centerOfGravityXc[t] +
                            1.0 / (n + 1) * tableSimplex[i, t];
                    Console.Write(tableSimplex[i, t] + "\t");
                }
                Console.WriteLine();
            }
            double centerY = TargetFunction(centerOfGravityXc);
            Console.WriteLine($"Центр тяжести симплекса: f({string.Join(", ", centerOfGravityXc)}) = {centerY}");

            // Проверка условий окончания поиска
            int checkEnd = 0;
            for (int i = 0; i < n + 1; i++)
                for (int t = 0; t < n + 1; t++)
                    if (t == n)
                        if (Math.Abs(tableSimplex[i, t] - centerY) < E)
                            checkEnd++;
            if (checkEnd == n + 1)
            {
                for (int i = 0; i < n + 1; i++)
                    for (int t = 0; t < n + 1; t++)
                        if (t == n)
                            arrayFuncValue[i] = tableSimplex[i, t];
                int minVertex = SearchMin(arrayFuncValue);
                double resultMin = tableSimplex[minVertex, n];
                Console.WriteLine("Минимум: " + resultMin);
                return;
            }
        }
    }

    public static double TargetFunction(double[] mas)
        => -3.3 * mas[0] +
        5.2 * Math.Pow(mas[0], 2) -
        4.2 * mas[1] +
        2.8 * Math.Pow(mas[1], 2);

    public static int SearchMax(double[] mas)
    {
        double max = mas[0];
        int ind = 0;
        for (int i = 1; i < mas.Length; i++)
            if (max < mas[i])
            {
                max = mas[i];
                ind = i;
            }
        return ind;
    }

    public static int SearchMin(double[] mas)
    {
        double min = mas[0];
        int ind = 0;
        for (int i = 1; i < mas.Length; i++)
            if (min > mas[i])
            {
                min = mas[i];
                ind = i;
            }
        return ind;
    }
}

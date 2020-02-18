using System;

public class Simplex
{
    public static void Main(string[] args)
    {
        int it = 0; // Счётчик итераций
        int n = 2;
        int tempN = 0;
        double E = 0.1;
        double m = 0.25;
        double[] mas = new double[n];
        mas[0] = 0.275;
        mas[1] = 0.8058;
        double[,] tableSimplex = new double[n + 1, n + 1];
        for (int i = 0; i < n + 1; i++)
            for (int j = 0; j < n; j++)
                if (i == 0)
                    tableSimplex[i, j] = mas[j];
                else
                    tableSimplex[i, j] = -0;
        tableSimplex[0, n] = objectiveFunction(mas);

        double increment1 =
            (double)(Math.Sqrt(n + 1) - 1) / (n * Math.Sqrt(2)) * m;
        double increment2 =
            (double)(Math.Sqrt(n + 1) + n - 1) / (n * Math.Sqrt(2)) * m;

        //Вычисление координат остальных вершин симплекса
        while (tempN < n)
        {
            double[] x = new double[n];
            for (int i = 0; i < n; i++)
                if (tempN == i)
                    x[i] = increment1 + mas[i];
                else
                    x[i] = increment2 + mas[i];
            for (int i = 0; i < n + 1; i++)
                for (int j = 0; j < n; j++)
                    if (i == tempN + 1) tableSimplex[i, j] = x[j];
            tableSimplex[tempN + 1, n] = objectiveFunction(x);
            tempN++;
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
            Console
                .WriteLine("\n" +
                "\n" +
                "Итерация k = " +
                it +
                "---------------------------");
            it++;
            double[] arrayFuncValue = new double[n + 1];
            double[] centerOfGravityXc = new double[n]; // Координаты центра тяжести
            for (int k = 0; k < n; k++) centerOfGravityXc[k] = 0;
            double[] reflectedVertex = new double[n]; // Координаты отраженной вершины
            for (int i = 0; i < n + 1; i++)
            {
                for (int t = 0; t < n + 1; t++)
                {
                    if (t == n) arrayFuncValue[i] = tableSimplex[i, t];
                }
            }
            int maxVertex = maxSearch(arrayFuncValue);
            for (int i = 0; i < n + 1; i++)
            {
                for (int t = 0; t < n; t++)
                {
                    if (i == maxVertex)
                        mas[t] = tableSimplex[i, t];
                    else
                        centerOfGravityXc[t] =
                            centerOfGravityXc[t] +
                            ((double)1 / n) * tableSimplex[i, t];
                }
            }

            // Координаты отраженной вершины
            for (int i = 0; i < n; i++)
            {
                reflectedVertex[i] = 2 * centerOfGravityXc[i] - mas[i];
            }
            double Fxc = 0;
            Console
                .WriteLine("Значение функции максимальной точки симлекса:" +
                objectiveFunction(mas) +
                ";     Значение функции в отраженной вершине:" +
                objectiveFunction(reflectedVertex));
            if (objectiveFunction(reflectedVertex) < objectiveFunction(mas))
            {
                //Наблюдается уменьшение целевой функции
                //Проверим условие окончания поиска
                //0) Заменяем рершину в таблице
                for (int i = 0; i < n + 1; i++)
                    for (int j = 0; j < n; j++)
                        if (i == maxVertex) tableSimplex[i, j] = reflectedVertex[j];
                tableSimplex[maxVertex, n] =
                    objectiveFunction(reflectedVertex);

                // 1) Вывод таблицы и поиск центра тяжести
                Console.WriteLine();
                for (int i = 0; i < centerOfGravityXc.Length; i++)
                    centerOfGravityXc[i] = 0;
                for (int i = 0; i < n + 1; i++)
                {
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (t < n)
                        {
                            centerOfGravityXc[t] =
                                centerOfGravityXc[t] +
                                ((double)1 / (n + 1)) * tableSimplex[i, t];
                        }
                        Console.Write(tableSimplex[i, t] + "\t");
                    }
                    Console.WriteLine();
                }
                Fxc = objectiveFunction(centerOfGravityXc);
                Console.WriteLine("Центр тяжести симплекса: " + Fxc);
                Console.Write("Координаты центра тяжести симплекса: ");
                for (int i = 0; i < centerOfGravityXc.Length; i++)
                    Console.Write(centerOfGravityXc[i] + "\t");
            }
            else
            {
                // Иначе не заменяем вершину в таблице. Выполняется операция редукции
                Console.WriteLine("НЕОБХОДИМА операция редукции");
                for (int i = 0; i < n + 1; i++)
                {
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (t == n) arrayFuncValue[i] = tableSimplex[i, t];
                    }
                }
                int minVertex = minSearch(arrayFuncValue);
                for (int i = 0; i < n + 1; i++)
                {
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (t != n & i != minVertex)
                        {
                            double temp = tableSimplex[i, t];
                            tableSimplex[i, t] =
                                tableSimplex[minVertex, t] +
                                ((double)1 / 2) *
                                (temp - tableSimplex[minVertex, t]);
                            mas[t] = tableSimplex[i, t];
                        }
                        else
                        {
                            if (t == n & i != minVertex)
                                tableSimplex[i, t] = objectiveFunction(mas);
                            for (int k = 0; k < mas.Length; k++) mas[k] = 0;
                        }
                    }
                }

                // 1) Вывод таблицы и поиск центра тяжести
                Console.WriteLine();
                for (int i = 0; i < centerOfGravityXc.Length; i++)
                    centerOfGravityXc[i] = 0;
                for (int i = 0; i < n + 1; i++)
                {
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (t < n)
                        {
                            centerOfGravityXc[t] =
                                centerOfGravityXc[t] +
                                ((double)1 / (n + 1)) * tableSimplex[i, t];
                        }
                        Console.Write(tableSimplex[i, t] + "\t");
                    }
                    Console.WriteLine();
                }
                Fxc = objectiveFunction(centerOfGravityXc);
                Console.WriteLine("Центр тяжести симплекса: " + Fxc);
            }

            // Проверка условий окончания поиска
            int checkEnd = 0;
            for (int i = 0; i < n + 1; i++)
            {
                for (int t = 0; t < n + 1; t++)
                {
                    if (t == n)
                    {
                        if (Math.Abs(tableSimplex[i, t] - Fxc) < E)
                            checkEnd++;
                    }
                }
            }
            if (checkEnd == n + 1)
            {
                for (int i = 0; i < n + 1; i++)
                {
                    for (int t = 0; t < n + 1; t++)
                    {
                        if (t == n) arrayFuncValue[i] = tableSimplex[i, t];
                    }
                }
                int minVertex = minSearch(arrayFuncValue);
                double resultMin = tableSimplex[minVertex, n];
                Console
                    .WriteLine("\n" +
                    "\n" +
                    "ОТВЕТ: минимальная вершина: " +
                    resultMin);
                return;
            }
        }
    }

    public static double objectiveFunction(double[] mas)
    {
        return ((double)13 / 5) * (Math.Pow(mas[0], 2)) -
        ((double)21 / 10) * mas[1] +
        ((double)7 / 5) * (Math.Pow(mas[1], 2));
    }

    public static int maxSearch(double[] mas)
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

    public static int minSearch(double[] mas)
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

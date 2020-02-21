using System;

public class PatternSearch
{
    public static void Main(string[] args)
    {
        int n = 2; // Размерность
        int d = 2; // Коэффициент уменьшения шага
        double E = 0.1; // Точность поиска
        double m = 0.5; // Ускоряющий множитель
        double h = 0.2; // Шаг

        double[] base_point = new double[n]; // Координаты базисной точки
        double[] xP = new double[n]; // Координаты точки для поиска по образцу
        double[] current_point = new double[n];
        double[] test_point = new double[n]; // Координаты тестовых точек временно хранятся в этом массиве

        for (int i = 0; i < n; i++)
            base_point[i] = 0;

        // Считаем тестовую, если она больше текущей, то производится поиск по следующим направлениям и координатам
        bool k = true;
        while (k)
        {
            for (int i = 0; i < n; i++)
            {
                if (i == 0)
                    for (int j = 0; j < n; j++)
                    {
                        test_point[j] = base_point[j];
                        current_point[j] = base_point[j];
                    }
                for (int l = 0; l < n; l++)
                    test_point[l] = current_point[l];
                double temp = test_point[i];
                test_point[i] = temp + h;
                if (TargetFunction(test_point) < TargetFunction(current_point))
                    for (int j = 0; j < n; j++)
                    {
                        current_point[j] = test_point[j];
                    }
                else
                {
                    for (int j = 0; j < n; j++)
                        test_point[j] = current_point[j];
                    temp = test_point[i];
                    test_point[i] = temp - h;
                    if (TargetFunction(test_point) < TargetFunction(current_point))
                    {
                        for (int j = 0; j < n; j++)
                        {
                            current_point[j] = test_point[j];
                        }
                    }
                }
            }
            // Сравнение с базисной точкой x0
            int flag = 0;
            for (int i = 0; i < n; i++)
                if (base_point[i] == current_point[i])
                    flag += 1;
            if (flag == n)
            { // Если х1 = х0, то уменьшаем шаг
                double tempH = h;
                h = (double)tempH / d;
                Console.WriteLine("НЕ нашли точку (x0=x1)");
                Console.WriteLine("Уменьшение шага, H = " + h + "\n\n");
            }
            else
            { // Если х1!=х0, то поиск по образцу
                Console.Write("Новая базисная точка x1: " + current_point[0] + "   " + current_point[1] + "\n");
                for (int j = 0; j < n; j++)
                    xP[j] = current_point[j] + (double)m * (current_point[j] - base_point[j]);
                if (TargetFunction(xP) < TargetFunction(current_point))
                {
                    for (int j = 0; j < n; j++)
                        base_point[j] = xP[j];
                    Console.WriteLine("Базисная точка х0 = хр: " + base_point[0] + "  " + base_point[1]);
                }
                else
                {
                    for (int j = 0; j < n; j++)
                        base_point[j] = current_point[j];
                    Console.WriteLine("Базисная точка х0 = х1: " + base_point[0] + "  " + base_point[1]);
                }
                Console.Write("Шаг  " + h + "\n \n");
                if (h < E)
                {
                    Console.WriteLine("Шаг = " + h + " < E = " + E);
                    Console.WriteLine("Минимальная точка:" + TargetFunction(base_point));
                    k = false;
                }
            }
        }
    }
    public static double TargetFunction(double[] mas)
        => -3.3 * mas[0] +
        5.2 * Math.Pow(mas[0], 2) -
        4.2 * mas[1] +
        2.8 * Math.Pow(mas[1], 2);
}

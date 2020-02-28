using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public static class GradientDescentConstStep
{
    public static void Main(string[] args)
    {
        double E = 0.1;
        double h = 0.4;
        double gradientNorm;
        int n = 2;
        double[] mas = new double[n];//координаты текущей точки
        for (int i = 0; i < n; i++)
            mas[i] = 0;
        double[] gradientMas = new double[n];
        double func = TargetFunction(mas);
        Element elem = new Element(mas[0], mas[1], func);
        List<Element> history = new List<Element>() { elem };
        gradientMas = gradientSearch(mas);
        double funcResMas;
        do
        {
            do
            {
                for (int i = 0; i < n; i++)
                {
                    double doubleTempMas = mas[i];
                    mas[i] = doubleTempMas - h * gradientMas[i];
                }
                funcResMas = TargetFunction(mas);
                if (history[^1].result < funcResMas)
                {//уменьшение шага
                    double tempH = h;
                    h = tempH / 2;
                    mas[0] = history[^1].x1;
                    mas[1] = history[^1].x2;
                }
            } while (history[^1].result < funcResMas);

            elem = new Element(mas[0], mas[1], funcResMas);
            history.Add(elem);

            gradientMas = gradientSearch(mas);
            gradientNorm = 0;
            for (int i = 0; i < n; i++)
                gradientNorm += Math.Pow(gradientMas[i], 2);
            double grTemp = gradientNorm;
            gradientNorm = Math.Sqrt(grTemp);
        } while (gradientNorm > E);


        Console.WriteLine("ОТВЕТ: x1 = " + history[^1].x1 + "\t x2 = " + history[^1].x2 + "\t f = " + history[^1].result);
        Console.WriteLine("\nИстория рассматриваемых точек (от минимальной до первой рассмариваемой)");
        int sizeHistory = history.Count;
        Console.WriteLine(TableToString(history, "f3"));
    }
    public static double TargetFunction(double x, double y)
        => -3.3 * x +
        5.2 * Math.Pow(x, 2) -
        4.2 * y +
        2.8 * Math.Pow(y, 2);

    public static double TargetFunction(double[] args)
        => TargetFunction(args[0], args[1]);

    public static double[] gradientSearch(double[] mas)
    {
        double[] masResult = new double[mas.Length];
        double delta = 0.000000001;
        double[] masTemp = new double[mas.Length];
        for (int i = 0; i < mas.Length; i++)
            masTemp[i] = mas[i];
        for (int i = 0; i < mas.Length; i++)
        {
            masTemp[i] += delta;
            double elem = (double)(TargetFunction(masTemp) - TargetFunction(mas)) / delta;
            masResult[i] = elem;
            for (int j = 0; j < mas.Length; j++)
                masTemp[j] = mas[j];
        }
        return masResult;
    }

    internal static string TableToString(this IReadOnlyCollection<Element> input, string format = null, Func<dynamic, object> renderForeach = null)
    {
        double[,] two = new double[input.Count, input.First().Count];
        {
            int i = 0;
            foreach (IEnumerable<double> line in input)
            {
                int j = 0;
                foreach (double element in line)
                    two[i, j++] = element;
                i++;
            }
        }
        return two.TableToString(format, renderForeach);
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

public class Element : IEnumerable<double>, ICollection<double>, IReadOnlyCollection<double>
{
    private readonly double[] memory = new double[3];
    public double x1 { set => memory[0] = value; get => memory[0]; }
    public double x2 { set => memory[1] = value; get => memory[1]; }
    public double result { set => memory[2] = value; get => memory[2]; }

    public int Count => ((ICollection<double>)memory).Count;

    public bool IsReadOnly => ((ICollection<double>)memory).IsReadOnly;

    public Element(double x1, double x2, double result)
    {
        this.x1 = x1;
        this.x2 = x2;
        this.result = result;
    }

    public IEnumerator<double> GetEnumerator()
    {
        return ((IEnumerable<double>)memory).GetEnumerator();
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        return ((IEnumerable<double>)memory).GetEnumerator();
    }

    public void Add(double item)
    {
        ((ICollection<double>)memory).Add(item);
    }

    public void Clear()
    {
        ((ICollection<double>)memory).Clear();
    }

    public bool Contains(double item)
    {
        return ((ICollection<double>)memory).Contains(item);
    }

    public void CopyTo(double[] array, int arrayIndex)
    {
        ((ICollection<double>)memory).CopyTo(array, arrayIndex);
    }

    public bool Remove(double item)
    {
        return ((ICollection<double>)memory).Remove(item);
    }
}

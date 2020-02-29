using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public static class GradientDescentConstStep
{
    public static void Main(string[] args)
    {
        Step1(out double E, out double h, out double[] x, out Func<double[], double> f, out Func<int, double[], double> df);
        Step2(out int k);
        Step3(in x, in f, out double fx);
        Step4(in x, in df, out double[] dfx);
        while (Step5(in E, in dfx, out double mdfx))
        {

        }
        Step8();
    }

    public static void Step1(out double E, out double h, out double[] x, out Func<double[], double> f, out Func<int, double[], double> df)
    {
        Console.WriteLine("Шаг 1.");
        E = 0.1;
        h = 0.4;
        x = new double[2] { 0.5, 0.5 };
        f = TargetFunction;
        df = GradientTargetFunction;
        Console.WriteLine($"ε = {E}; h = {h}.");
    }

    public static void Step2(out int k)
    {
        Console.WriteLine("Шаг 2.");
        k = 0;
        Console.WriteLine("k = {k}.");
    }

    public static void Step3(in double[] x, in Func<double[], double> f, out double fx)
    {
        Console.WriteLine("Шаг 3.");
        fx = f(x);
        Console.WriteLine($"f({string.Join("; ", x.EveryConverter(e => e.ToString("f3")))}) = {fx.ToString("f3")}.");
    }

    public static void Step4(in double[] x, in Func<int, double[], double> df, out double[] dfx)
    {
        Console.WriteLine("Шаг 4.");
        dfx = new double[x.Length];
        for (int i = 0; i < dfx.Length; i++)
            dfx[i] = df(i, x);
        Console.WriteLine($"Δf({string.Join("; ", x.EveryConverter(e => e.ToString("f3")))}) = ({string.Join("; ", dfx.EveryConverter(e => e.ToString("f3")))})");
    }

    public static bool Step5(in double E, in double[] dfx, out double mdfx)
    {
        Console.WriteLine("Шаг 5.");
        mdfx = 0;
        for (int i = 0; i < dfx.Length; i++)
            mdfx += dfx[i] * dfx[i];
        mdfx = Math.Sqrt(mdfx);
        return mdfx <= E;
    }

    public static double TargetFunction(double x, double y)
        => -3.3 * x +
        5.2 * Math.Pow(x, 2) -
        4.2 * y +
        2.8 * Math.Pow(y, 2);

    public static double TargetFunction(double[] args)
        => TargetFunction(args[0], args[1]);

    public static double GradientTargetFunctionX(double x, double y)
        => 10.4 * x - 3.3;

    public static double GradientTargetFunctionY(double x, double y)
        => 5.6 * y - 4.2;

    public static double GradientTargetFunction(int i, double[] args)
        => i == 0 ? GradientTargetFunctionX(args[0], args[1]) : i == 1 ? GradientTargetFunctionY(args[0], args[1]) : double.NaN;

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

public struct Element : IEnumerable<double>, IReadOnlyCollection<double>
{
    public double x { set; get; }
    public double y { set; get; }
    public double f { set; get; }

    public int Count => 3;

    public Element(double x, double y, double f)
    {
        this.x = x;
        this.y = y;
        this.f = f;
    }

    public IEnumerator<double> GetEnumerator()
    {
        yield return x;
        yield return y;
        yield return f;
    }

    IEnumerator IEnumerable.GetEnumerator()
        => GetEnumerator();
}
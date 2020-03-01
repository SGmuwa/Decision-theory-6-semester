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
    t4:
        Step4(in x, in df, out double[] dfx);
        if (Step5(in E, in x, in dfx, out double mdfx))  // modulus of delta f(x)
            goto t8;
    t6:
        Step6(in x, in h, in dfx, out double[] nx);
        if(Step7(ref x, in nx, in f, ref h, ref k, ref fx))
            goto t4;
        else
            goto t6;
    t8:
        Step8(in x, in fx);
    }

    public static void Step1(out double E, out double h, out double[] x, out Func<double[], double> f, out Func<int, double[], double> df)
    {
        Console.WriteLine("Шаг 1.");
        E = 0.1;
        h = 0.4;
        x = new double[2] { 0.5, 0.5 };
        f = TargetFunction;
        df = GradientTargetFunction;
        Console.WriteLine($"ε = {E:f3}; h = {h:f3}; x = {x.PointToString()}.");
    }

    public static void Step2(out int k)
    {
        Console.WriteLine("Шаг 2.");
        k = 0;
        Console.WriteLine($"k = {k}.");
    }

    public static void Step3(in double[] x, in Func<double[], double> f, out double fx)
    {
        Console.WriteLine("Шаг 3.");
        fx = f(x);
        Console.WriteLine($"f{x.PointToString()} = {fx:f3}.");
    }

    public static void Step4(in double[] x, in Func<int, double[], double> df, out double[] dfx)
    {
        Console.WriteLine("Шаг 4.");
        dfx = new double[x.Length];
        for (int i = 0; i < dfx.Length; i++)
            dfx[i] = df(i, x);
        Console.WriteLine($"Δf{x.PointToString()} = {dfx.PointToString()}");
    }

    public static bool Step5(in double E, in double[] x, in double[] dfx, out double mdfx)
    {
        Console.WriteLine("Шаг 5.");
        mdfx = 0;
        for (int i = 0; i < x.Length; i++)
            mdfx += dfx[i] * dfx[i];
        mdfx = Math.Sqrt(mdfx);
        Console.WriteLine($"||∇f{x.PointToString()}|| = √({string.Join(" + ", dfx.EveryConverter(e => e.ToString("f3") + '²'))}) = {mdfx:f3}; ε = {E:f3}.");
        if (mdfx <= E)
        {
            Console.WriteLine($"{mdfx:f3} ≤ {E:f3}: Переход к 8 шагу.");
            return true;
        }
        else
        {
            Console.WriteLine($"{mdfx:f3} > {E:f3}: Переход к 6 шагу.");
            return false;
        }
    }

    public static void Step6(in double[] x, in double h, in double[] dfx, out double[] nx)
    {
        Console.WriteLine("Шаг 6.");
        nx = new double[x.Length];
        Console.Write("x[k+1] = (");
        for (int i = 0; i < x.Length; i++)
        {
            nx[i] = x[i] - h * dfx[i];
            Console.Write($"{x[i]:f3} - {h:f3} * {dfx[i]:f3}{(i + 1 < x.Length ? "; " : ") = ")}");
        }
        Console.WriteLine($"{nx.PointToString()}.");
    }

    public static bool Step7(ref double[] x, in double[] nx, in Func<double[], double> f, ref double h, ref int k, ref double fx)
    {
        Console.WriteLine("Шаг 7.");
        double nfx = f(nx);
        Console.WriteLine($"f{nx.PointToString()} = {nfx:f3}.");
        if(nfx < fx)
        {
            k++;
            fx = nfx;
            x = nx;
            Console.WriteLine($"{nfx:f3} < {fx:f3}: k = {k}, переход к шагу 4.");
            return true;
        }
        else
        {
            Console.WriteLine($"{nfx:f3} ≥ {fx:f3}: h = {h:f3} / 2 = {h/=2:f3}; переход к шагу 6.");
            return false;
        }
    }

    public static void Step8(in double[] x, in double fx)
        => Console.WriteLine($"Шаг 8.\nОтвет: f{x.PointToString()} = {fx:f3}");

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

    internal static string PointToString(this double[] x, string format = "f3")
        => $"({string.Join("; ", x.EveryConverter(e => e.ToString(format)))})";

    internal static IEnumerable<O> EveryConverter<T, O>(this IEnumerable<T> that, Func<T, O> converter)
    {
        foreach (var e in that)
            yield return converter(e);
    }
}

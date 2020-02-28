using System;
using System.Collections.Generic;

public class GradientDescentConstStep {
    public static void Main(string[] args) {
        double E = 0.1;
        double h = 0.4;
        double gradientNorm;
        int n=2;
        double[] mas = new double[n];//координаты текущей точки
        for(int i=0;i<n;i++)
            mas[i] = 0;
        double[] gradientMas= new double[n];
        Stack<Element> stack = new Stack<Element>();
        double func = objectiveFunction(mas);
        Element elem = new Element(mas[0],mas[1],func);
        stack.Push(elem);
        gradientMas = gradientSearch(mas);
        double funcResMas;
        do{
            do{
                for(int i=0;i<n; i++){
                    double doubleTempMas = mas[i];
                    mas[i] = doubleTempMas - h*gradientMas[i];
                }
                funcResMas = objectiveFunction(mas);
                if(stack.Peek().result<funcResMas){//уменьшение шага 
                    double tempH = h;
                    h=tempH/2;
                    mas[0] = stack.Peek().x1;
                    mas[1] = stack.Peek().x2;
                }
            }while(stack.Peek().result<funcResMas);
            
            elem = new Element(mas[0],mas[1],funcResMas);
            stack.Push(elem);
        
            gradientMas = gradientSearch(mas);
            gradientNorm =0;
            for(int i=0;i<n; i++)
                gradientNorm+=Math.Pow(gradientMas[i],2);
            double grTemp=gradientNorm;
            gradientNorm = Math.Sqrt(grTemp);
        }while(gradientNorm>E);
        
       
        Console.WriteLine("ОТВЕТ: x1 = "+stack.Peek().x1+"\t x2 = "+stack.Peek().x2+"\t f = "+stack.Peek().result);
        Console.WriteLine("\nИстория рассматриваемых точек (от минимальной до первой рассмариваемой)");
        int sizeStack = stack.Count;
        for(int i=0;i<sizeStack ;i++){
            Element el= stack.Pop();
            Console.WriteLine("x1 = "+el.x1+"\t x2 = "+el.x2+"\t f = "+el.result);
        }
    }
    public static double objectiveFunction (double[] mas){
        return ((double) 13/5)*(Math.Pow(mas[0],2)) - ((double) 21/10)*mas[1] + ((double) 7/5)*(Math.Pow(mas[1],2)) ;
    }
    public static double[] gradientSearch (double[] mas){
        double[] masResult=new double[mas.Length];
        double delta = 0.000000001;
        double[] masTemp = new double[mas.Length];
        for(int i=0;i<mas.Length;i++)
            masTemp[i] = mas[i];
        for(int i=0;i<mas.Length;i++){
            masTemp[i]+=delta;
            double elem = (double) (objectiveFunction(masTemp) -objectiveFunction(mas))/delta;
            masResult[i]=elem;
            for(int j=0;j<mas.Length;j++)
                masTemp[j] = mas[j];
        }
        return masResult;    
    }
}

public class Element {
    public double x1;
    public double x2;
    public double result;
    public Element(double x1, double x2,double result) {
        this.x1 = x1;
        this.x2 = x2;
       this.result = result;
    }
}

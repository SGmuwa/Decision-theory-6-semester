
using System;
public class TPR_6sem_2Laba {
    public static void Main(string[] args) {
        int n = 2; int tempN = 0;
        double E = 0.1;
        double m = 1.0;//длина ребра многогранника
        double B = 2.8;//коэффициент растяжения
        double Y = 0.4;//коэффициент сжатия
        double [] mas = new double[n];
        mas[0] = 0.275;
        mas[1] = 0.8058;	
        
        double[, ] tableSimplex = new double[n+1, n+1];
        
        for(int i = 0; i < n+1; i++)
            for(int j=0;j<n;j++)
                if(i==0) tableSimplex[i, j] = mas[j];
                else tableSimplex[i, j] = -0;
        tableSimplex[0, n] = objectiveFunction(mas);
        //приращения
        double increment1 = (double) (Math.Sqrt(n+1)-1)/(n*Math.Sqrt(2))*m;
        double increment2 = (double) (Math.Sqrt(n+1)+n-1)/(n*Math.Sqrt(2))*m;
        //находим координаты остальных вершин
        while(tempN<n){
            double[] x = new double[n];
            for(int i=0;i<n;i++)
                if(tempN == i ) x[i] = increment1 + mas[i];
                else x[i] = increment2 + mas[i];
            for(int i = 0; i < n+1; i++)
                for(int j=0;j<n;j++)
                    if(i==tempN+1) tableSimplex[i, j] = x[j];
            tableSimplex[tempN+1, n] = objectiveFunction(x);
            tempN++;
        }
        //вывод таблицы
        for (int i = 0; i < n+1; i++) {
            for (int t = 0; t < n+1; t++) {
                Console.Write(tableSimplex[i, t] + "\t");
            }
            Console.WriteLine();
        }
     
        //ПУНКТ 3
        while(true){
            double[] arrayFuncValue = new double[n+1];
            double[] centerOfGravityXc = new double[n]; for(int k = 0;k<n;k++) centerOfGravityXc[k] = 0;
            double[] reflectedVertex = new double[n];//Координаты отраженной вершины

            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n+1; t++) {
                    if(t==n) arrayFuncValue[i] = tableSimplex[i, t];
                }
            }
            //Определяем максимальную, минимальную и следующую за максимальной вершиной
            int maxVertex = maxSearch(arrayFuncValue);
            int minValObjFunction = minSearch(arrayFuncValue);
            int followMaxIndex = followMaxValueSearch(arrayFuncValue);

            double maxValue = tableSimplex[maxVertex, n];
            double followMaxValue = tableSimplex[followMaxIndex, n];
            double minValue = tableSimplex[minValObjFunction, n];
            Console.WriteLine("\n\nМаксимальное значение Fh = "+maxValue);
            Console.WriteLine("Следующее за максимальным значением Fs = "+followMaxValue);
            Console.WriteLine("Минимальнок значение Fl = "+minValue);
            
            int currentId = maxVertex;

            //Считаем центр тяжести вершин симплекса (кроме максимальной)
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n; t++) {
                    if (i == maxVertex) mas[t] = tableSimplex[i, t];
                    else centerOfGravityXc[t] = centerOfGravityXc[t] + ((double) 1/n)*tableSimplex[i, t];
                }
            }
            //Находим координаты отраженной вершины
            for(int i=0;i<n;i++){
                reflectedVertex[i] = 2*centerOfGravityXc[i] - mas[i];
            }
            double fReflected = objectiveFunction(reflectedVertex);
            //Console.WriteLine("\n\n"+"Центр тяжести вершин симплекса, за исключением xk: "+centerOfGravityXc[0]+" \t"+centerOfGravityXc[1]);
            //Console.WriteLine("Координаты отраженной вершины: "+reflectedVertex[0]+"\t"+reflectedVertex[1]);
            Console.WriteLine("Значение функции в отраженной вершине: "+fReflected);
            double currentF = fReflected;

            //ПУНКТ 6
            if(fReflected < maxValue){
                //Заменяем рершину в таблице
                for(int i = 0; i < n+1; i++)
                    for(int j=0;j<n;j++)
                        if(i==maxVertex) {tableSimplex[i, j] = reflectedVertex[j];}
                tableSimplex[maxVertex, n] = fReflected;
                Console.WriteLine("После замены вершины на отраженную:");
                printMatrix(tableSimplex,n);
                //ПУНКТ 7
                if(currentF<minValue){
                    //Операция растяжения
                    double[] newVertexsStretch = new double[n]; 
                    for(int i = 0; i < n+1; i++)
                        for(int j=0;j<n;j++)
                            if(i==currentId) mas[j] = tableSimplex[i, j];
                    for(int r=0;r<n;r++){
                        newVertexsStretch[r] = centerOfGravityXc[r] + B*(mas[r]-centerOfGravityXc[r]);
                    }
                    Console.WriteLine("Значение функции после растяжения: "+objectiveFunction(newVertexsStretch));
                    //Пункт 8
                    if(objectiveFunction(newVertexsStretch)<currentF){
                        Console.WriteLine("Пункт 8");
                        //Заменяем рершину в таблице
                        for(int i = 0; i < n+1; i++)
                            for(int j=0;j<n;j++)
                                if(i==currentId) {tableSimplex[i, j] = newVertexsStretch[j];}
                        tableSimplex[currentId, n] = objectiveFunction(newVertexsStretch);
                        Console.WriteLine("После замены вершины на отраженную:");
                        printMatrix(tableSimplex,n);
                        bool proove = punkt12(tableSimplex, n, E);
                        if(proove == true) return;
                    }
                    else {
                        currentF = objectiveFunction(newVertexsStretch);//
                        bool proove = punkt9(tableSimplex,Y,E, currentF,currentId, maxValue, followMaxValue, n,centerOfGravityXc);
                        if(proove == true) return;
                    }
                } 
                else {
                    bool proove = punkt9(tableSimplex,Y,E, currentF,currentId, maxValue, followMaxValue, n,centerOfGravityXc);
                    if(proove == true) return;
                }
            }
            else {
                bool proove = punkt9(tableSimplex,Y,E, currentF,currentId, maxValue, followMaxValue, n,centerOfGravityXc);
                if(proove == true) return;
            }
        }
    }
    public static bool punkt9(double[, ] tableSimplex,double Y, double E,double currentF , int currentId,double maxValue, double followMaxValue, int n, double[] centerOfGravityXc){
        Console.WriteLine("Следующее за максимальным = "+followMaxValue+" \t f(x) = "+currentF+"\t максимальное = "+maxValue);
        double[] mas = new double[n];
        double[] arrayFuncValue = new double[n+1];
        if(currentF<maxValue & currentF > followMaxValue) {
            //Сжатие симплекса
            double[] newVertexsCompress = new double[n]; 
            for(int i = 0; i < n+1; i++)
                for(int j=0;j<n;j++)
                    if(i==currentId) mas[j] = tableSimplex[i, j];
            for(int r=0;r<n;r++){
                newVertexsCompress[r] = centerOfGravityXc[r] + Y*(mas[r]-centerOfGravityXc[r]);
            }
            Console.WriteLine("Сжатая вершина: "+ newVertexsCompress[0] + "   "+newVertexsCompress[1]+"    "+objectiveFunction(newVertexsCompress));
            Console.WriteLine("ТУТ"+ tableSimplex[currentId, n] + "   "+ currentId);
            //ПУНКТ 10
            if(objectiveFunction(newVertexsCompress)<tableSimplex[currentId, n]){
                //Заменяем рершину в таблице
                int tempInd =-1;
                for(int i = 0; i < n+1; i++)
                    for(int j=0;j<n;j++)
                        if(i==currentId) {
                            tableSimplex[i, j] = newVertexsCompress[j];
                            tempInd = i;
                        }
                tableSimplex[tempInd, n] = objectiveFunction(newVertexsCompress);
                //ПУНКТ 12
                bool proove = punkt12(tableSimplex, n, E);
                if(proove == true) return true;
            }
            //ПУНКТ 11
            else{//Редукция
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        if(t==n) arrayFuncValue[i] = tableSimplex[i, t];
                    }
                }
                int minVertex = minSearch(arrayFuncValue);
                Console.WriteLine("Редукция. Индекс минимальной вершины в таблице: "+minVertex);
                for (int i = 0; i < n+1; i++) 
                    for (int t = 0; t < n+1; t++) {
                        if(t!=n & i!=minVertex) {
                            double temp = tableSimplex[i, t];
                            tableSimplex[i, t] = tableSimplex[minVertex, t]+((double)1/2)*(temp-tableSimplex[minVertex, t]);
                            mas[t] = tableSimplex[i, t];
                        }
                        else{ if (t==n & i!=minVertex) tableSimplex[i, t] = objectiveFunction(mas);
                            for(int k =0;k<mas.Length;k++) mas[k]=0;
                        }
                    }
                //Вывод таблицы
                Console.WriteLine("Таблица после редукции:");
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        Console.Write(tableSimplex[i, t] + "\t");
                    }
                    Console.WriteLine();
                } 
                bool proove = punkt12(tableSimplex, n, E);
                if(proove == true) return true;
            }        
        }else { //Редукция
            // ПУНКТ 11
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n+1; t++) {
                    if(t==n) arrayFuncValue[i] = tableSimplex[i, t];
                }
            }
            int minVertex = minSearch(arrayFuncValue);
            Console.WriteLine("Редукция. Индекс минимальной вершины в таблице: "+minVertex);
            for (int i = 0; i < n+1; i++) 
                for (int t = 0; t < n+1; t++) {
                    if(t!=n & i!=minVertex) {
                        double temp = tableSimplex[i, t];
                        tableSimplex[i, t] = tableSimplex[minVertex, t]+((double)1/2)*(temp-tableSimplex[minVertex, t]);
                        mas[t] = tableSimplex[i, t];
                    }
                    else{ if (t==n & i!=minVertex) tableSimplex[i, t] = objectiveFunction(mas);
                        for(int k =0;k<mas.Length;k++) mas[k]=0;
                    }
                }
            //Вывод таблицы
            Console.WriteLine("Таблица после редукции:");
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n+1; t++) {
                    Console.Write(tableSimplex[i, t] + "\t");
                }
                Console.WriteLine();
            } 
            bool proove = punkt12(tableSimplex, n, E);
            if(proove == true) return true;
        }
        return false;
    }

    public static bool punkt12(double[, ] tableSimplex, int n, double E){
        //найдем ценрт тяжести всего симплекса
        Console.WriteLine("\n\nПроверка окончания поиска");
        double[] centerOfGravityXc = new double[n];
        double[] arrayFuncValue = new double[n+1];
        for(int i=0;i<centerOfGravityXc.Length;i++)
            centerOfGravityXc[i]=0;
        for (int i = 0; i < n+1; i++)
            for (int t = 0; t < n; t++) 
                centerOfGravityXc[t] = centerOfGravityXc[t] + ((double) 1/(n+1))* tableSimplex[i, t];
        double fxc = objectiveFunction(centerOfGravityXc);
        Console.WriteLine("Центр тяжести симплекса:  "+fxc);
        double sigma =0;
        for (int i = 0; i < n+1; i++)
            for (int t = 0; t < n+1; t++)
                if(t==n) sigma = sigma+ ((double) 1/(n+1))* Math.Pow((tableSimplex[i, t]-fxc),2);
        sigma = Math.Sqrt(sigma);
        
        //ПУНКТ 13
        if(sigma<E){
            Console.Write("Поиск окончен так как: ");
            Console.WriteLine("sigma  = "+sigma+" < E=" + E);
            for (int i = 0; i < n+1; i++) 
                for (int t = 0; t < n+1; t++) 
                    if(t==n) arrayFuncValue[i] = tableSimplex[i, t];

            int min = minSearch(arrayFuncValue);
            double resultMin = tableSimplex[min, n];
            Console.WriteLine("Минимальная вершина: "+resultMin);
            return true;
        }
        else { 
            Console.WriteLine("Поиск продолжается так как: sigma  = "+sigma+" > E=" + E);
            return false;
        }
    }
    public static void printMatrix(double[, ] tableSimplex, int n){
        for (int i = 0; i < n+1; i++) {
            for (int t = 0; t < n+1; t++) {
                Console.Write(tableSimplex[i, t] + "\t");
            }
            Console.WriteLine();
        } 
    }
    public static double objectiveFunction (double[] mas){
       return ((double) 13/5)*(Math.Pow(mas[0],2)) - ((double) 21/10)*mas[1] + ((double) 7/5)*(Math.Pow(mas[1],2)) ;// 60 2 переменные
    }
   public static int maxSearch (double[] mas){
       double max = mas[0]; int ind = 0;
       for(int i=1;i<mas.Length;i++)
           if(max<mas[i]) {max= mas[i];ind = i;}
       return ind;
   }
   public static int followMaxValueSearch(double[] mas){
       double max = mas[0]; double follow = max; int indMax = 0; int indFollow =0;
       for(int i=1;i<mas.Length;i++){
            if(i==1 & max<mas[i]) {follow = max; max= mas[i];indMax = i;indFollow = 0;}
            if(mas[i]>max) {follow = max; indFollow = indMax;max=mas[i];indMax = i;}
            if(mas[i]<max & mas[i]>follow){follow = mas[i];indFollow = i;}
       }   
       return indFollow;
   }
   public static int minSearch (double[] mas){
       double min = mas[0]; int ind = 0;
       for(int i=1;i<mas.Length;i++)
           if(min>mas[i]) {min= mas[i];ind = i;}
       return ind;
   }
}

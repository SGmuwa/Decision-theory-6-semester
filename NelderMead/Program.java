package tpr_6sem_2laba;
public class TPR_6sem_2Laba {
    public static void main(String[] args) {
        int n = 2; int tempN = 0;
        double E = 0.1;
        double m = 1.0;//длина ребра многогранника
        double B = 2.8;//коэффициент растяжения
        double Y = 0.4;//коэффициент сжатия
        double [] mas = new double[n];
        mas[0] = 0.275;
        mas[1] = 0.8058;	
        
        double[][] tableSimplex = new double[n+1][n+1];
        
        for(int i = 0; i < n+1; i++)
            for(int j=0;j<n;j++)
                if(i==0) tableSimplex[i][j] = mas[j];
                else tableSimplex[i][j] = -0;
        tableSimplex[0][n] = objectiveFunction(mas);
        //приращения
        double increment1 = (double) (Math.sqrt(n+1)-1)/(n*Math.sqrt(2))*m;
        double increment2 = (double) (Math.sqrt(n+1)+n-1)/(n*Math.sqrt(2))*m;
        //находим координаты остальных вершин
        while(tempN<n){
            double[] x = new double[n];
            for(int i=0;i<n;i++)
                if(tempN == i ) x[i] = increment1 + mas[i];
                else x[i] = increment2 + mas[i];
            for(int i = 0; i < n+1; i++)
                for(int j=0;j<n;j++)
                    if(i==tempN+1) tableSimplex[i][j] = x[j];
            tableSimplex[tempN+1][n] = objectiveFunction(x);
            tempN++;
        }
        //вывод таблицы
        for (int i = 0; i < n+1; i++) {
            for (int t = 0; t < n+1; t++) {
                System.out.print(tableSimplex[i][t] + "\t");
            }
            System.out.println();
        }
     
        //ПУНКТ 3
        while(true){
            double[] arrayFuncValue = new double[n+1];
            double[] centerOfGravityXc = new double[n]; for(int k = 0;k<n;k++) centerOfGravityXc[k] = 0;
            double[] reflectedVertex = new double[n];//Координаты отраженной вершины

            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n+1; t++) {
                    if(t==n) arrayFuncValue[i] = tableSimplex[i][t];
                }
            }
            //Определяем максимальную, минимальную и следующую за максимальной вершиной
            int maxVertex = maxSearch(arrayFuncValue);
            int minValObjFunction = minSearch(arrayFuncValue);
            int followMaxIndex = followMaxValueSearch(arrayFuncValue);

            double maxValue = tableSimplex[maxVertex][n];
            double followMaxValue = tableSimplex[followMaxIndex][n];
            double minValue = tableSimplex[minValObjFunction][n];
            System.out.println("\n\nМаксимальное значение Fh = "+maxValue);
            System.out.println("Следующее за максимальным значением Fs = "+followMaxValue);
            System.out.println("Минимальнок значение Fl = "+minValue);
            
            int currentId = maxVertex;

            //Считаем центр тяжести вершин симплекса (кроме максимальной)
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n; t++) {
                    if (i == maxVertex) mas[t] = tableSimplex[i][t];
                    else centerOfGravityXc[t] = centerOfGravityXc[t] + ((double) 1/n)*tableSimplex[i][t];
                }
            }
            //Находим координаты отраженной вершины
            for(int i=0;i<n;i++){
                reflectedVertex[i] = 2*centerOfGravityXc[i] - mas[i];
            }
            double fReflected = objectiveFunction(reflectedVertex);
            //System.out.println("\n\n"+"Центр тяжести вершин симплекса, за исключением xk: "+centerOfGravityXc[0]+" \t"+centerOfGravityXc[1]);
            //System.out.println("Координаты отраженной вершины: "+reflectedVertex[0]+"\t"+reflectedVertex[1]);
            System.out.println("Значение функции в отраженной вершине: "+fReflected);
            double currentF = fReflected;

            //ПУНКТ 6
            if(fReflected < maxValue){
                //Заменяем рершину в таблице
                for(int i = 0; i < n+1; i++)
                    for(int j=0;j<n;j++)
                        if(i==maxVertex) {tableSimplex[i][j] = reflectedVertex[j];}
                tableSimplex[maxVertex][n] = fReflected;
                System.out.println("После замены вершины на отраженную:");
                printMatrix(tableSimplex,n);
                //ПУНКТ 7
                if(currentF<minValue){
                    //Операция растяжения
                    double[] newVertexsStretch = new double[n]; 
                    for(int i = 0; i < n+1; i++)
                        for(int j=0;j<n;j++)
                            if(i==currentId) mas[j] = tableSimplex[i][j];
                    for(int r=0;r<n;r++){
                        newVertexsStretch[r] = centerOfGravityXc[r] + B*(mas[r]-centerOfGravityXc[r]);
                    }
                    System.out.println("Значение функции после растяжения: "+objectiveFunction(newVertexsStretch));
                    //Пункт 8
                    if(objectiveFunction(newVertexsStretch)<currentF){
                        System.out.println("Пункт 8");
                        //Заменяем рершину в таблице
                        for(int i = 0; i < n+1; i++)
                            for(int j=0;j<n;j++)
                                if(i==currentId) {tableSimplex[i][j] = newVertexsStretch[j];}
                        tableSimplex[currentId][n] = objectiveFunction(newVertexsStretch);
                        System.out.println("После замены вершины на отраженную:");
                        printMatrix(tableSimplex,n);
                        boolean proove = punkt12(tableSimplex, n, E);
                        if(proove == true) return;
                    }
                    else {
                        currentF = objectiveFunction(newVertexsStretch);//
                        boolean proove = punkt9(tableSimplex,Y,E, currentF,currentId, maxValue, followMaxValue, n,centerOfGravityXc);
                        if(proove == true) return;
                    }
                } 
                else {
                    boolean proove = punkt9(tableSimplex,Y,E, currentF,currentId, maxValue, followMaxValue, n,centerOfGravityXc);
                    if(proove == true) return;
                }
            }
            else {
                boolean proove = punkt9(tableSimplex,Y,E, currentF,currentId, maxValue, followMaxValue, n,centerOfGravityXc);
                if(proove == true) return;
            }
        }
    }
    public static boolean punkt9(double tableSimplex[][],double Y, double E,double currentF , int currentId,double maxValue, double followMaxValue, int n, double centerOfGravityXc[]){
        System.out.println("Следующее за максимальным = "+followMaxValue+" \t f(x) = "+currentF+"\t максимальное = "+maxValue);
        double[] mas = new double[n];
        double[] arrayFuncValue = new double[n+1];
        if(currentF<maxValue & currentF > followMaxValue) {
            //Сжатие симплекса
            double[] newVertexsCompress = new double[n]; 
            for(int i = 0; i < n+1; i++)
                for(int j=0;j<n;j++)
                    if(i==currentId) mas[j] = tableSimplex[i][j];
            for(int r=0;r<n;r++){
                newVertexsCompress[r] = centerOfGravityXc[r] + Y*(mas[r]-centerOfGravityXc[r]);
            }
            System.out.println("Сжатая вершина: "+ newVertexsCompress[0] + "   "+newVertexsCompress[1]+"    "+objectiveFunction(newVertexsCompress));
            System.out.println("ТУТ"+ tableSimplex[currentId][n] + "   "+ currentId);
            //ПУНКТ 10
            if(objectiveFunction(newVertexsCompress)<tableSimplex[currentId][n]){
                //Заменяем рершину в таблице
                int tempInd =-1;
                for(int i = 0; i < n+1; i++)
                    for(int j=0;j<n;j++)
                        if(i==currentId) {
                            tableSimplex[i][j] = newVertexsCompress[j];
                            tempInd = i;
                        }
                tableSimplex[tempInd][n] = objectiveFunction(newVertexsCompress);
                //ПУНКТ 12
                boolean proove = punkt12(tableSimplex, n, E);
                if(proove == true) return true;
            }
            //ПУНКТ 11
            else{//Редукция
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        if(t==n) arrayFuncValue[i] = tableSimplex[i][t];
                    }
                }
                int minVertex = minSearch(arrayFuncValue);
                System.out.println("Редукция. Индекс минимальной вершины в таблице: "+minVertex);
                for (int i = 0; i < n+1; i++) 
                    for (int t = 0; t < n+1; t++) {
                        if(t!=n & i!=minVertex) {
                            double temp = tableSimplex[i][t];
                            tableSimplex[i][t] = tableSimplex[minVertex][t]+((double)1/2)*(temp-tableSimplex[minVertex][t]);
                            mas[t] = tableSimplex[i][t];
                        }
                        else{ if (t==n & i!=minVertex) tableSimplex[i][t] = objectiveFunction(mas);
                            for(int k =0;k<mas.length;k++) mas[k]=0;
                        }
                    }
                //Вывод таблицы
                System.out.println("Таблица после редукции:");
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        System.out.print(tableSimplex[i][t] + "\t");
                    }
                    System.out.println();
                } 
                boolean proove = punkt12(tableSimplex, n, E);
                if(proove == true) return true;
            }        
        }else { //Редукция
            // ПУНКТ 11
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n+1; t++) {
                    if(t==n) arrayFuncValue[i] = tableSimplex[i][t];
                }
            }
            int minVertex = minSearch(arrayFuncValue);
            System.out.println("Редукция. Индекс минимальной вершины в таблице: "+minVertex);
            for (int i = 0; i < n+1; i++) 
                for (int t = 0; t < n+1; t++) {
                    if(t!=n & i!=minVertex) {
                        double temp = tableSimplex[i][t];
                        tableSimplex[i][t] = tableSimplex[minVertex][t]+((double)1/2)*(temp-tableSimplex[minVertex][t]);
                        mas[t] = tableSimplex[i][t];
                    }
                    else{ if (t==n & i!=minVertex) tableSimplex[i][t] = objectiveFunction(mas);
                        for(int k =0;k<mas.length;k++) mas[k]=0;
                    }
                }
            //Вывод таблицы
            System.out.println("Таблица после редукции:");
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n+1; t++) {
                    System.out.print(tableSimplex[i][t] + "\t");
                }
                System.out.println();
            } 
            boolean proove = punkt12(tableSimplex, n, E);
            if(proove == true) return true;
        }
        return false;
    }

    public static boolean punkt12(double tableSimplex[][], int n, double E){
        //найдем ценрт тяжести всего симплекса
        System.out.println("\n\nПроверка окончания поиска");
        double[] centerOfGravityXc = new double[n];
        double[] arrayFuncValue = new double[n+1];
        for(int i=0;i<centerOfGravityXc.length;i++)
            centerOfGravityXc[i]=0;
        for (int i = 0; i < n+1; i++)
            for (int t = 0; t < n; t++) 
                centerOfGravityXc[t] = centerOfGravityXc[t] + ((double) 1/(n+1))* tableSimplex[i][t];
        double fxc = objectiveFunction(centerOfGravityXc);
        System.out.println("Центр тяжести симплекса:  "+fxc);
        double sigma =0;
        for (int i = 0; i < n+1; i++)
            for (int t = 0; t < n+1; t++)
                if(t==n) sigma = sigma+ ((double) 1/(n+1))* Math.pow((tableSimplex[i][t]-fxc),2);
        sigma = Math.sqrt(sigma);
        
        //ПУНКТ 13
        if(sigma<E){
            System.out.print("Поиск окончен так как: ");
            System.out.println("sigma  = "+sigma+" < E=" + E);
            for (int i = 0; i < n+1; i++) 
                for (int t = 0; t < n+1; t++) 
                    if(t==n) arrayFuncValue[i] = tableSimplex[i][t];

            int min = minSearch(arrayFuncValue);
            double resultMin = tableSimplex[min][n];
            System.out.println("Минимальная вершина: "+resultMin);
            return true;
        }
        else { 
            System.out.println("Поиск продолжается так как: sigma  = "+sigma+" > E=" + E);
            return false;
        }
    }
    public static void printMatrix(double tableSimplex[][], int n){
        for (int i = 0; i < n+1; i++) {
            for (int t = 0; t < n+1; t++) {
                System.out.print(tableSimplex[i][t] + "\t");
            }
            System.out.println();
        } 
    }
    public static double objectiveFunction (double mas[]){
       return ((double) 13/5)*(Math.pow(mas[0],2)) - ((double) 21/10)*mas[1] + ((double) 7/5)*(Math.pow(mas[1],2)) ;// 60 2 переменные
    }
   public static int maxSearch (double mas[]){
       double max = mas[0]; int ind = 0;
       for(int i=1;i<mas.length;i++)
           if(max<mas[i]) {max= mas[i];ind = i;}
       return ind;
   }
   public static int followMaxValueSearch(double mas[]){
       double max = mas[0]; double follow = max; int indMax = 0; int indFollow =0;
       for(int i=1;i<mas.length;i++){
            if(i==1 & max<mas[i]) {follow = max; max= mas[i];indMax = i;indFollow = 0;}
            if(mas[i]>max) {follow = max; indFollow = indMax;max=mas[i];indMax = i;}
            if(mas[i]<max & mas[i]>follow){follow = mas[i];indFollow = i;}
       }   
       return indFollow;
   }
   public static int minSearch (double mas[]){
       double min = mas[0]; int ind = 0;
       for(int i=1;i<mas.length;i++)
           if(min>mas[i]) {min= mas[i];ind = i;}
       return ind;
   }
}

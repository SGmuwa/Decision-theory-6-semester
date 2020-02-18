package tpr_6sem_1laba;
public class TPR_6sem_1Laba {
    public static void main(String[] args) {
        int it =0;//Счётчик итераций
        int n = 2; int tempN = 0;
        double E = 0.1;
        double m = 0.25;
        double [] mas = new double[n];
        mas[0] = 0.275;
        mas[1] = 0.8058;
        double[][] tableSimplex = new double[n+1][n+1];
        for(int i = 0; i < n+1; i++)
            for(int j=0;j<n;j++)
                if(i==0) tableSimplex[i][j] = mas[j];
                else tableSimplex[i][j] = -0;
        tableSimplex[0][n] = objectiveFunction(mas);
        
        double increment1 = (double) (Math.sqrt(n+1)-1)/(n*Math.sqrt(2))*m;
        double increment2 = (double) (Math.sqrt(n+1)+n-1)/(n*Math.sqrt(2))*m; 
        //Вычисление координат остальных вершин симплекса
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
        for (int i = 0; i < n+1; i++) {
            for (int t = 0; t < n+1; t++) {
                System.out.print(tableSimplex[i][t] + "\t");
            }
            System.out.println();
        }
        while(true){
            System.out.println("\n"+"\n"+"Итерация k = "+ it +"---------------------------");
            it++;
            double[] arrayFuncValue = new double[n+1];
            double[] centerOfGravityXc = new double[n];//Координаты центра тяжести
            for(int k = 0;k<n;k++) centerOfGravityXc[k] = 0;
            double[] reflectedVertex = new double[n]; //Координаты отраженной вершины
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n+1; t++) {
                    if(t==n) arrayFuncValue[i] = tableSimplex[i][t];
                }
            }
            int maxVertex = maxSearch(arrayFuncValue);
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n; t++) {
                    if (i == maxVertex) mas[t] = tableSimplex[i][t];
                    else centerOfGravityXc[t] = centerOfGravityXc[t] + ((double) 1/n)*tableSimplex[i][t];
                }
            }
            //Координаты отраженной вершины
            for(int i=0;i<n;i++){
                reflectedVertex[i] = 2*centerOfGravityXc[i] - mas[i];
            }
            double Fxc=0;
            System.out.println("Значение функции максимальной точки симлекса:"+ objectiveFunction(mas)+";     Значение функции в отраженной вершине:"+objectiveFunction(reflectedVertex));
            if(objectiveFunction(reflectedVertex)< objectiveFunction(mas)){//Наблюдается уменьшение целевой функции
                //Проверим условие окончания поиска
                //0) Заменяем рершину в таблице
                for(int i = 0; i < n+1; i++)
                    for(int j=0;j<n;j++)
                        if(i==maxVertex) tableSimplex[i][j] = reflectedVertex[j];
                tableSimplex[maxVertex][n] = objectiveFunction(reflectedVertex);
                //1) Вывод таблицы и поиск центра тяжести
                System.out.println();
                for(int i=0;i<centerOfGravityXc.length;i++)
                    centerOfGravityXc[i]=0;
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        if(t<n){
                            centerOfGravityXc[t] = centerOfGravityXc[t] + ((double) 1/(n+1))* tableSimplex[i][t];
                        }
                        System.out.print(tableSimplex[i][t] + "\t");
                    }
                    System.out.println();
                }
                Fxc = objectiveFunction(centerOfGravityXc);
                System.out.println("Центр тяжести симплекса: "+Fxc);
                System.out.print("Координаты центра тяжести симплекса: ");
                for(int i=0;i<centerOfGravityXc.length;i++) System.out.print(centerOfGravityXc[i]+"\t");
            }
            else  {//Иначе не заменяем вершину в таблице. Выполняется операция редукции
                System.out.println("НЕОБХОДИМА операция редукции");
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        if(t==n) arrayFuncValue[i] = tableSimplex[i][t];
                    }
                }
                int minVertex = minSearch(arrayFuncValue);
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        if(t!=n & i!=minVertex) {
                            double temp = tableSimplex[i][t];
                            tableSimplex[i][t] = tableSimplex[minVertex][t]+((double)1/2)*(temp-tableSimplex[minVertex][t]);
                            mas[t] = tableSimplex[i][t];
                        }
                        else{   if (t==n & i!=minVertex) tableSimplex[i][t] = objectiveFunction(mas);
                                for(int k =0;k<mas.length;k++) mas[k]=0;
                        }
                    }
                }
                //1) Вывод таблицы и поиск центра тяжести
                System.out.println();
                for(int i=0;i<centerOfGravityXc.length;i++)
                    centerOfGravityXc[i]=0;
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        if(t<n){
                            centerOfGravityXc[t] = centerOfGravityXc[t] + ((double) 1/(n+1))* tableSimplex[i][t];
                        }
                        System.out.print(tableSimplex[i][t] + "\t");
                    }
                    System.out.println();
                }
                Fxc = objectiveFunction(centerOfGravityXc);
                System.out.println("Центр тяжести симплекса: "+ Fxc);
            }
            //Проверка условий окончания поиска
            int checkEnd=0;
            for (int i = 0; i < n+1; i++) {
                for (int t = 0; t < n+1; t++) {
                    if(t==n){
                        if(Math.abs(tableSimplex[i][t] - Fxc)<E) checkEnd++;
                    }
                }
            }
            if(checkEnd==n+1) {
                for (int i = 0; i < n+1; i++) {
                    for (int t = 0; t < n+1; t++) {
                        if(t==n) arrayFuncValue[i] = tableSimplex[i][t];
                    }
                }
                int minVertex = minSearch(arrayFuncValue);
                double resultMin = tableSimplex[minVertex][n];
                System.out.println("\n"+"\n"+"ОТВЕТ: минимальная вершина: "+resultMin);
                return;
            }
        }
    }
   public static double objectiveFunction (double mas[]){
      return ((double) 13/5)*(Math.pow(mas[0],2)) - ((double) 21/10)*mas[1] + ((double) 7/5)*(Math.pow(mas[1],2)) ; 
    }
   public static int maxSearch (double mas[]){
       double max = mas[0]; int ind = 0;
       for(int i=1;i<mas.length;i++)
           if(max<mas[i]) {max= mas[i];ind = i;}
       return ind;
   }
   public static int minSearch (double mas[]){
       double min = mas[0]; int ind = 0;
       for(int i=1;i<mas.length;i++)
           if(min>mas[i]) {min= mas[i];ind = i;}
       return ind;
   }
}

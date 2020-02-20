package tpr_6sem_3laba;
public class TPR_6sem_3Laba {
    public static void main(String[] args) {
        int n = 2;//Размерность
        int d=2;//Коэффициент уменьшения шага
        double E = 0.1;//Точность поиска
        double m=0.5;//Ускоряющий множитель
        double h=0.2;//Шаг
        
        double [] base_point = new double[n];//Координаты базисной точки
        double [] xP = new double[n];//Координаты точки для поиска по образцу
        double [] current_point = new double[n];//x1
        double [] test_point = new double[n];//координаты тестовых точек временно храняться в этом массиве
        
        for(int i=0;i<n;i++)
           base_point[i] = 0;
       
        //считаем тестовую, если она больше текущей, то производится поиск по следующим направлениям и координатам
        boolean k = true;
        while(k){
            for(int i=0; i<n;i++){
                if(i==0)
                    for(int j=0;j<n;j++){
                        test_point[j]=base_point[j];//x1
                        current_point[j]=base_point[j];//x1
                    }
                for(int l=0;l<n;l++)
                    test_point[l]=current_point[l];//x1
                double temp=test_point[i];
                test_point[i]=temp+h;
                if(objectiveFunction(test_point)<objectiveFunction(current_point))
                    for(int j=0;j<n;j++){
                        current_point[j]=test_point[j];
                    }
                else{
                    for(int j=0;j<n;j++)
                        test_point[j]=current_point[j];
                    temp=test_point[i];
                    test_point[i]=temp-h;
                    if(objectiveFunction(test_point)<objectiveFunction(current_point)){
                        for(int j=0;j<n;j++){
                            current_point[j]=test_point[j];
                        }
                    }
                }
            }
            //сравнение с базисной точкой x0
            int flag=0;
            for(int i=0; i<n;i++)
                if(base_point[i]==current_point[i])
                    flag+=1;
            if(flag==n){//если х1 = х0, то уменьшаем шаг
                double tempH=h;
                h=(double) tempH/d;
                System.out.println("НЕ нашли точку (x0=x1)");
                System.out.println("Уменьшение шага, H = "+h+"\n\n");
            }
            else{//если х1!=х0, то поиск по образцу
                System.out.print("Новая базисная точка x1: "+current_point[0]+"   "+current_point[1]+"\n");
                for(int j=0;j<n;j++)
                    xP[j]=current_point[j]+(double)m*(current_point[j] - base_point[j]);
                if(objectiveFunction(xP)<objectiveFunction(current_point)){
                    for(int j=0;j<n;j++)
                        base_point[j] = xP[j];
                    System.out.println("Базисная точка х0 = хр: "+base_point[0]+"  "+base_point[1]);
                }
                else{
                    for(int j=0;j<n;j++)
                        base_point[j] = current_point[j];
                    System.out.println("Базисная точка х0 = х1: "+base_point[0]+"  "+base_point[1]);
                }
                System.out.print("Шаг  "+h+"\n \n");
                if(h<E){
                    System.out.println("Шаг = "+h+" < E = "+E);
                    System.out.println("Минимальная точка:"+ objectiveFunction(base_point));
                    k=false;
                }
            }
        } 
    }
    public static double objectiveFunction (double mas[]){
        
        return ((double) 13/5)*(Math.pow(mas[0],2)) - ((double) 21/10)*mas[1] + ((double) 7/5)*(Math.pow(mas[1],2)) ;
    }
   }

import java.util.Random;
import java.util.Arrays;
public class binarySearch{
    static int num = 10000000;
    static int rep = 10000;
    public static void main(String[] args){
        // make array
        int[] array = new int[num];
        Random random = new Random(num);
        for(int i=0 ; i<array.length; i++){
            array[i] = random.nextInt();
        }
        Arrays.sort(array);
        int checkedNum = random.nextInt();
    
        // search
        long begintime = System.nanoTime();
        int index=0;
        for(int i=0;i<rep;i++)
            index = Arrays.binarySearch(array,checkedNum);
        long endtime = System.nanoTime();
        double costTime = (double)(endtime - begintime)/1000000;

        // check search
        int test = -1;
        for(int i=0 ; i<array.length-1; i++){
            if(array[i] == checkedNum){
                test = i;
                break;
            }
        }
        if(test != index && test<0 != index<0){
            System.err.println("Java binary search failed");
            System.exit(1);
        }

        System.out.println("Java binary search takes "+ String.valueOf(costTime) +"ms");
    }
}


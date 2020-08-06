import java.util.Random;
import java.util.Arrays;
public class sort{
    static int num = 100000;
    static int rep = 100;
    public static void main(String[] args){
        // make array
        int[] array = new int[num];
        Random random = new Random(num);
        for(int i=0 ; i<array.length; i++){
            array[i] = random.nextInt();
        }

        // sort
        long begintime = System.nanoTime();
        for(int i=0;i<rep;i++)
            Arrays.sort(array);
        long endtime = System.nanoTime();
        double costTime = (double)(endtime - begintime)/1000000;

        // check sort
        for(int i=0 ; i<array.length-1; i++){
            if(array[i]>array[i+1]){
                System.err.println("Java sort failed");
                System.exit(1);
            }
        }

        System.out.println("Java sort takes "+ String.valueOf(costTime) +"ms");
    }
}


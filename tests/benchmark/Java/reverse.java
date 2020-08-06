import java.util.Random;
import java.util.Collections;
import java.util.ArrayList;

public class reverse{
    static int num = 10000000;
    static int rep = 11;
    public static void main(String[] args){
        // make array
        int[] array = new int[num];
        ArrayList<Integer> reverse = new ArrayList<Integer>();
        Random random = new Random(num);
        for(int i=0 ; i<array.length; i++){
            int temp = random.nextInt();
            array[i] = temp;
            reverse.add(temp);
        }
        
        // reverse
        long begintime = System.nanoTime();
        for(int i=0; i<rep; i++)
            Collections.reverse(reverse);
        long endtime = System.nanoTime();
        double costTime = (double)(endtime - begintime)/1000000;

        // check reverse
        int test = -1;
        for(int i=0 ; i < array.length; i++){
            if(array[i] != reverse.get(array.length-1-i)){
                System.err.println("Java reverse failed");
                System.exit(1);
            }
        }

        System.out.println("Java reverse takes "+ String.valueOf(costTime) +"ms");
    }
}


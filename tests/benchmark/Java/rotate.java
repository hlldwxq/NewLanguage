import java.util.Random;
import java.util.Collections;
import java.util.ArrayList;
public class rotate{
    static int num = 1000000; 
    static int rep = 101;
    public static void main(String[] args){
        // make array
        int[] array = new int[num];
        ArrayList<Integer> rotate = new ArrayList<Integer>();
        Random random = new Random(num);
        for(int i=0 ; i<array.length; i++){
            int temp = random.nextInt();
            array[i] = temp;
            rotate.add(temp);
        }
        int rotateLocation = random.nextInt(num);

        // rotate
        long begintime = System.nanoTime();
        for(int i=0 ; i<rep ; i++){
            if(i%2==0)
                Collections.rotate(rotate, rotateLocation);
            else
                Collections.rotate(rotate, rotate.size()-rotateLocation);
        }
        long endtime = System.nanoTime();
        double costTime = (double)(endtime - begintime)/1000000;

        // check rotate
        for(int i=0 ; i < array.length; i++){
            if(i < rotateLocation){
                if( rotate.get(i) != array[i+(array.length-rotateLocation)]){
                    System.err.println("Java rotate failed1");
                    System.exit(1);
                }
            }else{
                if( rotate.get(i) != array[i-rotateLocation]){
                    System.err.println("Java rotate failed2");
                    System.exit(1);
                }
            }
        }

        System.out.println("Java rotate takes "+ String.valueOf(costTime) +"ms");
    }
}


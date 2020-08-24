import java.util.Random;
import java.util.Collections;
import java.util.ArrayList;
import java.io.*;

public class rotate{
    static int num = 0;
    static int rep = 0;
    static int rotateLocation = 0;

    public static int[] readFile(String fileName, int[] array){
        File file = new File(fileName);
        
        try (BufferedReader br = new BufferedReader(new FileReader(file));) {
            String line = null;

            line = br.readLine();  //num count
            num = Integer.valueOf(line);

            line = br.readLine();  //rep
            rep = Integer.valueOf(line);

            line = br.readLine();  //rep
            rotateLocation = Integer.valueOf(line);
            
            array = new int[num];
            int i=0;
            while (i<num && (line = br.readLine()) != null) {
                array[i] = Integer.valueOf(line);
                i++;
            }

            if(i!=num){
                System.out.println("not enough number");
                System.exit(1);
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
        return array;
    }

    public static void main(String[] args){
        // make array
        int[] array = null;
        ArrayList<Integer> rotate = new ArrayList<Integer>();
        
        String fileName = "tests/benchmark/Java/rotate.txt";
        array = readFile(fileName, array);

        for(int i=0 ; i<array.length; i++){
            rotate.add(array[i]);
        }

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

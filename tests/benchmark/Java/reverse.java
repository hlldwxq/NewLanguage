import java.util.Random;
import java.util.Collections;
import java.util.ArrayList;
import java.io.*;
public class reverse{
    static int num = 0;
    static int rep = 0;

    public static int[] readFile(String fileName, int[] array){
        File file = new File(fileName);

        try (BufferedReader br = new BufferedReader(new FileReader(file));) {
            String line = null;

            line = br.readLine();  //num count
            num = Integer.valueOf(line);

            line = br.readLine();  //rep
            rep = Integer.valueOf(line);

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
        ArrayList<Integer> reverse = new ArrayList<Integer>();
        
        String fileName = "tests/benchmark/Java/reverse.txt";
        array = readFile(fileName, array);

        for(int i=0 ; i<array.length; i++){
            reverse.add(array[i]);
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


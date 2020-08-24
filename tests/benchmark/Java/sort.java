import java.util.Random;
import java.util.Arrays;
import java.io.*;
public class sort{
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
        String fileName = "tests/benchmark/Java/sort.txt";
        array = readFile(fileName, array);

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


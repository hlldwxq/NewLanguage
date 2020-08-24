import java.util.Random;
import java.util.Arrays;
import java.io.*;
public class binarySearch{
    static int num = 0;
    static int rep = 0;
    static int checkedNum = 0;

    public static int[] readFile(String fileName, int[] array){
        File file = new File(fileName);

        try (BufferedReader br = new BufferedReader(new FileReader(file));) {
            String line = null;

            line = br.readLine();  //num / count
            num = Integer.valueOf(line);

            line = br.readLine();  //rep
            rep = Integer.valueOf(line);

            line = br.readLine();  //checked num
            checkedNum = Integer.valueOf(line);

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
        String fileName = "tests/benchmark/Java/binarySearch.txt";

        array = readFile(fileName, array);

        Arrays.sort(array);
        //int checkedNum = random.nextInt();
    
        // search 
        int index=0;

        long begintime = System.nanoTime();

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


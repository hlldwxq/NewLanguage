import java.util.Random;
import java.util.Collections;
import java.util.ArrayList;
import java.io.*;
public class disjoint{
    static int num = 0;
    static int rep = 0;


    static long checksum(ArrayList<Integer> a) {
      long s=0;
      for (int i=0; i<a.size(); ++i) s+=(((long)i) * a.get(i));
      return s;
    }


    public static void readFile(String fileName, ArrayList<Integer> array1, ArrayList<Integer> array2){
        File file = new File(fileName);
        try (BufferedReader br = new BufferedReader(new FileReader(file));) {
            
            String line = null;

            line = br.readLine();  //num count
            num = Integer.valueOf(line);

            line = br.readLine();  //rep
            rep = Integer.valueOf(line);

            int i=0;
            while (i<num && (line = br.readLine()) != null) {
                array1.add(Integer.valueOf(line));
                i++;
            }

            i=0;
            while (i<num && (line = br.readLine()) != null) {
                array2.add(Integer.valueOf(line));
                i++;
            }

            if(i!=num){
                System.out.println("not enough number");
                System.exit(1);
            }

        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public static void main(String[] args){
        // make array
        ArrayList<Integer> first = new ArrayList<Integer>();
        ArrayList<Integer> second = new ArrayList<Integer>();

        String fileName = "tests/benchmark/Java/disjoint.txt";
        readFile(fileName, first, second);

        System.out.println("a: " + Long.toUnsignedString(checksum(first)));
        System.out.println("b: " + Long.toUnsignedString(checksum(second)));

        // disjoint
        boolean isDisjoint = true;
        long begintime = System.nanoTime();
        for(int i=0; i< rep; i++){
            isDisjoint = Collections.disjoint(first, second);
        }
        long endtime = System.nanoTime();
        double costTime = (double)(endtime - begintime)/1000000;

        // check disjoint
        boolean test = true;
        for(int i=0 ; i < first.size(); i++){
            for(int j=0 ; j < second.size(); j++){
 
                if((int)first.get(i) == (int)second.get(j)){
                    test = false;
                    break;
                }
            }
        }

        if( test != isDisjoint ){
            System.err.println("Java disjoint failed");
            System.exit(1);
        }

        System.out.println("Java disjoint takes "+ String.valueOf(costTime) +"ms");
    }
}


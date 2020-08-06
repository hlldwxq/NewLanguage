import java.util.Random;
import java.util.Collections;
import java.util.ArrayList;
public class disjoint{
    static int num = 10000;
    static int rep = 10;
    public static void main(String[] args){
        // make array
        ArrayList<Integer> first = new ArrayList<Integer>();
        ArrayList<Integer> second = new ArrayList<Integer>();
        Random random = new Random(num);
        for(int i=0 ; i<num; i++){
            first.add(random.nextInt());
            second.add(random.nextInt());
        }

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
        for(int i=0 ; i < num; i++){
            for(int j=0 ; j < num; j++){
                if(first.get(i)== second.get(j)){
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


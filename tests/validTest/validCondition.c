#include <stdio.h>
//return the big one
 long long cond_gt_var(long long a,long long b);
 long long cond_lt_var(long long a,long long b);
 long long cond_ge_var(long long a,long long b);
 long long cond_le_var(long long a,long long b);
//# if a==b return 1 else return 0
 long long cond_eq_var(long long a,long long b);
 long long cond_ne_var(long long a,long long b);
//#if a>0 return a else return 0
 long long cond_gt_varnum(long long a);
 long long cond_gt_numvar(long long a);
 long long cond_lt_numvar(long long a);
 long long cond_lt_varnum(long long a);
 long long cond_ge_varnum(long long a);
 long long cond_ge_numvar(long long a);
 long long cond_le_numvar(long long a);
 long long cond_le_varnum(long long a);
//# if a==0 return 0 else return a
 long long cond_eq_numvar(long long a);
 long long cond_eq_varnum(long long a);
 long long cond_ne_numvar(long long a);
 long long cond_ne_varnum(long long a);
//# return 1
 long long cond_true();
 long long cond_false();
 long long cond_and_truefalse();
 long long cond_and_falsetrue();
 long long cond_andtruetrue();
 long long cond_andfalsefalse();
 long long cond_ortruefalse();
 long long cond_ortruetrue();
 long long cond_orfalsetrue();
 long long cond_unary_false();
 long long cond_unary_true();


long long max(long long a,long long b){
    if(a>b)
    return a;
    else
    return b;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    
    long long fail = 0;
    //# return 1
    if( 1 != cond_true() ){ printf("cond_true fails to pass the test"); fail++;}
    if( 1 != cond_false() ){ printf("cond_false fails to pass the test"); fail++;}
    if( 1 != cond_and_truefalse() ){ printf("cond_and_truefalse fails to pass the test\n"); fail++;}
    if( 1 != cond_and_falsetrue() ){ printf("cond_and_falsetrue fails to pass the test\n"); fail++;}
    if( 1 != cond_andtruetrue() ){ printf("cond_andtruetrue fails to pass the testv\n"); fail++;}
    if( 1 != cond_andfalsefalse() ){ printf("cond_andfalsefalse fails to pass the test\n"); fail++;}
    if( 1 != cond_ortruefalse() ){ printf("cond_ortruefalse fails to pass the test\n"); fail++;}
    if( 1 != cond_ortruetrue() ){ printf("cond_ortruetrue fails to pass the test\n"); fail++;}
    if( 1 != cond_orfalsetrue() ){ printf("cond_orfalsetrue fails to pass the test\n"); fail++;}
    if( 1 != cond_unary_false() ){ printf("cond_unary_false fails to pass the test\n"); fail++;}
    if( 1 != cond_unary_true() ){ printf("cond_unary_true fails to pass the test\n"); fail++;}
    
   // fail = 0;
    long long a = 10;
    long long b = 1;
    //printf("positive and positive:\n");

    //return the big one
    if(max(a,b) != cond_gt_var(a,b) ){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_lt_var(a,b)){ printf("cond_lt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_ge_var(a,b)){ printf("cond_ge_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_le_var(a,b)){ printf("cond_le_var fails to pass the test\n"); fail++;}
    //# if a==b return 1 else return 0
    if((a==b) != cond_eq_var(a,b)){ printf("cond_eq_var fails to pass the test\n"); fail++;}
    if((a==b) != cond_ne_var(a,b)){ printf("cond_ne_var fails to pass the test\n"); fail++;}

    //# if a==0 return 0 else return a
    if( a != cond_eq_numvar( a)){ printf("cond_eq_numvar fails to pass the test\n"); fail++;}
    if( a != cond_eq_varnum( a)){ printf("cond_eq_varnum fails to pass the test\n"); fail++;}
    if( a != cond_ne_numvar( a)){ printf("cond_ne_numvar fails to pass the test\n"); fail++;}
    if( a != cond_ne_varnum( a)){ printf("cond_ne_varnum fails to pass the test\n"); fail++;}
    //#if a>0 return a else return 0
    if( max(a,0) != cond_gt_varnum( a)){ printf("cond_gt_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_gt_numvar( a)){ printf("cond_gt_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_lt_numvar( a)){ printf("cond_lt_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_lt_varnum( a)){ printf("cond_lt_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_ge_varnum( a)){ printf("cond_ge_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_ge_numvar( a)){ printf("cond_ge_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_le_numvar( a)){ printf("cond_le_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_le_varnum( a)){ printf("cond_le_varnum fails to pass the test\n"); fail++;}

  

    //fail = 0;
    a = -10;
    b = -1;
   // printf("negative and negative:\n");
    //return the big one
    if(max(a,b) != cond_gt_var(a,b) ){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_lt_var(a,b)){ printf("cond_lt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_ge_var(a,b)){ printf("cond_ge_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_le_var(a,b)){ printf("cond_le_var fails to pass the test\n"); fail++;}
    //# if a==b return 1 else return 0
    if((a==b) != cond_eq_var(a,b)){ printf("cond_eq_var fails to pass the test\n"); fail++;}
    if((a==b) != cond_ne_var(a,b)){ printf("cond_ne_var fails to pass the test\n"); fail++;}

    //# if a==0 return 0 else return a
    if( a != cond_eq_numvar( a)){ printf("cond_eq_numvar fails to pass the test\n"); fail++;}
    if( a != cond_eq_varnum( a)){ printf("cond_eq_varnum fails to pass the test\n"); fail++;}
    if( a != cond_ne_numvar( a)){ printf("cond_ne_numvar fails to pass the test\n"); fail++;}
    if( a != cond_ne_varnum( a)){ printf("cond_ne_varnum fails to pass the test\n"); fail++;}
    //#if a>0 return a else return 0
    if( max(a,0) != cond_gt_varnum( a)){ printf("cond_gt_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_gt_numvar( a)){ printf("cond_gt_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_lt_numvar( a)){ printf("cond_lt_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_lt_varnum( a)){ printf("cond_lt_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_ge_varnum( a)){ printf("cond_ge_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_ge_numvar( a)){ printf("cond_ge_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_le_numvar( a)){ printf("cond_le_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_le_varnum( a)){ printf("cond_le_varnum fails to pass the test\n"); fail++;}
    //return the big one
    if(max(a,b) != cond_gt_var(a,b) ){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_gt_var(a,b)){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_lt_var(a,b)){ printf("cond_gt_var fails to pass the test\n"); fail++;};
    if(max(a,b) != cond_lt_var(a,b)){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_ge_var(a,b)){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_ge_var(a,b)){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_le_var(a,b)){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_le_var(a,b)){ printf("cond_gt_var fails to pass the test"); fail++;}
    //# if a==b return 1 else return 0
    if((a==b) != cond_eq_var(a,b)){ printf("cond_gt_var fails to pass the test\n\n"); fail++;}
    if((a==b) != cond_ne_var(a,b)){ printf("cond_gt_var fails to pass the test\n"); fail++;}



   // fail = 0;
    a = 10;
    b = -1;
   // printf("positive and negative:\n");
    //return the big one
    if(max(a,b) != cond_gt_var(a,b) ){ printf("cond_gt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_lt_var(a,b)){ printf("cond_lt_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_ge_var(a,b)){ printf("cond_ge_var fails to pass the test\n"); fail++;}
    if(max(a,b) != cond_le_var(a,b)){ printf("cond_le_var fails to pass the test\n"); fail++;}
    //# if a==b return 1 else return 0
    if((a==b) != cond_eq_var(a,b)){ printf("cond_eq_var fails to pass the test\n"); fail++;}
    if((a==b) != cond_ne_var(a,b)){ printf("cond_ne_var fails to pass the test\n"); fail++;}


   // fail = 0;
    a = 10;
    b = a;
   // printf("a == b:\n");
    if((a==b) != cond_eq_var(a,b)){ printf("cond_eq_var fails to pass the test\n"); fail++;}
    if((a==b) != cond_ne_var(a,b)){ printf("cond_ne_var fails to pass the test\n"); fail++;}


   // fail = 0;
    a = 0;
   // printf("zero:\n");
    //# if a==0 return 0 else return a
    if( a != cond_eq_numvar( a)){ printf("cond_eq_numvar fails to pass the test\n"); fail++;}
    if( a != cond_eq_varnum( a)){ printf("cond_eq_varnum fails to pass the test\n"); fail++;}
    if( a != cond_ne_numvar( a)){ printf("cond_ne_numvar fails to pass the test\n"); fail++;}
    if( a != cond_ne_varnum( a)){ printf("cond_ne_varnum fails to pass the test\n"); fail++;}
    //#if a>0 return a else return 0
    if( max(a,0) != cond_gt_varnum( a)){ printf("cond_gt_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_gt_numvar( a)){ printf("cond_gt_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_lt_numvar( a)){ printf("cond_lt_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_lt_varnum( a)){ printf("cond_lt_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_ge_varnum( a)){ printf("cond_ge_varnum fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_ge_numvar( a)){ printf("cond_ge_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_le_numvar( a)){ printf("cond_le_numvar fails to pass the test\n"); fail++;}
    if( max(a,0) != cond_le_varnum( a)){ printf("cond_le_varnum fails to pass the test\n"); fail++;}

    if(fail == 0){
        printf("Success!\n");
    }

    return 0;
}
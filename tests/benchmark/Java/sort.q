uint64 INSERTION_SORT_THRESHOLD = 47
def void sort(uint64* array, uint64 first, uint64 last, uint1 leftsort);

def uint64 div2(uint64 a){
    uint64 a1 = a/2
    if(a1*2==a)
    then return a/2
    else return (a-1)/2
}

def void DualPivotQuicksort(uint64* a, uint64 left, uint64 right, uint1 leftmost){
    uint64 length = right + 1 - left

    # Use insertion sort on tiny arrays
    if (length < INSERTION_SORT_THRESHOLD) 
    then{
        if (leftmost) 
        then{
            for uint64 i = left, i < right, 1 {
                uint1 flag = false
                uint64 j = i
                uint64 ai = a[i + 1]
                while (ai < a[j]) {
                    a[j + 1] = a[j]
                    if (j == left) 
                    then{
                        flag = true
                        break
                    }
                    else
                        j = j - 1
                }
                if(flag)
                then a[j] = ai
                else a[j + 1] = ai
               
            }
        } 
        else {
            
            # Skip the longest ascending sequence.
            if (left >= right) 
            then{
                return void
            }
            left = left + 1
            while (a[left] >= a[left - 1]) {
                if (left >= right) 
                then{
                    return void
                }
                left = left + 1
            } 

            
            for uint64 k = left, left+1 <= right, 0 {
                left = left + 1 #++left<=right
                uint64 a1 = a[k]
                uint64 a2 = a[left]

                if (a1 < a2) 
                then{
                    a2 = a1
                    a1 = a[left]
                }
                
                while (a1 < a[k-1]) {
                    k = k - 1 # for a[--k]
                    a[k + 2] = a[k]
                }
                k = k - 1 # for a[--k]

                k = k + 1
                a[k + 1] = a1

               
                while (a2 < a[k-1]) {
                    k = k - 1 # for a[--k]
                    a[k + 1] = a[k]
                } 
                k = k - 1 # for a[--k]

                a[k + 1] = a2

                left = left + 1  #k = ++left
                k = left
            }
            left = left + 1 #++left<=right

            uint64 last = a[right]
            
            while (last < a[right-1]) {
                right = right - 1
                a[right + 1] = a[right]
            }
            right = right - 1

            a[right + 1] = last
        }
        return void
    }

    # Inexpensive approximation of length / 7
    # int seventh = (length >> 3) + (length >> 6) + 1
    # uint64 seventh = (length/2/2/2) + (length/2/2/2/2/2/2) + 1

    uint64 seventh = div2(div2(div2(length))) + div2(div2(div2(div2(div2(div2(length)))))) + 1
    uint64 e3 = div2(left + right)   # The midpoint
    uint64 e2 = e3 - seventh
    uint64 e1 = e2 - seventh
    uint64 e4 = e3 + seventh
    uint64 e5 = e4 + seventh

    # Sort these elements using insertion sort
    if (a[e2] < a[e1]) 
    then{ 
        uint64 t = a[e2] 
        a[e2] = a[e1] 
        a[e1] = t 
    }
    if (a[e3] < a[e2]) 
    then{ 
        uint64 t = a[e3]
        a[e3] = a[e2]
        a[e2] = t
        if (t < a[e1]) then{ a[e2] = a[e1]  a[e1] = t }
    }
    if (a[e4] < a[e3]) 
    then{ 
        uint64 t = a[e4] 
        a[e4] = a[e3] 
        a[e3] = t
        if (t < a[e2]) 
        then{ 
            a[e3] = a[e2] 
            a[e2] = t
            if (t < a[e1]) then{ a[e2] = a[e1]  a[e1] = t }
        }
    }
    if (a[e5] < a[e4]) 
    then{ 
        uint64 t = a[e5] 
        a[e5] = a[e4] 
        a[e4] = t
        if (t < a[e3]) 
        then{ 
            a[e4] = a[e3] 
            a[e3] = t
            if (t < a[e2]) 
            then{ 
                a[e3] = a[e2] 
                a[e2] = t
                if (t < a[e1]) then{ a[e2] = a[e1]  a[e1] = t }
            }
        }
    }

    # Pointers
    uint64 less  = left    # The index of the first element of center part
    uint64 great = right   # The index before the first element of right part

    if (a[e1] != a[e2] & a[e2] != a[e3] & a[e3] != a[e4] & a[e4] != a[e5]) 
    then{
        
        uint64 pivot1 = a[e2]
        uint64 pivot2 = a[e4]

        a[e2] = a[left]
        a[e4] = a[right]

        while (a[less+1] < pivot1){
            less = less + 1
        }
        less = less + 1
        
        while (a[great-1] > pivot2){
            great = great - 1
        }
        great = great - 1

        #outer:
        uint1 flag = true
        while(flag){
            flag = false
            for uint64 k = less - 1, true, 0 {
                k = k+1   #for ==k<=great
                if (k <= great) 
                then{
                    uint64 ak = a[k]
                    if (ak < pivot1) 
                    then{ 
                        # Move a[k] to left part
                        a[k] = a[less]
                        a[less] = ak
                        less = less + 1
                    } 
                    else{ 
                        if (ak > pivot2) 
                        then{ 
                            # Move a[k] to right part
                            while (a[great] > pivot2) {
                                great = great - 1
                                if (great == k) 
                                then{
                                    flag = true
                                    break
                                }
                            }
                        
                            if (a[great] < pivot1) 
                            then{ 
                                a[k] = a[less]
                                a[less] = a[great]
                                less = less + 1
                            } 
                            else { 
                                a[k] = a[great]
                            }
                        
                            a[great] = ak
                            great = great - 1
                        }
                    }
                    
                }
                else{
                    break
                }
            }
        }

        # Swap pivots into their final positions
        a[left]  = a[less  - 1]
        a[less - 1] = pivot1
        a[right] = a[great + 1] 
        a[great + 1] = pivot2

        # Sort left and right parts recursively, excluding known pivots
        DualPivotQuicksort(a, left, less - 2, leftmost)
        DualPivotQuicksort(a, great + 2, right, false)

        if (less < e1 & e5 < great) 
        then{
            # Skip elements, which are equal to pivot values.

            while (a[less] == pivot1) {
                less = less + 1
            }

            while (a[great] == pivot2) {
                great = great - 1
            }

            #outer:
            uint1 flag = true
            while(flag){
                flag = false

                for uint64 k = less - 1, true, 0  {
                    k = k + 1  #for ++k<=great
                    if(k<=great) then{
                        uint64 ak = a[k]
                        if (ak == pivot1) 
                        then{ 
                            # Move a[k] to left part
                            a[k] = a[less]
                            a[less] = ak
                            less = less + 1
                        } 
                        else{ 
                            if (ak == pivot2) 
                            then{ 
                                # Move a[k] to right part
                                while (a[great] == pivot2) {
                                    great = great - 1
                                    if (great == k) 
                                    then{
                                        flag = true
                                        break
                                    }
                                }
                                if (a[great] == pivot1) 
                                then{
                                    # a[great] < pivot2
                                    a[k] = a[less]
                                    a[less] = pivot1
                                    less = less + 1
                                } else { 
                                    # pivot1 < a[great] < pivot2
                                    a[k] = a[great]
                                }
                                a[great] = ak
                                great = great - 1
                            }
                        }
                    }else{
                        break
                    }
                }
            }
        }

        # Sort center part recursively
        DualPivotQuicksort(a, less, great, false)

    } 
    else { 
        # Partitioning with one pivot
        uint64 pivot = a[e3]

        for uint64 k = less, k <= great, 1 {
            if (a[k] != pivot) 
            then{
                uint64 ak = a[k]
                if (ak < pivot) 
                then{ 
                    # Move a[k] to left part
                    a[k] = a[less]
                    a[less] = ak
                    less = less + 1
                } 
                else { 
                    # a[k] > pivot - Move a[k] to right part
                    while (a[great] > pivot) {
                        great = great - 1
                    }
                    if (a[great] < pivot) 
                    then{ 
                        # a[great] <= pivot
                        a[k] = a[less]
                        a[less] = a[great]
                        less = less + 1
                    } 
                    else { 
                        # a[great] == pivot
                        a[k] = pivot
                    }
                    a[great] = ak
                    great = great - 1
                }
            }
        }

        DualPivotQuicksort(a, left, less - 1, leftmost)
        DualPivotQuicksort(a, great + 1, right, false)
    }
}
def void sort(uint64* array, uint64 first, uint64 last, uint1 leftsort){
    if(last==first)
    then return void
    DualPivotQuicksort(array, first, last-1, leftsort)
}


def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }
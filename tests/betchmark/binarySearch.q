uint64 threshold=16

def uint64 q_lower_bound(uint64* array, uint64 first, uint64 last, uint64 val){
    uint64 len = last - first
    while (len > 0){
        uint64 half = len / 2
        uint64 mid = first
        mid = mid + half
        if array[mid]<val
        then{
            first = mid
            first = first + 1
            len = len - half - 1

        }else{
            len = half
        }
    }
    return first
}

def uint1 q_binary_search(uint64* array, uint64 first, uint64 last, uint64 val){
    uint64 i = q_lower_bound(array,first,last,val)
    uint1 b = i!=last
    if b == true
    then{
        b = b & !(val<array[i])
    }
    return b
}


# sort ======
def void swap(uint64* a, uint64 i, uint64 j) {
  uint64 t = a[i]
  a[i]=a[j]
  a[j]=t
}

def void median_to_first(uint64* ptr, uint64 r, uint64 a, uint64 b, uint64 c) {

  if ptr[a] < ptr[b] then {
    if ptr[b] < ptr[c] then
      swap(ptr,r,b)
    else if ptr[a]<ptr[c] then
      swap(ptr,r,c)
    else
      swap(ptr,r,a)
  } else if ptr[a] < ptr[c] then
    swap(ptr,r,a)
  else if ptr[b] < ptr[c] then
    swap(ptr,r,c)
  else
    swap(ptr,r,b)
}

def uint64 partition(uint64* a, uint64 first, uint64 last, uint64 p) {
  while (true) {
    while (a[first] < a[p]) {
      first=first+1
    }
    last=last-1
    while (a[p] < a[last]) {
      last=last-1
    }
    if (!(first < last)) 
    then return first
    
    swap(a,first,last)
    first=first+1
  }
  return 0 # dummy return, unreachable but compiler does not detect it
}

def uint64 partition_pivot(uint64* a, uint64 l, uint64 h) {
  uint64 mid = l + (h-l)/2
  median_to_first(a,l,l+1,mid,h-1)

  return partition(a,l+1,h,l)
}

def void move_cut( uint64* a, uint64 cut){
  uint64 val = a[0]
  for uint64 i=0, i<cut, 1{
    a[i] = a[i+1]
  }
  a[cut] = val
}



def void unguarded_linear_insert(uint64* arr, uint64 last){

  uint64 val = arr[last]
  uint64 next = last - 1

  while (val < arr[next] ) {
    arr[last] = arr[next]
    last = next 
    next = next - 1
  }

  arr[last] = val
}

def void moveToFirst(uint64* arr, uint64 first, uint64 i){
#   uint64 val = arr[i]
  uint64 index = i
  while ( index > first){
    arr[index] = arr[index-1]
    index = index - 1
  }
#   arr[first] = val
}

def void insertion_sort(uint64* arr, uint64 first, uint64 last) {      

  if (first == last) 
  then  return void       

  for uint64 i = first + 1, i != last, 1	{	  
    if ( arr[i] < arr[first] ) 
    then {	  
      uint64 val = arr[i] 
      moveToFirst( arr, first, i)	    
      arr[first] = val	 
    }	  
    else{	   
      unguarded_linear_insert(arr, i)
    }
  }    
}

def void unguarded_insertion_sort( uint64* arr, uint64 first, uint64 last) {           
    for uint64 i = first, i != last, 1
      unguarded_linear_insert(arr, i)   
}

def void final_insertion_sort(uint64* arr, uint64 first, uint64 last) {      
  
  if (last-first > threshold)	
  then {	  
    insertion_sort(arr, first, first+threshold)	 
    unguarded_insertion_sort(arr, first+threshold, last)	
  }     
  else{	
    insertion_sort(arr, first, last)  
  }
}


def void quicksort_aux( uint64* a, uint64 l, uint64 h) {

  if (h-l > threshold)
  then {
    uint64 cut = partition_pivot(a,l,h)
    quicksort_aux(a,cut,h)
    quicksort_aux(a,l,cut)
  }
}


def void sort(uint64* arr, uint64 first, uint64 last){
  if first != last
  then {
    quicksort_aux(arr, first, last)
    final_insertion_sort(arr, first, last)
  }
}

def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }


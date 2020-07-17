
uint64 threshold=16

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

def uint64 partition(uint64* a, uint64 l, uint64 h, uint64 p) {
  while (true) {
    while (a[l] < a[p]) l=l+1
    h=h-1
    while (a[p] < a[h]) h=h-1
    if (!(a[l] < a[h])) return l
    swap(a,l,h)
    l=l+1
  }
}


def uint64 partition_pivot(uint64* a, uint64 l, uint64 h) {
  uint64 mid = l + (h-l)/2
  median_to_first(l,l+1,mid,h-1)
  return partition(a,l+1,h,l)

  xxx, ctd here

      _RandomAccessIterator __mid = __first + (__last - __first) / 2;
      std::__move_median_to_first(__first, __first + 1, __mid, __last - 1,
				  __comp);
      return std::__unguarded_partition(__first + 1, __last, __first, __comp);

}


def void quicksort_aux( uint64* a, uint64 l, uint64 h) {
  if (h-l <= threshold) return;

  uint64 m = partition_pivot(a,l,h);



}





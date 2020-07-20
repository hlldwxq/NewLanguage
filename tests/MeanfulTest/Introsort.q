
uint64 threshold=16 # Threshold must be at least 2

def void pr(uint64 i);
def void prl();


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
    if (!(first < last)) then return first
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


def void quicksort_aux( uint64* a, uint64 l, uint64 h) {

  if (h-l <= threshold) then return void

  uint64 cut = partition_pivot(a,l,h)

  quicksort_aux(a,cut,h)
  quicksort_aux(a,l,cut)
}



xxx, ctd here: port final insertion sort
code in stl_algo.h (for me: /usr/include/c++/7/bits/stl_algo.h)


  template<typename _RandomAccessIterator, typename _Compare>
    void
    __unguarded_linear_insert(_RandomAccessIterator __last,
			      _Compare __comp)
    {
      typename iterator_traits<_RandomAccessIterator>::value_type
	__val = _GLIBCXX_MOVE(*__last);
      _RandomAccessIterator __next = __last;
      --__next;
      while (__comp(__val, __next))
	{
	  *__last = _GLIBCXX_MOVE(*__next);
	  __last = __next;
	  --__next;
	}
      *__last = _GLIBCXX_MOVE(__val);
    }








def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }





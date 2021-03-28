#include <cstddef>
#include <stdexcept>
#include <iostream>

#include <math.h>

void qsortd ( int first , int last , double *data )
{
   int lower, upper ;
   double ftemp, split ;

   split = data[(first+last)/2] ;
   lower = first ;
   upper = last ;

   do {
      while ( split > data[lower] )
         ++lower ;
      while ( split < data[upper] )
         --upper ;
      if (lower == upper) {
         ++lower ;
         --upper ;
         }
      else if (lower < upper) {
         ftemp = data[lower] ;
         data[lower++] = data[upper] ;
         data[upper--] = ftemp ;
         }
      } while ( lower <= upper ) ;

   if (first < upper)
      qsortd ( first , upper , data ) ;
   if (lower < last)
      qsortd ( lower , last , data ) ;
}

void qsortds ( int first , int last , double *data , double *slave )
{
   int lower, upper ;
   double ftemp, split ;

   split = data[(first+last)/2] ;
   lower = first ;
   upper = last ;

   do {
      while ( split > data[lower] )
         ++lower ;
      while ( split < data[upper] )
         --upper ;
      if (lower == upper) {
         ++lower ;
         --upper ;
         }
      else if (lower < upper) {
         ftemp = slave[lower] ;
         slave[lower] = slave[upper] ;
         slave[upper] = ftemp ;
         ftemp = data[lower] ;
         data[lower++] = data[upper] ;
         data[upper--] = ftemp ;
         }
      } while ( lower <= upper ) ;

   if (first < upper)
      qsortds ( first , upper , data , slave ) ;
   if (lower < last)
      qsortds ( lower , last , data , slave ) ;
}

void qsortdsi ( int first , int last , double *data , int *slave )
{
   int lower, upper, itemp ;
   double ftemp, split ;

   split = data[(first+last)/2] ;
   lower = first ;
   upper = last ;

   do {
      while ( split > data[lower] )
         ++lower ;
      while ( split < data[upper] )
         --upper ;
      if (lower == upper) {
         ++lower ;
         --upper ;
         }
      else if (lower < upper) {
         itemp = slave[lower] ;
         slave[lower] = slave[upper] ;
         slave[upper] = itemp ;
         ftemp = data[lower] ;
         data[lower++] = data[upper] ;
         data[upper--] = ftemp ;
         }
      } while ( lower <= upper ) ;

   if (first < upper)
      qsortdsi ( first , upper , data , slave ) ;
   if (lower < last)
      qsortdsi ( lower , last , data , slave ) ;
}

// log(std::cout, a);
template<typename T>
void log(std::ostream& o, const T& t) {
  o << type_name<decltype(t)>() << ": " << t << "\n";
}
// log(std::cout, "msg", a);
template<typename T>
void log(std::ostream& o, std::string msg, const T& t) {
  o << msg << ": " << type_name<decltype(t)>() << ": " << t << "\n";
}

// log(std::cout, a, b);
template<typename T>
void log(std::ostream& o, const T& a, const T& b) {
  o << "a: [" << type_name<decltype(a)>() << "] " << a << ", b: [" << type_name<decltype(b)>() << "] " << b << "\n";
}

// constexpr string
class str_const {
private:
  const char* const p_;
  const std::size_t sz_;
public:
  template<std::size_t N>
  constexpr str_const(const char(&a)[N]) : // ctor
    p_(a), sz_(N-1) {}
  constexpr char operator[](std::size_t n) { // []
    return n < sz_ ? p_[n] :
      throw std::out_of_range("");
  }
  constexpr std::size_t size() { return sz_; } // size()
};

// Compile-time array
template<int size, typename T, typename Func>
struct ConstArray {
  int arr[size];
  decltype(auto) func;

  ConstArray<int size_, std::decay<Func> func_>() : arr(T std::forward(arr_(size_))), func(std::forward(func_)){};

  constexpr ConstArray():arr() {
    for (int i = 0; i < size; i++) arr[i] = 0;
  }

  constexpr ConstArray():doublingArr() {
    if (size < 2) return;
    for (int i = 1; i < size; i++) arr[i] = arr[i-1]*2;
  }

  void print() const {
    for (int i = 0; i < size; i++)
      std::cout<<arr[i]<<"\n";
  }
};

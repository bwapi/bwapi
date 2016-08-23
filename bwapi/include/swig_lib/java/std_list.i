/* -----------------------------------------------------------------------------
 * std_list.i
 * ----------------------------------------------------------------------------- */

%include <std_common.i>

namespace std {

  template<class T> class list {
  public:
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    
    list();
    list(const list& other);
    size_type size() const;
    %rename(isEmpty) empty;
    bool empty() const;
  };
}


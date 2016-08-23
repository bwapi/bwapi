%{
  #include <functional>
  #include <iostream>

  #ifndef SWIG_DIRECTORS
  #error "Directors must be enabled in your SWIG module for std_function.i to work correctly"
  #endif
%}

// These are the things we actually use
#define param(num,type) $typemap(jstype,type) arg ## num
#define unpack(num,type) arg##num
#define lvalref(num,type) type&& arg##num
#define forward(num,type) std::forward<type>(arg##num)

// This is the mechanics
#define FE_0(...)
#define FE_1(action,a1) action(0,a1)
#define FE_2(action,a1,a2) action(0,a1), action(1,a2)
#define FE_3(action,a1,a2,a3) action(0,a1), action(1,a2), action(2,a3)
#define FE_4(action,a1,a2,a3,a4) action(0,a1), action(1,a2), action(2,a3), action(3,a4)
#define FE_5(action,a1,a2,a3,a4,a5) action(0,a1), action(1,a2), action(2,a3), action(3,a4), action(4,a5)

#define GET_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME
%define FOR_EACH(action,...) 
  GET_MACRO(__VA_ARGS__, FE_5, FE_4, FE_3, FE_2, FE_1, FE_0)(action,__VA_ARGS__)
%enddef

%define %std_function(Name, Ret, ...)

%feature("director") Name##Impl;
%typemap(javaclassmodifiers) Name##Impl "abstract class";

%{
  struct Name##Impl {
    virtual ~Name##Impl() {}
    virtual Ret call(__VA_ARGS__) = 0;
  };
%}

%javamethodmodifiers Name##Impl::call "abstract protected";
%typemap(javaout) Ret Name##Impl::call ";" // Suppress the body of the abstract method

struct Name##Impl {
  virtual ~Name##Impl();
protected:
  virtual Ret call(__VA_ARGS__) = 0;
};

%typemap(maybereturn) SWIGTYPE "return ";
%typemap(maybereturn) void "";

%typemap(javain) std::function<Ret(__VA_ARGS__)> "$javaclassname.getCPtr($javaclassname.makeNative($javainput))"
%typemap(javacode) std::function<Ret(__VA_ARGS__)> %{
  protected Name() {
    wrapper = new Name##Impl(){
      public $typemap(jstype, Ret) call(FOR_EACH(param, __VA_ARGS__)) {
    $typemap(maybereturn, Ret)Name.this.call(FOR_EACH(unpack, __VA_ARGS__));
      }
    };
    proxy = new $javaclassname(wrapper);
  }

  static $javaclassname makeNative($javaclassname in) {
    if (null == in.wrapper) return in;
    return in.proxy;
  }

  // Bot of these are retained to prevent garbage collection from happenign to early
  private Name##Impl wrapper;
  private $javaclassname proxy;
%}

%rename(Name) std::function<Ret(__VA_ARGS__)>;
%rename(call) std::function<Ret(__VA_ARGS__)>::operator();

namespace std {
  struct function<Ret(__VA_ARGS__)> {
    // Copy constructor
    function<Ret(__VA_ARGS__)>(const std::function<Ret(__VA_ARGS__)>&);

    // Call operator
    Ret operator()(__VA_ARGS__) const;

    // Conversion constructor from function pointer
    function<Ret(__VA_ARGS__)>(Ret(*const)(__VA_ARGS__));

    %extend {
      function<Ret(__VA_ARGS__)>(Name##Impl *in) {
    return new std::function<Ret(__VA_ARGS__)>([=](FOR_EACH(lvalref,__VA_ARGS__)){
          return in->call(FOR_EACH(forward,__VA_ARGS__));
    });
      }
    }
  };
}

%enddef
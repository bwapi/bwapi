%define type_template(T)
  //%rename(T ## s) BWAPI::T ## s;
  //%rename(T ## sEnum) BWAPI::T ## s::Enum;
  //%rename(T ## sEnum) BWAPI::T ## s::Enum::Enum;
  
  %nspace BWAPI::T ## s::None;
  %nspace BWAPI::T ## s::Unknown;
  %nspace BWAPI::T ## s::Enum::Enum;
  %nspace BWAPI::T;
  
  //%apply BWAPI::T ## s::Enum::Enum { BWAPI::T }
  
  //%ignore BWAPI::T ## s::Enum;
  
  //%rename("%(regex:/BWAPI::" #T "s::/\\2_\\4/)s", %$isnested, fullname=1) "";
  //%rename("$ignore", regextarget=1, fullname=1) "BWAPI::" #T "s::[^E]?[^n]?[^u]?[^m]?.*";
  //%rename("%s", regextarget=1, fullname=1) "BWAPI::" #T "s::Enum";
  
  
  
  %typemap(jstype) BWAPI::T "bwapi." #T

  %typemap(jtype) BWAPI::T "bwapi." #T

  %typemap(jni) BWAPI::T "jint"

  %typemap(javaout) BWAPI::T {
    return $jnicall;
  }

  %typemap(javain) BWAPI::T "$javainput"

  %typemap(out) BWAPI::T {
    jclass clazz = jenv->FindClass("bwapi/" #T);
    jmethodID jPositionConstructor = jenv->GetMethodID(clazz, "<init>", "(II)V");
    jobject jPosition = jenv->NewObject(clazz, jPositionConstructor, $1.x, $1.y);
    $result = jPosition;
  }

  %typemap(in) BWAPI::T {
    jclass clazz = jenv->FindClass("bwapi/" #T);
    jfieldID xfid = jenv->GetFieldID(clazz, "x", "I");
    jfieldID yfid = jenv->GetFieldID(clazz, "y", "I");
    jint x = jenv->GetIntField($input, xfid);
    jint y = jenv->GetIntField($input, yfid);
    $1 = BWAPI::T(x, y);
  }

%enddef



/*
 * Inline constants
 */
%javaconst(1);

/*
 * Make enums more basic
 */
%include <enums.swg>

%typemap(javain) enum SWIGTYPE "$javainput.ordinal()"
%typemap(javaout) enum SWIGTYPE {
    return $javaclassname.class.getEnumConstants()[$jnicall];
  }
%typemap(javabody) enum SWIGTYPE ""

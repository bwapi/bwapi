%define position_template(PositionType)

  %typemap(jstype) BWAPI::PositionType "bwapi." #PositionType

  %typemap(jtype) BWAPI::PositionType "bwapi." #PositionType

  %typemap(jni) BWAPI::PositionType "jobject"

  %typemap(javaout) BWAPI::PositionType {
    return $jnicall;
  }

  %typemap(javain) BWAPI::PositionType "$javainput"

  %typemap(out) BWAPI::PositionType {
    jclass clazz = jenv->FindClass("bwapi/" #PositionType);
    jmethodID jPositionConstructor = jenv->GetMethodID(clazz, "<init>", "(II)V");
    jobject jPosition = jenv->NewObject(clazz, jPositionConstructor, $1.x, $1.y);
    $result = jPosition;
  }

  %typemap(in) BWAPI::PositionType {
    jclass clazz = jenv->FindClass("bwapi/" #PositionType);
    jfieldID xfid = jenv->GetFieldID(clazz, "x", "I");
    jfieldID yfid = jenv->GetFieldID(clazz, "y", "I");
    jint x = jenv->GetIntField($input, xfid);
    jint y = jenv->GetIntField($input, yfid);
    $1 = BWAPI::PositionType(x, y);
  }

%enddef
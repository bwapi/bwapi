#define SVN_REV $WCREV$
#define SVN_REV_STR "$WCMODS?$WCREV$:$WCREV$$"

#ifdef _DEBUG
  #define BUILD_DEBUG 1
#else
  #define BUILD_DEBUG 0
#endif

#ifdef _DEBUG
  #define BUILD_STR "DEBUG"
#else
  #define BUILD_STR "RELEASE"
#endif

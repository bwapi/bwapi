#pragma once

// Static assert ripped out of boost, will work for now since I don't feel like setting up boost fully just yet :P

namespace boost{

   // HP aCC cannot deal with missing names for template value parameters
   template <bool x> struct STATIC_ASSERTION_FAILURE;

   template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };

   // HP aCC cannot deal with missing names for template value parameters
   template<int x> struct static_assert_test{};

}

#define BOOST_JOIN( X, Y ) BOOST_DO_JOIN( X, Y )
#define BOOST_DO_JOIN( X, Y ) BOOST_DO_JOIN2(X,Y)
#define BOOST_DO_JOIN2( X, Y ) X##Y

#define BOOST_STATIC_ASSERT( B ) \
   typedef ::boost::static_assert_test<\
   sizeof(::boost::STATIC_ASSERTION_FAILURE< (bool)( B ) >)>\
   BOOST_JOIN(boost_static_assert_typedef_, __COUNTER__)
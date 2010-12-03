#ifndef __IENUMERATOR_H__
#define __IENUMERATOR_H__
#pragma once

namespace Collections
{
	template<class T>
	__interface IEnumerator
	{
		bool MoveNext();
		void Reset();
		T GetCurrent();
	};

	template<class T>
	__interface IEnumerable
	{
		IEnumerator<T>* GetEnumerator();
	};
}

#endif // __IENUMERATOR_H__
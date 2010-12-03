#ifndef __QUEUE_H__
#define __QUEUE_H__
#pragma once

#include "IEnumerator.h"
#include "LinkedList.h"
#include <windows.h>

namespace Collections
{
	template<class T>
	class Queue
	{
	public:
		Queue()
		{
			this->count=0;
			this->m_nHead=NULL;
			this->m_nTail=NULL;
		}
		~Queue()
		{
			this->count=0;
			if( this->m_nHead )
				delete this->m_nHead;
		}

		void Enqueue(T item)
		{
			if( this->count == 0 )
			{
				this->m_nHead = new LinkedListNode<T>(item,NULL,NULL);
			}
			else if( this->count == 1 )
			{
				this->m_nTail = this->m_nHead;
				this->m_nHead = new LinkedListNode<T>(item,NULL,this->m_nTail);
				this->m_nTail->m_nPrev = this->m_nHead;
			}
			else
			{
				this->m_nTail=new LinkedListNode<T>(item,this->m_nTail,NULL);
				this->m_nHead->m_nPrev->m_nNext=this->m_nTail;
			}

			count++;
		}

		T Dequeue()
		{
			if( count == 0 )
				return NULL;

			T ret = this->m_nHead->m_tItem;
			this->m_nHead = this->m_nHead->m_nNext;
			if( this->m_nHead )
				this->m_nHead->m_nPrev = NULL;

			count--;

			return ret;
		}

		T Peek()
		{
			if( count == 0 )
				return NULL;

			return this->m_nHead->m_tItem;
		}
		
		int GetCount() { return this->count; }

	private:
		LinkedListNode<T> *m_nHead, *m_nTail;
		int count;
	};
}

#endif // __QUEUE_H__
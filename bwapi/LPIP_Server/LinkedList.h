#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#pragma once

#include "IEnumerator.h"
#include <windows.h>

namespace Collections
{
	// Classes in this file
	template<class T>
	class LinkedListNode;

	template<class T>
	class LinkedList;

	template<class T>
	class LinkedListIterator;

	// *************************************************************************************** //

	template<class T>
	class LinkedListNode
	{
	public:
		LinkedListNode(T item, LinkedListNode<T> *prev = NULL, LinkedListNode<T> *next = NULL)
		{
			this->m_tItem = item;
			this->m_nPrev = prev;
			this->m_nNext = next;
		}
		~LinkedListNode()
		{
			this->m_tItem=NULL;
			if( this->m_nNext )
			{
				delete this->m_nNext;
				this->m_nNext=NULL;
			}
			if( this->m_nPrev )
			{
				delete this->m_nPrev;
				this->m_nPrev=NULL;
			}
		}

	public:
		T m_tItem;
		LinkedListNode<T> *m_nPrev, *m_nNext;
	};

	// *************************************************************************************** //

	template<class T>
	class LinkedList : public IEnumerable<T>
	{
	public:
		LinkedList()
		{
			this->count=0;
			this->m_nHead=NULL;
			this->m_nTail=NULL;
		}
		~LinkedList()
		{
			this->count=0;
			if( this->m_nHead )
				delete this->m_nHead;
		}

		void Add(T item)
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
				this->m_nHead=new LinkedListNode<T>(item,NULL,this->m_nHead);
				this->m_nHead->m_nNext->m_nPrev=this->m_nHead;
			}

			count++;
		}

		void Remove(T item)
		{
			if( count == 0 )
				return;

			if( this->m_nHead->m_tItem == item )
			{
				this->m_nHead = this->m_nHead->m_nNext;
				if( this->m_nHead )
					this->m_nHead->m_nPrev = NULL;

				count--;
			}
			else if( this->m_nTail && this->m_nTail->m_tItem == item )
			{
				this->m_nTail = this->m_nTail->m_nPrev;
				if( this->m_nTail )
					this->m_nTail->m_nNext = NULL;

				count--;
			}
			else if( count > 3 )
			{
				for( LinkedListNode<T> *curr=this->m_nHead->m_nNext; curr != this->m_nTail; curr = curr->m_nNext )
				{
					if( curr->m_tItem == item )
					{
						curr->m_nPrev->m_nNext = curr->m_nNext;
						curr->m_nNext->m_nPrev = curr->m_nPrev;

						count--;
						return;
					}
				}
			}
		}

		T operator[] (int index)
		{
			if( index < 0 || index >= count )
				return NULL;

			int currIndex=0;
			LinkedListNode<T> *curr;
			for( curr=this->m_nHead; currIndex<index; curr=curr->m_nNext )
				currIndex++;

			return curr->m_tItem;
		}

		T Get(int index)
		{
			if( index < 0 || index >= count )
				return NULL;

			int currIndex=0;
			LinkedListNode<T> *curr;
			for( curr=this->m_nHead; currIndex<index; curr=curr->m_nNext )
				currIndex++;

			return curr->m_tItem;
		}
		
		int GetCount() { return this->count; }

		IEnumerator<T>* GetEnumerator()
		{
			return new LinkedListIterator<T>(this);
		}

	private:
		LinkedListNode<T> *m_nHead, *m_nTail;
		int count;

		friend class LinkedListIterator<T>;
	};

	// *************************************************************************************** //

	template<class T>
	class LinkedListIterator : public IEnumerator<T>
	{
	public:
		LinkedListIterator(LinkedList<T>* list)
		{
			this->m_colList = list;
			this->started=false;
		}
		~LinkedListIterator()
		{
			this->m_colList = NULL;
		}

		bool MoveNext()
		{
			if( !this->started )
			{
				this->m_nCurr = this->m_colList->m_nHead;
				this->started=true;
			}
			else if( this->m_nCurr )
				this->m_nCurr = this->m_nCurr->m_nNext;

			if( this->m_nCurr )
				return true;

			return false;
		}

		void Reset()
		{
			this->m_nCurr=NULL;
			this->started=false;
		}

		T GetCurrent()
		{
			if( !this->started )
				return NULL;

			return this->m_nCurr->m_tItem;
		}

	private:
		LinkedList<T>* m_colList;
		LinkedListNode<T>* m_nCurr;
		bool started;
	};
}

#endif // __LINKEDLIST_H__
#pragma once

#include <list>

namespace Util
{
  /**
   * Structure representing 2D space containing objects of the specified type. This structure is optimised for
   * effective way of finding of objects in certain area. The top level of the structure contains single list
   * of all objects included, it divides into 2-4 partitions containing lists of units on respective quoters
   * of the space, and so on until sthe width/height size is reached. The structure works with list of
   * pointers of the type and doesn't act as owner, so removing items or deleting the whole quad tree will not
   * result in delatation of contained items.
   */
  template <class Type>
  class RegionQuadTree
   {
     public :
       /**
        * Creates the quad tree with the specified dimensions.
        * @param width Width of the highest detail level partition.
        * @param height Height of the highest detail level partition.
        */
       RegionQuadTree(unsigned int width = 1, unsigned int height = 1);
       /** Destorys the array, but doesn't delete inserted objects. */
       ~RegionQuadTree(void);
       /**
        * Gets the width of the array.
        * @return width of the array.
        */
       unsigned int getWidth(void) const;
       /**
        * Gets the height of the array.
        * @return height of the array.
        */
       unsigned int getHeight(void) const;
       /**
        * Gets list of items in the specified region.
        * @param x horizontal index of the region.
        * @param y vertical index of the region.
        * @return list of items on the specified region
        */
       std::list<Type*>* getItems(unsigned int x, unsigned int y, unsigned int level = 0);
       /**
        * Sets item of the array on the specified position.
        * @param x horizontal index of the array position.
        * @param y vertical index of the array position.
        * @param item new value of the field.
        */
       void addItem(unsigned int x, unsigned int y, Type *item);
       void clear(unsigned int x, unsigned int y);
     private :
       /** width of array */
       unsigned int width;
       /** height of array */
       unsigned int height;
       /** array of rectangle arrays of lists of objects. 
        * The 1. item of the array corresponds with the lowest (most detailed) level of the region resolution.
        * Every other level correspons to 4 times less detailed resolution.
        */
       RectangleArray<std::list<Type*> >* data;
       /** depth = log2(max(width,height)), but is here for optimalisation reasons. */
       unsigned int depth;
   };
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  template <class Type>
  RegionQuadTree<Type>::RegionQuadTree(unsigned int width, unsigned int height)
  :width(width)
  ,height(height)
  ,depth(ceil(log2(max(width,height)))
  {
    this->data = new RectangleArray<std::list<Type*> >[depth];
    unsigned int localWidth = width;
    unsigned int localHeight = height;
    for (unsigned int i = 0; i < this->depth; i++)
    {
      this->data[i].resize(localWidth, localHeight);
      localWidth = (localWidth >= 2) (localWidth+1)/2 : 1;
      localHeight = (localHeight >=2) (localHeight+1)/2 : 1;
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  template <class Type>
  RectangleArray<Type>::~RectangleArray(void)
  {
    delete [] data;
  }
  //----------------------------------------------- GET WIDTH ------------------------------------------------
  template <class Type>
  unsigned int RectangleArray<Type>::getWidth(void) const
  {
    return this->width;
  }
  //----------------------------------------------- SET WIDTH ------------------------------------------------
  template <class Type>
  void RectangleArray<Type>::setWidth(unsigned int width)
  {
    this->width = width;
  }
  //----------------------------------------------- GET HEIGHT -----------------------------------------------
  template <class Type>
  unsigned int RectangleArray<Type>::getHeight(void) const
  {
    return this->height;
  }
  //------------------------------------------------ GET ITEM ------------------------------------------------
  template <class Type>
  std::list<Type*>* getItems(unsigned int x, unsigned int y, unsigned int level = 0);
  {
    return this->data[level][x][y];
  }
  //------------------------------------------------ ADD ITEM ------------------------------------------------
  template <class Type>
  void RectangleArray<Type>::addItem(unsigned int x, unsigned int y, Type* item)
  {
    for (unsigned int i = 0; i < this->depth; i++)
      this->data[i][x<<i][y<<i].push_bach(item);
  }
  //------------------------------------------------- CLEAR --------------------------------------------------
  template <class Type>
  void RectangleArray<Type>::clear(unsigned int x, unsigned int y)
  {
    for (unsigned int i = 0; i < this->depth; i++)
      this->data[i][x<<i][y<<i].clear();
  }  
  //----------------------------------------------------------------------------------------------------------
}
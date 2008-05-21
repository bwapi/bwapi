#pragma once
/**
 * Template used for work with dynamically initialized array with dimension 2.
 */
template <class Type,class Iterator = unsigned int>
class RectangleArray
 {
   private :
     /** width of array */
     Iterator width;
     /** height of array */
     Iterator height;
     /** Array data, stored as linear array of size width*height */
     Type *data;
     /** Pointers to begins of lines*/
     Type **columns;
     /**
      * Gets data item on the specified index
      * @param index index of the data to be returned.
      */
     Type getData(Iterator index);
     /**
      * Gets the pointer in data to the beginning of line with the specified index.
      * @param index index of the line.
      */
     Type *getColumn(Iterator index);
     /**
      * Sets the width of the array.
      * @param width New width of the array.
      */
     void setWidth(Iterator width);
     /**
      * Sets the height of the array.
      * @param height New height of the array.
      */
     void setHeight(Iterator height);
   public :
     /**
      * Creates the array with the specified proportions.
      * @param width Width of the new array.
      * @param height Height of the new array.
      */
     RectangleArray(Iterator width,Iterator height);
     /**
      * Destorys the array and deletes all content of array.
      */
     ~RectangleArray(void);
     /**
      * Gets the width of the array.
      * @return width of the array.
      */
     Iterator getWidth(void);
     /**
      * Gets the height of the array.
      * @return height of the array.
      */
     Iterator getHeight(void);
     /**
      * Gets item of the array on the specified position.
      * @param x horizontal index of the array position.
      * @param y vertical index of the array position.
      * @return item on the specified position.
      */
     Type* getItem(Iterator x, Iterator y);
     /**
      * Sets item of the array on the specified position.
      * @param x horizontal index of the array position.
      * @param y vertical index of the array position.
      * @param item new value of the field.
      */
     inline Type* operator[](int i) { return this->getColumn(i); }

     inline Type const * const operator[](int i) const {return this->getColumn(i); }
     void setItem(Iterator x, Iterator y, Type *item);
     void resize(Iterator width, Iterator height);
 };
//-------------------------------- CONSTRUCTOR ---------------------------------
template <class Type, class Iterator>
RectangleArray<Type, Iterator>::RectangleArray(Iterator width = 0, Iterator height = 0)
 {
  this->setWidth(width);
  this->setHeight(height);
  data = new Type[width*height];

  columns = new Type*[this->width];
  Iterator i = 0;
  for (Iterator position = 0;i < width; i ++,position += height)
    columns[i] = &data[position];
 }
//-------------------------------- DESTRUCTOR ----------------------------------
template <class Type,class Iterator>
RectangleArray<Type, Iterator>::~RectangleArray(void)
{
   delete [] columns;
   delete [] data;
}
//-------------------------------- GET WIDTH -----------------------------------
template <class Type,class Iterator>
Iterator RectangleArray<Type,Iterator>::getWidth(void)
{
  return this->width;
}
//-------------------------------- SET WIDTH -----------------------------------
template <class Type, class Iterator>
void RectangleArray<Type,Iterator>::setWidth(Iterator width)
{
  this->width = width;
}
//-------------------------------- GET HEIGHT ----------------------------------
template <class Type, class Iterator>
Iterator RectangleArray<Type,Iterator>::getHeight(void)
{
  return this->height;
}
//-------------------------------- SET HEIGHT ----------------------------------
template <class Type, class Iterator>
void RectangleArray<Type, Iterator>::setHeight(Iterator height)
{
  this->height = height;
}
//-------------------------------- GET ITEM ------------------------------------
template <class Type,class Iterator>
Type* RectangleArray<Type,Iterator>::getItem(Iterator x, Iterator y)
{
  return this->getColumn(x)[y];
}
//-------------------------------- SET ITEM ------------------------------------
template <class Type,class Iterator>
void RectangleArray<Type, Iterator>::setItem(Iterator x, Iterator y, Type* item)
{
  this->getColumn(x)[y] = item;
}
//-------------------------------- GET LINE ------------------------------------
template <class Type, class Iterator>
Type* RectangleArray<Type, Iterator>::getColumn(Iterator index)
{
  return columns[index];
}
//------------------------------- RESIZE ---------------------------------------
template <class Type, class Iterator>
void RectangleArray<Type, Iterator>::resize(Iterator width, Iterator height)
{
  if (this->getWidth() == width &&
      this->getHeight() == height)
    return;

  delete [] this->columns;
  delete [] this->data;  

  this->setWidth(width);
  this->setHeight(height);

  this->data = new Type[this->width * this->height];

  this->columns = new Type*[this->width];
  Iterator i = 0;
  for (Iterator position = 0;i < this->width; i ++,position += this->height)
    columns[i] = &data[position];
}
//------------------------------------------------------------------------------

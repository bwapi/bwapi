#pragma once

#include "Exceptions.h"

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
      * Gets the pointer in data to the beginning of line with the specified index.
      * @param index index of the line.
      */
     const Type *getColumn(Iterator index) const;
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
      * Copy constructor
      */
     RectangleArray(const RectangleArray<Type,Iterator>& rectangleArray);
     /**
      * Destorys the array and deletes all content of array.
      */
     ~RectangleArray(void);
     /**
      * Gets the width of the array.
      * @return width of the array.
      */
     Iterator getWidth(void) const;
     /**
      * Gets the height of the array.
      * @return height of the array.
      */
     Iterator getHeight(void) const;
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
     void printToFile(FILE* f);
     void saveToFile(const std::string& fileName);
 };
//-------------------------------- CONSTRUCTOR ---------------------------------
template <class Type, class Iterator>
RectangleArray<Type, Iterator>::RectangleArray(Iterator width = 0, Iterator height = 0)
{
  this->setWidth(width);
  this->setHeight(height);
  this->data = new Type[this->getWidth()*this->getHeight()];

  columns = new Type*[this->getWidth()];
  Iterator i = 0;
  for (Iterator position = 0;i < width; i ++,position += height)
    columns[i] = &data[position];
}
//-------------------------------- CONSTRUCTOR ---------------------------------
template <class Type, class Iterator>
RectangleArray<Type, Iterator>::RectangleArray(const RectangleArray<Type,Iterator>& rectangleArray)
{
  this->setWidth(rectangleArray.getWidth());
  this->setHeight(rectangleArray.getHeight());
  this->data = new Type[this->getWidth()*this->getHeight()];
  columns = new Type*[this->getWidth()];
  
  Iterator i = 0;
  for (Iterator position = 0;i < width; i ++,position += height)
    columns[i] = &data[position];
  memcpy(this->data, rectangleArray.data, sizeof(Type)*this->getWidth()*this->getHeight());
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
Iterator RectangleArray<Type,Iterator>::getWidth(void) const
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
Iterator RectangleArray<Type,Iterator>::getHeight(void) const
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
//-------------------------------- GET LINE ------------------------------------
template <class Type, class Iterator>
const Type* RectangleArray<Type, Iterator>::getColumn(Iterator index) const
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
//----------------------------------- PRINT TO FILE ----------------------------
template <class Type, class Iterator>
void RectangleArray<Type, Iterator>::printToFile(FILE* f)
{
  for (Iterator y = 0; y < this->getHeight(); y++)
  {
    for (Iterator x = 0; x < this->getWidth(); x++)
    {
      char ch = this->getColumn(x)[y];
      fprintf(f, "%c", ch);
    }
    fprintf(f, "\n");
  }
}
//---------------------------------- SAVE TO FILE ------------------------------
template <class Type, class Iterator>
void RectangleArray<Type, Iterator>::saveToFile(const std::string& fileName)
{
  FILE* f = fopen(fileName.c_str(), "wt");
  if (!f)
    throw FileException("RectangleArray::saveToFile Couldn't open file " + fileName + "for writing");
  this->printToFile(f);
  fclose(f);
}
//------------------------------------------------------------------------------

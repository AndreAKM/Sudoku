/*
 * Field.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Andrey Moiseenko
 */

#ifndef FIELD_H_
#define FIELD_H_
#include <memory>
#include <cstring>

template<class T>
class Field {
	std::unique_ptr<T[]> data;
public:
	using Coordinate = std::tuple<int, int>;
	Field() : data(new T[9*9]){}
	Field(const Field& other): data(new T[9*9]) {
		memcpy(data.get(),other.data.get(), 9*9);
	}
	Field& operator=(const Field& other) {
		memcpy(data.get(),other.data.get(), 9*9);
	}
	void set(int x, int y, T value) {
		data[x + y * 9] = value;
	}
	T& get(int x, int y) const {
    	return data[x + y * 9];
    }
	T& get(int x, int y) {
		return data[x + y * 9];
	}
};

template<class T>
class BlockIteratorBase: public std::iterator<std::input_iterator_tag, T>
{
public:
    BlockIteratorBase(int x, int y, std::shared_ptr<Field<T>> data)
	:x(x), y(y), data(data) {}

    bool operator!=(BlockIteratorBase const& other) const {
    	return x!= other.x || y!=other.y;
    }
    typename BlockIteratorBase::reference operator*() const {
    	return data->get(x, y);
    }
    virtual BlockIteratorBase& operator++() =0;
    typename Field<T>::Coordinate coordinate() {
    	return {x, y};
    }
protected:
    int x, y;
    std::shared_ptr<Field<T>> data;
};

template<class I,class T>
class Data {
	int sx, sy;
	int ex, ey;
	std::shared_ptr<Field<T>> data;
public:
	Data (int startx, int starty, int endx, int endy, std::shared_ptr<Field<T>> data )
	:sx(startx), sy(starty), ex(endx), ey(endy), data(data){}
	I begin() { return I(sx, sy, data);}
	I end() { return I(ex, ey, data);}
};
template<class T>
class RowIterator: public BlockIteratorBase<T>
{
public:
    RowIterator(int x, int y, std::shared_ptr<Field<T>> data):BlockIteratorBase<T>(x,y,data){}
    BlockIteratorBase<T>& operator++(){
    	++BlockIteratorBase<T>::x;
    	return *this;
    }
};
template<class T>
class ColumnIterator: public BlockIteratorBase<T>
{
public:
    ColumnIterator(int x, int y, std::shared_ptr<Field<T>> data):BlockIteratorBase<T>(x,y,data){}
    BlockIteratorBase<T>& operator++(){
    	++BlockIteratorBase<T>::y;
    	return *this;
    }
};
template<class T>
class BlockIterator: public BlockIteratorBase<T>
{
private:
	int ex, ey;
public:
    BlockIterator(int x, int y, std::shared_ptr<Field<T>> data)
	:BlockIteratorBase<T>(x,y,data), ex(x + 3), ey(y + 3) {}
    BlockIteratorBase<T>& operator++();
};
template<class T>
Data<RowIterator<T>, T> getRow(int x, int y, std::shared_ptr<Field<T>> field){
	return Data<RowIterator<T>, T>(0, y, 9, y,field);
}
template<class T>
Data<ColumnIterator<T>, T> getColumn(int x, int y, std::shared_ptr<Field<T>> field){
	return Data<ColumnIterator<T>, T>(x, 0, x, 9, field);
}
template<class T>
Data<BlockIterator<T>, T> getBlock(int x, int y, std::shared_ptr<Field<T>> field){
	int startx = x/3 * 3, starty = y/3 *3;
	return Data<BlockIterator<T>, T>(startx, starty, startx + 3, starty + 3, field);
}
template<class T>
BlockIteratorBase<T>& BlockIterator<T>::operator ++(){
	if (this->x < this->ex) ++this->x;
	if (this->x == this->ex && this->y < ey) ++this->y;
	if(this->x == this->ex && this->y != this->ey) this->x -= 3;
	return *this;
}

#endif /* FIELD_H_ */

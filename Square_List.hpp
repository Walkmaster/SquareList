#if !defined(__SQUARE_LIST_HPP__)
#define __SQUARE_LIST_HPP__

/*
	square_list.hpp
	Brad Walker
	2014 - 02 - 07
	
	a sorted container that keeps its elements in a square link chain. Every (size square root) nth element is a designated column head for a column (size square root) deep.
*/

#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <math.h>

using namespace std;

template <class T_>
class square_list
{
public:
	typedef T_													value_type;
	typedef std::size_t											size_type;
	typedef T_ &												reference;
	typedef T_ *												pointer;
	typedef T_ const &											const_reference;
	typedef typename std::list<T_>::iterator					iterator;
	typedef typename std::list<T_>::const_iterator				const_iterator;
	typedef typename std::list<T_>::const_pointer				const_pointer;
	typedef typename std::list<T_>::reverse_iterator			r_iterator;
	typedef std::ptrdiff_t										difference_type;

	std::list<value_type>		_data;
	std::list<iterator>			_head; // contains header of each column
	size_type					_size;
	int							_columns; // number of supposed operating columns/ length of each operating column

	square_list() { _size = 0; _data; };
	//~square_list();

	// class functions that do not change container size
	bool empty() const
	{
		if(_size == 0)
			return true;
		else
			return false;
	}

	size_t size() const { return _size; }
	
	// insert method with auto sort placement
	void insert(value_type t) // single T_ insert function
	{

		// if else statements utilized to ensure all complex iteration check are made when list is 2 or more _head deep
		if( _size == 0 ) // simple insert for squarelist of 0x0
		{
			_data.push_front( t );
			_size++;
		}
		else if ( _size == 1 ) // simple insert for squarelist 1x1
		{
			if ( t < *_data.begin() )
				_data.push_front( t );
			else
				_data.push_back( t );

			_size++;
		}
		else if ( _size == 2 ) // simple insert for squarelist 2x1 to ensure _head is at least 2 elements deep
		{
			auto data_iter = _data.begin();
			data_iter++;
			if ( t < *_data.begin() )
				_data.push_front( t );
			else if ( t < *data_iter )
				_data.insert( data_iter, t );
			else
				_data.push_back( t );

			_size++;
		}
		else // if _head has at least 2 elements, begin square insert sort (skip every head until end is reached or value t is greater than current node value and less than next node value)
		{
			for( auto head_iter = _head.begin(); head_iter != _head.end(); ++head_iter) // iterate through head coloumn
			{
				auto head_iter_above = next(head_iter); // grab next iterator in header

				// if head node is less that value t, insert at front
				if(t < **_head.begin())
				{
					_data.push_front( t );
					_size++;
					break;
				}
				else if ( head_iter == prev(_head.end()) ) // if current node is end, work way down node
				{
					//cout << " current node is end " << endl;
					for( auto data_iter = *head_iter; data_iter != _data.end(); ++data_iter ) // iterate through data coloumn
					{
						auto data_itr_above = next(data_iter); // grab next data in header

						if ( data_itr_above == _data.end() ) // if the next iteration is end of data, simple insert
						{
							_data.push_back( t );
							_size++;
							break;
						}
						else if( t >= *data_iter && t <= *data_itr_above )
						{
							_data.insert( next(data_iter), t );
							_size++;
							break;
						}

					}
					break;
				}
				else if ( t >= **head_iter && t <= **head_iter_above )
				{
					for( auto data_iter = *head_iter; data_iter != _data.end(); ++data_iter ) // iterate through data coloumn
					{
						auto data_itr_above = next(data_iter); // grab next data in header
						if( t >= *data_iter && t <= *data_itr_above ) // if t value fits between this itr and next itr
						{
							_data.insert( next(data_iter), t );
							_size++;
							break;
						}
					}
					break;
				}
				// if next node is greater than this node but less than next
			}
		}

		_head.clear(); // quickly iterate through elements and retrieve new heads, fix for _column change

		int c = 0;
		for(auto it = _data.begin(); it != _data.end(); ++it)
		{
			if (c == _columns || it == _data.begin()) // if counter is equal to maximum column size/count (), then push back header container. Also include begin.
			{
				_head.push_back(it); // push back reference pointer 
				c = 0;
			}

			square_calc(); // calculate the square root for container
			c++;
		}

		// check to see the amount of elements and columns to see if new column must be added
	}

	// debug insert with iterational dumps
	/*void debuginsert(value_type t) // single T_ insert function
	{

		// if else statements utilized to ensure all complex iteration check are made when list is 2 or more _head deep
		if( _size == 0 ) // simple insert for squarelist of 0x0
		{
			_data.push_front( t );
			_size++;
		}
		else if ( _size == 1 ) // simple insert for squarelist 1x1
		{
			if ( t < *_data.begin() )
				_data.push_front( t );
			else
				_data.push_back( t );

			_size++;
		}
		else if ( _size == 2 ) // simple insert for squarelist 2x1 to ensure _head is at least 2 elements deep
		{
			auto data_iter = _data.begin();
			data_iter++;
			if ( t < *_data.begin() )
				_data.push_front( t );
			else if ( t < *data_iter )
				_data.insert( data_iter, t );
			else
				_data.push_back( t );

			_size++;
		}
		else // if _head has at least 2 elements, begin square insert sort (skip every head until end is reached or value t is greater than current node value and less than next node value)
		{
			for( auto head_iter = _head.begin(); head_iter != _head.end(); ++head_iter) // iterate through head coloumn
			{
				auto head_iter_above = next(head_iter); // grab next iterator in header

				// if head node is less that value t, insert at front
				if(t < **_head.begin())
				{
					_data.push_front( t );
					_size++;
					break;
				}
				else if ( head_iter == prev(_head.end()) ) // if current node is end, work way down node
				{
					//cout << " current node is end " << endl;
					for( auto data_iter = *head_iter; data_iter != _data.end(); ++data_iter ) // iterate through data coloumn
					{
						auto data_itr_above = next(data_iter); // grab next data in header
						if ( data_itr_above == _data.end() )
						{
							cout << " end found with " << t << " at iter " << *data_iter << endl;
							_data.insert( next(data_iter), t );
							_size++;
							break;
						}
						else if( t >= *data_iter && t <= *data_itr_above )
						{
							cout << " t of " << t << " is greater than " << *data_iter << " but less than " << *data_itr_above << endl;
							_data.insert( next(data_iter), t );
							_size++;
							break;
						}
					}
					break;
				}
				else if ( t >= **head_iter && t <= **head_iter_above )
				{
					cout << " t of " << t << " is greater than h " << **head_iter << " but less than " << **head_iter_above << endl;
					for( auto data_iter = *head_iter; data_iter != _data.end(); ++data_iter ) // iterate through data coloumn
					{
						auto data_itr_above = next(data_iter); // grab next data in header
						if( t >= *data_iter && t <= *data_itr_above ) // if t value fits between this itr and next itr
						{
							cout << " t of " << t << " is greater than " << *data_iter << " but less than " << *data_itr_above << endl;
							_data.insert( next(data_iter), t );
							_size++;
							break;
						}
					}
					break;
				}
				// if next node is greater than this node but less than next
			}
		}

		_head.clear(); // quickly iterate through elements and retrieve new heads, fix for _column change

		int c = 0;
		for(auto it = _data.begin(); it != _data.end(); ++it)
		{
			if (c == _columns || it == _data.begin()) // if counter is equal to maximum column size/count (), then push back header container. Also include begin.
			{
				_head.push_back(it); // push back reference pointer 
				c = 0;
			}

			square_calc(); // calculate the square root for container
			c++;
		}

		// check to see the amount of elements and columns to see if new column must be added
	}*/

	// erase method
	void erase(iterator tp)
	{
		_head.clear(); // erase head for elements in _data that have iterators in _head

		_data.erase(tp);
		_size--;

		int c = 0;
		for(auto it = _data.begin(); it != _data.end(); ++it)
		{
			if (c == _columns || it == _data.begin()) // if counter is equal to maximum column size/count (), then push back header container. Also include begin.
			{
				_head.push_back(it); // push back reference pointer 
				c = 0;
			}

			square_calc(); // calculate the square root for container
			c++;
		}
	}

	// clear method
	void clear()
	{
		_head.clear();
		_data.clear();
		
		_size = 0;
		
		square_calc();
	}

	// infor dump
	void dump()
	{
		cout << "Printing Visual Demo of Square_List" << endl;
		
		auto itv = _head.begin();
		int size_count = 0;
		bool s_count = false;

		for(auto it = _data.begin(); it != _data.end(); ++it)
		{
			
			if (s_count == true)
				size_count++;

			if(size_count == _columns)
			{
				itv++;
				size_count = 0;
			}

			if ( &**itv == &*it ) // compare addresses
			{
				cout << endl;
				cout << "H" << *it;
				s_count = true;
			}
			else
			{
				cout << " - " << *it;
			}
		}

		cout << endl << "Printing vector of pointers to list" << endl;

		for(auto it = _head.begin(); it != _head.end(); ++it)
		{
			cout << **it << endl;
		}

		cout << "End Print" << endl;

		cout << "Printing List of values to list" << endl;

		for(auto it = _data.begin(); it != _data.end(); ++it)
		{
			//T_* i = (*it);
			cout << *it << endl;
		}

		//cout << "End Print" << endl;
	}

	void square_calc()
	{
		// find the size and assign element to equal columns

		double	_ceiling;

		_ceiling = sqrt(_size);
		_ceiling = ceil(_ceiling);

		_columns = (int)_ceiling;
	}

	iterator				begin() { return _data.begin(); }
	iterator				end() { return _data.end(); }
	const_iterator			begin() const { return _data.cbegin(); }
	const_iterator			end() const { return _data.cend(); }
	const_iterator			cbegin() const { return _data.cbegin(); }
	const_iterator			cend() const { return _data.cend(); }

	reference				front() { return _data.front(); }
	reference				back() { return _data.back(); }
	const_iterator			find(value_type f)
	{ 
		for( auto data_iter = _data.begin(); data_iter != _data.end(); ++data_iter) // iterate through _data
		{
			if( f == *data_iter ) // if value is found return iter
				return data_iter;
		}

		return _data.end(); // if no match is found, return end
	}
	
	// Operators
	void operator = ( square_list const& rhs ) 
	{ // reference
		if(this != &rhs)
		{
			this->_data = rhs._data;
			this->_head = rhs._head;
			this->_size = rhs._size;
		}
	}

	bool operator == ( square_list const& rhs ) const 
	{ // reference
		return this->_data == rhs._data;
	}

	bool operator != ( square_list const& rhs ) const 
	{ // reference
		return this->_data != rhs._data;
	}
};

#endif
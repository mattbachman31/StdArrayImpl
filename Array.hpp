#ifndef ARR_H
#define ARR_H

#include <iostream>


namespace cs540{

	class OutOfRange : public std::exception{};

	template<typename T, size_t Dim1, size_t... Dims>
	class Array{
	public:

	struct FirstDimensionMajorIterator{
		bool is_end = false;
		size_t index;
		Array<T, Dim1, Dims...>* outer;
		bool topLevel = true;
		typename Array<T, Dims...>::FirstDimensionMajorIterator nested;
		FirstDimensionMajorIterator() : index(0), outer(NULL) {
			nested.topLevel = false;
		}
		FirstDimensionMajorIterator(Array<T, Dim1, Dims...>* theOuter) : index(0), outer(theOuter){
			nested.topLevel = false;
		}
		FirstDimensionMajorIterator(bool end) : is_end(end), index(Dim1) {
			nested.topLevel = false;
		}
		bool fdm_inc(){
			if (nested.fdm_inc()) {
                nested.index = 0;
                if (++index == Dim1) {
                    return true;
                }
            }
            return false;
		}

		void print_val(){
			std::cout << index << std::endl;
			nested.print_val();
		}

		T& operator*(){
			nested.outer = &(outer->helpyhelp[index]);
			return *(nested);
		}

		bool operator==(const FirstDimensionMajorIterator& it){
			if(this->nested == it.nested){
				if(index == it.index && outer == it.outer){
					return true;
				}
			}
			else if(index == it.index && is_end && it.is_end && topLevel && it.topLevel){
				return true;
			}
			return false;
		}

		bool operator !=(const FirstDimensionMajorIterator& it){
			return !((*this) == it);
		}

		FirstDimensionMajorIterator& operator++(){
			if(fdm_inc()){
				is_end = true;
			}
			return (*this);
		}

		FirstDimensionMajorIterator operator++(int){
			FirstDimensionMajorIterator it = (*this);
			++(*this);
			return it;
		}
	};

	struct LastDimensionMajorIterator{
		bool is_end = false;
		size_t index;
		Array<T, Dim1, Dims...>* outer;
		bool topLevel = true;
		typename Array<T, Dims...>::LastDimensionMajorIterator nested;
		LastDimensionMajorIterator() : index(0), outer(NULL) {
			nested.topLevel = false;
		}
		LastDimensionMajorIterator(Array<T, Dim1, Dims...>* theOuter) : index(0), outer(theOuter){
			nested.topLevel = false;
		}
		LastDimensionMajorIterator(bool) : index(0){
			nested.topLevel = false;
			setEnd();
		}
		void ldm_inc(){
			++index;
			if(index == Dim1){
				index = 0;
				nested.ldm_inc();
			}
		}

		void setEnd(){
			nested.setEnd();
		}

		bool getEnd() const{
			return nested.getEnd();
		}

		void print_val(){
			std::cout << index << std::endl;
			nested.print_val();
		}

		T& operator*(){
			nested.outer = &(outer->helpyhelp[index]);
			return *(nested);
		}

		bool operator==(const LastDimensionMajorIterator& it){
			if(this->nested == it.nested){
				if(index == it.index && outer == it.outer){
					return true;
				}
			}
			else if(index == it.index && getEnd() == true && it.getEnd() == true && topLevel){
				return true;
			}
			return false;
		}

		bool operator !=(const LastDimensionMajorIterator& it){
			return !((*this) == it);
		}

		LastDimensionMajorIterator& operator++(){
			/*if(fdm_inc()){
				is_end = true;
			}*/
			ldm_inc();
			return (*this);
		}

		LastDimensionMajorIterator operator++(int){
			LastDimensionMajorIterator it = (*this);
			++(*this);
			return it;
		}
	};

	public:
		typedef T ValueType;
		size_t n;
		using retType = Array<T, Dims...>;
		retType helpyhelp[Dim1];


		FirstDimensionMajorIterator fmbegin(){
			FirstDimensionMajorIterator f1(this);
			return f1;
		}

		LastDimensionMajorIterator lmbegin(){
			LastDimensionMajorIterator f1(this);
			return f1;
		}

		FirstDimensionMajorIterator fmend(){
			FirstDimensionMajorIterator f1(true);
			return f1;
		}

		LastDimensionMajorIterator lmend(){
			LastDimensionMajorIterator f1(true);
			return f1;
		}

		retType& operator[](size_t i){
			if(i >= n){
				OutOfRange o;
				throw o;
			}
			return helpyhelp[i];
		}

		const retType& operator[](size_t i) const{
			if(i >= n){
				OutOfRange o;
				throw o;
			}
			return helpyhelp[i];
		}

		Array& operator=(const Array<T,Dim1,Dims...>& toCopy){
			if(&toCopy == this){return (*this);}
			for(size_t i=0; i<this->n; ++i){
				this->helpyhelp[i] = toCopy.helpyhelp[i];
			}
			return (*this);
		}

		template <typename U>
		Array& operator=(const Array<U, Dim1, Dims...>& toCopy){		
			for(size_t i=0; i<this->n; ++i){
				this->helpyhelp[i] = toCopy.helpyhelp[i];
			}
			return (*this);
		}

		Array(const Array& toCopy) : n(toCopy.n){
			for(size_t i=0; i<this->n; ++i){
				this->helpyhelp[i] = toCopy.helpyhelp[i];
			}
		}

		template<typename U>
		Array(const Array<U, Dim1, Dims...>& toCopy) : n(toCopy.n){
			for(size_t i=0; i<this->n; ++i){
				this->helpyhelp[i] = toCopy.helpyhelp[i];
			}
		}

		Array(){
			static_assert((Dim1 > 0), "Invalid Size");
			n = Dim1;
			//std::cout << sizeof...(Dims) + 1 << std::endl;
		}

	};

	template<typename T, size_t Dim>
	class Array<T, Dim>{
	public:
		struct FirstDimensionMajorIterator{
			bool is_end = false;
			size_t index;
			Array<T, Dim>* outer;
			bool topLevel = true;
			FirstDimensionMajorIterator() : index(0), outer(NULL) {}
			FirstDimensionMajorIterator(Array<T, Dim>* theOuter) : index(0), outer(theOuter) {}
			FirstDimensionMajorIterator(bool end) : is_end(end), index(Dim) {}
			bool fdm_inc(){
				if (++index == Dim) {
                        return true;
                    }
                return false;
			}

			T& operator*() const{
				return outer->arr[index];
			}

			void print_val(){
				std::cout << index << std::endl;
			}

			bool operator==(const FirstDimensionMajorIterator& it){
				if(index == it.index && outer == it.outer){
					return true;
				}
				else if(index == it.index && is_end && it.is_end && topLevel && it.topLevel){
					return true;
				}
				return false;
			}

			bool operator !=(const FirstDimensionMajorIterator& it){
				return !((*this) == it);
			}

			FirstDimensionMajorIterator& operator++(){
				if(fdm_inc()){
					is_end = true;
				}
				return (*this);
			}

			FirstDimensionMajorIterator operator++(int){
				FirstDimensionMajorIterator it = (*this);
				++(*this);
				return it;
			}
		};

		struct LastDimensionMajorIterator{
			bool is_end = false;
			size_t index;
			Array<T, Dim>* outer;
			bool topLevel = true;
			LastDimensionMajorIterator() : index(0), outer(NULL) {}
			LastDimensionMajorIterator(Array<T, Dim>* theOuter) : index(0), outer(theOuter) {}
			LastDimensionMajorIterator(bool){
				setEnd();
			}
			void ldm_inc(){
				if(++index == Dim){
					is_end = true;
				}
			}

			void setEnd(){
				is_end = true;
				index = Dim;
			}

			bool getEnd() const{
				if(is_end && index == Dim){
					return true;
				}
				return false;
			}

			T& operator*() const{
				return outer->arr[index];
			}

			void print_val(){
				std::cout << index << std::endl;
			}

			bool operator==(const LastDimensionMajorIterator& it){
				if(index == it.index && outer == it.outer){
					return true;
				}
				else if(index == it.index && getEnd() == true && it.getEnd() == true && topLevel){
					return true;
				}
				return false;
			}

			bool operator !=(const LastDimensionMajorIterator& it){
				return !((*this) == it);
			}

			LastDimensionMajorIterator& operator++(){
				ldm_inc();
				return (*this);
			}

			LastDimensionMajorIterator operator++(int){
				LastDimensionMajorIterator it = (*this);
				++(*this);
				return it;
			}
		};

	public:


		FirstDimensionMajorIterator fmbegin(){
			FirstDimensionMajorIterator f1(this);
			return f1;
		}

		LastDimensionMajorIterator lmbegin(){
			LastDimensionMajorIterator f1(this);
			return f1;
		}

		FirstDimensionMajorIterator fmend(){
			FirstDimensionMajorIterator f1(true);
			return f1;
		}

		LastDimensionMajorIterator lmend(){
			LastDimensionMajorIterator f1(true);
			return f1;
		}

		Array(const Array& toCopy) : n(toCopy.n){
			for(size_t i=0; i<this->n; ++i){
				this->arr[i] = toCopy.arr[i];
			}
		}

		template<typename U>
		Array(const Array<U, Dim>& toCopy) : n(toCopy.n){
			for(size_t i=0; i<this->n; ++i){
				this->arr[i] = toCopy.arr[i];
			}
		}

		Array(){
			static_assert((Dim > 0), "Invalid Size");
			n = Dim;
			//std::cout << "MADE IT" << std::endl;
		}
		T arr[Dim];
		size_t n;
		typedef T ValueType;

		Array& operator=(const Array<T,Dim>& toCopy){
			if(&toCopy == this){return (*this);}
			for(size_t i=0; i<this->n; ++i){
				this->arr[i] = toCopy.arr[i];
			}
			return (*this);
		}

		template <typename U>
		Array& operator=(const Array<U, Dim>& toCopy){		
			for(size_t i=0; i<this->n; ++i){
				this->arr[i] = toCopy.arr[i];
			}
			return (*this);
		}

		T& operator[](size_t i){
			if(i >= n){
				OutOfRange o;
				throw o;
			}
			return arr[i];
		}

		const T& operator[](size_t i) const{
			if(i >= n){
				OutOfRange o;
				throw o;
			}
			return arr[i];
		}
	};
}

#endif
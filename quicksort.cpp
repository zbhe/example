// bobo@netease，2019-10-6
#include <iostream>

template<int...T> struct IntList{
	private:
		template<int F, int... L> static void DoPrint(){
			std::cout << F;
			if constexpr ( sizeof...(L) > 0 ){
				std::cout << ", ";
				DoPrint<L...>();
			}else{
				std::cout << std::endl;
			}
		}
	public:
		static void Print(){
			if constexpr ( sizeof...(T) > 0 ){
				DoPrint<T...>();
			}else{
				std::cout << "empty!!!\n";
			}
		}
	constexpr static int size = sizeof...(T);
};

template<int M, typename R>struct Insert;
template<int M, int... R>struct Insert<M, IntList<R...> >{
	using type = IntList<M, R...>;
};

template<int M, typename L> struct Append;
template<int M, int... L> struct Append<M, IntList<L...> >{
	using type = IntList<L..., M>;
};

template< typename, typename>struct Merge;
template<int... L, int... R>struct Merge<IntList<L...>, IntList<R...> >{
	using type = IntList<L..., R...>;
};

template< bool, typename Then, typename Else> struct IF{
	using type = Then;
};
template<typename Then, typename Else> struct IF<false, Then, Else>{
	using type = Else;
};

template< typename > struct SortList;
template<int M, typename> struct Group;
template<int M, int Next> struct Group<M, IntList<Next> >{
	using Left = typename IF< M < Next,
					IntList<>,
					IntList<Next> >::type;
	using Right = typename IF< M < Next,
					IntList<Next>,
					IntList<> >::type;
	using type = typename Merge< typename Append<M, Left>::type, Right>::type;
};
template<int M, int Next, int... Last> struct Group<M, IntList<Next, Last...> >{
	using LastGroup = Group<M, IntList<Last...> >;
	using Left = typename IF< M < Next, 
					typename LastGroup::Left, 
					typename Append<Next, typename LastGroup::Left>::type
				   >::type;
	using Right = typename IF< M < Next, 
					typename Append<Next, typename LastGroup::Right>::type, 
					typename LastGroup::Right
					>::type;
	using type = typename Merge<
				typename Append<M, typename SortList<Left>::type>::type,
				typename SortList<Right>::type
		>::type;
};

template <int Head> struct SortList<IntList<Head>>{
	using type = IntList<Head>;
};
template <> struct SortList<IntList<>>{
	using type = IntList<>;
};
template < int Head, int... Tail> struct SortList< IntList<Head, Tail...> >{
	using type = typename Group<Head, IntList<Tail...> >::type;
};
template <int... Nums> struct QuickSort{
	using type = typename SortList<IntList<Nums...>>::type;
};

int main()
{
	QuickSort<4, 3, 4, 90, 23, 87, 23, 55, 54, 98, 12>::type::Print();
	return 0;
}


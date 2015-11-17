#include <vector>
#include <unordered_map>

template<typename T> class Type{
	public:
		typedef T type;
};

class BaseValue
{
	public:
		virtual ~BaseValue() {}
		virtual BaseValue* clone()const = 0;
};
template<typename T> class RealValue:public BaseValue
{
	private:
		T Value;
	public:
		RealValue(const T& v):Value(v){}
		RealValue():Value(T()){}
		T Get()const{ return Value;}
		void Set( const T& v){ Value = v;}
		BaseValue* clone() const override {return new RealValue<T>(Value);}

};

class Value
{
	private:
		BaseValue* RValuePtr;
	public:
		template<typename T>
		explicit Value(const BaseValue& V)
		{
			RValuePtr = V.clone();
		}
		~Value()
		{
			delete RValuePtr;
			RValuePtr = nullptr;
		}
		template<typename T> T Get() const
		{
			return dynamic_cast<RealValue<T>*>(RValuePtr)->Get();
		}
		template<typename T> void Set(const T& v)
		{
			return dynamic_cast<RealValue<T>*>(RValuePtr)->Set(v);
		}
};

class Object
{
	private:
		std::unordered_map<std::string, Value*> Attribut;
		Value* FindAttr(const std::string& Name)
		{
			return Attribut[Name];
		}
	public:
		template<typename T>
		bool AddAttr(const std::string& Name, Type<T> TypeObj)
		{
			using AttrType = decltype(Attribut)::value_type;
			using VType = decltype(decltype(TypeObj)::type);
			Attribut.insert(AttrType(Name, new Value(RealValue<VType>())));
		}
		bool SetAttr(const std::string& Name, const Value& V)
		{
			Value* PValue = FindAttr(Name);
			if( nullptr == PValue){
				return false;
			}
			PValue->Set
		}
};

int main()
{
	return 0;
}

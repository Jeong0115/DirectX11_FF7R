#pragma once

namespace ff7r
{
	template<typename T>
	class Ptr
	{
	private:
		T* res;

	public:
		T* Get() const { return res; }

		T* operator -> () const
		{
			return res;
		}

		void operator = (T* _res)
		{
			if (nullptr != res)
				res->Release();

			res = _res;

			if (nullptr != res)
				res->AddRef();
		}

		void operator = (const Ptr<T>& _res)
		{
			if (nullptr != res)
				res->Release();

			res = _res.res;

			if (nullptr != res)
				res->AddRef();
		}

		bool operator == (T* _other)
		{
			return res == _other;
		}

		bool operator != (T* _other)
		{
			return res != _other;
		}

		bool operator == (const Ptr<T>& _other)
		{
			return res == _other.res;
		}

		bool operator != (const Ptr<T>& _other)
		{
			return res != _other.res;
		}



	public:
		Ptr()
			: res(nullptr)
		{

		}

		Ptr(T* _res)
			: res(_res)
		{
			if (nullptr != res)
				res->AddRef();
		}

		Ptr(const Ptr<T>& _res)
			: res(_res.res)
		{
			if (nullptr != res)
				res->AddRef();
		}

		~Ptr()
		{
			if (nullptr != res)
			{
				res->Release();
			}
		}
	};


	template<typename T>
	bool operator == (void* _res, const Ptr<T>& _ptr)
	{
		return _res == _ptr.Get();
	}

	template<typename T>
	bool operator != (void* _res, const Ptr<T>& _ptr)
	{
		return _res != _ptr.Get();
	}
}

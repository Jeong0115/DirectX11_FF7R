#pragma once

typedef void (*EXIT)(void);

namespace ff7r
{
	template<typename T>
	class Singleton
	{
	private:
		static T* m_Inst;

	public:
		static T* GetInst();
		static void Destroy();

	protected:
		Singleton() { }
		virtual ~Singleton() {}
	};

	template<typename T>
	T* Singleton<T>::m_Inst = nullptr;

	template<typename T>
	inline T* Singleton<T>::GetInst()
	{
		if (nullptr == m_Inst)
		{
			m_Inst = new T;
			atexit((EXIT)&Singleton<T>::Destroy);
		}

		return m_Inst;
	}

	template<typename T>
	inline void Singleton<T>::Destroy()
	{
		if (nullptr != m_Inst)
		{
			delete m_Inst;
			m_Inst = nullptr;
		}
	}
}
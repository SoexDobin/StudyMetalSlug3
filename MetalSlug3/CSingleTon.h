#pragma once

template <typename T>
class CSingleTon
{
protected:
	CSingleTon() {};
	virtual ~CSingleTon() {};

private:
	CSingleTon(const T&)		= delete;
	CSingleTon(const T&&)		= delete;
	T& operator=(const T&)		= delete;
	T& operator=(const T&&)		= delete;

public:
	static T& GetInstance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = new T;

		return *m_pInstance;
	}
	static void DeleteInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

protected:
	static T* m_pInstance;
};

template <typename T>
T* CSingleTon<T>::m_pInstance = nullptr;

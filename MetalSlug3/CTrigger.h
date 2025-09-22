#pragma once
#include "CCollider.h"
#include <functional>

class CTrigger : public CCollider
{
public:
	CTrigger();
	virtual ~CTrigger() override;

public:
	void	SetCallBack(function<void()>* _pCallBack)			{ m_pCallBack = _pCallBack; }
	void	SetPredicate(function<bool()>* _pColPredicate)		{m_pColPredicate = _pColPredicate; }

	bool	UpdateCollision() override;

private:
	function<void()>*	m_pCallBack;
	function<bool()>*	m_pColPredicate;
};


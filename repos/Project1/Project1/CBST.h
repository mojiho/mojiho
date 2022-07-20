#pragma once

template<typename T1, typename T2>
struct tPair
{
	T1 first;
	T2 second;
};


template<typename T1, typename T2>
struct tBSTNode
{
	tPair pair;

	tBSTNode* pParent;
	tBSTNode* pLeftChild;
	tBSTNode* pRightChild;

	//data
	//�θ���
	//�ڽĳ��
};


template<typename T1, typename T2>
class CBST
{
private:
	tBSTNode* m_pRoot; //��Ʈ��� �ּ�
	int m_iCount; // ������ ����


public:
	void insert(const tPair& _pair);

public:
	CBST()
		:m_pRoot(nullptr)
		, m_iCount(0)
	{}
};

template<typename T1, typename T2>
inline void CBST<T1, T2>::insert(const tPair<T1, T2>& _pair)
{
	tBSTNode<T1, T2>* pNewNode = new tBSTNode<T1, T2>();

	pNewNode->pair = _pair;
	pNewNode->pParent = nullptr;
	pNewNode->pLeftChild = nullptr;
	pNewNode->pRihhtChild = nullptr;

	//ù��° ������ ������
	if (nullptr == m_pRoot)
	{
		m_pRoot = pNewNode;
	}
}

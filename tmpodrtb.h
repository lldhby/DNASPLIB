//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	TMPODRTB.H
//  Template class management of realtime database table
//������ص���������һ������ͨ������������ݴ�����������������ݵļ���Ч��
//��������һ����ģ�壬���ģ�������Ӧ���е�ͨ���࣬���Լ��ٳ�������������߼���Ч��
//�й��������ģ����Բο��ܶ�ļ�����鼮
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if !defined(__TMPODRTB_H)
#define	__TMPODRTB_H
//#include "tmptable.h"

////////////////////////////////////////////////////////////////////////////////
//  class ChildPtr
//ר��Ϊ���ҹ�ϵ���������������࣬����ҵ�ͬ�����׵����ݣ�������������У�����
//���У�������׵�ַ���أ�ѭ��������࣬ÿ��ȡ��һ�����ݣ�ֱ��ȫ��ȡ����
class ChildPtr1
{
public:
		
	int			Ptr;
	ChildPtr1*	Next;

public:

	//constructors
	ChildPtr1()
	{
		Ptr = -1;
		Next = 0;
	}

	ChildPtr1(int m_ptr)
	{
		Ptr = m_ptr;
		Next = 0;
	}

	//assign value operator
	const ChildPtr1& operator = (const ChildPtr1& m_childptr)
	{
		Ptr = m_childptr.Ptr;
		Next = m_childptr.Next;

		return *this;
	}

	//free child pointer list
	friend void FreeChildPtr(ChildPtr1* m_childptr)
	{
		ChildPtr1 *head, *temp;

		head = m_childptr;
		while(head)
		{
			temp = head->Next;
			delete head;
			head = temp;
		}
	}
};
void FreeChildPtr(ChildPtr1* m_childptr);
////////////////////////////////////////////////////////////////////////////////
//  class DTable	��ģ�壬���е��豸�඼�����ɸ�ģ������
template <class T> class DTable
{
public:

	T*		Data;		//record data,���ݵĽṹ��
	int*	Ch;			//chain for data maintenance�������������е�λ�ã�ÿ�ζ���Phλ�ô�1������ָ�����
	int*	Ph;			//physical position corresponding to logic position//����λ��
	int*	Log;		//logic position corresponding to physical position//�߼�λ�ã����ݽǱ�
	int		Size;		//table size������ĳߴ�
	int		Count;		//record count�����������ݵĸ���
	int		Head;		//chain head�������ͷ
	int		Tail;		//chain tail�������β
	int		Next;		//next position for record inserting����¼�������һ��λ��
	int		Sequent;	//indicate if in sequent storage���Ƿ�������л�

public:

	//constructor and destructor
	DTable(int m_size);//����һ���յ������Ƚ��ڴ�ߴ翪����
	~DTable();

	//get tale size
	int GetSize() const 
	{
		return Size;
	}

	//get record count
	int GetCount() const
	{
		return Count;
	}

	//get next physical position����ȡ�����дһ������λ��
	int GetNext() const
	{
		return Next;
	}

	//check if sequent
	int IsSequent() const
	{
		return Sequent;
	}

	//get table pointer���õ����ͷָ�룬����������Ʒ���
	T* GetTable()
	{
		return Data;
	}

	//get physical position by logic one��ͨ���߼��ŵõ�����λ��
	int GetPhByLog(int log) const
	{
		return Ph[log];
	}

	//get logic position by physical one��ͨ������ŵõ��߼�λ��
	int	GetLogByPh(int ph) const
	{
		return Log[ph];
	}

//Ҫ��õ����ݣ�������Ҫ����������λ�ã�Ҳ�����ڴ�ĵ�ַ
//�߼�λ��ֻ��Ϊ�˷�����Ҷ�����һ����ǣ�����߼��ź������Ҫһ��
	//get data by logic position
	T* GetDataByLog(int log)
	{
		return &Data[Ph[log]];
	}

	//get data by physical position
	T* GetDataByPh(int ph)
	{
		return &Data[ph];
	}

	//copy data in table������������һ��������
	void CopyTable(T* buffer) const;

	//copy data by logic position
	void CopyDataByLog(T* buffer, int log) const
	{
		*buffer = Data[Ph[log]];
	}

	//copy data by physical position
	void CopyDataByPh(T* buffer, int ph) const
	{
		*buffer = Data[ph];
	}

	//update data in table�������ݽ��и���
	void UpdateTable(T* buffer);

	//update data by logic position�����²�������
	void UpdateDataByLog(T* buffer, int log)
	{
		Data[Ph[log]] = *buffer;
	}

	//update data by physical position
	void UpdateDataByPh(T* buffer, int ph)
	{
		Data[ph] = *buffer;
	}

	//find data����������
	T* Find( T& t );

	//find data�����ַ���������type=0 �����ݣ�1 ������ţ�2 ���߼���
	T* BiFind( T& t);
	
	//���ֲ�������ź��߼���
	int BiFind( T& t, int type);

	//find physical position
	int	FindPh( T& t );

	//find logical position
	int FindLog( T& t );

	//insert data�����뵥������
	int	Insert( T& t );

	//insert a number of data��������������
	int Insert(T* buffer, int num);

	//append a number of data��׷�����ݣ��������߼��ź������
	//static int __cdecl Cmp( const void *a , const void *b ); 
	void myQsort(T* t, size_t num);
	int Append(T& t);
	int	Sort(int num,int type=1);

	//remove data��ɾ������
	int	Remove( const T& t );

	//empty table����ձ�
	int	Empty();

	//sequence table
	int	Sequence();

	//build chain for sequent data
	int BuildChain();

	//assign value operator
	const DTable<T>& operator = (const DTable<T>& t);

	//read or write table
//	void Serialize( CArchive& ar );

public:

	//methods designed for relation table

	//find parent pointer by child id
	int	FindParentPtr1(const char* c_id) const;

	//find parent pointer by relation pointer value
	int	FindParentPtr1(int r_ptr) const;

	//find child pointer list by parent id
	ChildPtr1* FindChildPtr1(const char* p_id) const;

	//find child pointer list by relation pointer value
	ChildPtr1* FindChildPtr1(int r_ptr) const;

	//find child number by parent id
	int	FindChildNum(const char* p_id) const;

	//find child number by relation pointer value
	int FindChildNum(int r_ptr) const;
};

///////////////////////////////////////////////////////////////////////////////
//  constructor
template <class T> DTable<T>::DTable(int m_size)
{
	int		i;

	Data = new T[m_size];	//�������飬�洢һ�����ݵĽṹ��
	Ch = new int[m_size];	//�������У��洢������,Ch���߼��Ż���һ�£�log+1
	Ph = new int[m_size];	//�������У��洢���������ţ�Data�ĽǱ�
	Log = new int[m_size];	//�������У��洢����߼���ţ���˳������
	Size = m_size;	//�ߴ�
	Count = 0;
	Head = -1;
	Tail = -1;
	Next = 0;
	Sequent = 1;

	for(i = 0; i < m_size; i++)
	{
		Ch[i] = i+1;	//�������Ŵ�1��ʼ,chָ�����ǱȺ����һλ����Ҫ��Ϊ������˳������
		Ph[i] = -1;		//����λ�ú��߼�λ�ö���Ϊ-1
		Log[i] = -1;
	}

	Ch[m_size-1] = -1;	//���������һ�������Ϊ-1
}
	
///////////////////////////////////////////////////////////////////////////////
// destructor	
template <class T> DTable<T>::~DTable()
{
	delete[] Data;
	delete[] Ch;
	delete[] Ph;
	delete[] Log;
}

///////////////////////////////////////////////////////////////////////////////
//  CopyTable()
//  Attention:	This function will not check the size of buffer.
//�ѵ�ǰ�����е����ݿ�����һ���������棬�������Ľṹ������Ľṹһ��
template <class T> void DTable<T>::CopyTable( T* buffer ) const
{
	int		pos, cur;

	pos = 0;
	cur = Head;//���������ĵ�һ�����ݿ�ʼ����
	while(cur != -1)
	{
		buffer[pos++] = Data[cur];
		cur = Ch[cur];//���������ƶ�һ��ָ�룬���������Ǵ�1��ʼ��
		//Ҳ����ch[0]=1;ch[1]=2;ch[2]=3;...ch[n]=-1;,��ch[]�����һ��ʱΪ-1��ѭ��ֹͣ
	}
}

//copy data in table�������������Ķ��ಿ�ֽص���ֻ����ǰ��Ĳ���
//lld edit 2018-4-24
//template <class T> void DTable<T>::TrimTable(int cnt) const
//{
//	int		pos, cur;
//	T*	buffer = new T[cnt];	//�������飬�洢һ�����ݵĽṹ��
//	CopyTable(T);
//	delete[] Data;
//	Data = new T[cnt];	//�������飬�洢һ�����ݵĽṹ��
//	pos = 0;
//	cur = Head;//���������ĵ�һ�����ݿ�ʼ����
//	while(cur != -1)
//	{
//		Data[pos++] = buffer[cur];
//		cur = Ch[cur];//���������ƶ�һ��ָ�룬���������Ǵ�1��ʼ��
//		//Ҳ����ch[0]=1;ch[1]=2;ch[2]=3;...ch[n]=-1;,��ch[]�����һ��ʱΪ-1��ѭ��ֹͣ
//	}
//	//Tail = cnt-1;
//	//Size = cnt;
//	delete[] buffer;
//}

///////////////////////////////////////////////////////////////////////////////
//  UpdateTable()
//  Attention:	This function will not check the size of buffer.
//��һ�����棨���������Ľṹһ�������������ݸ��µ�������
template <class T> void DTable<T>::UpdateTable( T* buffer ) 
{
	int		pos, cur;

	pos = 0;
	cur = Head;
	while(cur != -1)
	{
		Data[cur] = buffer[pos++];
		cur = Ch[cur];
	}
}

///////////////////////////////////////////////////////////////////////////////
//  BiFind() ���ֲ��ң��������е�Ԫ�ض����߼��Ŵ�С��������
//�߼���ʵ��������ŵ��±꣬Ҳ�������ݵ���������˳��
template <class T> T* DTable<T>::BiFind( T& t )
{
	//int log;//�߼���
	int high = Count - 1;
//	if (high == 0)//ֻ��һ������
//		return 0;
	
	int low = 0;
	int middle;

    while(low <= high)
	{
        middle = (low + high) / 2;

        if(t == Data[Ph[middle]])
		{  //�ҵ�,�����±�����ֵ
            return &Data[Ph[middle]];
        }
		else if(t<Data[Ph[middle]])
		{  //����ֵ�ڵͰ���
            high = middle - 1;
        }
		else
		{  //����ֵ�ڸ߰���
            low = middle + 1;
        }
    }
    return 0;  //�Ҳ���
}	


///////////////////////////////////////////////////////////////////////////////
//  Find()  ʵ��������id���ڲ���
template <class T> T* DTable<T>::Find( T& t )
{
	return BiFind(t);


	int		cur;

	cur = Head;//��ͷ��ʼ����
	while(cur != -1)
	{
		//�ж���������������Ƿ���ȣ������Ҫ�ж�ÿ��ʵ���������ݣ�һ�����
		//������ID�Ż��߼���ID�����ж�
		if(Data[cur] == t)
		{
			return &Data[cur];
		}
		cur = Ch[cur];
	}

	return 0;
}

template <class T> int DTable<T>::BiFind( T& t , int type)
{
	if (Count == 0)
		return -1;

	int high = Count - 1;
	int low = 0;
	int middle;

	//������
	if ((type==0) || (type==1))
	{
		while(low <= high)
		{
			middle = (low + high) / 2;

			if(t == Data[Ph[middle]])
			{  //�ҵ�,�����±�����ֵ
				if (type==0)//FindPh
				{
					t = Data[Ph[middle]];//�����е��ҵ��ĸ�����ʱ����
					return Ph[middle];
				}
				else if (type==1)//FindLog
				{
					t = Data[Ph[middle]];//�����е��ҵ��ĸ�����ʱ����
					return middle;
				}
			}
			else if(t<Data[Ph[middle]])
			{  //����ֵ�ڵͰ���
				high = middle - 1;
			}
			else
			{  //����ֵ�ڸ߰���
				low = middle + 1;
			}
		}
	}
	else//���ҿ��Բ����λ��
	{
		if(t<Data[Ph[low]])//����С�Ļ�ҪС
		{
			//���������
			return Ph[low];
		}
		
		int dist=10000000;
		//high++;//������ 
		while(low <= high)//�����λ�ò�������ȫ�Ǻ�
		{
			middle = (low + high) / 2;

			if ((high-low) < dist)
			{
				dist = high-low;//�øߵ����価����С���������������˵���ҵ���
			}
			else
			{
				//���������
				return Ph[high];
			}

			if(t<Data[Ph[middle]])
			{  //����ֵ�ڵͰ���
				high = middle;
			}
			else
			{  //����ֵ�ڸ߰���
				low = middle;
			}
		}

	}

	return -1;
}
///////////////////////////////////////////////////////////////////////////////
//  FindPh()���������ݵ�����λ�ã�һ��������ݵ�˳����λ��
template <class T> int DTable<T>::FindPh(  T& t )
{
	return BiFind( t , 0);

	int		cur;

	cur = Head;
	while(cur != -1)
	{
		if(Data[cur] == t)
		{
			return cur;
		}
		cur = Ch[cur];
	}

	return -1;
}	

///////////////////////////////////////////////////////////////////////////////
//  FindLog()
template <class T> int DTable<T>::FindLog( T& t )
{
	return BiFind( t , 1);

	int		cur;

	cur = Head;
	while(cur != -1)
	{
		if(Data[cur] == t)
		{
			return Log[cur];
		}
		cur = Ch[cur];
	}

	return -1;
}	

///////////////////////////////////////////////////////////////////////////////
//  Insert()
//  Insert a record into the table by ascending order.
//�ؼ��Է����������ݲ��뵽������
template <class T> int DTable<T>::Insert( T& t )
{
	int		cur, curr, pos, phpos, prev, temp;

	//check if table is full
	//˵��ָ���Ѿ�������Ҫ�����ڴ�
	if(Next == -1)
	{
		return -1;
	}

	//make sure the data is unique
/*
	if(Find(t))
	{
		return -1;
	}

*/
	//insert data 
	if(Count == 0)
	{
		//��ʼ����ʱ��Count = 0;	Head = -1;	Tail = -1;	Next = 0;

		Data[Next] = t;		//Data[0] = t����ֵ
		phpos = Next;		//phpos=0�����εõ��ı�ţ��������
		Head = Next;		//Head=0
		Tail = Next;		//Tail=0
		Ph[Count] = Next;	//Ph[0]=0������ţ�����ĽǱ꣬�����һֱ���Ǵ�0��ʼ����
		Log[Next] = Count++;//Log[0]=0���߼��ţ�����������ӣ��߼���Ҳ�Ǵ�0��ʼ���������п������������
		Next = Ch[Next];	//Next=Ch[0]=1��ch����ָ�����,��ʼ����ʱ��ͱ��Լ��ĽǱ��1
		Ch[Tail] = -1;		//Ch[0]=-1 ��������һ����Чλ��Ϊ-1
	}
	else
	{		
		if(Data[Tail] < t)//�жϴ�СҪ��ʵ�����йأ�һ����ID��
		{
			//insert data after tail
			Data[Next] = t;
			phpos = Next;
			Ch[Tail] = Next;
			Tail = Next;
			Ph[Count] = Next;
			Log[Next] = Count++;
			Next = Ch[Next];
			Ch[Tail] = -1;
		}
		else
		{
			cur = BiFind(t,2);//���Բ��������λ�ã�����������߼��������һ��λ��
			pos = Log[cur];//���׼�����Ƶ��߼�λ��
			if (pos==0)
				prev = -1;
			else
				prev = Ph[pos-1];//ǰһ������λ�õ�����

			Data[Next] = t;
			phpos = Next;
			Ph[pos] = Next;
			Log[Next] = pos;
			
			//�������Ƶ����ݵ��߼�λ��
			Ph[++pos] = cur;//�����Ƶ����ݵ�����λ�ú��߼�λ�õĶ�Ӧ��ϵ�ı�
			Log[cur]++;  // == pos�����Ƶ����ݵ��߼��ż�1
			
			curr = Ch[cur];//�жϺ��Ƶ������Ƿ�������һ������
			while(curr != -1)//�������һ�����ݣ�������ǰ�������һ�����ݣ����������λ����Ҫ���Ųһ��
			{
				//�߼��ź���һλ
				Ph[++pos] = curr;//���������ά����ʹ��ʼ�պ��߼��ű���ͬ��
				Log[curr]++;
				curr = Ch[curr];
			}
			//����ǵ�һ�β��룬ֱ�ӽ�Nextλ�ø�ֵ
			//������������ݣ�ÿ�β���ʱ��Ҫ���������ά��
			if(prev == -1)  //insert data before head
			{
				Head = Next;
				temp = Next;
				Next = Ch[Next];
				Ch[temp] = cur;
			}
			else
			{
				Ch[prev] = Next;
				temp = Next;
				Next = Ch[Next];
				Ch[temp] = cur;
			}
			Sequent = 0;//ֻҪ�в�����������ƻ��߼��ź�����ŵ�˳��
			Count++;



/*

			//insert data by ascending order
			cur = Head;
			pos = 0;
			prev = -1;
			while(cur != -1)//��ͷ��ʼ���м���
			{
				if(t < Data[cur])//�ҵ����Բ���ĵط�
				{
					Data[Next] = t;
					phpos = Next;
					Ph[pos] = Next;
					Log[Next] = pos;
					Ph[++pos] = cur;
					Log[cur]++;  // == pos
					curr = Ch[cur];
					while(curr != -1)//�������������м䣬������ǰ�������һ�����ݣ����������λ����Ҫ���Ųһ��
					{
						Ph[++pos] = curr;
						Log[curr]++;
						curr = Ch[curr];
					}
					//����ǵ�һ�β��룬ֱ�ӽ�Nextλ�ø�ֵ
					if(prev == -1)  //insert data before head
					{
						Head = Next;
						temp = Next;
						Next = Ch[Next];
						Ch[temp] = cur;
					}
					else
					{
						Ch[prev] = Next;
						temp = Next;
						Next = Ch[Next];
						Ch[temp] = cur;
					}
					Sequent = 0;
					Count++;
					break;
				}
				pos++;
				prev = cur;
				cur = Ch[cur];
			}
*/
			
		}
	}
	
	return phpos;
}

///////////////////////////////////////////////////////////////////////////////
//  Insert()
template <class T> int DTable<T>::Insert( T* buffer, int num )
{
	int		i;

	for(i = 0; i < num; i++)
	{
		if(Insert(buffer[i]) == -1)
		{
			break;
		}
	}

	return i;
}

///////////////////////////////////////////////////////////////////////////////
//  Append()
//  Append a record into the table by ascending order.
//�ؼ��Է����������ݲ��뵽�����У�������˳����������������
template <class T> int DTable<T>::Append( T& t )
{
	int	phpos;

	//check if table is full
	//˵��ָ���Ѿ����ڣ���Ҫ�����ڴ�
	if(Next == -1)
	{
		return -1;
	}

	//һֱ�����������߼��ź������һ�£�������ϵ��ʱҪ�õ������ 2018-4-26
	Data[Next] = t;
	phpos = Next;
	//Ch[Tail] = Next;
	Tail = Next;
	Ph[Count] = Next;
	Log[Next] = Count++;
	Next = Ch[Next];
	Ch[Tail] = -1;

/* lld comment 2018-4-26
	Data[Next] = t;		//Data[0] = t
	phpos = Next;		//phpos=0
//	Head = Next;		//Head=0
	Tail = Next;		//Tail=0
	Next = Ch[Next];	//Next=Ch[0]=1��ch����ָ�����,��ʼ����ʱ��ͱ��Լ��ĽǱ��1
	Ch[Tail] = -1;		//Ch[0]=-1 ��������һ����Чλ��Ϊ-1
	Count++;//�����ۼ�
*/
	return phpos;
}

//template <class T> int __cdecl DTable<T>::Cmp( const void *a , const void *b ) 
//int Cmp( const void *a , const void *b ) 
//{ 
//	return a < b; 
//} 

#define exchange_(A, B, C)  ((C) = (A), (A) = (B),(B)=(C))    
//template <class T> int DTable<T>::Sort(int num)
template <class T> void DTable<T>::myQsort(T* Data, size_t num)
{
	T temp;
	size_t i, j;
	/*
	* Check the simple cases first:
	* If fewer than 2 elements, already sorted
	* If exactly 2 elements, just swap them (if needed).
	  */
	if (num <2)
			return;
	else if (num==2)
	{
	   if (Data[1] < Data[0])
			 exchange_ (Data[0], Data[1] ,temp);
	}
	/*
	* Partition the array using the middle (num/2)
	element as the dividing element.
	  */
	exchange_ (Data[0], Data[num / 2], temp);
	i=1;
	j=num;
	for (; ;)
	{
		 /*
		   * Sweep forward until and element is found that
		   * belongs in the second partition.
		   */
		  while (i<j)
		  {
			  if (Data[i] == Data[0])
			  {
				  //Ph[i] = 1;//��ǳ��ظ���¼					
				  i++;
				  continue;
			  }
			  if (Data[i] < Data[0])
			  {
				i++;
				continue;
			  }
			  break;
		  }
		  /*
			* Then sweep backward until an element
			* is found that belongs in the first
			* partition.
			*/
		 while (i<j)
		 {
			 if (Data[0] == Data[j-1])
			 {
   			     //Ph[j-1] = 1;//��ǳ��ظ���¼					
				 j--;
				  continue;
			 }
			 if (Data[0] < Data[j-1])
			 {
				  j--;
				  continue;
			 }
			 break;
		 }
		  /* If no out-of-place elements, you're done */
		 if (i>=j)
			  break;
		   /* Else, swap the two out-of-place elements */
		  exchange_(Data[i], Data[j-1], temp);
	}
	/* Restore dividing element */
	exchange_(Data[0], Data[i-1], temp);
	/* Now apply quick sort to each partition */
	myQsort (Data, i-1 );
	myQsort (Data + i, num-i);
}
        
                           
//��append��ȥ��ȫ�����ݽ�������ɾ���ظ���¼�������趨�߼��š�����š������
template <class T> int DTable<T>::Sort(int num,int type)
{
	int		i,  cnt;
	T*		temp;
	T tmp;//

	//��Ҫ����
	if (type == 1)
		myQsort(Data,num);
	//qsort(Data,num,sizeof(T),Cmp);

	
//ɾ���ظ���¼
	temp = new T[Count];

//	pos = 0;
//	cur = Head;

	for(i = 0; i < Count; i++)
	{
		temp[i] = Data[i];
	}
		
//	while(cur != -1)//�����ݴ�ͷ��βȫ��д����ʱ����������
//	{
//		temp[pos++] = Data[cur];
//		cur = Ch[cur];
//	}

	//����ʱ��ȫ���ٵ���Data���ݱ����棬������ź��߼���ͳһ����
	Data[0] = temp[0];
	Ch[0] = 1;
	Ph[0] = 0;
	Log[0] = 0;
	cnt = 1;
	tmp = Data[0];
		
	for(i = 1; i < Count; i++)
	{
		if (temp[i] == tmp)
		{
		}
		else
		{
			Data[cnt] = temp[i];
			Ch[cnt] = cnt + 1;
			Ph[cnt] = cnt;
			Log[cnt] = cnt;
			tmp = Data[cnt];
			
			cnt++;
		}
	}
	Count = cnt;
	Head = 0;
	Tail = Count - 1;
	Ch[Tail] = -1;

	//���������еĿհײ���
	for(i = Count; i < Size; i++)
	{
		Data[i] = T();//�����ÿհ����
		Ch[i] = i + 1;
		Ph[i] = -1;//����ź��߼��Ŷ�����-1
		Log[i] = -1;
	}
	Next = Count;//�����������������鲿�ֵ���һ�������ڽ��Ĳ���
	Ch[Size-1] = -1;

	Sequent = 1;//��ʾ�Ѿ��������
	delete[] temp;//����ʱ���������

	return Count;
}

///////////////////////////////////////////////////////////////////////////////
//  Remove()
template <class T> int DTable<T>::Remove( const T& t )
{
	int		cur, curr, pos, prev;

	cur = Head;
	pos = 0;
	prev = -1;
	while(cur != -1)
	{
		if(Data[cur] == t)
		{
			Data[cur] = T();
			if(cur == Head) 
			{
				Head = Ch[cur];
			}
			if(cur == Tail)
			{
				Tail = prev;
			}
			Ph[pos++] = Ch[cur];
			Log[cur] = -1;
			curr = Ch[cur];
			while(curr != -1)
			{
				Ph[pos++] = Ch[curr];
				Log[curr]--;
				curr = Ch[curr];
			}
			if(prev != -1)
			{
				Ch[prev] = Ch[cur];
			}
			Ch[cur] = Next;
			Next = cur;
			Count--;
			Sequent = 0;
			return 1;
		}
		pos++;
		prev = cur;
		cur = Ch[cur];
	}

	return 0;
}
		
///////////////////////////////////////////////////////////////////////////////
//  Empty()
//  Remove all records in table.
template <class T> int DTable<T>::Empty()
{
	int		i, cur, pos;

	cur = Head;
	pos = 0;
	while(cur != -1)
	{
		Data[cur] = T();
		Ph[pos++] = -1;
		Log[cur] = -1;
		cur = Ch[cur];
	}

	for(i = 0; i < Size-1; i++)
	{
		Ch[i] = i+1;
	}
	Ch[Size-1] = -1;

	Head = -1;
	Tail = -1;
	Next = 0;
	Count = 0;
	Sequent = 1;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  Sequence()
//  Sort the records in table by sequence order. After executing
//  this method, the logic positions and physical positions of
//  all records in the table are the same.
template <class T> int DTable<T>::Sequence()
{
	int		i, cur, pos;
	T*		temp;

	if(Sequent == 1)//�����Ѿ��Ź�����
	{
		return 1;
	}

	if(Count == 0)//�ѿ�������������һ��
	{
		for(i = 0; i < Size-1; i++)
		{
			Ch[i] = i+1;
		}
		Ch[Size-1] = -1;
		Next = 0;
		Sequent = 1;
	}
	else
	{
		//����һ����ʱ����������������������ݵĲ���������
		//���д����ķ����ݲ���ҲҪ����
		temp = new T[Count];

		pos = 0;
		cur = Head;
		while(cur != -1)//�����ݴ�ͷ��βȫ��д����ʱ����������
		{
			temp[pos++] = Data[cur];
			cur = Ch[cur];
		}

		//����ʱ��ȫ���ٵ���Data���ݱ����棬������ź��߼���ͳһ����
		for(i = 0; i < Count; i++)
		{
			Data[i] = temp[i];
			Ch[i] = i + 1;
			Ph[i] = i;
			Log[i] = i;
		}
		Head = 0;
		Tail = Count - 1;
		Ch[Tail] = -1;

		//���������еĿհײ���
		for(i = Count; i < Size; i++)
		{
			Data[i] = T();//�����ÿհ����
			Ch[i] = i + 1;
			Ph[i] = -1;//����ź��߼��Ŷ�����-1
			Log[i] = -1;
		}
		Next = Count;//�����������������鲿�ֵ���һ�������ڽ��Ĳ���
		Ch[Size-1] = -1;

		Sequent = 1;//��ʾ�Ѿ��������
		delete[] temp;//����ʱ���������
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  BuildChain()
//���������´����������漰������
template <class T> int DTable<T>::BuildChain()
{
	int		i;

	if(Sequent != 1)
	{
		return 0;
	}

	if(Count == 0)
	{
		return 0;
	}

	Head = 0;
	Tail = Count-1;
	Next = Count;

	for(i = 0; i < Count; i++)
	{
		Ch[i] = i+1;
		Ph[i] = i;
		Log[i] = i;
	}

	Ch[Tail] = -1;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  operator = 
//������ֵ����һ����������һ�������������ݺ�λ����Ϣ
template <class T> const DTable<T>& DTable<T>::operator = (const DTable<T>& t)
{
	int		i;

	//����µĳߴ���ϵĲ�һ�������ϵ�ȫ����������·����ڴ�
	if(Size != t.Size)
	{
		delete[] Data;
		Data = new T[t.Size];
	}

	for(i = 0; i < t.Size; i++)
	{
		Data[i] = t.Data[i];
		Ch[i] = t.Ch[i];
		Ph[i] = t.Ph[i];
		Log[i] = t.Log[i];
	}

	Size = t.Size;
	Count = t.Count;
	Head = t.Head;
	Tail = t.Tail;
	Next = t.Next;
	Sequent = t.Sequent;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  Serialize()
/*
template <class T> void DTable<T>::Serialize(CArchive& ar)
{
	int		i;

	if(ar.IsStoring())
	{
		ar << Size;
		ar << Count;
		ar << Head;
		ar << Tail;
		ar << Next;
		for(i = 0; i < Size; i++)
		{
			ar << Ch[i];
			ar << Ph[i];
			ar << Log[i];
		}
	}
	else
	{
		ar >> Size;
		ar >> Count;
		ar >> Head;
		ar >> Tail;
		ar >> Next;
		for(i = 0; i < Size; i++)
		{
			ar >> Ch[i];
			ar >> Ph[i];
			ar >> Log[i];
		}
	}

	i = Head;
	while(i != -1)
	{
		Data[i].Serialize(ar);
		i = Ch[i];
	}

//	for(i = 0; i < Size; i++)
//	{
//		Data[i].Serialize(ar);
//	}

}
*/

////////////////////////////////////////////////////////////////////////////////
//  FindParentPtr()
//���ӹ�ϵ����Ҫ���ڸ��ֹ�ϵ���У���Ҫ�ӹ�ϵ�������ҵ������Ӧ������
//ͨ���ӵ�id�ŵõ�����
//ѭ��������ϵ���ҵ�������׵�id�ź󣬽�������ŵõ�
template <class T> 	int DTable<T>::FindParentPtr1(const char* c_id) const
{
	int high = Count - 1;
	int low = 0;
	int middle;

    while(low <= high)
	{
        middle = (low + high) / 2;

		if(strcmp(Data[Ph[middle]].GetChildId(), c_id) == 0)
		{  //�ҵ�,�����±�����ֵ
            return Data[Ph[middle]].GetParentPtr();
        }
		else if ( strcmp(c_id , Data[Ph[middle]].GetChildId()) < 0 )
		{  //����ֵ�ڵͰ���
            high = middle - 1;
        }
		else
		{  //����ֵ�ڸ߰���
            low = middle + 1;
        }
    }
    return -1;  //�Ҳ���
//////////////////////////////////////////////////////////////////////////




	int		cur;

	//�������������ѭ��������ѭ��
	cur = Head;
	while(cur != -1)
	{
		if(strcmp(Data[cur].GetChildId(), c_id) == 0)
		{
			return Data[cur].GetParentPtr();
		}
		cur = Ch[cur];
	}

	return -1;
}


////////////////////////////////////////////////////////////////////////////////
//  FindParentPtr()
//ͨ�����ݵ�����ŵõ����׵�id��
//��֪��������ڹ�ϵ���еĶ�Ӧλ��
//nd r_sec
//2  6
//��ʾ�ڵ��Ϊ2 �Ľڵ���r_nd_sec��ϵ���еĵ�6��λ�ÿ����ҵ�����ڵ�����һ���߶�
template <class T> 	int DTable<T>::FindParentPtr1(int r_ptr) const
{
	if(r_ptr != -1)
	{
		return Data[r_ptr].GetParentPtr();
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//  FindChildPtr()
//����id�ŷ����Ӽ����׵�ַ������λ��
//���븸�׵�����λ�ã����λ���Ǹ��׵�һ�γ����ڹ�ϵ���е�λ�ã������ٳ��ֵĶ����Ժ������
//һ������²������������������Ҫ���ٶ�̫��
template <class T> 	ChildPtr1* DTable<T>::FindChildPtr1(const char* p_id) const
{
	
	int			cur;
	ChildPtr1	*head, *tail, *temp;

	head = 0;
	tail = 0;
	temp = 0;

//���������Ҫ�ӹ�ϵ����ʼλ�ý������������Ȳ���֪���ڹ�ϵ���е�һ�γ��ֵ�λ��
	cur = Head;
	while(cur != -1)
	{
		if(strcmp(Data[cur].GetParentId(), p_id) == 0)
		{
			//����һ�����Ӽ�Ԫ��
			temp = new ChildPtr1(Data[cur].GetChildPtr());
			if(!head)
			{
				head = temp;
				tail = temp;
			}
			else
			{
				tail->Next = temp;
				tail = temp;
			}
		}
		cur = Ch[cur];
	}

	return head;
}

///////////////////////////////////////////////////////////////////////////////
//  FindChildPtr()
template <class T> 	ChildPtr1* DTable<T>::FindChildPtr1(int r_ptr) const
{
	int			fptr;
	ChildPtr1	*head, *tail, *temp;

	head = 0;
	tail = 0;
	temp = 0;

	fptr = r_ptr;//���ָ���Ǹ��׵�һ���ڹ�ϵ���г��ֵ�λ��
	while(fptr != -1)
	{
		temp = new ChildPtr1(Data[fptr].GetChildPtr());
		if(!head)
		{
			head = temp;
			tail = temp;
		}
		else
		{
			tail->Next = temp;
			tail = temp;
		}
		fptr = Data[fptr].fptr;//ͬ���ĸ��׵ڶ��Σ���һ�Σ����ֵ�λ�ã�������ά��
	}

	return head;
}

///////////////////////////////////////////////////////////////////////////////
//  FindChildNum()
template <class T> 	int DTable<T>::FindChildNum(const char* p_id) const
{
	int		cur, num;

	cur = Head;
	num = 0;
	while(cur != -1)
	{
		if(strcmp(Data[cur].GetParentId(), p_id) == 0)
		{
			num++;
		}
		cur = Ch[cur];
	}

	return num;
}

///////////////////////////////////////////////////////////////////////////////
//  FindChildNum()
template <class T> 	int DTable<T>::FindChildNum(int r_ptr) const
{
	int		fptr, num;

	fptr = r_ptr;
	num = 0;
	while(fptr != -1)
	{
		num++;
		fptr = Data[fptr].fptr;
	}

	return num;
}

#endif
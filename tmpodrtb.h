//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	TMPODRTB.H
//  Template class management of realtime database table
//该类的重点在于做了一个链表，通过链表进行数据处理，这样可以提高数据的检索效率
//另外做了一个类模板，这个模板可以适应所有的通用类，可以减少程序量，不能提高检索效率
//有关链表和类模板可以参考很多的计算机书籍
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if !defined(__TMPODRTB_H)
#define	__TMPODRTB_H
//#include "tmptable.h"

////////////////////////////////////////////////////////////////////////////////
//  class ChildPtr
//专门为查找关系表中数据做的子类，如果找到同样父亲的数据，将其存入子类中，挨个
//排列，最后将其首地址返回，循环这个子类，每次取下一个数据，直到全部取出来
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
//  class DTable	类模板，所有的设备类都可以由该模板生成
template <class T> class DTable
{
public:

	T*		Data;		//record data,数据的结构体
	int*	Ch;			//chain for data maintenance，数据在链表中的位置，每次都比Ph位置大1，便于指针后移
	int*	Ph;			//physical position corresponding to logic position//物理位置
	int*	Log;		//logic position corresponding to physical position//逻辑位置，数据角标
	int		Size;		//table size，链表的尺寸
	int		Count;		//record count，链表中数据的个数
	int		Head;		//chain head，链表的头
	int		Tail;		//chain tail，链表的尾
	int		Next;		//next position for record inserting，记录插入的下一个位置
	int		Sequent;	//indicate if in sequent storage，是否进行序列化

public:

	//constructor and destructor
	DTable(int m_size);//构造一个空的链表，先将内存尺寸开出来
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

	//get next physical position，获取链表的写一个物理位置
	int GetNext() const
	{
		return Next;
	}

	//check if sequent
	int IsSequent() const
	{
		return Sequent;
	}

	//get table pointer，得到表的头指针，将数组的名称返回
	T* GetTable()
	{
		return Data;
	}

	//get physical position by logic one，通过逻辑号得到物理位置
	int GetPhByLog(int log) const
	{
		return Ph[log];
	}

	//get logic position by physical one，通过物理号得到逻辑位置
	int	GetLogByPh(int ph) const
	{
		return Log[ph];
	}

//要想得到数据，还是需要在它的物理位置，也就是内存的地址
//逻辑位置只是为了方便查找而做的一个标记，最后逻辑号和物理号要一致
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

	//copy data in table，拷贝整个表到一个缓存里
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

	//update data in table，对数据进行更新
	void UpdateTable(T* buffer);

	//update data by logic position，更新部分数据
	void UpdateDataByLog(T* buffer, int log)
	{
		Data[Ph[log]] = *buffer;
	}

	//update data by physical position
	void UpdateDataByPh(T* buffer, int ph)
	{
		Data[ph] = *buffer;
	}

	//find data，检索数据
	T* Find( T& t );

	//find data，二分法检索数据type=0 找数据，1 找物理号，2 找逻辑号
	T* BiFind( T& t);
	
	//二分查找物理号和逻辑号
	int BiFind( T& t, int type);

	//find physical position
	int	FindPh( T& t );

	//find logical position
	int FindLog( T& t );

	//insert data，插入单个数据
	int	Insert( T& t );

	//insert a number of data，批量插入数据
	int Insert(T* buffer, int num);

	//append a number of data，追加数据，不考虑逻辑号和物理号
	//static int __cdecl Cmp( const void *a , const void *b ); 
	void myQsort(T* t, size_t num);
	int Append(T& t);
	int	Sort(int num,int type=1);

	//remove data，删除数据
	int	Remove( const T& t );

	//empty table，清空表
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

	Data = new T[m_size];	//数据数组，存储一个数据的结构体
	Ch = new int[m_size];	//整形序列，存储表的序号,Ch和逻辑号基本一致，log+1
	Ph = new int[m_size];	//整形序列，存储表的物理序号，Data的角标
	Log = new int[m_size];	//整形序列，存储表的逻辑序号，按顺序排列
	Size = m_size;	//尺寸
	Count = 0;
	Head = -1;
	Tail = -1;
	Next = 0;
	Sequent = 1;

	for(i = 0; i < m_size; i++)
	{
		Ch[i] = i+1;	//链表的序号从1开始,ch指针总是比后面多一位，主要是为了链表顺利后移
		Ph[i] = -1;		//物理位置和逻辑位置都置为-1
		Log[i] = -1;
	}

	Ch[m_size-1] = -1;	//将链的最后一个序号置为-1
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
//把当前链表中的数据拷贝到一个缓存里面，这个缓存的结构和链表的结构一样
template <class T> void DTable<T>::CopyTable( T* buffer ) const
{
	int		pos, cur;

	pos = 0;
	cur = Head;//从这个链表的第一个数据开始拷贝
	while(cur != -1)
	{
		buffer[pos++] = Data[cur];
		cur = Ch[cur];//链表向下移动一个指针，链表的序号是从1开始的
		//也就是ch[0]=1;ch[1]=2;ch[2]=3;...ch[n]=-1;,当ch[]到最后一个时为-1，循环停止
	}
}

//copy data in table，整理表，将后面的多余部分截掉，只保留前面的部分
//lld edit 2018-4-24
//template <class T> void DTable<T>::TrimTable(int cnt) const
//{
//	int		pos, cur;
//	T*	buffer = new T[cnt];	//数据数组，存储一个数据的结构体
//	CopyTable(T);
//	delete[] Data;
//	Data = new T[cnt];	//数据数组，存储一个数据的结构体
//	pos = 0;
//	cur = Head;//从这个链表的第一个数据开始拷贝
//	while(cur != -1)
//	{
//		Data[pos++] = buffer[cur];
//		cur = Ch[cur];//链表向下移动一个指针，链表的序号是从1开始的
//		//也就是ch[0]=1;ch[1]=2;ch[2]=3;...ch[n]=-1;,当ch[]到最后一个时为-1，循环停止
//	}
//	//Tail = cnt-1;
//	//Size = cnt;
//	delete[] buffer;
//}

///////////////////////////////////////////////////////////////////////////////
//  UpdateTable()
//  Attention:	This function will not check the size of buffer.
//把一个缓存（和这个链表的结构一样）的所有数据更新到表里面
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
//  BiFind() 二分查找，假设所有的元素都按逻辑号从小到大排列
//逻辑号实际是物理号的下标，也就是数据的依次排列顺序
template <class T> T* DTable<T>::BiFind( T& t )
{
	//int log;//逻辑号
	int high = Count - 1;
//	if (high == 0)//只有一个数据
//		return 0;
	
	int low = 0;
	int middle;

    while(low <= high)
	{
        middle = (low + high) / 2;

        if(t == Data[Ph[middle]])
		{  //找到,返回下标索引值
            return &Data[Ph[middle]];
        }
		else if(t<Data[Ph[middle]])
		{  //查找值在低半区
            high = middle - 1;
        }
		else
		{  //查找值在高半区
            low = middle + 1;
        }
    }
    return 0;  //找不到
}	


///////////////////////////////////////////////////////////////////////////////
//  Find()  实际上是拿id号在查找
template <class T> T* DTable<T>::Find( T& t )
{
	return BiFind(t);


	int		cur;

	cur = Head;//从头开始查找
	while(cur != -1)
	{
		//判断链表里面的数据是否相等，这个需要判断每个实例化的数据，一般情况
		//下是用ID号或者几个ID号来判断
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

	//闭括弧
	if ((type==0) || (type==1))
	{
		while(low <= high)
		{
			middle = (low + high) / 2;

			if(t == Data[Ph[middle]])
			{  //找到,返回下标索引值
				if (type==0)//FindPh
				{
					t = Data[Ph[middle]];//将所有的找到的赋给临时数据
					return Ph[middle];
				}
				else if (type==1)//FindLog
				{
					t = Data[Ph[middle]];//将所有的找到的赋给临时数据
					return middle;
				}
			}
			else if(t<Data[Ph[middle]])
			{  //查找值在低半区
				high = middle - 1;
			}
			else
			{  //查找值在高半区
				low = middle + 1;
			}
		}
	}
	else//查找可以插入的位置
	{
		if(t<Data[Ph[low]])//比最小的还要小
		{
			//返回物理号
			return Ph[low];
		}
		
		int dist=10000000;
		//high++;//闭括弧 
		while(low <= high)//插入的位置不可能完全吻合
		{
			middle = (low + high) / 2;

			if ((high-low) < dist)
			{
				dist = high-low;//让高低区间尽可能小，如果不能做到，说明找到了
			}
			else
			{
				//返回物理号
				return Ph[high];
			}

			if(t<Data[Ph[middle]])
			{  //查找值在低半区
				high = middle;
			}
			else
			{  //查找值在高半区
				low = middle;
			}
		}

	}

	return -1;
}
///////////////////////////////////////////////////////////////////////////////
//  FindPh()，查找数据的物理位置，一般就是数据的顺序存放位置
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
//关键性方法，将数据插入到链表中
template <class T> int DTable<T>::Insert( T& t )
{
	int		cur, curr, pos, phpos, prev, temp;

	//check if table is full
	//说明指针已经满，需要扩大内存
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
		//初始化的时候：Count = 0;	Head = -1;	Tail = -1;	Next = 0;

		Data[Next] = t;		//Data[0] = t，赋值
		phpos = Next;		//phpos=0，本次得到的编号，给物理号
		Head = Next;		//Head=0
		Tail = Next;		//Tail=0
		Ph[Count] = Next;	//Ph[0]=0，物理号，数组的角标，物理号一直都是从0开始增长
		Log[Next] = Count++;//Log[0]=0，逻辑号，如果都是增加，逻辑号也是从0开始增长，但有可能是乱序进入
		Next = Ch[Next];	//Next=Ch[0]=1，ch负责将指针后移,初始化的时候就比自己的角标大1
		Ch[Tail] = -1;		//Ch[0]=-1 链表的最后一个有效位置为-1
	}
	else
	{		
		if(Data[Tail] < t)//判断大小要和实例类有关，一般是ID号
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
			cur = BiFind(t,2);//可以插入的物理位置，将这个数据逻辑号向后推一个位置
			pos = Log[cur];//这个准备后移的逻辑位置
			if (pos==0)
				prev = -1;
			else
				prev = Ph[pos-1];//前一个物理位置的数据

			Data[Next] = t;
			phpos = Next;
			Ph[pos] = Next;
			Log[Next] = pos;
			
			//处理被后移的数据的逻辑位置
			Ph[++pos] = cur;//被后移的数据的物理位置和逻辑位置的对应关系改变
			Log[cur]++;  // == pos被后移的数据的逻辑号加1
			
			curr = Ch[cur];//判断后移的数据是否是最后的一个数据
			while(curr != -1)//不是最后一个数据，在它的前面插入了一个数据，后面的数据位置需要向后挪一下
			{
				//逻辑号后移一位
				Ph[++pos] = curr;//对链表进行维护，使其始终和逻辑号保持同步
				Log[curr]++;
				curr = Ch[curr];
			}
			//如果是第一次插入，直接将Next位置赋值
			//用链表管理数据，每次插入时，要将链表进行维护
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
			Sequent = 0;//只要有插入操作都会破坏逻辑号和物理号的顺序
			Count++;



/*

			//insert data by ascending order
			cur = Head;
			pos = 0;
			prev = -1;
			while(cur != -1)//从头开始进行检索
			{
				if(t < Data[cur])//找到可以插入的地方
				{
					Data[Next] = t;
					phpos = Next;
					Ph[pos] = Next;
					Log[Next] = pos;
					Ph[++pos] = cur;
					Log[cur]++;  // == pos
					curr = Ch[cur];
					while(curr != -1)//插在数据链的中间，在它的前面插入了一个数据，后面的数据位置需要向后挪一下
					{
						Ph[++pos] = curr;
						Log[curr]++;
						curr = Ch[curr];
					}
					//如果是第一次插入，直接将Next位置赋值
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
//关键性方法，将数据插入到链表中，不考虑顺序，在最后做完后排序
template <class T> int DTable<T>::Append( T& t )
{
	int	phpos;

	//check if table is full
	//说明指针已经满内，需要扩大内存
	if(Next == -1)
	{
		return -1;
	}

	//一直都在增长，逻辑号和物理号一致，在做关系表时要用到物理号 2018-4-26
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
	Next = Ch[Next];	//Next=Ch[0]=1，ch负责将指针后移,初始化的时候就比自己的角标大1
	Ch[Tail] = -1;		//Ch[0]=-1 链表的最后一个有效位置为-1
	Count++;//个数累加
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
				  //Ph[i] = 1;//标记成重复记录					
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
   			     //Ph[j-1] = 1;//标记成重复记录					
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
        
                           
//将append进去的全部数据进行排序，删除重复记录，重新设定逻辑号、物理号、链表号
template <class T> int DTable<T>::Sort(int num,int type)
{
	int		i,  cnt;
	T*		temp;
	T tmp;//

	//需要排序
	if (type == 1)
		myQsort(Data,num);
	//qsort(Data,num,sizeof(T),Cmp);

	
//删除重复记录
	temp = new T[Count];

//	pos = 0;
//	cur = Head;

	for(i = 0; i < Count; i++)
	{
		temp[i] = Data[i];
	}
		
//	while(cur != -1)//把数据从头到尾全部写到临时的数组里面
//	{
//		temp[pos++] = Data[cur];
//		cur = Ch[cur];
//	}

	//把临时表全部再倒到Data数据表里面，把物理号和逻辑号统一起来
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

	//处理链表中的空白部分
	for(i = Count; i < Size; i++)
	{
		Data[i] = T();//数据用空白替代
		Ch[i] = i + 1;
		Ph[i] = -1;//物理号和逻辑号都给出-1
		Log[i] = -1;
	}
	Next = Count;//给出这个链表的有数组部分的下一个，便于今后的插入
	Ch[Size-1] = -1;

	Sequent = 1;//表示已经整理过了
	delete[] temp;//把临时的数组清掉

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

	if(Sequent == 1)//本身已经排过序了
	{
		return 1;
	}

	if(Count == 0)//把空链表重新整理一下
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
		//先做一个临时的链表出来，链表中有数据的部分是少数
		//还有大量的非数据部分也要处理
		temp = new T[Count];

		pos = 0;
		cur = Head;
		while(cur != -1)//把数据从头到尾全部写到临时的数组里面
		{
			temp[pos++] = Data[cur];
			cur = Ch[cur];
		}

		//把临时表全部再倒到Data数据表里面，把物理号和逻辑号统一起来
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

		//处理链表中的空白部分
		for(i = Count; i < Size; i++)
		{
			Data[i] = T();//数据用空白替代
			Ch[i] = i + 1;
			Ph[i] = -1;//物理号和逻辑号都给出-1
			Log[i] = -1;
		}
		Next = Count;//给出这个链表的有数组部分的下一个，便于今后的插入
		Ch[Size-1] = -1;

		Sequent = 1;//表示已经整理过了
		delete[] temp;//把临时的数组清掉
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  BuildChain()
//仅仅是重新处理链表，不涉及到数据
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
//给链表赋值，把一个链表赋给另一个链表，包括数据和位置信息
template <class T> const DTable<T>& DTable<T>::operator = (const DTable<T>& t)
{
	int		i;

	//如果新的尺寸和老的不一样，将老的全部清掉，重新分配内存
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
//父子关系，主要用在各种关系表中，需要从关系表里面找到和其对应的数据
//通过子的id号得到父亲
//循环整个关系表，找到这个父亲的id号后，将其物理号得到
template <class T> 	int DTable<T>::FindParentPtr1(const char* c_id) const
{
	int high = Count - 1;
	int low = 0;
	int middle;

    while(low <= high)
	{
        middle = (low + high) / 2;

		if(strcmp(Data[Ph[middle]].GetChildId(), c_id) == 0)
		{  //找到,返回下标索引值
            return Data[Ph[middle]].GetParentPtr();
        }
		else if ( strcmp(c_id , Data[Ph[middle]].GetChildId()) < 0 )
		{  //查找值在低半区
            high = middle - 1;
        }
		else
		{  //查找值在高半区
            low = middle + 1;
        }
    }
    return -1;  //找不到
//////////////////////////////////////////////////////////////////////////




	int		cur;

	//对整个链表进行循环，线性循环
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
//通过数据的物理号得到父亲的id号
//已知这个数据在关系表中的对应位置
//nd r_sec
//2  6
//表示节点号为2 的节点在r_nd_sec关系表中的第6个位置可以找到这个节点连的一个线段
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
//根据id号返回子集的首地址，物理位置
//输入父亲的物理位置，这个位置是父亲第一次出现在关系表中的位置，后面再出现的都是以后的数据
//一般情况下不主张用这个函数，主要是速度太慢
template <class T> 	ChildPtr1* DTable<T>::FindChildPtr1(const char* p_id) const
{
	
	int			cur;
	ChildPtr1	*head, *tail, *temp;

	head = 0;
	tail = 0;
	temp = 0;

//这个函数需要从关系表的最开始位置进行搜索，事先并不知道在关系表中第一次出现的位置
	cur = Head;
	while(cur != -1)
	{
		if(strcmp(Data[cur].GetParentId(), p_id) == 0)
		{
			//构造一个新子集元素
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

	fptr = r_ptr;//这个指针是父亲第一次在关系表中出现的位置
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
		fptr = Data[fptr].fptr;//同样的父亲第二次（下一次）出现的位置，用链表维护
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
// ExpandableHashMap.h
#include <vector>
using namespace std;


template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct Node
    {
        KeyType key;
        ValueType val;
        
        Node *nextNode;
    };
    
    void resize();
    
    vector<Node*> m_values;
    int m_size;
    int m_maxSize;
    int m_bucketSize;
    double m_load;
};

//ExpandableHashMap constructor that initializes all variables
template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    m_bucketSize = 8;
    if (m_load <= 0)
        m_load = 0.5;
    else
        m_load = maximumLoadFactor;
    m_maxSize = m_bucketSize*m_load;
    //creates a new vector that is the bucket size
    for (int a = 0; a < m_bucketSize; a++)
    {
        m_values.push_back(nullptr);
    }
}

//ExpandableHashMap destructor that deletes all dynamically allocated variables
template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    for (int a = 0; a < m_bucketSize; a++)
    {
        Node* temp = m_values[a];
        while (temp != nullptr)
        {
         
            Node *p = temp;
            temp = temp->nextNode;
            delete p;
        }
        delete temp;
    }

    m_values.clear();
}

//resets expandableHashMap to default so that bucket size is 8 
template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    m_bucketSize = 8;
    m_maxSize = 8*m_load;
    //deletes every value and deallocates pointers in the vector
    for (int a = 0; a < m_bucketSize; a++)
    {
        Node* temp = m_values[a];
        while (temp != nullptr)
        {
            
            Node *p = temp;
            temp = temp->nextNode;
            delete p;
        }
        delete temp;
    }
    //resets the map to the correct size
    m_values.resize(8);
    for (int a = 0; a < m_values.size(); a++)
    {
        m_values[a] = nullptr;
    }
}

//returns current size of map
template <typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_size;
}


template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    
    int n = hasher(key)%m_bucketSize;
    if (m_values[n] == nullptr)
    {
        Node *p = new Node;
        if (m_size >= m_maxSize)
            resize();
        p->key = key;
        p->val = value;
        p->nextNode = nullptr;
        m_size++;
        m_values[n] = p;
    }
    else if (m_values[n]->key == key)
        m_values[n]->val = value;
    else
    {
        Node *prev = m_values[n];
        Node *temp = m_values[n]->nextNode;
        for (int a = 1; a < m_maxSize; a++)
        {
            
            if (temp == nullptr)
            {
                if (m_size >= m_maxSize)
                    resize();
                Node *p = new Node;
                prev->nextNode = p;
                p->key = key;
                p->val = value;
                p->nextNode = nullptr;
                m_size++;
                return;
            }
            if (temp->key == key)
            {
                temp->val = value;
                return;
            }
            
            prev = prev->nextNode;
            temp = temp->nextNode;
        }
    }
}

template <typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    int n = hasher(key)%m_bucketSize;
    const Node *temp = m_values[n];
    while (temp != nullptr)
    {
        if (temp->key == key)
        {
            const ValueType *value = &(temp->val);
            return value;
        }
        temp = temp->nextNode;
    }
    return nullptr;
}

//private functions
template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::resize()
{
    int size = m_values.size();
    m_maxSize = size*m_load;
    vector<Node*> temp(size*2,nullptr);
    for (int a = 0; a < size*2; a++)
    {
        if (a < size && m_values[a] != nullptr)
        {
            int n = hasher(m_values[a]->key)%m_bucketSize;
            temp[n] = m_values[a];
        }
    }
    for (int a = 0; a < m_bucketSize; a++)
    {
        Node* temp = m_values[a];
        while (temp != nullptr)
        {

            Node *p = temp;
            temp = temp->nextNode;
            delete p;
        }
        delete temp;
    }
    m_values.resize(size*2);
    for (int a = 0; a < m_values.size(); a++)
    {
        m_values[a] = temp[a];
    }
    m_bucketSize = 2*m_bucketSize;
}

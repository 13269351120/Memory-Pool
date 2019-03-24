#include <memory>

template <typename T>
struct StackNode_
{
    T data;
    StackNode_* prev;
};

template <typename T, typename Alloc = std::allocator<T>>
class StackAlloc
{
    public:
        typedef StackNode_<T> Node;
        typedef typename Alloc::template rebind<Node>::other allocator;
        
        StackAlloc() { head_ = 0; }

        ~StackAlloc() { clear(); }

        bool empty() { return (head_ == 0); }

        void clear()
        {
            Node* cur = head_;
            while(cur != 0)
            {
                Node* tmp = cur->prev;
                allocator_.destroy(cur);
                allocator_.deallocate(cur,1);
                cur = tmp;
            }
            head_ = 0;
        }

        void push(T element)
        {
            Node* newNode = allocator_.allocate(1);
            allocator_.construct(newNode, Node());
            newNode->data = element;
            newNode->prev = head_;
            head_ = newNode;
        }

        T pop()
        {
            T res = head_->data;
            Node* tmp = head_->prev;
            allocator_.destroy(head_);
            allocator_.deallocate(head_, 1);
            head_ = tmp;
            return res;
        }

        T top() { return (head_->data); }

    private:    
        allocator allocator_;

        Node* head_;

};


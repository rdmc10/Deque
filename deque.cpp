#include <iostream>
#include <vector>
#define block_size 8

template <class T>
class Deque{
    private:
        T** blocks;
        int nr_blocks,poz_sus,poz_jos;
        int index_front,index_back;
        int size,blocks_used;
    public:
        Deque();
        void push_back(T key);
        void push_front(T key);
        void pop_back();
        void pop_front();
        void display();
        bool empty();
        T front();
        T back();
        int get_size();
        T& at(int index);
        T& operator[](int index);
        void insert(int index,T key);
        void erase(int index);
        void shrink_to_fit();
};

template <class T> Deque<T>::Deque(){
    nr_blocks = 5;
    blocks = new T*[nr_blocks];
    for(int i=0;i<nr_blocks;i++)blocks[i]=new T[block_size];
    poz_sus=block_size/2;
    poz_jos=block_size/2 - 1;
    index_front = nr_blocks/2;
    index_back = nr_blocks/2;
    blocks_used=0;
    size=0;
};

template <class T> void Deque<T>::display(){
    for(int i=0;i<nr_blocks;i++){
        for(int j=0;j<block_size;j++){
            std::cout << blocks[i][j] << " ";
        }
        std::cout << std::endl;
    }
    //for(int i=index_front;i<=index_back;i++){
    //    for(int j=0;j<block_size;j++){
    //        std::cout << blocks[i][j] << " ";
    //    }
    //    std::cout << std::endl;
    //}
}

template <class T> T Deque<T>::front(){
    return blocks[index_front][poz_sus];
}

template <class T> T Deque<T>::back(){
    return blocks[index_back][poz_jos];
}

template <class T> int Deque<T>::get_size(){
    return this->size;
}

template <class T> bool Deque<T>::empty(){
    return this->size == 0;
}

template <class T> void Deque<T>::push_front(T key){
    if(size == 0)blocks_used++;
    if(poz_sus!=0){
        poz_sus--;
        blocks[index_front][poz_sus]=key;
    }else if(index_front != 0){
        index_front--;
        poz_sus=block_size-1;
        blocks[index_front][poz_sus]=key;
        blocks_used++;
    }
    else{
        int h=0;
        blocks_used++;
        nr_blocks*=2;
        poz_sus=block_size-1;
        T** new_deque = new T*[nr_blocks];

        index_front = (nr_blocks - blocks_used)/2;
        index_back = index_front + blocks_used-2;

        for(int i=0;i<index_front;i++)new_deque[i]=new T[block_size];
        for(int i=index_front;i<=index_back;i++)new_deque[i]=blocks[h++];
        for(int i=index_back+1;i<nr_blocks;i++)new_deque[i]=new T[block_size];
        index_front--;

        new_deque[index_front][poz_sus]=key;
        T** temp = blocks;
        blocks = new_deque;
        delete[] temp;
    }
    std::cout << "Blocks: " << nr_blocks << std::endl;
    std::cout << "Blocks used: " << blocks_used << std::endl;
    std::cout << "Index front: " << index_front << std::endl;
    std::cout << "Index back: " << index_back << std::endl;
    std::cout << std::endl << std::endl;
    size++;
}

template <class T> void Deque<T>::push_back(T key){
    if(size == 0)blocks_used++;
    if(poz_jos!=block_size-1){
        poz_jos++;
        blocks[index_back][poz_jos]=key;
    }else if(index_back!=nr_blocks-1){
        index_back++;
        poz_jos=0;
        blocks[index_back][poz_jos]=key;
        blocks_used++;
    }else{
        blocks_used++;
        nr_blocks*=2;
        poz_jos=0;
        T** new_deque = new T*[nr_blocks];
        int old_index_front = index_front;
        index_front = (nr_blocks-blocks_used)/2;
        index_back = index_front + blocks_used-1;

        for(int i=0;i<index_front;i++)new_deque[i]=new T[block_size];
        for(int i=index_front;i<index_back;i++)new_deque[i]=blocks[old_index_front++];
        for(int i=index_back;i<nr_blocks;i++)new_deque[i]=new T[block_size];

        new_deque[index_back][poz_jos] = key;
        T** temp = blocks;
        blocks = new_deque;
        delete[] temp;
    }
    std::cout << "Blocks: " << nr_blocks << std::endl;
    std::cout << "Blocks used: " << blocks_used << std::endl;
    std::cout << "Index front: " << index_front << std::endl;
    std::cout << "Index back: " << index_back << std::endl;
    std::cout << std::endl << std::endl;
    size++;
}

template <class T> void Deque<T>::pop_back(){
    if(size){
        blocks[index_back][poz_jos]=-20;
        poz_jos--;
        if(poz_jos == -1){
            poz_jos=block_size-1;
            index_back--;
            blocks_used--;
        }
        size--;
    }
}

template <class T> void Deque<T>::pop_front(){
    if(size){
        blocks[index_front][poz_sus]=-20;
        poz_sus++;
        if(poz_sus == block_size){
            poz_sus=0;
            index_front++;
            blocks_used--;
        }
        size--;
    }
}

template <class T> T& Deque<T>::operator[](int index){
    if(index >= size || index < 0){
        std::cout << "Index out of bounds" << std::endl;
        int a = INT32_MIN;
        return a;
    }else if(index < block_size-poz_sus)return blocks[index_front][poz_sus+index];
    else{
        int block_index = (index-(block_size-poz_sus)) / block_size + 1+index_front;
        int col_index = ((index-(block_size-poz_sus)) % block_size );
        return (blocks[block_index][col_index]);
    }
}

template <class T> T& Deque<T>::at(int index){
    if(index < 0){
        std::cout << "Index out of bounds" << std::endl;
        int a = INT32_MIN;
        return a;
    }else if(index < block_size-poz_sus)return (blocks[index_front][poz_sus+index]);
    else{
        int block_index = (index-(block_size-poz_sus)) / block_size + 1+index_front;
        int col_index = ((index-(block_size-poz_sus)) % block_size );
        return (blocks[block_index][col_index]);
    }
}

template <class T> void Deque<T>::insert(int index,T key){
    if(poz_jos == block_size-1){
        blocks_used++;
        nr_blocks*=2;
        poz_jos=0;
        T** new_deque = new T*[nr_blocks];
        int old_index_front = index_front;
        index_front = (nr_blocks-blocks_used)/2;
        index_back = index_front + blocks_used-1;

        for(int i=0;i<index_front;i++)new_deque[i]=new T[block_size];
        for(int i=index_front;i<index_back;i++)new_deque[i]=blocks[old_index_front++];
        for(int i=index_back;i<nr_blocks;i++)new_deque[i]=new T[block_size];

        T** temp = blocks;
        blocks = new_deque;
        delete[] temp;
    }else
        poz_jos++;
    for(int i=size-1;i>=index;i--)
        this->at(i+1)=this->at(i);
    this->at(index)=key;
    size++;
}

template <class T> void Deque<T>::erase(int index){
    if(poz_jos==0){
        index_back--;
        blocks_used--;
        poz_jos = block_size-1;
    }else
        poz_jos--;
    for(int i=index;i<size;i++)
        this->at(i)=this->at(i+1);
    size--;
}

template <class T> void Deque<T>::shrink_to_fit(){
    for(int i=0;i<blocks_used;i++)blocks[i]=blocks[index_front++];
    index_front=0;
    index_back=blocks_used-1;
    nr_blocks=blocks_used;
}

int main(){
    Deque<int> deq;
    for(int i=1;i<=60;i++)
        deq.push_back(i);
    for(int i=2;i<=80;i++)
        deq.push_front(i);

    for(int i=0;i<29;i++){
        deq.pop_back();
        deq.pop_front();
    }
    for(int i=100;i<=200;i++)
        deq.push_back(i);
    for(int i=150;i<=200;i++)
        deq.push_front(i);



    std::cout << std::endl;
    deq.insert(0,2000);
    deq.erase(deq.get_size()-1);
    deq.push_back(7000);
    std::cout << deq.front() << std::endl << deq.back();
    std::cout << std::endl;
    std::cout << std::endl;
    deq.shrink_to_fit();
    deq.display();

    std::cout << deq[2] <<  std::endl;
}

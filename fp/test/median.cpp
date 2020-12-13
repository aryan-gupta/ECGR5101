#include <iostream>
#include <queue>
#include <vector>
using namespace std;


// Helper function to print queue for debugging 

template<typename T> void printQueue(T& q) {
    T q_copy(q);
    while(!q_copy.empty()) {
        cout << q_copy.top() << " ";
        q_copy.pop();
    }
    cout << endl;
}
 

vector<int> findMedian(vector<int>& data) {
    priority_queue<int> max_heap; // STL Max-heap
    priority_queue<int, vector<int>, greater<int> > min_heap; // STL Min-heap
    
    vector<int> res;
    
    double median = data[0]; // insert the median
    max_heap.push(data[0]);
    res.push_back(median);

    for (int idx = 1; idx < data.size(); idx++){ // for each index shift

        int next = data[idx]; // next value (child value)

        if(max_heap.size() > min_heap.size()){ // if the max(left) > min(right)
            
            if (next < median){
                max_heap.push(max_heap.top()); // add the parent value
                max_heap.pop(); // remove the end
                max_heap.push(next); // add next value in
            }

            else{ // the end step
                min_heap.push(next); // if not, then push next value into min
                median = (double)(max_heap.top() + min_heap.top()) /2; // even middle, find the avergae
            }
        }



        else if(max_heap.size() == min_heap.size())     // when both heaps have equal elements
        { 
            if(next < median){ 
                max_heap.push(next); 
                median = (double)max_heap.top(); 
            } 
            else{ 
                min_heap.push(next); 
                median = (double)min_heap.top(); 
            } 
        } 






        else { //if (max_heap.size() < min_heap.size()){ // when right have more elements

            if (next > median){
                max_heap.push(min_heap.top());
                max_heap.pop();
                max_heap.push(next);
            }

            else{ // the end step
                max_heap.push(next);
                median = (double)(max_heap.top() + min_heap.top() ) /2; 
            }
        }
        
        res.push_back(median); 
    }
    
    return res;
}



//////////////////////////////////////////////////////////////////////////////////
int main() {
    vector<int> data_stream = {5, 42, 29, 85, 95, 99, 2, 15};
    // i-th element of median_stream is median of first i elements of input 
    vector<int> median_stream = findMedian(data_stream) ;
    for (auto ele: median_stream) {
        cout << ele << " "; // Answer should be - 5 23 29 35 42 63 42 35 
    }
    cout << endl;
}

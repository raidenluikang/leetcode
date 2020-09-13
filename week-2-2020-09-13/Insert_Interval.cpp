class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        
        auto iter = std::lower_bound(intervals.begin(), intervals.end(), newInterval);
        
        //  (*iter) >= newInterval
        if (iter == intervals.end()) {
            if (iter == intervals.begin()) {  // intervals is empty
                intervals.push_back(newInterval);
            } else {
                --iter;
                auto& v = *iter;
                // It's guaranteed that, 
                //v[0] < newInterval[0]  or v[0] == newInterval[0]  and v[1] < newInterval[1]
                if (v[1] < newInterval[0]) {
                    intervals.push_back(newInterval);
                } else if (v[1] >= newInterval[1]) {
                    // newInterval within v
                    // do nothing.
                } else { // v[1] < newwInterval[1]
                    v[1] = newInterval[1];
                }
            }
        } else if (iter == intervals.begin()) {
            auto& v = *iter;
            // v[0] > newInterval[0]   or  v[0] == newInterval[0]  and v[1] >= newInterval[1]
            if (v[0] > newInterval[1]){
                intervals.insert(iter, newInterval);
            } else if (v[1] >= newInterval[1] ) {
                v[0] = std::min(v[0], newInterval[0]);
            } else {
                auto next_iter = ++iter;
                while (next_iter != intervals.end() && (*next_iter)[1] <= newInterval[1])
                    ++next_iter;
                
                v[0] = std::min(v[0], newInterval[0]);
                v[1] = std::max(v[1], newInterval[1]);
                
                if (next_iter != intervals.end() && (*next_iter)[0] <= newInterval[1]){
                    v[1] = std::max(v[1], (*next_iter)[1]);
                    ++next_iter;
                }
                intervals.erase(iter, next_iter);
            }
        } else {
            // (*iter)[0] > newInterval[0]
            auto& v = *iter;
            if (v[0] > newInterval[1]){
                --iter;
                auto&w = *iter;
                if (w[1] < newInterval[0]){
                    intervals.insert(++iter, newInterval);
                } else {
                    // w[0] < newInterval[0].  w[1] >= newInterval[0]
                    w[1] = std::max(w[1], newInterval[1]);
                }
            } else if (v[0] == newInterval[1]) {
                --iter;
                auto& w = *iter;
                if (w[1] < newInterval[0]){
                    v[0] = std::min(v[0], newInterval[0]);
                } else {
                    v[0] = std::min(v[0], newInterval[0]);
                    v[0] = std::min(v[0], w[0]);
                    
                    intervals.erase(iter); // remove w.
                }
            } else { // v[0] < newInterval[1]
                auto prev_iter = iter; --prev_iter;
                auto& w = *prev_iter;
                if (w[1] < newInterval[0]){
                    v[0] = std::min(v[0], newInterval[0]);
                    v[1] = std::max(v[1], newInterval[1]);
                    ++prev_iter;
                } else {
                    --iter;
                    w[0] = std::min(w[0], newInterval[0]);
                    w[1] = std::max(w[1], newInterval[1]);
                    
                }
                
                auto next_iter = ++iter;
                
                while (next_iter != intervals.end() && (*next_iter)[1] <= newInterval[1])
                    ++next_iter;
                auto& u = *prev_iter;
                
                if (next_iter != intervals.end() && (*next_iter)[0] <= newInterval[1]){
                    u[1] = std::max(u[1], (*next_iter)[1]);
                    ++next_iter;
                }
                intervals.erase(iter, next_iter);
                    

            }
        }
        
        return intervals;
        
    }
};

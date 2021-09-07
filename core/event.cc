// #include "event.h"

// void No::Event::Event::On(KEY key, VALUE value) {
//     auto vec = _map.find(key);
//     if (vec == _map.end()) {
//         vector<shared_ptr<VALUE> v;
//         v.push_back(value);
//         _map.insert(map<KEY,  vector<shared_ptr<VALUE>>::value_type(key, v));
//     }
//     vec->second.push_back(value);
// }

// void No::Event::Event::Off(KEY key, uint64_t id) {
//     auto vec = _map.find(key);
//     if (vec != _map.end()) {
//         vector<shared_ptr<VALUE>>::iterator it;
//         for(it=vec->second.begin();it!=vec->second.end();)
//         {
//             if (*it.id == id) {
//                 vec.erase(it);
//             } else {
//                  it++;
//             }
//         }
//     }
// }